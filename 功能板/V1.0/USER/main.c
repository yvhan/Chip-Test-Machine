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
 T832MD功能板程序
 作者：刘宇航
 时间：2020年5月8日
 版本：V1.0
 版权所有，盗版必究。
 Copyright(C) 深圳市天微电子股份有限公司
 All rights reserved
************************************************/

#define ON 1
#define OFF 0
int data_cnt;
int main(void)
{	
	Cache_Enable();                //打开L1-Cache
	HAL_Init();				        		//初始化HAL库
	Stm32_Clock_Init(160,5,2,4); //设置系统时钟,400Mhz 
	delay_init(400);						//延时初始化
	
	GPIO_ALL_Init();          //通用IO初始化
	uart2_init(115200);				//串口2初始化
	DAC1_Init();              //DAC1初始化
	ADC1_Init();              //ADC1初始化 
//	IWDG_Init(IWDG_PRESCALER_64,1000); 	//分频数为64,重载值为1000,溢出时间为2s	 
	while(1)
	{ 
		//***********测试****************//
		if(START)//开始测量
		{
      data_cnt=2;	//数组
			
			Bad_flag=0; //
			
			BinEOT_Init();//开始一次新的测量，重置bin脚为初始状态,以免误报
			
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
			//**********测试项*****************//
			Connec();//连接性测试
			if(!Bad_flag)
			{
				DecoFreq();//解码频率测试
			}
			if(!Bad_flag)
			{
				Func();//匹配测试
			}
			if(!Bad_flag)
			{
				ChDutyCycle();//占空比测试
			}
			if(!Bad_flag)
			{
				QuiesCurr();//静态电流
			}
			if(!Bad_flag)
			{
				DriveCap();//恒流测试
			}
			if(!Bad_flag)
			{
				LeakCurr();//漏电流测试
			}
			//***********良品不良品指示灯控制************//
			if(Bad_flag)
			{
				Bad_flag=0;
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
			}
			else
			{
				SetBin(1);
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);//灯亮表示良品
			}
			//***********测试完成置位EOT****************//
			HAL_GPIO_WritePin(GPIOF,GPIO_PIN_8,GPIO_PIN_RESET);//测试完成,置位EOT 0
			//************重置芯片引脚状态*************//
			GPIO_Init_ExcBin();//恢复除BIN和EOT引脚外的所有引脚初始状态
			
		}
		//*************上传详细测试数据*****************//
		if(Usart2_Data_Requst())//收到上传详细信息的指令
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
  Pmu_MV(2,-0.3,-0.8,5,2);//OUTR对GND反向保护二极管
	Pmu_MV(3,-0.3,-0.8,5,2);
	Pmu_MV(4,-0.3,-0.8,5,2);
	Pmu_FI(-400,0,5000);//(float fi,float pv,int ir)
	delay_ms(2);
	Pmu_MV(5,-0.3,-0.8,5,2);
	Pmu_FI(0,0,5000);//为后面的档位做准备清DAC，否则出现变换尖峰毛刺
	
	Dps_FV(0,0,50000);//正向测试时需要形成回路
	SetDpsR(ON);//开DPS继电器 FORCE和SENCE
	delay_ms(5);
	
	Pmu_FI(200,0,5000);
  Pmu_MV(1,0.8,0.3,5,2);//DIN对VDD正向保护二极管
	Pmu_MV(5,0.8,0.3,5,2);//DO....
	SetDpsR(OFF);
}
	
