/*
 * AnalogI2C.h
 *
 *  Created on: 2024年2月5日
 *      Author: 25085
 */

#ifndef ANALOGI2C_ANALOGI2C_H_
#define ANALOGI2C_ANALOGI2C_H_

#include "stm32f1xx_hal.h"
#include "main.h"

void MyI2C_Init(void);
void MyI2C_Start(void);
void MyI2C_Stop(void);
void MyI2C_SendByte(uint8_t Byte);
uint8_t MyI2C_ReceiveByte(void);
void MyI2C_SendAck(uint8_t AckBit);
uint8_t MyI2C_ReceiveAck(void);

#endif /* ANALOGI2C_ANALOGI2C_H_ */
