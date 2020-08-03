#include "Pmu_Dps_FM_VI.h"
#include "gpio.h"
#include "adc.h"
#include "dac.h"
#include "main.h"
#include "usart.h" 

//PMU FVMI
//ch 继电器通道
//fv 施加电压值
//pi 限制档位选择
//hl 检测上限
//ll 检测下限
//ir 电流档位
//dt 测试前延时时间
void Pmu_FV(float fv,float pi,int ir)//电压固定为6V
{
//恢复上电引脚状态,避免冲突
//PD11、14 进入FVMI L
//由pi确定，PD8、9、10设置限流限压档位1/4 1/2 3/4当前档位 H
//由ir确定，PD2、3、4、5、6设置电流档位5uA 50 500 5000 50000uA L
//由fv正负确定，PD15、7 DAC输出正负极性选择 L
//PA4设置DAC输出电压 满量程6V

GPIO_PinD_Reset();//避免未设置位不是上电状态值
	
HAL_GPIO_WritePin(GPIOD,GPIO_PIN_11,GPIO_PIN_RESET);//低有效
HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_RESET);
	
if(pi==0)
{
//即上电默认状态
}
else if(pi==0.25)
{
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_8,GPIO_PIN_SET);
}
else if(pi==0.5)
{
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,GPIO_PIN_SET);
}
else if(pi==0.75)
{
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,GPIO_PIN_SET);
}	

switch(ir)
{
	case 5: HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);break;
	case 50: HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_RESET);break;
  case 500: HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_RESET);break;
  case 5000: HAL_GPIO_WritePin(GPIOD,GPIO_PIN_5,GPIO_PIN_RESET);break;
  case 50000: HAL_GPIO_WritePin(GPIOD,GPIO_PIN_6,GPIO_PIN_RESET);break;	
}
delay_ms(2);
if(fv>=0)
{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_RESET);//低有效
}
else if(fv<0)
{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_RESET);//低有效
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_SET);
	fv=-fv;
}
delay_ms(2);
DAC1CH1_Set_Vol(fv);
	
}

void Pmu_FV_Set(float fv,float pi,int ir)//电压固定为6V
{
//恢复上电引脚状态,避免冲突
//PD11、14 进入FVMI L
//由pi确定，PD8、9、10设置限流限压档位1/4 1/2 3/4当前档位 H
//由ir确定，PD2、3、4、5、6设置电流档位5uA 50 500 5000 50000uA L
//由fv正负确定，PD15、7 DAC输出正负极性选择 L
//PA4设置DAC输出电压 满量程6V

	GPIO_PinD_Reset();//避免未设置位不是上电状态值
		
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_11,GPIO_PIN_RESET);//低有效
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_RESET);
		
	if(pi==0)
	{
	//即上电默认状态
	}
	else if(pi==0.25)
	{
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_8,GPIO_PIN_SET);
	}
	else if(pi==0.5)
	{
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,GPIO_PIN_SET);
	}
	else if(pi==0.75)
	{
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,GPIO_PIN_SET);
	}	

	switch(ir)
	{
		case 5: HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);break;
		case 50: HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_RESET);break;
		case 500: HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_RESET);break;
		case 5000: HAL_GPIO_WritePin(GPIOD,GPIO_PIN_5,GPIO_PIN_RESET);break;
		case 50000: HAL_GPIO_WritePin(GPIOD,GPIO_PIN_6,GPIO_PIN_RESET);break;	
	}

	if(fv>=0)
	{
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_RESET);//低有效
	}
	else if(fv<0)
	{
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_RESET);//低有效
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_SET);
		fv=-fv;
	}	
}
void Pull_Set(float fv,float pi,int ir,int ch,u8 sw)
{
	if(sw)
	{
		Pmu_FV_Set(fv,pi,ir);
		if(ch&0x01)//上拉端口，开SENCE 
		{
			PullUp(1,1);
		}
		if(ch&0x02)
		{
			PullUp(2,1);
		}
		if(ch&0x04)
		{
			PullUp(3,1);
		}
		if(ch&0x08)
		{
			PullUp(4,1);
		}
		if(ch&0x10)
		{
			PullUp(5,1);
		}
		if(ch&0x20)
		{
			PullUp(6,1);
		}
		if(ch&0x40)
		{
			PullUp(7,1);
		}
		if(ch&0x80)
		{
			PullUp(8,1);
		}
		
		DAC1CH1_Set_Vol(fv);
	}
	else
	{
		Pmu_FV_Set(0,pi,ir);
		if(ch&0x01)//上拉端口，开SENCE 
		{
			PullUp(1,0);
		}
		if(ch&0x02)
		{
			PullUp(2,0);
		}
		if(ch&0x04)
		{
			PullUp(3,0);
		}
		if(ch&0x08)
		{
			PullUp(4,0);
		}
		if(ch&0x10)
		{
			PullUp(5,0);
		}
		if(ch&0x20)
		{
			PullUp(6,0);
		}
		if(ch&0x40)
		{
			PullUp(7,0);
		}
		if(ch&0x80)
		{
			PullUp(8,0);
		}
		
		DAC1CH1_Set_Vol(0);
	}
}

