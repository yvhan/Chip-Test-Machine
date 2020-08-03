#ifndef _GPIO_H
#define _GPIO_H
#include "sys.h"

#define START       HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_9)  //��ʼ�ź�

extern u8 Bad_flag;

void GPIO_ALL_Init(void);  //IO��ʼ������
void GPIO_Init_ExcBin(void);//��ʼ����BIN��EOT�����������
void BinEOT_Init(void);//��ʼ��BIN��EOT����

void GPIO_PinA_Reset(void);
void GPIO_PinB_Reset(void);//���Żָ��ϵ�״̬
void GPIO_PinC_Reset(void);
void GPIO_PinD_Reset(void);
void GPIO_PinE_Reset(void);
void GPIO_PinF_Reset(void);
void GPIO_PinG_Reset(void);
void GPIO_PinI_Reset(void);

void PAInit_DeTIM1(void);
void PCInit_DeTIM1(void);

void PMU_FS(int ch,u8 sw);
void SetDutR(u8 ch, u8 sw);
void Direct_MCUtoCH(int ch,u8 sw);//��:CHtoMCU �ߣ�MCUtoCH
void Fix(u8 cfn,u8 vol);
void PullUp(int ch,u8 sw);
u8 KEY_Scan(u8 mode); //����ɨ�躯��
#endif
