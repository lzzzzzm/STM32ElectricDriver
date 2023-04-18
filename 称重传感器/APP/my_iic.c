#include "my_iic.h"

void IIC_Init()
{
		IIC_SDA_1;
		IIC_SCL_1; 
}
	
//����IIC��ʼ�ź�
void IIC_Start(void)
{
		SDA_OUT();     //sda�����
		IIC_SDA_1;	  	  
		IIC_SCL_1;
		HAL_Delay(4);
		IIC_SDA_0;//START:when CLK is high,DATA change form high to low 
		HAL_Delay(4);
		IIC_SCL_0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  


//����IICֹͣ�ź�
void IIC_Stop(void)
{
		SDA_OUT();//sda�����
		IIC_SCL_0;
		IIC_SDA_0;//STOP:when CLK is high DATA change form low to high
		HAL_Delay(4);
		IIC_SCL_1; 
		HAL_Delay(4);			
		IIC_SDA_1;//����I2C���߽����ź�				   	
}

uint8_t IIC_Wait_Ack(void)
{
		uint8_t ucErrTime=0;
		SDA_IN();      //SDA����Ϊ����  
		IIC_SDA_1;HAL_Delay(1);   
		IIC_SCL_1;HAL_Delay(1); 	 
		while(READ_SDA)
		{
			ucErrTime++;
			if(ucErrTime>250)
			{
				IIC_Stop();
				return 1;
			}
		}
		IIC_SCL_0;//ʱ�����0 	   
		return 0;  
} 

//����ACKӦ��
void IIC_Ack(void)
{
		IIC_SCL_0;
		SDA_OUT();
		IIC_SDA_0;
		HAL_Delay(2);
		IIC_SCL_1;
		HAL_Delay(2);
		IIC_SCL_0;
}

//������ACKӦ��		    
void IIC_NAck(void)
{
		IIC_SCL_0;
		SDA_OUT();
		IIC_SDA_1;
		HAL_Delay(2);
		IIC_SCL_1;
		HAL_Delay(2);
		IIC_SCL_0;
}					 				 
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t,IIC_SDA;   
		SDA_OUT(); 	    
    IIC_SCL_0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;  //1000 0000
        if(IIC_SDA) IIC_SDA_1;
				else IIC_SDA_0;
				txd<<=1;
				HAL_Delay(2);
				IIC_SCL_1;
				HAL_Delay(2);
				IIC_SCL_0;
				HAL_Delay(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint8_t IIC_Read_Byte(unsigned char ack)
{
		unsigned char i,receive=0;
		SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
		{
        IIC_SCL_0; 
        HAL_Delay(2);
				IIC_SCL_1;
        receive<<=1;
        if(READ_SDA)receive++;   
				HAL_Delay(1); 
    }					 
    if (!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}


