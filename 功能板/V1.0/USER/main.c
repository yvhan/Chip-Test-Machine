#include "sys.h"
#include "main.h"
#include "delay.h"
#include "usart.h" 
#include "data.h"
#include "gpio.h"
#include "timer.h"
#include "adc.h"
#include "dac.h"
#include "iwdg.h"
#include "PeriodCheck.h"
#include "Pmu_Dps_FM_VI.h"

/************************************************
 T832MD���ܰ����
 ���ߣ����
 ʱ�䣺2020��5��8��
 �汾��V1.0
 ��Ȩ���У�����ؾ���
 Copyright(C) ��������΢���ӹɷ����޹�˾
 All rights reserved
************************************************/

#define ON 1
#define OFF 0
int data_cnt;
int main(void)
{	
	Cache_Enable();                //��L1-Cache
	HAL_Init();				        		//��ʼ��HAL��
	Stm32_Clock_Init(160,5,2,4); //����ϵͳʱ��,400Mhz 
	delay_init(400);						//��ʱ��ʼ��
	
	GPIO_ALL_Init();          //ͨ��IO��ʼ��
	uart2_init(115200);				//����2��ʼ��
	DAC1_Init();              //DAC1��ʼ��
	ADC1_Init();              //ADC1��ʼ�� 
//	IWDG_Init(IWDG_PRESCALER_64,1000); 	//��Ƶ��Ϊ64,����ֵΪ1000,���ʱ��Ϊ2s	 
	while(1)
	{ 
		//***********����****************//
		if(START)//��ʼ����
		{
      data_cnt=2;	//����
			
			Bad_flag=0; //
			
			BinEOT_Init();//��ʼһ���µĲ���������bin��Ϊ��ʼ״̬,������
			
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
			//**********������*****************//
			Connec();//�����Բ���
			if(!Bad_flag)
			{
				DecoFreq();//����Ƶ�ʲ���
			}
			if(!Bad_flag)
			{
				Func();//ƥ�����
			}
			if(!Bad_flag)
			{
				ChDutyCycle();//ռ�ձȲ���
			}
			if(!Bad_flag)
			{
				QuiesCurr();//��̬����
			}
			if(!Bad_flag)
			{
				DriveCap();//��������
			}
			if(!Bad_flag)
			{
				LeakCurr();//©��������
			}
			//***********��Ʒ����Ʒָʾ�ƿ���************//
			if(Bad_flag)
			{
				Bad_flag=0;
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
			}
			else
			{
				SetBin(1);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);//������ʾ��Ʒ
			}
			//***********���������λEOT****************//
			HAL_GPIO_WritePin(GPIOF,GPIO_PIN_8,GPIO_PIN_RESET);//�������,��λEOT 0
			//************����оƬ����״̬*************//
			GPIO_Init_ExcBin();//�ָ���BIN��EOT��������������ų�ʼ״̬
			
		}
		//*************�ϴ���ϸ��������*****************//
		if(Usart2_Data_Requst())//�յ��ϴ���ϸ��Ϣ��ָ��
		{
			USART2_TX_BUF[0]=0x23c227;
			USART2_TX_BUF[1]=data_cnt;
			HAL_UART_Transmit(&UART2_Handler,(uint8_t*)USART2_TX_BUF,88,2000);	
			while(__HAL_UART_GET_FLAG(&UART2_Handler,UART_FLAG_TC)!=SET);
		}
//		IWDG_Feed();
	}
}

void Connec(void)
{
  Pmu_FI(-200,0,5000);//(float fi,float pv,int ir)
  Pmu_MV(1,-0.3,-0.8,5,2);//((int ch,int ir,float hl,float ll,int dt,int bin)
  Pmu_MV(2,-0.3,-0.8,5,2);//OUTR��GND���򱣻�������
	Pmu_MV(3,-0.3,-0.8,5,2);
	Pmu_MV(4,-0.3,-0.8,5,2);
	Pmu_FI(-400,0,5000);//(float fi,float pv,int ir)
	delay_ms(2);
	Pmu_MV(5,-0.3,-0.8,5,2);
	Pmu_FI(0,0,5000);//Ϊ����ĵ�λ��׼����DAC��������ֱ任���ë��
	
	Dps_FV(0,0,50000);//�������ʱ��Ҫ�γɻ�·
	SetDpsR(ON);//��DPS�̵��� FORCE��SENCE
	delay_ms(5);
	
	Pmu_FI(200,0,5000);
  Pmu_MV(1,0.8,0.3,5,2);//DIN��VDD���򱣻�������
	Pmu_MV(5,0.8,0.3,5,2);//DO....
	SetDpsR(OFF);
}
	
