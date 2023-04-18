/**
  * @auther whlphlg
  * @brief task.c用来定义要运行的任务函数，该文件可随意更改，自由度较大
  * @changedate 2020.09.13
  */
	
//至少要include所有的main.c里面的
#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"
#include "user_init.h"
#include "tftlcd.h"
#include "stdio.h"
#include "hx711.h"
#include "pcf8574.h"
//下面的是作者写的，必须include！！！
#include "task.h"
#include "sys.h"
#define OverWeight 150
#define OverNum 1

extern uint8_t phone_connect_flag;
extern uint8_t buffer[100];  //LCD缓存
extern uint8_t Receive_data[20];
uint8_t Red_flag = 0;

uint8_t data_buffer[2];
//task1通过LED的指示来测试整个程序初始化正常
//1s的频率更新人数
//每1s翻转电平
void task1()
{
	HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
	uint8_t person_number = 0;
	//person_number = Receive_data[0];
	//sprintf((char *)buffer,"Person number:%d       ",person_number);
	//TFTLCD_ShowStr(10,170,370,32,32,buffer);
	if(person_number>OverNum) TFTLCD_Fill(0,400,700,1000,RED);
	else if(person_number<OverNum)
	{
		TFTLCD_Fill(0,400,700,1000,GREEN);
	}
	
}
//检测蓝牙是否连接
void task2()
{
	if(phone_connect_flag == 1)
	{
			sprintf((char *)buffer,"Phone connect:%s","connected  ");
			TFTLCD_ShowStr(10,80,370,32,32,buffer);
		
			System_TaskDisable(TASK2);  //结束任务2
			//System_TaskEnable(TASK1); //开启任务1
			//System_TaskEnable(TASK3);  //开启任务3
	}
}
void task3()
{
	float weight[2];
	float total_weight = 0;
	//获取重量
	for(uint8_t i=0; i<2; i++)
	{
			weight[i] = Get_Weight(i);
	}
	//手动校准
	for(uint8_t i=0; i<2; i++)
	{
			total_weight += weight[i];
	}
	total_weight = total_weight*3/5;
	/*
	sprintf((char *)buffer,"Weight1 :%.2fkg      ",weight_01);
	TFTLCD_ShowStr(10,120,370,32,32,buffer);
	sprintf((char *)buffer,"Weight2 :%.2fkg      ",weight_02);
	TFTLCD_ShowStr(10,150,370,32,32,buffer);
	*/
	
	sprintf((char *)buffer,"Total Weight :%.2fkg      ",total_weight);
	TFTLCD_ShowStr(10,120,370,32,32,buffer);
	
	if(total_weight > OverWeight)
	{
			//Beep_On;  //打开蜂鸣器
			printf("Overweight!!!\r\n");
			printf("Now Weight is %.2f kg",total_weight);
			TFTLCD_Fill(0,400,700,1000,RED);
			Red_flag = 1;
	}
	else
	{
			//Beep_Off;  //关闭蜂鸣器
			TFTLCD_Fill(0,400,700,1000,GREEN); 
	}
	
	
}

void task4()
{
	//HAL_UART_Receive_IT(&huart3,(uint8_t *)data_buffer,1);
}



