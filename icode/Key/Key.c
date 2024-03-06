/*
 * Key.c
 *
 *  Created on: Feb 3, 2024
 *      Author: 25085
 */

#include "Key.h"
#include "gpio.h"

uint8_t Key_Get(void)
{
	uint8_t KeyNumber=0;

	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12)==GPIO_PIN_RESET){HAL_Delay(20);while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12)==GPIO_PIN_RESET);HAL_Delay(20);KeyNumber=1;};
//	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_13)==GPIO_PIN_RESET){HAL_Delay(20);while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_13)==GPIO_PIN_RESET);HAL_Delay(20);KeyNumber=2;};
//	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_14)==GPIO_PIN_RESET){HAL_Delay(20);while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_14)==GPIO_PIN_RESET);HAL_Delay(20);KeyNumber=3;};
//	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15)==GPIO_PIN_RESET){HAL_Delay(20);while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15)==GPIO_PIN_RESET);HAL_Delay(20);KeyNumber=4;};

	return KeyNumber;
}
