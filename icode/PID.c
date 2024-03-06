/*
 * PID.c
 *
 *  Created on: 2024年3月1日
 *      Author: 25085
 */

#include "PID.h"
#include <math.h>

PID_typedef gimbal_yaw_speed_pid;
PID_typedef gimbal_yaw_angle_pid;

#define LIMIT_MIN_MAX(input, max)   \
    {                          \
        if (input > max)       \
        {                      \
            input = max;       \
        }                      \
        else if (input < -max) \
        {                      \
            input = -max;      \
        }                      \
    }


void PID_Init(PID_typedef *pid,
              float kp,
              float ki,
              float kd,
              float i_max,
              float out_max)//PID初始化函数
{
  pid->Kp      = kp;
  pid->Ki      = ki;
  pid->Kd      = kd;
  pid->Max_iout   = i_max;
  pid->Max_out = out_max;
}


float PID_Calc(PID_typedef *PID, float measure, float target)//PID运算
{
	if(PID == NULL)
		return 0;
	PID->error[2] = PID->error[1];
	PID->error[1] = PID->error[0];
	PID->measure = measure;
	PID->target = target;
	PID->error[0] =target - measure;
		if(PID->mode == PID_POSITION_SPEED || PID->mode == PID_POSITION_ANGLE){
			if(PID->mode == PID_POSITION_ANGLE){
				if(PID->error[0]>4096)	PID->error[0]=PID->error[0]-8191;  //避免电机失控
				else if(PID->error[0]<-4096)	PID->error[0]=PID->error[0]+8191;
			}
			PID->Pout = PID->Kp * PID->error[0];
			PID->Iout += PID->Ki * PID->error[0];
			PID->D_item = (PID->error[0] - PID->error[1]);
			PID->Dout = PID->Kd * PID->D_item;
			LIMIT_MIN_MAX(PID->Iout,PID->Max_iout);
			PID->OUT = PID->Pout + PID->Iout + PID->Dout;
			LIMIT_MIN_MAX(PID->OUT,PID->Max_out);
		}
		else if(PID->mode == PID_DELTA_SPEED){
			PID->Pout = PID->Kp * (PID->error[0] - PID->error[1]);
			PID->Iout = PID->Ki * PID->error[0];
			PID->D_item = (PID->error[0] - 2.0f*PID->error[1] + PID->error[2]);
			PID->Dout = PID->Kd * PID->D_item;
			PID->OUT += PID->Pout + PID->Iout + PID->Dout;
			LIMIT_MIN_MAX(PID->OUT, PID->Max_out);
		}
	else{
        PID->OUT=0;
	}
	return PID->OUT;
}

void gimbal_PID_Init()
{
	PID_Init(&gimbal_yaw_speed_pid, 30, 0, 0, 30000, 30000);//P=30,I=0,D=0
	PID_Init(&gimbal_yaw_angle_pid, 400, 0, 0, 0, 320);//P=400,I=0,D=0
}
