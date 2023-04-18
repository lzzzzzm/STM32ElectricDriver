#include "key.h"

Key key;
uint8_t a = 0;

extern TIM_HandleTypeDef htim2;

void Key_Init()
{
		key.mode = 2;
		key.press_state = 0;
		key.flag = 0;
		HAL_TIM_Base_Start_IT(&htim2);
}	

void Key_Check()
{
		static uint8_t state = 0, key_time = 0, twice_time = 0;
	
		if(key.flag == 1)
		{
				twice_time++;
				if(twice_time == 100)			//双击间隔太长
				{
						twice_time = 0;
						key.flag = 0;
				}
		}
		
		switch(state)
		{
			case 0:
				if(IOSTATE == key.press_state) state = 1;
				break;
			
			case 1:
				if(IOSTATE == key.press_state)
				{
						if(key.mode == 0)	a++;			//短按模式
						else if(key.mode == 2)			//双击模式
						{
								key.flag++;
								if(key.flag == 2)
								{
										a++;
										key.flag = 0;
										twice_time = 0;
								}
						}
						state = 2;
			
				}
				else state = 0;
				break;
				
			case 2:
				if(IOSTATE != key.press_state)
				{
						state = 0;
						key_time = 0;
				}
				else	if(key.mode == 1)									//长按模式
				{
						key_time++;	
						if(key_time == 50)									//长按时间
						{
								a++;
						}
				}
				break;
		}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
		if(htim->Instance == htim2.Instance)
		{
					Key_Check();
		}
}
