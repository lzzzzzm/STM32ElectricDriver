#ifndef __KEY_H
#define __KEY_H

#include "main.h"

#define IOSTATE HAL_GPIO_ReadPin(GPIOH, GPIO_PIN_3)			//����GPIO��״̬

typedef struct
{
		uint8_t flag;			//��ǰ״̬
		uint8_t mode;				//����ģʽ
		uint8_t press_state;
}Key;

extern Key key;

void Key_Check(void);
void Key_Init(void);

#endif
