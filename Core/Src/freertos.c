/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Retarget.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "usart.h"
#include "dma.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
osThreadId USART_TaskHandle;
osThreadId OLED_MPU6050_TaskHandle;
osThreadId Key_Printf_TaskHandle;
osThreadId Dead_TaskHandle;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define __HAL_DMA_GET_COUNTER(__HANDLE__) ((__HANDLE__)->Instance->CNDTR)
#define USART_DMA_RX_BUFFER_MAXIMUM 128
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
double_t AX, AY, AZ, GX, GY, GZ;
uint8_t receiveData[USART_DMA_RX_BUFFER_MAXIMUM];
uint8_t Data[USART_DMA_RX_BUFFER_MAXIMUM];
uint8_t Usart1_Rx_Len;
uint8_t n=0;
uint8_t Num=0;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId StartTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void Start_StartTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of StartTask */
  osThreadDef(StartTask, Start_StartTask, osPriorityRealtime, 0, 128);
  StartTaskHandle = osThreadCreate(osThread(StartTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_Start_StartTask */
void Start_USART_Task(void const * argument);
void Start_OLED_MPU6050_Task(void const * argument);
void Start_Key_Printf_Task(void const * argument);
void Start_Dead_Task(void const * argument);
/**
* @brief Function implementing the StartTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_StartTask */
void Start_StartTask(void const * argument)
{
  /* USER CODE BEGIN Start_StartTask */
  /* Infinite loop */
	taskENTER_CRITICAL();
	  HAL_UARTEx_ReceiveToIdle_DMA(&huart1, receiveData, sizeof(receiveData));
	   __HAL_DMA_DISABLE_IT(&hdma_usart1_rx,DMA_IT_HT);

	 osThreadDef(USART_Task, Start_USART_Task, osPriorityRealtime, 0, 128);
	 USART_TaskHandle = osThreadCreate(osThread(USART_Task), NULL);

	 osThreadDef(OLED_MPU6050_Task, Start_OLED_MPU6050_Task, osPriorityRealtime, 0, 128);
	 OLED_MPU6050_TaskHandle = osThreadCreate(osThread(OLED_MPU6050_Task), NULL);

	 osThreadDef(Key_Printf_Task, Start_Key_Printf_Task, osPriorityNormal, 0, 128);
	 Key_Printf_TaskHandle = osThreadCreate(osThread(Key_Printf_Task), NULL);

	 osThreadDef(Dead_Task, Start_Dead_Task, osPriorityLow, 0, 128);
	 Dead_TaskHandle = osThreadCreate(osThread(Dead_Task), NULL);

	 osThreadTerminate(StartTaskHandle);

	taskEXIT_CRITICAL();
  /* USER CODE END Start_StartTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

void Start_USART_Task(void const * argument)
{
  /* USER CODE BEGIN Start_USART_Task */
	uint32_t PreviousWakeTime=osKernelSysTick();
	  HAL_UARTEx_ReceiveToIdle_DMA(&huart1, receiveData, sizeof(receiveData));
	   __HAL_DMA_DISABLE_IT(&hdma_usart1_rx,DMA_IT_HT);
  /* Infinite loop */
	while(1)
	{
		osDelayUntil(&PreviousWakeTime,15);
		if(n==1)
		{
		printf(" %d",Usart1_Rx_Len);
		n=0;
		}
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);
	}
  /* USER CODE END Start_USART_Task */
}

#include "../../icode/OLED/OLED.h"
#include "../../icode/MDKDelay/MDKDelay.h"
#include "../../icode/MPU6050/MPU6050.h"
#include "../../icode/Kalman/Kalman.h"
void Start_OLED_MPU6050_Task(void const * argument)
{
  /* USER CODE BEGIN Start_OLED_MPU6050_Task */
	 uint32_t PreviousWakeTime=osKernelSysTick();
	 OLED_Init();
	 OLED_Clear();
	 MPU6050_Init();
	 Kalman_Init();
  /* Infinite loop */
	while(1)
	{
		 osDelayUntil(&PreviousWakeTime,10);
		 Kalman_Calculate();
		 Kalman_Yaw();
		 OLED_ShowSignedNum(0,0,pitch,4,OLED_8X16);
		 OLED_ShowSignedNum(0,16,roll,4,OLED_8X16);
		 OLED_ShowSignedNum(0,32,yaw2,4,OLED_8X16);
		 AX=((AccX/2048.0)*9.80665);
		 AY=((AccY/2048.0)*9.80665);
		 AZ=((AccZ/2048.0)*9.80665-1.2);
		 OLED_ShowFloatNum(48,0,AX,3,2,OLED_8X16);
	     OLED_ShowFloatNum(48,16,AY,3,2,OLED_8X16);
		 OLED_ShowFloatNum(48,32,AZ,3,2,OLED_8X16);
		 OLED_ShowSignedNum(48,48,Usart1_Rx_Len,4,OLED_8X16);
		 OLED_Update();
	}
  /* USER CODE END Start_OLED_MPU6050_Task */
}

void Start_Key_Printf_Task(void const * argument)
{
  /* USER CODE BEGIN Start_Key_Printf_Task */
  /* Infinite loop */
	while(1)
	{
		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12)==0)
		{
			osDelay(20);
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12)==0)
			{
			if(Num<20)
			{
				xTaskNotifyGive(Dead_TaskHandle);
				Num++;
				HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_3);
				printf("累计发弹量：%d\r\n",Num);
			}
			else
			{
				printf("弹夹已空#\r\n");
			}
			}
			while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12)==0);
		}
	}
  /* USER CODE END Start_Key_Printf_Task */
}

void Start_Dead_Task(void const * argument)
{
  /* USER CODE BEGIN  Start_Dead_Task */
  /* Infinite loop */
	while(1)
	{
		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15)==0)
		{
			osDelay(20);
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15)==0)
			{
			if(ulTaskNotifyTake(pdFALSE, 0)>0)
			{
				Num--;
				HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2);
				printf("当前不可发弹量：%d\r\n",Num);
			}
			else
			{
				printf("扣血#\r\n");
			}
			}
			while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15)==0);
		}
	}
  /* USER CODE END  Start_Dead_Task */
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
