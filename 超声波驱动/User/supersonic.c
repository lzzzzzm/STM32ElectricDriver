#include "supersonic.h"

/**********private variables************/
Supersonic supersonic;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
uint8_t sample_time = 0;
uint8_t sample_num = 0;
uint8_t a = 0;
//延迟us函数
void delay_us(__IO uint32_t delay)
{
    int last, curr, val;
    int temp;

    while (delay != 0)
    {
        temp = delay > 900 ? 900 : delay;
        last = SysTick->VAL;
        curr = last - CPU_FREQUENCY_MHZ * temp;
        if (curr >= 0)
        {
            do
            {
                val = SysTick->VAL;
            }
            while ((val < last) && (val >= curr));
        }
        else
        {
            curr += CPU_FREQUENCY_MHZ * 1000;
            do
            {
                val = SysTick->VAL;
            }
            while ((val <= last) || (val > curr));
        }
        delay -= temp;
    }
}

void Supersonic_Init()
{
		supersonic.distance = 0;
		supersonic.max_distance = 1000;									//毫米为单位
		supersonic.rising_time = 0;
		supersonic.falling_time = 0;
		supersonic.capture_state = 0;
		supersonic.time = 0;
		HAL_TIM_IC_Start_IT(&htim1,TIM_CHANNEL_1);	//开启输入捕获中断
		HAL_TIM_Base_Start_IT(&htim2);
		
}

//给一个20us的脉冲，作为启动信号
void Supersonic_Start()
{
		Trig(1);
		delay_us(20);
		Trig(0);
}



//重写定时器捕获中断
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == htim1.Instance)
	{
			if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
			{
					if(supersonic.capture_state == 0) 	//	上升沿捕获
					{
						supersonic.rising_time = HAL_TIM_ReadCapturedValue(&htim1, TIM_CHANNEL_1);
						__HAL_TIM_SET_CAPTUREPOLARITY(&htim1, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);			//改为下降沿捕获
						
						HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_1);
						supersonic.capture_state = 1;
					}
					else if(supersonic.capture_state == 1)	//下降沿捕获
					{
						supersonic.falling_time = HAL_TIM_ReadCapturedValue(&htim1, TIM_CHANNEL_1);
						__HAL_TIM_SET_CAPTUREPOLARITY(&htim1, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);		//改为上升沿捕获
						
						
						supersonic.capture_state = 0;
						if(supersonic.falling_time > supersonic.rising_time) supersonic.time = supersonic.falling_time - supersonic.rising_time;
						else supersonic.time = supersonic.falling_time + RELOADVALUE - supersonic.rising_time + 1;
						
						supersonic.distance = (float)supersonic.time*340/(2*1000);

					}
			}
		
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
		if(htim->Instance ==	htim2.Instance)	//10ms中断
		{
				sample_time++;
				if(supersonic.capture_state == 0 && sample_time==SAMPLENUM)   //每隔50ms发送一次
				{
						sample_time = 0;
						Supersonic_Start();
						HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_1);
				}
		}
}



