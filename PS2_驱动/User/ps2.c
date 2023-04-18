#include "ps2.h"

void PS2_ClearData(void);
#define DELAY_TIME  delay_us(5);

uint16_t Handkey;
uint8_t Comd[2]={0x01,0x42};	//¿ªÊ¼ÃüÁî¡£ÇëÇóÊı¾İ
uint8_t Data[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //Êı¾İ´æ´¢Êı×é
uint16_t MASK[]={
    PSB_SELECT,
    PSB_L3,
    PSB_R3 ,
    PSB_START,
    PSB_PAD_UP,
    PSB_PAD_RIGHT,
    PSB_PAD_DOWN,
    PSB_PAD_LEFT,
    PSB_L2,
    PSB_R2,
    PSB_L1,
    PSB_R1 ,
    PSB_GREEN,
    PSB_RED,
    PSB_BLUE,
    PSB_PINK
};	//°´¼üÖµÓë°´¼üÃ

//ÑÓ³Ùusº¯Êı
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


//ÏòÊÖ±ú·¢ËÍÃüÁî
void PS2_Cmd(uint8_t CMD)
{
	volatile uint16_t ref=0x01;
	Data[1] = 0;
	for(ref=0x01;ref<0x0100;ref<<=1)
	{
		if(ref&CMD)
		{
			DO_H;                   //Êä³öÒ»Î»¿ØÖÆÎ»
		}
		else DO_L;

		CLK_H;                        //Ê±ÖÓÀ­¸ß
		DELAY_TIME;
		CLK_L;
		DELAY_TIME;
		CLK_H;
		if(DI)
			Data[1] = ref|Data[1];
	}
	delay_us(16);
}

//ÅĞ¶ÏÊÇ·ñÎªºìµÆÄ£Ê½
//·µ»ØÖµ£»0£¬ºìµÆÄ£Ê½
//		  ÆäËû£¬ÆäËûÄ£Ê½
uint8_t PS2_RedLight(void)
{
	CS_L;
	PS2_Cmd(Comd[0]);  //¿ªÊ¼ÃüÁî
	PS2_Cmd(Comd[1]);  //ÇëÇóÊı¾İ
	CS_H;
	if(Data[1] == 0X73)   return 0 ;
	else return 1;

}


//¶ÁÈ¡ÊÖ±úÊı¾İ
void PS2_ReadData(void)
{
	volatile uint8_t byte=0;
	volatile uint16_t ref=0x01;
	CS_L;
	PS2_Cmd(Comd[0]);  //¿ªÊ¼ÃüÁî
	PS2_Cmd(Comd[1]);  //ÇëÇóÊı¾İ
	for(byte=2;byte<9;byte++)          //¿ªÊ¼½ÓÊÜÊı¾İ
	{
		for(ref=0x01;ref<0x100;ref<<=1)
		{
			CLK_H;
			DELAY_TIME;
			CLK_L;
			DELAY_TIME;
			CLK_H;
		      if(DI)
		      Data[byte] = ref|Data[byte];
		}
        delay_us(16);
	}
	CS_H;
}

//¶Ô¶Á³öÀ´µÄPS2µÄÊı¾İ½øĞĞ´¦Àí      Ö»´¦ÀíÁË°´¼ü²¿·Ö         Ä¬ÈÏÊı¾İÊÇºìµÆÄ£Ê½  Ö»ÓĞÒ»¸ö°´¼ü°´ÏÂÊ±
//°´ÏÂÎª0£¬ Î´°´ÏÂÎª1
uint8_t PS2_DataKey()
{
	uint8_t index;

	PS2_ClearData();
	PS2_ReadData();

	Handkey=(Data[4]<<8)|Data[3];     //ÕâÊÇ16¸ö°´¼ü  °´ÏÂÎª0£¬ Î´°´ÏÂÎª1
	for(index=0;index<16;index++)
	{	    
		if((Handkey&(1<<(MASK[index]-1)))==0)
		return index+1;
	}
	return 0;          //Ã»ÓĞÈÎºÎ°´¼ü°´ÏÂ
}

//Çå³ıÊı¾İ»º³åÇø
void PS2_ClearData()
{
	uint8_t a;
	for(a=0;a<9;a++)
		Data[a]=0x00;
}

void PS2_Vibration(uint8_t motor1, uint8_t motor2)
{
	CS_L;
	delay_us(16);
    PS2_Cmd(0x01);  //¿ªÊ¼ÃüÁî
	PS2_Cmd(0x42);  //ÇëÇóÊı¾İ
	PS2_Cmd(0X00);
	PS2_Cmd(motor1);
	PS2_Cmd(motor2);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	CS_H;
	delay_us(16);  
}
//short poll
void PS2_ShortPoll(void)
{
	CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x42);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x00);
	CS_H;
	delay_us(16);	
}
//½øÈëÅäÖÃ
void PS2_EnterConfing(void)
{
    CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x43);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x01);
	PS2_Cmd(0x00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	CS_H;
	delay_us(16);
}
//·¢ËÍÄ£Ê½ÉèÖÃ
void PS2_TurnOnAnalogMode(void)
{
	CS_L;
	PS2_Cmd(0x01);  
	PS2_Cmd(0x44);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x01); //analog=0x01;digital=0x00  Èí¼şÉèÖÃ·¢ËÍÄ£Ê½
	PS2_Cmd(0xEE); //Ox03Ëø´æÉèÖÃ£¬¼´²»¿ÉÍ¨¹ı°´¼ü¡°MODE¡±ÉèÖÃÄ£Ê½¡£
				   //0xEE²»Ëø´æÈí¼şÉèÖÃ£¬¿ÉÍ¨¹ı°´¼ü¡°MODE¡±ÉèÖÃÄ£Ê½¡£
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	CS_H;
	delay_us(16);
}
//Õñ¶¯ÉèÖÃ
void PS2_VibrationMode(void)
{
	CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x4D);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0X01);
	CS_H;
	delay_us(16);	
}
//Íê³É²¢±£´æÅäÖÃ
void PS2_ExitConfing(void)
{
    CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x43);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	CS_H;
	delay_us(16);
}
//ÊÖ±úÅäÖÃ³õÊ¼»¯
void PS2_SetInit(void)
{
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_EnterConfing();		//½øÈëÅäÖÃÄ£Ê½
	PS2_TurnOnAnalogMode();	//¡°ºìÂÌµÆ¡±ÅäÖÃÄ£Ê½£¬²¢Ñ¡ÔñÊÇ·ñ±£´æ
	PS2_VibrationMode();	//¿ªÆôÕğ¶¯Ä£Ê½
	PS2_ExitConfing();		//Íê³É²¢±£´æÅäÖÃ
}
