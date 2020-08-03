#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"

extern TIM_HandleTypeDef TIM3_Handler;      //��ʱ��3PWM��� 
extern TIM_OC_InitTypeDef TIM3_CH4Handler; 	//��ʱ��3ͨ��4���
extern TIM_HandleTypeDef TIM8_Handler;
void TIM3_Init(u16 arr,u16 psc);    		//��ʱ����ʼ��
void TIM8CH1_PWM_Init(u16 arr,u16 psc);
void TIM8CH1_PWM(u16 arr,u16 psc,u32 compare);
void TIM_SetTIM3Compare4(u32 compare);
u32 TIM_GetTIM3Capture4(void);


void TIM1_Cap_Init(u16 arr,u16 psc,u8 ch);
void TIM8_Cap_Init(u16 arr,u16 psc,u8 ch);

void TIM1_Cap_DeInit(void);
void TIM8_Cap_DeInit(void);

extern u8  TIM1CH1_CAPTURE_STA;		//���벶��״̬		    				
extern u16	TIM1CH1_CAPTURE_VAL[8];	//���벶��ֵ
extern u16 TIM1CH1_CAPTURE_PerioVAL[8];//���벶������ֵ

extern u8  TIM1CH2_CAPTURE_STA;		//���벶��״̬		    				
extern u16	TIM1CH2_CAPTURE_VAL;	//���벶��ֵ
extern u16 TIM1CH2_CAPTURE_PerioVAL;//���벶������ֵ

extern u8  TIM1CH3_CAPTURE_STA;		//���벶��״̬		    				
extern u16	TIM1CH3_CAPTURE_VAL;	//���벶��ֵ
extern u16 TIM1CH3_CAPTURE_PerioVAL;//���벶������ֵ

extern u8  TIM1CH4_CAPTURE_STA;		//���벶��״̬		    				
extern u16	TIM1CH4_CAPTURE_VAL;	//���벶��ֵ
extern u16 TIM1CH4_CAPTURE_PerioVAL;//���벶������ֵ

extern u8  TIM8CH1_CAPTURE_STA;		//���벶��״̬		    				
extern u16 TIM8CH1_CAPTURE_VAL;	//���벶��ֵ 
extern u16 TIM8CH1_CAPTURE_PerioVAL;//���벶������ֵ

extern u8  TIM8CH2_CAPTURE_STA;	//���벶��״̬		    				
extern u16	TIM8CH2_CAPTURE_VAL;	//���벶��ֵ
extern u16 TIM8CH2_CAPTURE_PerioVAL;//���벶������ֵ

extern u8  TIM8CH3_CAPTURE_STA;	//���벶��״̬		    				
extern u16	TIM8CH3_CAPTURE_VAL;	//���벶��ֵ
extern u16 TIM8CH3_CAPTURE_PerioVAL;//���벶������ֵ

extern u8  TIM8CH4_CAPTURE_STA;	//���벶��״̬		    				
extern u16	TIM8CH4_CAPTURE_VAL;	//���벶��
extern u16 TIM8CH4_CAPTURE_PerioVAL;//���벶������ֵ

#endif

