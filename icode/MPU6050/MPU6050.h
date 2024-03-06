/*
 * MPU6050.h
 *
 *  Created on: 2024年2月5日
 *      Author: 25085
 */

#ifndef MPU6050_MPU6050_H_
#define MPU6050_MPU6050_H_

#include "stm32f1xx_hal.h"
#include "main.h"

#define MPU_INT_EN_REG 0X38
#define MPU_USER_CTRL_REG 0X6A
#define MPU_FIFO_EN_REG 0X23
#define MPU_INTBP_CFG_REG 0X37

// if AD0->GND,MPU_ADDR=0x68.else 0x69
#define MPU6050_ADDRESS		0xD0

uint8_t MPU6050_Init(void);
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data);
uint8_t MPU6050_ReadReg(uint8_t RegAddress);
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ,
		               int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ);
uint8_t MPU6050_GetID(void);

#endif /* MPU6050_MPU6050_H_ */
