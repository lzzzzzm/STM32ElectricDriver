#include "my_usart.h"

User_USART OpennMv_Data;		//接收数据

//初始化函数
void User_USART_Init(User_USART *Data)
{
		for(uint8_t i=0; i < RXBUFFER_LEN; i++)	Data->RxBuffer[i] = 0;
		Data->frame_head[0] = 0x2C;
		Data->frame_head[1] = 0x12;
		Data->frame_tail = 0x5B;
		Data->Rx_flag = 0;
		Data->Rx_len = 0;
		Data->x = 0;
		Data->y = 0;
		Data->w = 0;
		Data->h = 0;
}

void OpenMvData_Process(uint8_t *RxBuffer)
{
		//检查帧头帧尾
		if(RxBuffer[0] != OpennMv_Data.frame_head[0]) return;
		if(RxBuffer[1] != OpennMv_Data.frame_head[1]) return;
		if(RxBuffer[10] != OpennMv_Data.frame_tail) return;
		
		OpennMv_Data.x = (RxBuffer[3]<<8)|RxBuffer[2];
		OpennMv_Data.y = (RxBuffer[5]<<8)|RxBuffer[4];
		OpennMv_Data.w = (RxBuffer[7]<<8)|RxBuffer[6];
		OpennMv_Data.h = (RxBuffer[9]<<8)|RxBuffer[8];
}
