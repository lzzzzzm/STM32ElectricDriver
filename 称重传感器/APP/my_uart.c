#include "My_UART.h"

#define RXBUFFER_LEN 20

extern uint8_t Rx_len ;   //总共接收的长度
extern uint8_t RX_flag ;  //接收完成的标准
extern uint8_t RxBuffer[RXBUFFER_LEN];  //接收存储
extern uint8_t phone_connect_flag;
static uint8_t openmv[18];	//
uint8_t Receive_data[20];

int fputc(int c,FILE *stream)  //重写printf函数
{
		HAL_UART_Transmit(&huart2,(unsigned char *)&c,1,1000);
		return 1;
}

//编写串口发送中断函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	 if(huart->Instance==USART2)  //是从串口2来的中断
	 {
			if(phone_connect_flag==0) phone_connect_flag = 1; //表示连接成功
	 }
}

void Receive_OpenMv_data(uint8_t data)
{
		
		static uint8_t state = 0;	
		static uint8_t bit_number=0;	
		if(state==0&&data==0xb3)  //接收到帧头，判断是否为正确的帧头
		{
			state=1;
			//openmv[bit_number++]=data;
		}
		else if(state==1&&data==0xb3)   //接收到正确的帧头
		{
			state=2;
			//openmv[bit_number++]=data;
		}
		else if(state==2)
		{
			if(data==0x0d) state = 3;   //接收到结束帧头，判断是否结束
			else openmv[bit_number++] = data;
		}
		else if(state==3)		
		{
						if(data == 0x0a)  //正确结束标志位
						{
							state = 0;
							for(uint8_t i=0;i<bit_number;i++)
							{
								Receive_data[i] = openmv[i];
								openmv[i] = 0;
							}
							bit_number = 0;
						}
						else if(data != 0x0a)  //接收到错误信息
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

