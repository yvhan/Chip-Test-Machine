#ifndef __ADC_H
#define __ADC_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32H7������
//ADC��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/8/13
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

extern ADC_HandleTypeDef ADC1_Handler;//ADC���

void ADC1_Init(void); 				//ADCͨ����ʼ��
void MY_ADC3_Init(void);
u16 Get_Adc(ADC_HandleTypeDef ADCx,u32 ch);	//���ĳ��ͨ��ֵ 
u16 Get_Adc_Average(ADC_HandleTypeDef ADCx,u32 ch,u8 times);//�õ�ĳ��ͨ����������������ƽ��ֵ
short Get_Temprate(void);
#endif 
