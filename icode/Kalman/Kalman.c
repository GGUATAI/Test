/*
 * Kalman.c
 *
 *  Created on: 2024年2月8日
 *      Author: 25085
 */

#include "Kalman.h"
#include "../../icode/MPU6050/MPU6050.h"
#include "../../icode/MDKDelay/MDKDelay.h"
#include <math.h>


KalmanFilter kf_pitch;
KalmanFilter kf_roll;
int16_t AccX, AccY, AccZ, GYROX, GYROY, GYROZ;
float gyroXrate,gyroYrate,gyroZrate,accPitch,accRoll;
float pitch,roll;
float yaw_g,yaw1,yaw2;

//初始化配置
void KalmanFilter_Init(KalmanFilter *kf)
{
    kf->Q_angle = 0.001f;//超参数
    kf->Q_bias = 0.003f;
    kf->R_measure = 0.03f;

    kf->angle = 0.0f;
    kf->bias = 0.0f;

    kf->P[0][0] = 0.0f;//矩阵初始化赋值0
    kf->P[0][1] = 0.0f;
    kf->P[1][0] = 0.0f;
    kf->P[1][1] = 0.0f;
}

//更新数据
float KalmanFilter_Update(KalmanFilter *kf, float newAngle, float newRate, float dt)//Angle为陀螺仪数据，Rate为加速度计数据
{
    kf->rate = newRate - kf->bias;//先验估计 g=k+△v
    kf->angle += dt * kf->rate;

    kf->P[0][0] += dt * (dt*kf->P[1][1] - kf->P[0][1] - kf->P[1][0] + kf->Q_angle);
    kf->P[0][1] -= dt * kf->P[1][1];
    kf->P[1][0] -= dt * kf->P[1][1];
    kf->P[1][1] += kf->Q_bias * dt;

    float S = kf->P[0][0] + kf->R_measure;
    float K[2];
    K[0] = kf->P[0][0] / S;
    K[1] = kf->P[1][0] / S;

    float y = newAngle - kf->angle;
    kf->angle += K[0] * y;
    kf->bias += K[1] * y;

    float P00_temp = kf->P[0][0];//更新协方差矩阵
    float P01_temp = kf->P[0][1];

    kf->P[0][0] -= K[0] * P00_temp;
    kf->P[0][1] -= K[0] * P01_temp;
    kf->P[1][0] -= K[1] * P00_temp;
    kf->P[1][1] -= K[1] * P01_temp;

    return kf->angle;
}

//启动运算
void Kalman_Init()
{
	KalmanFilter_Init(&kf_pitch);
    KalmanFilter_Init(&kf_roll);
}

//循环输出数据
void Kalman_Calculate()
{
		MPU6050_GetData(&AccX, &AccY, &AccZ, &GYROX, &GYROY, &GYROZ);
		gyroXrate = GYROX / 131.0;
		gyroYrate = GYROY / 131.0;
		accPitch = atan2f(AccY, AccZ) * 180 / 3.14159265358979323846;
		accRoll = atan2f(AccX, AccZ) * 180 / 3.14159265358979323846;
		pitch = KalmanFilter_Update(&kf_pitch, accPitch, gyroYrate, 0.01);
		roll = KalmanFilter_Update(&kf_roll, accRoll, gyroXrate, 0.01);
}

void Kalman_Yaw()
{
	MPU6050_GetData(&AccX, &AccY, &AccZ, &GYROX, &GYROY, &GYROZ);//单纯角速度积分
	yaw_g=yaw1+(GYROZ/16.384)*0.10+0.2222222;
	yaw1=yaw_g;
	yaw2=(yaw1*1.005+yaw1*0.905)/2;
	if(yaw2<-361)
	{
		yaw1=0;
		yaw2=0;
	}
	if(yaw2>361)
	{
		yaw1=0;
		yaw2=0;
	}
}
