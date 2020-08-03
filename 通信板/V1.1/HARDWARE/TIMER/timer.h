#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"

extern TIM_HandleTypeDef TIM3_Handler;      //��ʱ��3PWM��� 
extern TIM_OC_InitTypeDef TIM3_CH4Handler; 	//��ʱ��3ͨ��4���

void TIM3_Init(u16 arr,u16 psc);    		//��ʱ����ʼ��
void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM_SetTIM3Compare4(u32 compare);
u32 TIM_GetTIM3Capture4(void);


void TIM1_Cap_Init(u16 arr,u16 psc,u8 ch);
void TIM8_Cap_Init(u16 arr,u16 psc,u8 ch);

void TIM1_Cap_DeInit(void);
void TIM8_Cap_DeInit(void);

#endif

