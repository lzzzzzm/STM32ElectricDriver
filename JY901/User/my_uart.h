#ifndef _MY_USART_H
#define _MY_USART_H

#include "main.h"
#include "JY901.h"
#include "stdio.h"
#include "string.h"


#define RXBUFFER_LEN 33

typedef struct
{
	float angle[3];
}Angle;

typedef struct
{
	float a[3];
}Acc;

typedef struct
{
	float w[3];
}SGyro;

typedef struct User_USART
{
		uint8_t Rx_flag;												//接收完成标志
		uint8_t Rx_len;													//接收长度
		uint8_t frame_head;											//帧头
		uint8_t RxBuffer[RXBUFFER_LEN];					//数据存储
		Angle angle;
		Acc acc;
		SGyro w;
}User_USART;



extern User_USART JY901_data;

void JY901_Process(void);
void User_USART_Init(User_USART *Data);

#endif

