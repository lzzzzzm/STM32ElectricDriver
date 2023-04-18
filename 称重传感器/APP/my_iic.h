#ifndef _MY_IIC_H
#define _MY_IIC_H

#include "main.h"

//IO��������
#define SDA_IN()  {GPIOH->MODER&=~(3<<(5*2));GPIOH->MODER|=0<<5*2;}	//PH5����ģʽ
#define SDA_OUT() {GPIOH->MODER&=~(3<<(5*2));GPIOH->MODER|=1<<5*2;} //PH5���ģʽ
//IO����
#define IIC_SCL_0 HAL_GPIO_WritePin(GPIOH,GPIO_PIN_4,GPIO_PIN_RESET)
#define IIC_SCL_1 HAL_GPIO_WritePin(GPIOH,GPIO_PIN_4,GPIO_PIN_SET)
#define IIC_SDA_0 HAL_GPIO_WritePin(GPIOH,GPIO_PIN_5,GPIO_PIN_RESET)
#define IIC_SDA_1 HAL_GPIO_WritePin(GPIOH,GPIO_PIN_5,GPIO_PIN_SET)

#define READ_SDA  HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_5)


//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(uint8_t txd);			//IIC����һ���ֽ�
uint8_t IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
uint8_t IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�



#endif