void DecoFreq(void)
{
	long CYCLE;
	int i=0;
	//���̵�������ͨ���ͨ��
	Pmu_FV(4,0,50000);//(float fv,float pi,int ir)����D0
	PMU_FS(5,ON);//��PMU 5
	delay_ms(5);
	SetDutR(0x01,ON);//(u8 ch, u8 sw);��1ͨ���̵���
	Dps_FV(4,0,50000);//��ѹ��VDD�ϵ�
	SetDpsR(ON);//��DPS�̵��� FORCE��SENCE
	delay_ms(5);
	
  TIM8CH1_PWM(18-1,16-1,9); //PWM���0XFF  715ns 1432ns
	PMU_FS(5,OFF);//��PMU 5 �ͷ�DO���Ӷ�����оƬ����ģʽ
	delay_ms(5);
	SetDutR(0x10,ON);//(u8 ch, u8 sw);��5ͨ���̵�����������ط��ſ�������ΪPMU5�˵ĵ�����ѹ������оƬCH5����ֱ�ӽ�ͨ������ᵼ��оƬ����
	TIM1CH1_CAPTURE_STA=0;
	TIM1_Cap_Init(0XFFFF,1-1,0x01); //PA8 IO5 TIM1CH1 PWM����
	while(!(TIM1CH1_CAPTURE_STA&0X80))
	{
		i++;
		delay_ms(5);
		if(i>30)
		{
			i=0;
			break;
		}
	}
  CYCLE=5*TIM1CH1_CAPTURE_PerioVAL[5];//�������� 
	USART2_TX_BUF[data_cnt++]=CYCLE;//�����������
	TIM1CH1_CAPTURE_STA=0;
	SetDutR(0x10,OFF);//(u8 ch, u8 sw);��5ͨ���̵���	
	if((CYCLE<241)||(CYCLE>366))//���ݲ��������������һ���ֶ���
	{
    SetBin(6);
	}	
	if(!Bad_flag)
	{
		if((CYCLE>241)&&(CYCLE<295))//��Ҫ�޵�
		{
			Fix(1,0);//��CF1 5V
			delay_ms(15);
			Fix(0,0);//��CF 5V
			delay_ms(15);
			PMU_FS(5,ON);//��PMU 5 	����D0
			TIM8CH1_PWM(18-1,16-1,9); //PWM���0XFF  715ns 1432ns
			delay_ms(5);
			PMU_FS(5,OFF);//��PMU 5 �ͷ�DO���Ӷ�����оƬ����ģʽ
			SetDutR(0x10,ON);//(u8 ch, u8 sw);��5ͨ���̵���
			TIM1_Cap_Init(0XFFFF,1-1,0x01); //PA8 IO5 TIM1CH1 PWM����
			i=0;
			while(!(TIM1CH1_CAPTURE_STA&0X80))//�ȴ��ɼ����
			{
				i++;
				delay_ms(2);
				if(i>30)
				{
					i=0;
					break;
				}
			}
			CYCLE=5*TIM1CH1_CAPTURE_PerioVAL[5];//�������� 
			USART2_TX_BUF[data_cnt++]=CYCLE;//�����������
			TIM1CH1_CAPTURE_STA=0;
		}
		if((CYCLE<295)||(CYCLE>366))//���ݲ��������������һ���ֶ���
		{
			SetBin(5);
		}
  }
	
	TIM1_Cap_DeInit();//�رն�ʱ��1���ܣ�ֹͣ��������
	PAInit_DeTIM1();//��ʼ��TIM1��Ϊ��ͨIO��
	HAL_TIM_PWM_DeInit(&TIM8_Handler);//�ر�PWM
	PCInit_DeTIM8();//��ʼ��Ϊ��ͨIO��
	
	Pmu_FV(0,0,50000);
	SetDutR(0x11,OFF);
}

void Func(void)
{
	int i,j=0;
	//��DIN��������
	Dps_FV(5,0,50000);//VDD 5V
	SetDpsR(ON);//��DPS
	SetDutR(0x11,ON);
	delay_ms(1);
	TIM8CH1_PWM(18-1,16-1,9); //PWM���0XFF  715ns 1432ns
	delay_ms(1);
	
	//��DO���������
	TIM1_Cap_Init(0XFFFF,1-1,0x01); //PA8 IO5 TIM1CH1 PWM���� ע�⣺�ɼ���ɺ󣬾ͻὫPWM�Ͳ����ܶ��رգ��ٴ�ʹ����Ҫ�ٴγ�ʼ��
	while(!(TIM1CH1_CAPTURE_STA&0X80))//�ȴ��ɼ����
	{
		j++;
		delay_ms(2);
		if(j>30)
		{
			j=0;
			break;
		}
	}
	for(i=4;i<7;i++)//��Ϊǰ�沿�ֵ����ݿ��ܲ�׼ȷ���ʲ����ж�
	{
	  if((5*TIM1CH1_CAPTURE_VAL[i]<560)||(5*TIM1CH1_CAPTURE_VAL[i]>732))
		{
			SetBin(7);
		}
	}
	USART2_TX_BUF[data_cnt++]=(5*TIM1CH1_CAPTURE_VAL[5]);//�����������
	TIM1CH1_CAPTURE_STA=0;
  delay_ms(5);
}
	
