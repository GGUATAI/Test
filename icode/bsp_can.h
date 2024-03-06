/*
 * bsp_can.h
 *
 *  Created on: 2024年3月1日
 *      Author: 25085
 */

#ifndef BSP_CAN_H_
#define BSP_CAN_H_

#include "stm32f4xx_hal.h"
#include "main.h"
#include "can.h"

#define CAN_6020_M1_ID 0x205
#define CAN_6020_M2_ID 0x206
#define CAN_6020_M3_ID 0x207
#define CAN_6020_M4_ID 0x208

extern uint8_t can_1_rx[8];    // 接收数据
extern uint8_t can_1_tx[8];    // 发�?�数�??

extern CAN_RxHeaderTypeDef CAN_1_Rx_Header;    // 接收报文�??
extern CAN_TxHeaderTypeDef CAN_1_Tx_Header;    // 发�?�报文头

extern uint32_t Mail_Tx;     // 发�?�邮箱编�??

typedef struct
{
    uint16_t can_id;//电机ID
    int16_t  set_voltage;//设定的电压值
    uint16_t rotor_angle;//机械角度
    int16_t  rotor_speed;//转速
    int16_t  torque_current;//扭矩电流
    uint8_t  temp;//温度
}moto_info_t;

void CAN_Filter_Init(void);
void set_GM6020_motor_voltage(CAN_HandleTypeDef* hcan,int16_t v1);

#endif
/* BSP_CAN_H_ */
