/*
 * USART_Printf.h
 *
 *  Created on: 2024年2月4日
 *      Author: 25085
 */

#ifndef USART_PRINTF_USART_PRINTF_H_
#define USART_PRINTF_USART_PRINTF_H_

#include "stm32f1xx_hal.h"
#include "main.h"


void UART_Debug_Printf(const char* format, ...);
void UART_Debug_Remap(UART_HandleTypeDef *huart);
void UART_Debug_HAL_Status(HAL_StatusTypeDef status);


#endif /* USART_PRINTF_USART_PRINTF_H_ */
