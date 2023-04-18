#include "My_UART.h"

#define RXBUFFER_LEN 20

extern uint8_t Rx_len ;   //�ܹ����յĳ���
extern uint8_t RX_flag ;  //������ɵı�׼
extern uint8_t RxBuffer[RXBUFFER_LEN];  //���մ洢


int fputc(int c,FILE *stream)  //��дprintf����
{
		HAL_UART_Transmit(&huart2,(unsigned char *)&c,1,1000);
		return 1;
}

void Check_Rx()
{
		if(RX_flag == 1)   //�ж��Ƿ�������
		{
				printf("Rx_len = %d\n",Rx_len);
				printf("I got the message %s",RxBuffer);
				for(uint8_t i=0;i<Rx_len;i++)  //������յı�־
				{
						RxBuffer[i] = 0;
				}
				Rx_len=0;
				RX_flag = 0;
		}
}