void DecoFreq(void)
{
	long CYCLE;
	int i=0;
	//开继电器，接通相关通道
	Pmu_FV(4,0,50000);//(float fv,float pi,int ir)拉高D0
	PMU_FS(5,ON);//开PMU 5
	delay_ms(5);
	SetDutR(0x01,ON);//(u8 ch, u8 sw);开1通道继电器
	Dps_FV(4,0,50000);//放压，VDD上电
	SetDpsR(ON);//开DPS继电器 FORCE和SENCE
	delay_ms(5);
	
  TIM8CH1_PWM(18-1,16-1,9); //PWM输出0XFF  715ns 1432ns
	PMU_FS(5,OFF);//关PMU 5 释放DO，从而进入芯片测试模式
	delay_ms(5);
	SetDutR(0x10,ON);//(u8 ch, u8 sw);开5通道继电器，在这个地方才开，是因为PMU5端的电流电压和主控芯片CH5不能直接接通，否则会导致芯片卡死
	TIM1CH1_CAPTURE_STA=0;
	TIM1_Cap_Init(0XFFFF,1-1,0x01); //PA8 IO5 TIM1CH1 PWM捕获
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
  CYCLE=5*TIM1CH1_CAPTURE_PerioVAL[5];//测量周期 
	USART2_TX_BUF[data_cnt++]=CYCLE;//保存测试数据
	TIM1CH1_CAPTURE_STA=0;
	SetDutR(0x10,OFF);//(u8 ch, u8 sw);关5通道继电器	
	if((CYCLE<241)||(CYCLE>366))//根据测量结果，进行下一部分动作
	{
    SetBin(6);
	}	
	if(!Bad_flag)
	{
		if((CYCLE>241)&&(CYCLE<295))//需要修调
		{
			Fix(1,0);//开CF1 5V
			delay_ms(15);
			Fix(0,0);//关CF 5V
			delay_ms(15);
			PMU_FS(5,ON);//开PMU 5 	拉高D0
			TIM8CH1_PWM(18-1,16-1,9); //PWM输出0XFF  715ns 1432ns
			delay_ms(5);
			PMU_FS(5,OFF);//关PMU 5 释放DO，从而进入芯片测试模式
			SetDutR(0x10,ON);//(u8 ch, u8 sw);开5通道继电器
			TIM1_Cap_Init(0XFFFF,1-1,0x01); //PA8 IO5 TIM1CH1 PWM捕获
			i=0;
			while(!(TIM1CH1_CAPTURE_STA&0X80))//等待采集完成
			{
				i++;
				delay_ms(2);
				if(i>30)
				{
					i=0;
					break;
				}
			}
			CYCLE=5*TIM1CH1_CAPTURE_PerioVAL[5];//测量周期 
			USART2_TX_BUF[data_cnt++]=CYCLE;//保存测试数据
			TIM1CH1_CAPTURE_STA=0;
		}
		if((CYCLE<295)||(CYCLE>366))//根据测量结果，进行下一部分动作
		{
			SetBin(5);
		}
  }
	
	TIM1_Cap_DeInit();//关闭定时器1功能，停止复用引脚
	PAInit_DeTIM1();//初始化TIM1口为普通IO口
	HAL_TIM_PWM_DeInit(&TIM8_Handler);//关闭PWM
	PCInit_DeTIM8();//初始化为普通IO口
	
	Pmu_FV(0,0,50000);
	SetDutR(0x11,OFF);
}

void Func(void)
{
	int i,j=0;
	//从DIN输入数据
	Dps_FV(5,0,50000);//VDD 5V
	SetDpsR(ON);//开DPS
	SetDutR(0x11,ON);
	delay_ms(1);
	TIM8CH1_PWM(18-1,16-1,9); //PWM输出0XFF  715ns 1432ns
	delay_ms(1);
	
	//读DO输出的数据
	TIM1_Cap_Init(0XFFFF,1-1,0x01); //PA8 IO5 TIM1CH1 PWM捕获 注意：采集完成后，就会将PWM和捕获功能都关闭，再次使用需要再次初始化
	while(!(TIM1CH1_CAPTURE_STA&0X80))//等待采集完成
	{
		j++;
		delay_ms(2);
		if(j>30)
		{
			j=0;
			break;
		}
	}
	for(i=4;i<7;i++)//因为前面部分的数据可能不准确，故不作判断
	{
	  if((5*TIM1CH1_CAPTURE_VAL[i]<560)||(5*TIM1CH1_CAPTURE_VAL[i]>732))
		{
			SetBin(7);
		}
	}
	USART2_TX_BUF[data_cnt++]=(5*TIM1CH1_CAPTURE_VAL[5]);//保存测试数据
	TIM1CH1_CAPTURE_STA=0;
  delay_ms(5);
}
	
