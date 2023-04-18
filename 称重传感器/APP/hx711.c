#include "hx711.h"

#define CPU_FREQUENCY_MHZ    168		// STM32主频

//临时存储缓冲
struct Weight HX711_Buffer[2];

//传感器初始重量
uint32_t Weight_Maopi_01;
uint32_t Weight_Maopi_02;

//延迟us函数
void delay_us(__IO uint32_t delay)
{
    int last, curr, val;
    int temp;

    while (delay != 0)
    {
        temp = delay > 900 ? 900 : delay;
        last = SysTick->VAL;
        curr = last - CPU_FREQUENCY_MHZ * temp;
        if (curr >= 0)
        {
            do
            {
                val = SysTick->VAL;
            }
            while ((val < last) && (val >= curr));
        }
        else
        {
            curr += CPU_FREQUENCY_MHZ * 1000;
            do
            {
                val = SysTick->VAL;
            }
            while ((val <= last) || (val > curr));
        }
        delay -= temp;
    }
}

void Weight_Var_Init()
{
		HX711_Buffer[0].GapValue = 10500.5;
		HX711_Buffer[0].MaoPi = 0;
		HX711_Buffer[0].Real_Weight = 0;
		
		HX711_Buffer[1].GapValue = 10500.5;
		HX711_Buffer[1].MaoPi = 0;
		HX711_Buffer[1].Real_Weight = 0;
}

unsigned long HX711_Read_01()        //增益128
{
			unsigned long count; 
			unsigned char i; 
		
			HX711_SCK_01=0; 
			count=0; 
			delay_us(1);
			while(HX711_DOUT_01);
			for(i=0;i<24;i++)
			{ 
						HX711_SCK_01=1; 
						count=count<<1; 
						delay_us(1);
						HX711_SCK_01=0; 
						if(HX711_DOUT_01) count++; 
						delay_us(1);
			} 
			HX711_SCK_01=1; 
			count=count^0x800000;//第25个脉冲下降沿来时，转换数据
			delay_us(1);
			HX711_SCK_01=0;  
			return(count);
}

unsigned long HX711_Read_02(void)        //增益128
{
        unsigned long count; 
        unsigned char i; 

          HX711_SCK_02=0; 
          count=0; 
          delay_us(1);
          while(HX711_DOUT_02);
          for(i=0;i<24;i++)
					{ 
                HX711_SCK_02=1; 
                count=count<<1; 
                delay_us(1);
                HX711_SCK_02=0; 
                if(HX711_DOUT_02) count++; 
                delay_us(1);
					} 
					HX711_SCK_02	=1; 
					count=count^0x800000;//第25个脉冲下降沿来时，转换数据
					delay_us(1);
					HX711_SCK_02=0;  
					return(count);
}

//获取传感器1的初始重量
void Get_Maopi_01()
{
	Weight_Maopi_01 = HX711_Read_01();	
}
void Get_Maopi_02()
{
	Weight_Maopi_02 = HX711_Read_02();	
} 
float Get_Weight(uint8_t num)
{
	float Weight_Shiwu = 0;
	float Weight_Maopi = 0;
	HX711_Buffer[num].Real_Weight = HX711_Read_01();
	if(HX711_Buffer[num].Real_Weight > Weight_Maopi_01)			
	{
		Weight_Shiwu = HX711_Buffer[num].Real_Weight;
		
		if(num==0) Weight_Maopi = Weight_Maopi_01;
		else Weight_Maopi = Weight_Maopi_02;
		
		Weight_Shiwu = Weight_Shiwu - Weight_Maopi;		//获取实物的AD采样数值。
	
		Weight_Shiwu = (float)(Weight_Shiwu/HX711_Buffer[num].GapValue); 	//计算实物的实际重量
																											//因为不同的传感器特性曲线不一样，因此，每一个传感器需要矫正这里的GapValue这个除数。
																											//当发现测试出来的重量偏大时，增加该数值。
																											//如果测试出来的重量偏小时，减小改数值。
	}
	return Weight_Shiwu;
}





