#ifndef __SUPERSONIC_H
#define __SUPERSONIC_H

#include "main.h"
#include "tim.h"
#include "stdio.h"

#define CPU_FREQUENCY_MHZ    72		// STM32主频 
#define SAMPLENUM 10
#define Trig(state) HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6, (GPIO_PinState)(state))  //触发引脚：改成自己设定的引脚
#define Echo 	HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7)	//接收信号引脚，改成自己设定的引脚
#define RELOADVALUE 0xffff


typedef struct
{
	float distance;
	float max_distance;
	uint32_t rising_time;
	uint32_t falling_time;
	uint8_t capture_state;
	uint32_t time;
}Supersonic;

extern Supersonic supersonic;

/**********function define**************/
void Supersonic_Dect(void);
void Supersonic_Init(void);
void Supersonic_Start(void);

#endif