void ChDutyCycle(void)
{
	float DR,DG,DB;//ռ�ձȱ���
	int i;
	u8 Data[3]={0x55,0xAA,0x55};	
	
	Dps_FV(5,0,50000);//VDD 5V
	SetDpsR(ON);//��DPS
	delay_ms(5);
	SetDutR(0x01,ON);//��DIN 
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);
	delay_ms(2);
	
	Pull_Set(3.3,0,50000,0x0E,1);//(float fv,float pi,int ir,int ch)����R G B 2 3 4
	delay_ms(5);

  for(i=0;i<5;i++)//RGB������֣����Կ����ȷ������ݣ�����
  {
		IO_Data(1,Data,720,530,360,890);//(u8 ch,u8 *data,int ONE_HT,int ONE_LT,int ZERO_HT,int ZERO_LT)//��������55-aa-55
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);
		delay_ms(2);
	}
	SetDutR(0x0E,ON);//R G B
	//delay_ms(2);
	TIM8_Cap_Init(0XFFFF,200-1,0x0e); //��ʱ��8CH2\3\4�ɼ����ܶ�Ӧͨ������ IO2\3\4	
	delay_ms(30);//����ʱ����ʼ��ʱ�䣬����ʡ�ԣ�������ܻ�δ��⵽
	for(i=0;i<5;i++)//���⿪ʼ���Ĳ�׼ȷ
  {
		DR=TIM8_CH2_PeriodCheck();
		DG=TIM8_CH3_PeriodCheck();
		DB=TIM8_CH4_PeriodCheck();
	}
	
	if((DR>68)||(DG>36)||(DB>68)||(DR<64)||(DG<31)||(DB<64))
	{
		SetBin(4);
	}
	USART2_TX_BUF[data_cnt++]=DR;//�����������
	USART2_TX_BUF[data_cnt++]=DG;//�����������
	USART2_TX_BUF[data_cnt++]=DB;//�����������
	

	SetDutR(0x1F,OFF);
	
	Pull_Set(0,0,50000,0x0E,0);//��SENCE 2 3 4
	
	TIM8_Cap_DeInit();//������Ų���ģʽ���ã����ܻ�Ӱ�����ݵķ���
}
	
void QuiesCurr(void)//��̬����
{
	SetDutR(0x7f,OFF);//(u8 ch, u8 sw);��1-7ͨ���̵���
	Dps_FV(5,0,50000);//VDD 5V
	SetDpsR(ON);
	delay_ms(1);//�Ż�
	Dps_MI(5000,950,600,5,3);//(int ir,float hl,float ll,int dt,int bin)

}
	
void DriveCap(void)
{
	int i;
  u8 data[3]={0xff,0xff,0xff};
	
	Dps_FV(5,0,50000);//VDD 5V
	SetDpsR(ON);//��DPS

	delay_ms(2);//�Ż�
	
	for(i=0;i<5;i++)//��һ�����ݲ�׼ȷ�����ᱨ��
  {
		IO_Data(1,data,720,530,360,890);//(u8 ch,u8 *data,int ONE_HT,int ONE_LT,int ZERO_HT,int ZERO_LT)
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);
		delay_ms(5);
	}
	
	delay_ms(2);//�Ż�
	Pmu_FV(1.01,0,50000);//(float fv,float pi,int ir)
	Pmu_MI(2,50000,16250,9000,5,8);//(int ch,int ir,float hl,float ll,int dt,int bin)
	Pmu_MI(3,50000,16250,9000,5,8);
	Pmu_MI(4,50000,16250,9000,5,8);
	
	SetDutR(0x01,OFF);//(u8 ch, u8 sw);��1ͨ���̵����������ݷ��ͺ����лᱻ�򿪣����е����ر�һ��
	delay_ms(5);
}
	
void LeakCurr(void)
{
	int i;
//  u8 data[3]={0x00,0x00,0x00};//��������00-00-00��DIN����RGB
	Dps_FV(5,0,50000);//VDD 5V
	SetDpsR(ON);//��DPS
	delay_ms(5);
	
	SetDutR(0x01,ON);
	TIM8CH1_PWM(18-1,15-1,14);
  delay_ms(1);
//	for(i=0;i<5;i++)//��һ�����ݲ�׼ȷ�����ᱨ��
//  {
//		IO_Data(1,data,720,530,360,1000);//(u8 ch,u8 *data,int ONE_HT,int ONE_LT,int ZERO_HT,int ZERO_LT)
//		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);
//		delay_ms(1);
//	}
	HAL_TIM_PWM_DeInit(&TIM8_Handler);//�ر�PWM
	PCInit_DeTIM8();//��ʼ��Ϊ��ͨIO��
	delay_ms(1);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);
	delay_ms(1);
	SetDutR(0x0e,OFF);//��CH2-4
	
	Pmu_FV(5,0,5);//(float fv,float pi,int ir)
	Pmu_MI(2,5,1,-1,3,8);//(int ch,int ir,float hl,float ll,int dt,int bin)	
	Pmu_MI(3,5,1,-1,3,8);
	Pmu_MI(4,5,1,-1,3,8);
	Direct_MCUtoCH(1,0);//��:CHtoMCU �ߣ�MCUtoCH CH1����ΪCHtoMCU
	
//	delay_ms(10);
}