void ChDutyCycle(void)
{
	float DR,DG,DB;//占空比变量
	int i;
	u8 Data[3]={0x55,0xAA,0x55};	
	
	Dps_FV(5,0,50000);//VDD 5V
	SetDpsR(ON);//开DPS
	delay_ms(5);
	SetDutR(0x01,ON);//开DIN 
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);
	delay_ms(2);
	
	Pull_Set(3.3,0,50000,0x0E,1);//(float fv,float pi,int ir,int ch)拉高R G B 2 3 4
	delay_ms(5);

  for(i=0;i<5;i++)//RGB输出保持，所以可以先发送数据，后检测
  {
		IO_Data(1,Data,720,530,360,890);//(u8 ch,u8 *data,int ONE_HT,int ONE_LT,int ZERO_HT,int ZERO_LT)//发送数据55-aa-55
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);
		delay_ms(2);
	}
	SetDutR(0x0E,ON);//R G B
	//delay_ms(2);
	TIM8_Cap_Init(0XFFFF,200-1,0x0e); //定时器8CH2\3\4采集功能对应通道开启 IO2\3\4	
	delay_ms(30);//给定时器初始化时间，不可省略，否则可能还未检测到
	for(i=0;i<5;i++)//避免开始检测的不准确
  {
		DR=TIM8_CH2_PeriodCheck();
		DG=TIM8_CH3_PeriodCheck();
		DB=TIM8_CH4_PeriodCheck();
	}
	
	if((DR>68)||(DG>36)||(DB>68)||(DR<64)||(DG<31)||(DB<64))
	{
		SetBin(4);
	}
	USART2_TX_BUF[data_cnt++]=DR;//保存测试数据
	USART2_TX_BUF[data_cnt++]=DG;//保存测试数据
	USART2_TX_BUF[data_cnt++]=DB;//保存测试数据
	

	SetDutR(0x1F,OFF);
	
	Pull_Set(0,0,50000,0x0E,0);//关SENCE 2 3 4
	
	TIM8_Cap_DeInit();//清除引脚捕获模式配置，可能会影响数据的发送
}
	
void QuiesCurr(void)//静态电流
{
	SetDutR(0x7f,OFF);//(u8 ch, u8 sw);关1-7通道继电器
	Dps_FV(5,0,50000);//VDD 5V
	SetDpsR(ON);
	delay_ms(1);//优化
	Dps_MI(5000,950,600,5,3);//(int ir,float hl,float ll,int dt,int bin)

}
	
void DriveCap(void)
{
	int i;
  u8 data[3]={0xff,0xff,0xff};
	
	Dps_FV(5,0,50000);//VDD 5V
	SetDpsR(ON);//开DPS

	delay_ms(2);//优化
	
	for(i=0;i<5;i++)//第一次数据不准确，检测会报错
  {
		IO_Data(1,data,720,530,360,890);//(u8 ch,u8 *data,int ONE_HT,int ONE_LT,int ZERO_HT,int ZERO_LT)
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);
		delay_ms(5);
	}
	
	delay_ms(2);//优化
	Pmu_FV(1.01,0,50000);//(float fv,float pi,int ir)
	Pmu_MI(2,50000,16250,9000,5,8);//(int ch,int ir,float hl,float ll,int dt,int bin)
	Pmu_MI(3,50000,16250,9000,5,8);
	Pmu_MI(4,50000,16250,9000,5,8);
	
	SetDutR(0x01,OFF);//(u8 ch, u8 sw);关1通道继电器，在数据发送函数中会被打开，所有单独关闭一下
	delay_ms(5);
}
	
void LeakCurr(void)
{
	int i;
//  u8 data[3]={0x00,0x00,0x00};//发送数据00-00-00到DIN，关RGB
	Dps_FV(5,0,50000);//VDD 5V
	SetDpsR(ON);//开DPS
	delay_ms(5);
	
	SetDutR(0x01,ON);
	TIM8CH1_PWM(18-1,15-1,14);
  delay_ms(1);
//	for(i=0;i<5;i++)//第一次数据不准确，检测会报错
//  {
//		IO_Data(1,data,720,530,360,1000);//(u8 ch,u8 *data,int ONE_HT,int ONE_LT,int ZERO_HT,int ZERO_LT)
//		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);
//		delay_ms(1);
//	}
	HAL_TIM_PWM_DeInit(&TIM8_Handler);//关闭PWM
	PCInit_DeTIM8();//初始化为普通IO口
	delay_ms(1);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);
	delay_ms(1);
	SetDutR(0x0e,OFF);//关CH2-4
	
	Pmu_FV(5,0,5);//(float fv,float pi,int ir)
	Pmu_MI(2,5,1,-1,3,8);//(int ch,int ir,float hl,float ll,int dt,int bin)	
	Pmu_MI(3,5,1,-1,3,8);
	Pmu_MI(4,5,1,-1,3,8);
	Direct_MCUtoCH(1,0);//低:CHtoMCU 高：MCUtoCH CH1设置为CHtoMCU
	
//	delay_ms(10);
}
