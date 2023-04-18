#include "My_UART.h"

#define RXBUFFER_LEN 20

extern uint8_t Rx_len ;   //总共接收的长度
extern uint8_t RX_flag ;  //接收完成的标准
extern uint8_t RxBuffer[RXBUFFER_LEN];  //接收存储


int fputc(int c,FILE *stream)  //重写printf函数
{
		HAL_UART_Transmit(&huart2,(unsigned char *)&c,1,1000);
		return 1;
}

void Check_Rx()
{
		if(RX_flag == 1)   //判断是否接收完成
		{
				printf("Rx_len = %d\n",Rx_len);
				printf("I got the message %s",RxBuffer);
				for(uint8_t i=0;i<Rx_len;i++)  //清除接收的标志
				{
						RxBuffer[i] = 0;
				}
				Rx_len=0;
				RX_flag = 0;
		}
}

