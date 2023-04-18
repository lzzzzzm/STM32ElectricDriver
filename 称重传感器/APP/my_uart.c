#include "My_UART.h"

#define RXBUFFER_LEN 20

extern uint8_t Rx_len ;   //�ܹ����յĳ���
extern uint8_t RX_flag ;  //������ɵı�׼
extern uint8_t RxBuffer[RXBUFFER_LEN];  //���մ洢
extern uint8_t phone_connect_flag;
static uint8_t openmv[18];	//
uint8_t Receive_data[20];

int fputc(int c,FILE *stream)  //��дprintf����
{
		HAL_UART_Transmit(&huart2,(unsigned char *)&c,1,1000);
		return 1;
}

//��д���ڷ����жϺ���
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	 if(huart->Instance==USART2)  //�ǴӴ���2�����ж�
	 {
			if(phone_connect_flag==0) phone_connect_flag = 1; //��ʾ���ӳɹ�
	 }
}

void Receive_OpenMv_data(uint8_t data)
{
		
		static uint8_t state = 0;	
		static uint8_t bit_number=0;	
		if(state==0&&data==0xb3)  //���յ�֡ͷ���ж��Ƿ�Ϊ��ȷ��֡ͷ
		{
			state=1;
			//openmv[bit_number++]=data;
		}
		else if(state==1&&data==0xb3)   //���յ���ȷ��֡ͷ
		{
			state=2;
			//openmv[bit_number++]=data;
		}
		else if(state==2)
		{
			if(data==0x0d) state = 3;   //���յ�����֡ͷ���ж��Ƿ����
			else openmv[bit_number++] = data;
		}
		else if(state==3)		
		{
						if(data == 0x0a)  //��ȷ������־λ
						{
							state = 0;
							for(uint8_t i=0;i<bit_number;i++)
							{
								Receive_data[i] = openmv[i];
								openmv[i] = 0;
							}
							bit_number = 0;
						}
						else if(data != 0x0a)  //���յ�������Ϣ
						{
									state = 0;
									for(uint8_t i=0;i<bit_number;i++)
									{
											 openmv[i]=0x00;
									}           
									bit_number = 0;
						}
		}    
}

