#include "my_uart.h"

User_USART JY901_data;
extern UART_HandleTypeDef huart6;
extern UART_HandleTypeDef huart1;

struct SAcc 		stcAcc;
struct SGyro 		stcGyro;
struct SAngle 	stcAngle;

uint8_t a =0;

int fputc(int c,FILE *stream)  //重写printf函数
{
		HAL_UART_Transmit(&huart1,(unsigned char *)&c,1,1000);
		return 1;
}

//初始化函数
void User_USART_Init(User_USART *Data)
{
		for(uint16_t i=0; i < RXBUFFER_LEN; i++)	Data->RxBuffer[i] = 0;
		Data->frame_head = 0x55;
		Data->Rx_flag = 0;
		Data->Rx_len = 0;
}

void JY901_Process()
{
		if(JY901_data.Rx_len < RXBUFFER_LEN) return;   	//数据长度不对

		for(uint8_t i=0;i<4;i++)
		{
				if(JY901_data.RxBuffer[i*11]!= JY901_data.frame_head) continue;
				switch(JY901_data.RxBuffer[i*11+1])
				{
						case 0x51:	
							memcpy(&stcAcc,&JY901_data.RxBuffer[2 + i*11],8);
							for(uint8_t j = 0; j < 3; j++) JY901_data.acc.a[j] = (float)stcAcc.a[j]/32768*16;									//加速度
						break;
						case 0x52:	
							memcpy(&stcGyro,&JY901_data.RxBuffer[2 + i*11],8);
							for(uint8_t j = 0; j < 3; j++) JY901_data.w.w[j] = (float)stcGyro.w[j]/32768*2000;								//角速度
						break;
						case 0x53:	
							memcpy(&stcAngle,&JY901_data.RxBuffer[2 + i*11],8);
							for(uint8_t j = 0; j < 3; j++) JY901_data.angle.angle[j] = (float)stcAngle.Angle[j]/32768*180;		//角度
						break;
				}
				
		}

}


