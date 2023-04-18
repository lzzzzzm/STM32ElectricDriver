#ifndef __HX711_H
#define __HX711_H

#include "main.h"

struct Weight
{
		float GapValue;				//传感器校正值
		float MaoPi; 					//毛皮值
		float Real_Weight;    	//真实值
		uint16_t HX711_DOUT;			//对应传感器引脚
		uint16_t HX711_SCK;				//对应传感器引脚 
};


//IO模拟
//PB14:输入IO读取DOUT
#define HX711_DOUT_0 HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_RESET)
#define HX711_DOUT_1 HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_SET)
//PB15:输出IO
#define HX711_SCK_0  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_RESET)
#define HX711_SCK_1  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET)

#define HX711_SCK_01 PEout(5)// PE5
#define HX711_DOUT_01 PEin(6)// PB6

#define HX711_SCK_02 PEout(3)// PE3
#define HX711_DOUT_02 PEin(4)// PB4


unsigned long HX711_Read_01(void);
unsigned long HX711_Read_02(void);

void Get_Maopi_01(void);
void Get_Maopi_02(void);

float Get_Weight(uint8_t num);
#endif

