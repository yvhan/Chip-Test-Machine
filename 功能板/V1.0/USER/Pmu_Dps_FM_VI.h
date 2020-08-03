#ifndef __PMU_DPS_H
#define __PMU_DPS_H
#include "sys.h"


void Pmu_FV(float fv,float pi,int ir);
void Pmu_MI(int ch,int ir,float hl,float ll,int dt,int bin);
void Pull_Set(float fv,float pi,int ir,int ch,u8 sw);
	
void Pmu_FI(float fi,float pv,int ir);
void Pmu_MV(int ch,float hl,float ll,int dt,int bin);

void Dps_FV(float fv,float pi,int ir);
void Dps_MI(int ir,float hl,float ll,int dt,int bin);
void SetDpsR(u8 sw);















#endif
