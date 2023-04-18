#include "Motor.h"
#include "tim.h"

/**************Private variables*******************/
Motor motor;
enum Mode mode = Angle;
/**************内部函数声明*******************/
void Motor_Send(enum Mode mode);

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==htim6.Instance)		//定时器10ms
	{
		
		int16_t pluse = COUNTERNUM - RELOADVALUE/2;																	//脉冲数
		motor.totalAngle = pluse + motor.loopNum * RELOADVALUE/2;										//转过的圈数
		motor.speed = (float)(motor.totalAngle - motor.lastAngle)/(4*13*RR)*6000;			//输出轴的rpm
		motor.lastAngle = motor.totalAngle;
		Motor_Send(mode);
	}
	else if(htim->Instance == htim1.Instance)
	{
		if(COUNTERNUM < 10000)	motor.loopNum++;
		else if(COUNTERNUM > 10000)	motor.loopNum--;
		__HAL_TIM_SetCounter(&htim1, 10000);			
	}
}

void Motor_Init()
{
	HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
	HAL_TIM_Base_Start_IT(&htim6);
	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_2);
	__HAL_TIM_SET_COUNTER(&htim1, 10000);
	__HAL_TIM_ENABLE_IT(&htim1,TIM_IT_UPDATE);
	if(mode==Speed) PID_Init(&motor.pid, 150, 4, 0, 5000, 10000);
	else
	{
		PID_Init(&motor.anglePID.inner, 250, 1.7, 0, 5000, 10000);
		PID_Init(&motor.anglePID.outer, 1.7, 0, 0, 5000, 10000);
	}
	motor.loopNum = 0;
}

void Motor_Send(enum Mode mode)
{
	float output = 0;
	if(mode == Speed)
	{
		PID_SingleCalc(&motor.pid, motor.targetSpeed, motor.speed);
		output = motor.pid.output; 
	}
	else if(mode == Angle)
	{
		PID_CascadeCalc(&motor.anglePID, motor.targetAngle, motor.totalAngle, motor.speed);
		output = motor.anglePID.output;
	}
	
	if(output > 0)	//正转
	{
		__HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_2, (uint32_t)output);
		IN1(0);
		IN2(1);
	}
	else										//反转
	{
		__HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_2, (uint32_t)(-output));
		IN1(1);
		IN2(0);
	}
}