void Pull_UnSet(float fv,float pi,int ir,int ch)
{
	Pmu_FV_Set(fv,pi,ir);
	if(ch&0x01)//上拉端口，开SENCE 
	{
		PullUp(1,0);
	}
	if(ch&0x02)
	{
		PullUp(2,0);
	}
	if(ch&0x04)
	{
		PullUp(3,0);
	}
	if(ch&0x08)
	{
		PullUp(4,0);
	}
	if(ch&0x10)
	{
		PullUp(5,0);
	}
	if(ch&0x20)
	{
		PullUp(6,0);
	}
	if(ch&0x40)
	{
		PullUp(7,0);
	}
	if(ch&0x80)
	{
		PullUp(8,0);
	}
	DAC1CH1_Set_Vol(fv);
}
void Pmu_MI(int ch,int ir,float hl,float ll,int dt,int bin)
{
	//PB0-8开继电器CH1F-CH8F
	//延时0.5ms
	//PB8-15开继电器CH1S-CH8S
	//延时dt ms
	int i;
	float PADC_MI;	
	PMU_FS(ch,1);
	delay_ms(dt);
	//PA6 ADC测量芯片电流
	PADC_MI=(float)(Get_Adc_Average(ADC1_Handler,ADC_CHANNEL_3,8)-32767)*(float)ir/32767;
	USART2_TX_BUF[data_cnt++]=PADC_MI;//保存测试数据
	//PB0-15关继电器CH1F-CH8F和CH1S-CH8S
	PMU_FS(ch,0);
	if((PADC_MI>hl)||(PADC_MI<ll))
	{
		SetBin(bin);
		//HAL_GPIO_WritePin(GPIOE,GPIO_PIN_1,GPIO_PIN_SET);
	
	//for(i=0;i<1000;i++)
	//{}
	//HAL_GPIO_WritePin(GPIOE,GPIO_PIN_1,GPIO_PIN_RESET);
	}
}
//PMU FIMV功能
//ch 继电器通道
//fi 施加电流值
//pv 限制档位选择
//hl 检测上限
//ll 检测下限
//ir 电流档位
//dt 测试前延时时间
void Pmu_FI(float fi,float pv,int ir)
{
//恢复上电引脚状态,避免冲突
//PD12、13 进入FIMV L
//由pv确定，PD8、9、10设置限流限压档位1/4 1/2 3/4当前档位 H
//由ir确定，PD2、3、4、5、6设置电流档位5uA 50 500 5000 50000uA L
//由fi正负确定，PD15、7 DAC输出正负极性选择 L
//PA4设置DAC输出电压 满量程6V
	
GPIO_PinD_Reset();//避免未设置位不是上电状态值
	
HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);//低有效
HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_RESET);

if(pv==0)
{
//即上电默认状态
}
else if(pv==0.25)
{
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_8,GPIO_PIN_SET);
}
else if(pv==0.5)
{
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,GPIO_PIN_SET);
}
else if(pv==0.75)
{
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,GPIO_PIN_SET);
}	

switch(ir)
{
	case 5: HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);break;
	case 50: HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_RESET);break;
  case 500: HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_RESET);break;
  case 5000: HAL_GPIO_WritePin(GPIOD,GPIO_PIN_5,GPIO_PIN_RESET);break;
  case 50000: HAL_GPIO_WritePin(GPIOD,GPIO_PIN_6,GPIO_PIN_RESET);break;	
	
}
delay_ms(2);
if(fi>=0)
{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_RESET);//低有效
}
else if(fi<0)
{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_RESET);//低有效
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_SET);
	fi=-fi;
} 
DAC1CH1_Set_Cur(fi/(float)ir);
	
}

