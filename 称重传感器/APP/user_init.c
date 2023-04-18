#include "user_init.h"
#include "tftlcd.h"
#include "pcf8574.h"
#include "stdio.h"
#include "usart.h"
#include "hx711.h"
#include "sys.h"

extern uint8_t buffer[100];  //LCD����
uint8_t test_buffer[2];
uint8_t phone_connect_flag = 0;


void User_Init()
{
		float weight_01=0;  //��ʼ������
		uint8_t person_number = 0;
		PCF8574_Init();  //����PCF8574��ʼ��
 		TFTLCD_Init();   //LCD��ʼ��
	
		//LCD��ʾ��ʼ��
		TFTLCD_ShowStr(10,40,260,32,32,"Apollo STM32F429");
		sprintf((char *)buffer,"Phone connect:%s","disconnect");
		TFTLCD_ShowStr(10,80,370,32,32,buffer);
	
		sprintf((char *)buffer,"Total Weight :%.2fkg       ",weight_01);
		TFTLCD_ShowStr(10,120,370,32,32,buffer);
	
		sprintf((char *)buffer,"Person number:%d       ",person_number);
		TFTLCD_ShowStr(10,170,370,32,32,buffer);
	
		//�����жϣ��ж��Ƿ���������
		HAL_UART_Receive_IT(&huart2,(uint8_t *)test_buffer,1);
	
		//���ѹ����������ʼ����
		Get_Maopi_01();
		
		HAL_Delay(1000);
		HAL_Delay(1000);
		Get_Maopi_01();
		
		Get_Maopi_02();
		HAL_Delay(1000);
		HAL_Delay(1000);
		Get_Maopi_02();
		//������г�ʼ��
		System_Init();
		
		//Beep_On;
		
}




