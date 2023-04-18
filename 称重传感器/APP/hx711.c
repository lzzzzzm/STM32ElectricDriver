#include "hx711.h"

#define CPU_FREQUENCY_MHZ    168		// STM32��Ƶ

//��ʱ�洢����
struct Weight HX711_Buffer[2];

//��������ʼ����
uint32_t Weight_Maopi_01;
uint32_t Weight_Maopi_02;

//�ӳ�us����
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

unsigned long HX711_Read_01()        //����128
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
			count=count^0x800000;//��25�������½�����ʱ��ת������
			delay_us(1);
			HX711_SCK_01=0;  
			return(count);
}

unsigned long HX711_Read_02(void)        //����128
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
					count=count^0x800000;//��25�������½�����ʱ��ת������
					delay_us(1);
					HX711_SCK_02=0;  
					return(count);
}

//��ȡ������1�ĳ�ʼ����
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
		
		Weight_Shiwu = Weight_Shiwu - Weight_Maopi;		//��ȡʵ���AD������ֵ��
	
		Weight_Shiwu = (float)(Weight_Shiwu/HX711_Buffer[num].GapValue); 	//����ʵ���ʵ������
																											//��Ϊ��ͬ�Ĵ������������߲�һ������ˣ�ÿһ����������Ҫ���������GapValue���������
																											//�����ֲ��Գ���������ƫ��ʱ�����Ӹ���ֵ��
																											//������Գ���������ƫСʱ����С����ֵ��
	}
	return Weight_Shiwu;
}





