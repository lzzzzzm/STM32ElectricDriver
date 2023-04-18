#ifndef __MOTOR_H
#define __MOTOR_H

#include "main.h"
#include "PID.h"

#define RR 30u    //减速比
#define RELOADVALUE __HAL_TIM_GetAutoreload(&htim1)
#define COUNTERNUM __HAL_TIM_GetCounter(&htim1)
#define IN1(state) HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,(GPIO_PinState)(state))
#define IN2(state) HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,(GPIO_PinState)(state))

typedef struct _Motor
{
	int32_t lastAngle;
	int32_t totalAngle;
	int16_t loopNum;
	float speed;
	float targetSpeed;      //RPM
	int32_t targetAngle;		//目标角度(编码器值)
	PID pid;
	CascadePID anglePID;
}Motor;

enum Mode
{
		Speed,
		Angle
};

extern Motor motor;

void Motor_Init(void);


#endif
