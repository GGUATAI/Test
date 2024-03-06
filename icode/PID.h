/*
 * PID.h
 *
 *  Created on: 2024年3月1日
 *      Author: 25085
 */

#ifndef PID_H_
#define PID_H_

#include "stm32f4xx_hal.h"
#include "main.h"

typedef enum
{
	PID_POSITION_SPEED = 0,
	PID_POSITION_ANGLE,
	PID_DELTA_SPEED
}PID_mode;

typedef struct
{
	PID_mode mode;
	float Kp; //比例
	float Ki; //积分
	float Kd; //微分
	float Max_iout; //积分限幅
	float Max_out;  //输出限幅
	float measure;
	float target;
	float Pout;
	float Iout;
	float Dout;
	float D_item;
	float error[3];
	float OUT;
}PID_typedef;

void PID_Init(PID_typedef *pid,
              float kp,
              float ki,
              float kd,
              float i_max,
              float out_max);

void gimbal_PID_Init(void);
float PID_Calc(PID_typedef *PID, float measure, float target);

#endif /* PID_H_ */
