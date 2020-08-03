#ifndef __PMU_DPS_H
#define __PMU_DPS_H
#include "sys.h"


void Pmu_FVMI(int ch,float fv,float pi,float hl,float ll,int ir,int dt);
void Pmu_FIMV(int ch,float fi,float pv,float hl,float ll,int ir,int dt);
void Dps_FV(int ch,float fv,float pi,float hl,float ll,int ir,int dt);
void Dps_MI(int ir);
void SetDpsR(u8 sw);















#endif
