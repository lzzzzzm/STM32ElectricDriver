#ifndef _MY_USART_H
#define _MY_USART_H

#include "main.h"

#define RXBUFFER_LEN 20

typedef struct User_USART
{
		uint8_t Rx_flag;									//������ɱ�־
		uint8_t Rx_len;										//���ճ���
		uint8_t frame_head[2];						//֡ͷ
		uint8_t frame_tail;								//֡β
		int x,y,w,h;
		uint8_t RxBuffer[RXBUFFER_LEN];		//���ݴ洢
}User_USART;

void OpenMvData_Process(uint8_t *RxBuffer);
void User_USART_Init(User_USART *Data);
#endif

