/*
 * OLED_Data.h
 *
 *  Created on: Feb 4, 2024
 *      Author: 25085
 */

#ifndef OLED_DATA_OLED_DATA_H_
#define OLED_DATA_OLED_DATA_H_

#include "stm32f1xx_hal.h"
#include "main.h"

/*
extern const unsigned char F6x8[][6];
extern const unsigned char F8X16[];
extern const unsigned char Hzk[][32];
extern unsigned char BMP1[];
//extern unsigned char BMP2[].........
*/
#include <stdint.h>

/*中文字符字节宽度*/
#define OLED_CHN_CHAR_WIDTH			3		//UTF-8编码格式给3，GB2312编码格式给2

/*字模基本单元*/
typedef struct
{
	char Index[OLED_CHN_CHAR_WIDTH + 1];	//汉字索引
	uint8_t Data[32];						//字模数据
} ChineseCell_t;

/*ASCII字模数据声明*/
extern const uint8_t OLED_F8x16[][16];
extern const uint8_t OLED_F6x8[][6];

/*汉字字模数据声明*/
extern const ChineseCell_t OLED_CF16x16[];

/*图像数据声明*/
extern const uint8_t Diode[];
/*按照上面的格式，在这个位置加入新的图像数据声明*/
//...



#endif /* OLED_DATA_OLED_DATA_H_ */
