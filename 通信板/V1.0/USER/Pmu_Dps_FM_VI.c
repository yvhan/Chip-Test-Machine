#include "Pmu_Dps_FM_VI.h"
#include "gpio.h"
#include "adc.h"
#include "dac.h"

//PMU FVMI
//ch 通道ch1-8
//fv 施加电流值
//pi 限制档位选择
//hl 检测上限
//ll 检测下限
//ir 电流档位
//dt 测试前延时时间
void Pmu_FVMI(int ch,float fv,float pi,float hl,float ll,int ir,int dt)//电压固定为6V
{
//恢复上电引脚状态,避免冲突
//PD11、14 进入FVMI L
//由pi确定，PD8、9、10设置限流限压档位1/4 1/2 3/4当前档位 H
//由ir确定，PD2、3、4、5、6设置电流档位5uA 50 500 5000 50000uA L
//由fv正负确定，PD15、7 DAC输出正负极性选择 L
//PA4设置DAC输出电压 满量程6V
u16 PADC_MI;	
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
}
DAC1CH1_Set_Vol(fv);
//PB0-8开继电器CH1F-CH8F
//延时0.5ms
//PB8-15开继电器CH1S-CH8S
//延时dt ms
PMU_FS_Open(ch);
delay_ms(dt);
//PA6 ADC测量芯片电流
PADC_MI=(Get_Adc_Average(ADC1_Handler,3,8)-32767)*ir/32767;
//PB0-15关继电器CH1F-CH8F和CH1S-CH8S
GPIO_PinB_Reset();	
}

//PMU FIMV功能
//ch 通道ch1-8
//fi 施加电流值
//pv 限制档位选择
//hl 检测上限
//ll 检测下限
//ir 电流档位
//dt 测试前延时时间
void Pmu_FIMV(int ch,float fi,float pv,float hl,float ll,int ir,int dt)
{
//恢复上电引脚状态,避免冲突
//PD12、13 进入FIMV L
//由pv确定，PD8、9、10设置限流限压档位1/4 1/2 3/4当前档位 H
//由ir确定，PD2、3、4、5、6设置电流档位5uA 50 500 5000 50000uA L
//由fi正负确定，PD15、7 DAC输出正负极性选择 L
//PA4设置DAC输出电压 满量程6V
u16 PADC_MV;	
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

if(fi>=0)
{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_RESET);//低有效
}
else if(fi<0)
{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_RESET);//低有效
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_SET);
}
DAC1CH1_Set_Vol(fi);
//PB0-8开继电器CH1F-CH8F
//延时0.5ms
//PB8-15开继电器CH1S-CH8S
//延时dt ms
PMU_FS_Open(ch);
delay_ms(dt);
//PA6 ADC测量芯片电压
PADC_MV=(Get_Adc_Average(ADC1_Handler,3,8)-32767)*1.5/32767;
//PB0-15关继电器CH1F-CH8F和CH1S-CH8S
GPIO_PinB_Reset();	
}

void Pmu_Measure(u16 set_I,u16 ch,u16 restrit,u16 cu,u8 vol)
{
//PB0-8开继电器CH1F-CH8F
//延时0.5ms
//PB8-15开继电器CH1S-CH8S
//延时2ms

//PA6 ADC测量芯片电流
	
//PB0-15关继电器CH1F-CH8F和CH1S-CH8S	
}

//DPS FV
void Dps_FV(int ch,float fv,float pi,float hl,float ll,int ir,int dt)
{
//PE9、8 DAC输出正负极性选择 L
//PE0、1、2设置限流限压档位1/4 1/2 3/4当前档位 H
//PE3、4、5、6、7设置电流档位5uA 50 500 5000 50000uA L	
//PA5 DAC输出电压 CH2
	GPIO_PinE_Reset();

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
	 //即上电默认状态
	}
	else if(pi==0.25)
	{
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_0,GPIO_PIN_SET);
	}
	else if(pi==0.5)
	{
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_1,GPIO_PIN_SET);
	}
	else if(pi==0.75)
	{
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_SET);
	}	

	switch(ir)
	{
		case 5: HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_RESET);break;
		case 50: HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_RESET);break;
		case 500: HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_RESET);break;
		case 5000: HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_RESET);break;
		case 50000: HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,GPIO_PIN_RESET);break;	
	}
	DAC1CH2_Set_Vol(fv);
}
	
//DPS MI
void Dps_MI(int ir)
{
  u16 DADC_MV;
//PE3、4、5、6、7设置测量电流档位5uA 50 500 5000 50000uA L
//PC4 ADC测量芯片电流 ADC12_14
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
  DADC_MV=(Get_Adc_Average(ADC1_Handler,14,8)-32767)*ir/32767;	
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

	}
	else if(sw==0)
	{
   //DFORCE和DSENCE关闭
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12,GPIO_PIN_SET);
	}
	
}

