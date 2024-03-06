/*
 * Kalman.h
 *
 *  Created on: 2024年2月8日
 *      Author: 25085
 */

#ifndef KALMAN_KALMAN_H_
#define KALMAN_KALMAN_H_

#include "stm32f1xx_hal.h"
#include "main.h"

void Kalman_Init(void);
void Kalman_Calculate(void);
extern float pitch,roll,yaw1,yaw2;

typedef struct {
    float Q_angle;
    float Q_bias;
    float R_measure;
    float angle;//1角速度 2 姿态角
    float bias;//偏移 噪声
    float rate;//加速度
    float P[2][2];//协方差矩阵
} KalmanFilter;

extern int16_t AccX, AccY, AccZ, GYROX, GYROY, GYROZ;
extern float gyroXrate,gyroYrate,gyroZrate,accPitch,accRoll;

void Kalman_Init(void);
void Kalman_Calculate(void);
void KalmanFilter_Init(KalmanFilter *kf);
float KalmanFilter_Update(KalmanFilter *kf, float newAngle, float newRate, float dt);
void Kalman_Yaw(void);


#endif /* KALMAN_KALMAN_H_ */
