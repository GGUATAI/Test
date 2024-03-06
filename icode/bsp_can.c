/*
 * bsp_can.c
 *
 *  Created on: 2024年3月1日
 *      Author: 25085
 */

#include "bsp_can.h"

moto_info_t motor_yaw_info;
uint16_t can_cnt;
uint32_t Mail_Tx = CAN_TX_MAILBOX0;
uint8_t can_1_rx[8];
uint8_t can_1_tx[8];
CAN_RxHeaderTypeDef CAN_1_Rx_Header;    // 接收报文头
CAN_TxHeaderTypeDef CAN_1_Tx_Header;    // 发送报文头

void CAN_Filter_Init(void)
{
    CAN_FilterTypeDef filterconfig;

    filterconfig.FilterActivation     = ENABLE;                // 启用过滤�??
    filterconfig.FilterBank           = 14;                     // 将要初始化的过滤器组
    filterconfig.SlaveStartFilterBank = 14;                     // 从模式下的过滤器�??
    filterconfig.FilterMode           = CAN_FILTERMODE_IDMASK; // 掩码模式
    filterconfig.FilterScale          = CAN_FILTERSCALE_32BIT; // 32位宽
    filterconfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;      // 配置邮箱0
    filterconfig.FilterIdHigh         = 0x0000;                // 高位0
    filterconfig.FilterIdLow          = 0x0000;                // 低位0
    filterconfig.FilterMaskIdHigh     = 0x0000;                // 掩码高位不检
    filterconfig.FilterMaskIdLow      = 0x0000;                // 掩码低位不检
    HAL_CAN_ConfigFilter(&hcan1, &filterconfig);
    HAL_CAN_Start(&hcan1);
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
}

//中断回调
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  if(hcan==&hcan1)
  {
  HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &CAN_1_Rx_Header, can_1_rx); //receive can data
  switch(CAN_1_Rx_Header.StdId)
	{
	  case CAN_6020_M4_ID:
	{
    motor_yaw_info.rotor_angle    = ((can_1_rx[0] << 8) | can_1_rx[1]);
    motor_yaw_info.rotor_speed    = ((can_1_rx[2] << 8) | can_1_rx[3]);
    motor_yaw_info.torque_current = ((can_1_rx[4] << 8) | can_1_rx[5]);
    motor_yaw_info.temp           =   can_1_rx[6];
		break;
	}
	}
  }
}

void set_GM6020_motor_voltage(CAN_HandleTypeDef* hcan,int16_t VSS_ONE)//电压控制
{

  CAN_1_Tx_Header.StdId = 0x1ff;
  CAN_1_Tx_Header.IDE   = CAN_ID_STD;
  CAN_1_Tx_Header.RTR   = CAN_RTR_DATA;
  CAN_1_Tx_Header.DLC   = 8;

  can_1_tx[0] = (VSS_ONE>>8)&0xff;
  can_1_tx[1] = (VSS_ONE)&0xff;

  HAL_CAN_AddTxMessage(&hcan1, &CAN_1_Tx_Header, can_1_tx,(uint32_t*)Mail_Tx);
}