void Pmu_MV(int ch,float hl,float ll,int dt,int bin)
{
//PB0-8开继电器CH1F-CH8F
//延时0.5ms
//PB8-15开继电器CH1S-CH8S
//延时dt ms
	float PADC_MV;
	PMU_FS(ch,1);
	delay_ms(dt);
	//PA6 ADC测量芯片电压
	PADC_MV=(float)(Get_Adc_Average(ADC1_Handler,ADC_CHANNEL_3,8)-32767)*6/32767;
	USART2_TX_BUF[data_cnt++]=PADC_MV;//保存测试数据
	//PB0-15关继电器CH1F-CH8F和CH1S-CH8S
	PMU_FS(ch,0);
	if((PADC_MV>hl)||(PADC_MV<ll))
	{
		SetBin(bin);
	}
}

//DPS FV
void Dps_FV(float fv,float pi,int ir)
{
//PE9、8 DAC输出正负极性选择 L
//PE0、1、2设置限流限压档位1/4 1/2 3/4当前档位 H
//PE3、4、5、6、7设置电流档位5uA 50 500 5000 50000uA L	
//PA5 DAC输出电压 CH2
//	GPIO_PinE_Reset();
//HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_SET);
//		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12,GPIO_PIN_SET);
	if(fv>=0)
	{
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_RESET);//低有效
	}
	else if(fv<0)
	{
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_RESET);//低有效
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_SET);
	}
	if(pi==0)
	{
	  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_0,GPIO_PIN_RESET);	//PE0置0
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_1,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_RESET);//即上电默认状态
	}
	else if(pi==0.25)
	{
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_0,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_1,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_RESET);
	}
	else if(pi==0.5)
	{
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_0,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_1,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_RESET);
	}
	else if(pi==0.75)
	{
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_0,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_1,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_SET);
	}	

	switch(ir)
	{
		case 5: HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_SET);
						HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET);
						HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_SET);
						HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,GPIO_PIN_SET);break;
		case 50: HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_SET);
							HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_RESET);
							HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET);
							HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_SET);
							HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,GPIO_PIN_SET);break;
		case 500: HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_SET);
							HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_SET);
							HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_RESET);
							HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_SET);
							HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,GPIO_PIN_SET);break;
		case 5000: HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_SET);
								HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_SET);
								HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET);
								HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_RESET);
								HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,GPIO_PIN_SET);break;
		case 50000: HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_SET);
								HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_SET);
								HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET);
								HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_SET);
								HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,GPIO_PIN_RESET);break;
	}
	delay_ms(5);
	DAC1CH2_Set_Vol(fv);
}
	
//DPS MI
void Dps_MI(int ir,float hl,float ll,int dt,int bin)
{
  float DADC_MV;
//PE3、4、5、6、7设置测量电流档位5uA 50 500 5000 50000uA L
//PC4 ADC测量芯片电流 ADC12_4
	switch(ir)
	{
		case 5: HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_SET);
						HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET);
						HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_SET);
						HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,GPIO_PIN_SET);break;
		case 50: HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_SET);
						 HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_RESET);
						 HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET);
						 HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_SET);
						 HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,GPIO_PIN_SET);break;
		case 500: HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_SET);
							HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_SET);
							HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_RESET);
							HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_SET);
							HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,GPIO_PIN_SET);break;
		case 5000: HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_SET);
							 HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_SET);
							 HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET);
							 HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_RESET);
							 HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,GPIO_PIN_SET);break;
		case 50000: HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_SET);
								HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_SET);
								HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET);
								HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_SET);
								HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,GPIO_PIN_RESET);break;	
								
	}
	delay_ms(5);
  DADC_MV=(float)(Get_Adc_Average(ADC1_Handler,ADC_CHANNEL_4,8)-32767)*ir/32767;
	USART2_TX_BUF[data_cnt++]=DADC_MV;//保存测试数据
  if((DADC_MV>hl)||(DADC_MV<ll))
	{
		SetBin(bin);
	}	
}

//开关DFORCE和DSENCE
//sw 1:开 0：关
void SetDpsR(u8 sw)
{
	if(sw==1)
	{
	//PE11 DFORCE开
	//延时0.5ms
	//PE12 DSENCE开
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_RESET);
		delay_ms(1);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12,GPIO_PIN_RESET);
		delay_ms(1);
	}
	else if(sw==0)
	{
   //DFORCE和DSENCE关闭
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12,GPIO_PIN_SET);
		
	}	
}
