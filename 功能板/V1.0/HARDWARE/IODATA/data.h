#ifndef _IODATA_H
#define _IODATA_H
#include "sys.h"

//LED�˿ڶ���
//#define LED0(n)		(n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET))
//#define LED0_Toggle (HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1)) //LED0�����ƽ��ת
//#define LED1(n)		(n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET))
//#define LED1_Toggle (HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0)) //LED1�����ƽ��ת

void IO_Data(u8 ch,u8 *data,int ONE_HT,int ONE_LT,int ZERO_HT,int ZERO_LT);


#endif
