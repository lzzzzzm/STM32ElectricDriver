#include "user_init.h"
#include "tftlcd.h"
#include "pcf8574.h"
#include "stdio.h"
#include "usart.h"
#include "hx711.h"
#include "sys.h"

extern uint8_t buffer[100];  //LCD缓存
uint8_t test_buffer[2];
uint8_t phone_connect_flag = 0;


void User_Init()
{
		float weight_01=0;  //初始化重量
		uint8_t person_number = 0;
		PCF8574_Init();  //挂载PCF8574初始化
 		TFTLCD_Init();   //LCD初始化
	
		//LCD显示初始化
		TFTLCD_ShowStr(10,40,260,32,32,"Apollo STM32F429");
		sprintf((char *)buffer,"Phone connect:%s","disconnect");
		TFTLCD_ShowStr(10,80,370,32,32,buffer);
	
		sprintf((char *)buffer,"Total Weight :%.2fkg       ",weight_01);
		TFTLCD_ShowStr(10,120,370,32,32,buffer);
	
		sprintf((char *)buffer,"Person number:%d       ",person_number);
		TFTLCD_ShowStr(10,170,370,32,32,buffer);
	
		//接收中断，判断是否连接正常
		HAL_UART_Receive_IT(&huart2,(uint8_t *)test_buffer,1);
	
		//获得压力传感器初始重量
		Get_Maopi_01();
		
		HAL_Delay(1000);
		HAL_Delay(1000);
		Get_Maopi_01();
		
		Get_Maopi_02();
		HAL_Delay(1000);
		HAL_Delay(1000);
		Get_Maopi_02();
		//任务队列初始化
		System_Init();
		
		//Beep_On;
		
}




