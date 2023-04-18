#ifndef __KEY_H
#define __KEY_H

#include "main.h"

#define IOSTATE HAL_GPIO_ReadPin(GPIOH, GPIO_PIN_3)			//按键GPIO的状态

typedef struct
{
		uint8_t flag;			//当前状态
		uint8_t mode;				//按键模式
		uint8_t press_state;
}Key;

extern Key key;

void Key_Check(void);
void Key_Init(void);

#endif
