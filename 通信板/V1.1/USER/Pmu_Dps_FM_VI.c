#include "Pmu_Dps_FM_VI.h"
#include "gpio.h"
#include "adc.h"
#include "dac.h"

//PMU FVMI
//ch ͨ��ch1-8
//fv ʩ�ӵ���ֵ
//pi ���Ƶ�λѡ��
//hl �������
//ll �������
//ir ������λ
//dt ����ǰ��ʱʱ��
void Pmu_FVMI(int ch,float fv,float pi,float hl,float ll,int ir,int dt)//��ѹ�̶�Ϊ6V
{
//�ָ��ϵ�����״̬,�����ͻ
//PD11��14 ����FVMI L
//��piȷ����PD8��9��10����������ѹ��λ1/4 1/2 3/4��ǰ��λ H
//��irȷ����PD2��3��4��5��6���õ�����λ5uA 50 500 5000 50000uA L
//��fv����ȷ����PD15��7 DAC�����������ѡ�� L
//PA4����DAC�����ѹ ������6V
u16 PADC_MI;	
GPIO_PinD_Reset();//����δ����λ�����ϵ�״ֵ̬
	
HAL_GPIO_WritePin(GPIOD,GPIO_PIN_11,GPIO_PIN_RESET);//����Ч
HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_RESET);
	
if(pi==0)
{
//���ϵ�Ĭ��״̬
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
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_RESET);//����Ч
}
else if(fv<0)
{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_RESET);//����Ч
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_SET);
}
DAC1CH1_Set_Vol(fv);
//PB0-8���̵���CH1F-CH8F
//��ʱ0.5ms
//PB8-15���̵���CH1S-CH8S
//��ʱdt ms
PMU_FS_Open(ch);
delay_ms(dt);
//PA6 ADC����оƬ����
PADC_MI=(Get_Adc_Average(ADC1_Handler,3,8)-32767)*ir/32767;
//PB0-15�ؼ̵���CH1F-CH8F��CH1S-CH8S
GPIO_PinB_Reset();	
}

//PMU FIMV����
//ch ͨ��ch1-8
//fi ʩ�ӵ���ֵ
//pv ���Ƶ�λѡ��
//hl �������
//ll �������
//ir ������λ
//dt ����ǰ��ʱʱ��
void Pmu_FIMV(int ch,float fi,float pv,float hl,float ll,int ir,int dt)
{
//�ָ��ϵ�����״̬,�����ͻ
//PD12��13 ����FIMV L
//��pvȷ����PD8��9��10����������ѹ��λ1/4 1/2 3/4��ǰ��λ H
//��irȷ����PD2��3��4��5��6���õ�����λ5uA 50 500 5000 50000uA L
//��fi����ȷ����PD15��7 DAC�����������ѡ�� L
//PA4����DAC�����ѹ ������6V
u16 PADC_MV;	
GPIO_PinD_Reset();//����δ����λ�����ϵ�״ֵ̬
	
HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);//����Ч
HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_RESET);

if(pv==0)
{
//���ϵ�Ĭ��״̬
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
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_RESET);//����Ч
}
else if(fi<0)
{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_RESET);//����Ч
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_SET);
}
DAC1CH1_Set_Vol(fi);
//PB0-8���̵���CH1F-CH8F
//��ʱ0.5ms
//PB8-15���̵���CH1S-CH8S
//��ʱdt ms
PMU_FS_Open(ch);
delay_ms(dt);
//PA6 ADC����оƬ��ѹ
PADC_MV=(Get_Adc_Average(ADC1_Handler,3,8)-32767)*1.5/32767;
//PB0-15�ؼ̵���CH1F-CH8F��CH1S-CH8S
GPIO_PinB_Reset();	
}

void Pmu_Measure(u16 set_I,u16 ch,u16 restrit,u16 cu,u8 vol)
{
//PB0-8���̵���CH1F-CH8F
//��ʱ0.5ms
//PB8-15���̵���CH1S-CH8S
//��ʱ2ms

//PA6 ADC����оƬ����
	
//PB0-15�ؼ̵���CH1F-CH8F��CH1S-CH8S	
}

//DPS FV
void Dps_FV(int ch,float fv,float pi,float hl,float ll,int ir,int dt)
{
//PE9��8 DAC�����������ѡ�� L
//PE0��1��2����������ѹ��λ1/4 1/2 3/4��ǰ��λ H
//PE3��4��5��6��7���õ�����λ5uA 50 500 5000 50000uA L	
//PA5 DAC�����ѹ CH2
	GPIO_PinE_Reset();

	if(fv>=0)
	{
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_RESET);//����Ч
	}
	else if(fv<0)
	{
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_RESET);//����Ч
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_SET);
	}
	if(pi==0)
	{
	 //���ϵ�Ĭ��״̬
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
//PE3��4��5��6��7���ò���������λ5uA 50 500 5000 50000uA L
//PC4 ADC����оƬ���� ADC12_14
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

//����DFORCE��DSENCE
//sw 1:�� 0����
void SetDpsR(u8 sw)
{
	if(sw==1)
	{
	//PE11 DFORCE��
	//��ʱ0.5ms
	//PE12 DSENCE��
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_RESET);
		delay_ms(1);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12,GPIO_PIN_RESET);

	}
	else if(sw==0)
	{
   //DFORCE��DSENCE�ر�
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12,GPIO_PIN_SET);
	}
	
}

