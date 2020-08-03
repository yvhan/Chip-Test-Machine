#ifndef __DAC_H
#define __DAC_H
#include "sys.h"

extern DAC_HandleTypeDef DAC1_Handler;//DAC¾ä±ú

void DAC1_Init(void);
void DAC1CH1_Set_Vol(float vol);
void DAC1CH1_Set_Cur(float cur);
void DAC1CH2_Set_Vol(float vol);
#endif
