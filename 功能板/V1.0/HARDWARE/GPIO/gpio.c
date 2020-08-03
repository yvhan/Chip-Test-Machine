#include "gpio.h"
#include "delay.h"
#include "timer.h"

GPIO_InitTypeDef GPIO_Initure;
//所有普通控制引脚上电初始化
void GPIO_ALL_Init(void)
{

	  __HAL_RCC_GPIOA_CLK_ENABLE();           //开启GPIO时钟
	  __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE(); 
    __HAL_RCC_GPIOD_CLK_ENABLE();
	  __HAL_RCC_GPIOE_CLK_ENABLE();
	  __HAL_RCC_GPIOF_CLK_ENABLE();
	  __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();	

//A脚复用，在串口 ADC DAC TIM1相关功能中初始化
//PA8-11 上电初始化为 普通输出口，复用在TIM1中初始化
//    GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;          
//    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //输出
//    GPIO_Initure.Pull=GPIO_PULLUP;        //上拉
//    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //高速
//    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
//		
//		GPIO_PinA_Reset();	
	
//PB0-7 CH1-7PMU输出继电器 CH1F-CH8F 上电初始化为高 低有效
//PB8-15 CH1-7PMU输出反馈继电器 CH1S-CH8S  上电初始化为高 低有效		
	  GPIO_Initure.Pin=GPIO_PIN_All;          
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //输出
    GPIO_Initure.Pull=GPIO_PULLUP;        //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
		
		GPIO_PinB_Reset();
 
//PC4-5 DPS ADC在相关模块中初始化
//PC6-9 TIM8 在定时器模块中初始化
//PC13 LED指示灯，开始测试点亮，结束关闭	
    GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_13;           
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //高速
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
		
		GPIO_PinC_Reset();

//PD PMU FIMV FVMI
//PD0 PCL PMU输出内部负载电容控制位，对地滤波电容 低有效
//PD1 FORCE-SENCE 两者短接控制位，IO上拉时用 低有效
//PD2-6 P5uA P50uA P500uA P5mA P50mA 加流测流档控制位 低有效
//PD7 -PDAC PMU输出负极性信号控制位 低有效
//PD8-10 PL0.25 0.5 0.75 当前档位限流限压控制位 高有效
//PD11 PFV 加压控制位 低有效
//PD12 PFI 加流控制位 低有效
//PD13 PMV 测压控制位 低有效
//PD14 PMI 测流控制位 低有效
//PD15 +PDAC PMU输出正极性信号控制位 低有效
		GPIO_Initure.Pin=GPIO_PIN_All;   
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;//输出
    GPIO_Initure.Pull=GPIO_PULLUP;        //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH; 
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);
		
		GPIO_PinD_Reset();
		

//PE DPS FV MI
//PE0-2 DL0.75 0.5 0.25 DPS当前档位限流控制位 高有效
//PE3-7 D5uA 50uA 500uA 5mA 50mA DPS测流档位控制 低有效
//PE8 -DDAC DPS输出-信号控制位，低有效
//PE9 +DDAC DPS输出+信号控制位，低有效
//PE10 DCL DPS内部电容负载开关控制位， 低有效
//PE11 DFORCE DPS输出开关控制位，低有效
//PE12 DSENCE DPS输出反馈开关控制位，低有效
//其余没有使用
		GPIO_Initure.Pin=GPIO_PIN_All;   
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;//输出
    GPIO_Initure.Pull=GPIO_PULLUP;        //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH; 
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
		
		GPIO_PinE_Reset();

//PF0-7 BIN1-7输出  高有效，上电初始化为低
//PF8 EOT输出 低有效，上电初始化为高
		GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8;   
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;//输出
    GPIO_Initure.Pull=GPIO_PULLUP;        //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH; 
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);
		
    GPIO_PinF_Reset();
		
//PF9 START信号输入
		GPIO_Initure.Pin=GPIO_PIN_9;   
    GPIO_Initure.Mode=GPIO_MODE_INPUT;//输入
    GPIO_Initure.Pull=GPIO_NOPULL;        //
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH; 
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);

//PG0-7 CH1-8通道逻辑信号方向控制脚DIR1-8 H:MCU->CH L:CH->MCU 上电初始化为低
		GPIO_Initure.Pin=GPIO_PIN_All;   
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;//输出
    GPIO_Initure.Pull=GPIO_PULLUP;        //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH; 
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);
		
		GPIO_PinG_Reset();

//PH 除了0\1两个晶振脚，其余都没有使用   
//    GPIO_Initure.Pin=GPIO_PIN_2|GPIO_PIN_3; //PH2,3 
//    HAL_GPIO_Init(GPIOH,&GPIO_Initure);

//PI0-7 CH1-8通道逻辑控制继电器 L有效 CH1D-CH8D控制通道是否和单片机口连接
//PI8 L:5V和 H:3.3V电压选择位 
//PI9-11 CF1/CF2/CF3 烧熔丝选择位 H有效
    GPIO_Initure.Pin=GPIO_PIN_All;   
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;//输出
    GPIO_Initure.Pull=GPIO_PULLUP;        //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH; 
    HAL_GPIO_Init(GPIOI,&GPIO_Initure);
		
		GPIO_PinI_Reset();
}

void GPIO_Init_ExcBin(void)
{

	  __HAL_RCC_GPIOA_CLK_ENABLE();           //开启GPIO时钟
	  __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE(); 
    __HAL_RCC_GPIOD_CLK_ENABLE();
	  __HAL_RCC_GPIOE_CLK_ENABLE();
	  __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();	
/***********关闭PMU和DPS功能，设为一个通用状态**************/
		Pmu_FV(0,0,5000);
		delay_ms(3);
		Dps_FV(0,0,50000);
		delay_ms(3);
		SetDpsR(OFF);//关DPS
/***********关闭定时器功能**************/	  
	  TIM1_Cap_DeInit();//关闭定时器1功能，停止复用引脚
	  PAInit_DeTIM1();//初始化TIM1口为普通IO口
	  HAL_TIM_PWM_DeInit(&TIM8_Handler);//关闭PWM
	  PCInit_DeTIM8();//初始化为普通IO口
	
//PB0-7 CH1-7PMU输出继电器 CH1F-CH8F 上电初始化为高 低有效
//PB8-15 CH1-7PMU输出反馈继电器 CH1S-CH8S  上电初始化为高 低有效		
	  GPIO_Initure.Pin=GPIO_PIN_All;          
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //输出
    GPIO_Initure.Pull=GPIO_PULLUP;        //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
		
		GPIO_PinB_Reset();
 
//PC4-5 DPS ADC在相关模块中初始化
//PC6-9 TIM8 在定时器模块中初始化
//PC13 LED指示灯,在这里不用初始化
    GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;           
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //高速
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET);	

//PD PMU FIMV FVMI
//PD0 PCL PMU输出内部负载电容控制位，对地滤波电容 低有效
//PD1 FORCE-SENCE 两者短接控制位，IO上拉时用 低有效
//PD2-6 P5uA P50uA P500uA P5mA P50mA 加流测流档控制位 低有效
//PD7 -PDAC PMU输出负极性信号控制位 低有效
//PD8-10 PL0.25 0.5 0.75 当前档位限流限压控制位 高有效
//PD11 PFV 加压控制位 低有效
//PD12 PFI 加流控制位 低有效
//PD13 PMV 测压控制位 低有效
//PD14 PMI 测流控制位 低有效
//PD15 +PDAC PMU输出正极性信号控制位 低有效
		GPIO_Initure.Pin=GPIO_PIN_All;   
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;//输出
    GPIO_Initure.Pull=GPIO_PULLUP;        //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH; 
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);
		
		GPIO_PinD_Reset();
		

//PE DPS FV MI
//PE0-2 DL0.75 0.5 0.25 DPS当前档位限流控制位 高有效
//PE3-7 D5uA 50uA 500uA 5mA 50mA DPS测流档位控制 低有效
//PE8 -DDAC DPS输出-信号控制位，低有效
//PE9 +DDAC DPS输出+信号控制位，低有效
//PE10 DCL DPS内部电容负载开关控制位， 低有效
//PE11 DFORCE DPS输出开关控制位，低有效
//PE12 DSENCE DPS输出反馈开关控制位，低有效
//其余没有使用
		GPIO_Initure.Pin=GPIO_PIN_All;   
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;//输出
    GPIO_Initure.Pull=GPIO_PULLUP;        //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH; 
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
		
		GPIO_PinE_Reset();

//PF0-7 BIN1-7输出  高有效，上电初始化为低
//PF8 EOT输出 低有效，上电初始化为高
//这个函数用于保留BIN信号和EOT情况，所以不初始化这个部分
		
//PF9 START信号输入
//START初始化在这个函数中也是多余

//PG0-7 CH1-8通道逻辑信号方向控制脚DIR1-8 H:MCU->CH L:CH->MCU 上电初始化为低
		GPIO_Initure.Pin=GPIO_PIN_All;   
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;//输出
    GPIO_Initure.Pull=GPIO_PULLUP;        //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH; 
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);
		
		GPIO_PinG_Reset();

//PH 除了0\1两个晶振脚，其余都没有使用   


//PI0-7 CH1-8通道逻辑控制继电器 L有效 CH1D-CH8D控制通道是否和单片机口连接
//PI8 L:5V和 H:3.3V电压选择位 
//PI9-11 CF1/CF2/CF3 烧熔丝选择位 H有效
    GPIO_Initure.Pin=GPIO_PIN_All;   
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;//输出
    GPIO_Initure.Pull=GPIO_PULLUP;        //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH; 
    HAL_GPIO_Init(GPIOI,&GPIO_Initure);
		
		GPIO_PinI_Reset();
}

void BinEOT_Init(void)
{
		__HAL_RCC_GPIOF_CLK_ENABLE();
	
//PF0-7 BIN1-7输出  高有效，上电初始化为低
//PF8 EOT输出 低有效，上电初始化为高
		GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8;   
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;//输出
    GPIO_Initure.Pull=GPIO_PULLUP;        //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH; 
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);
		
    GPIO_PinF_Reset();
}

void GPIO_PinA_Reset(void)
{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET);
}
void GPIO_PinB_Reset(void)
{
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);	//PB0置1
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET);
}

void GPIO_PinC_Reset(void)
{
	 HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);
   HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET);
   HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);
   HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET);		
	 HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
}

void GPIO_PinD_Reset(void)
{
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_5,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_6,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_8,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_11,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_15,GPIO_PIN_SET);
}

void GPIO_PinE_Reset(void)
{
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_0,GPIO_PIN_RESET);	//PE0置0
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_1,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_14,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15,GPIO_PIN_SET);
}

void GPIO_PinF_Reset(void)
{
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_0,GPIO_PIN_RESET);	//PF0置0
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_1,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_2,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_3,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_4,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_5,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_6,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_7,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_8,GPIO_PIN_SET); //EOT  上电为1
}

void GPIO_PinG_Reset(void)
{
    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_0,GPIO_PIN_RESET);	//PG0置0
		HAL_GPIO_WritePin(GPIOG,GPIO_PIN_1,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOG,GPIO_PIN_2,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOG,GPIO_PIN_3,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOG,GPIO_PIN_4,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOG,GPIO_PIN_5,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOG,GPIO_PIN_6,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOG,GPIO_PIN_7,GPIO_PIN_RESET);
}

void GPIO_PinI_Reset(void)
{
    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_0,GPIO_PIN_SET);	//PI0置1
		HAL_GPIO_WritePin(GPIOI,GPIO_PIN_1,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOI,GPIO_PIN_2,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOI,GPIO_PIN_3,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOI,GPIO_PIN_4,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOI,GPIO_PIN_5,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOI,GPIO_PIN_6,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOI,GPIO_PIN_7,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOI,GPIO_PIN_8,GPIO_PIN_RESET);//5V
		HAL_GPIO_WritePin(GPIOI,GPIO_PIN_9,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOI,GPIO_PIN_10,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOI,GPIO_PIN_11,GPIO_PIN_RESET);
}

//PMU开关
void PMU_FS(int ch,u8 sw)
{
	if(sw)
	{
		switch(ch)
		{
			case 1: HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);break;//FORCE
			case 2: HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);break;
			case 3: HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_RESET);break;
			case 4: HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);break;
			case 5: HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET);break;
			case 6: HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET);break;
			case 7: HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_RESET);break;
			case 8: HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);break;
			
		}
		delay_ms(1);
		switch(ch)
		{
			case 1: HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET);break;//SENCE
			case 2: HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);break;
			case 3: HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);break;
			case 4: HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_RESET);break;
			case 5: HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);break;
			case 6: HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET);break;
			case 7: HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);break;
			case 8: HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);break;
			
		}
	}
	else
	{
		switch(ch)
		{
			case 1: HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);break;//SENCE
			case 2: HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);break;
			case 3: HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);break;
			case 4: HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_SET);break;
			case 5: HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);break;
			case 6: HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET);break;
			case 7: HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);break;
			case 8: HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET);break;
		
		}
		delay_ms(1);
		switch(ch)
		{
			case 1: HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);break;//FORCE
			case 2: HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);break;
			case 3: HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_SET);break;
			case 4: HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);break;
			case 5: HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_SET);break;
			case 6: HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET);break;
			case 7: HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_SET);break;
			case 8: HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);break;
		}
	}
}

//开通道CH1-CH8，和主控芯片IO口相连通道选择方式：8位从低到高分别代表CH1-CH8
void SetDutR(u8 ch, u8 sw)
{
	  if(ch&0x01)
		{
			if(sw)
			{
				HAL_GPIO_WritePin(GPIOI,GPIO_PIN_0,GPIO_PIN_RESET);	
			}
			else
			{
				HAL_GPIO_WritePin(GPIOI,GPIO_PIN_0,GPIO_PIN_SET);
			}
			
		}
		if(ch&0x02)
		{
			if(sw)
			{
				HAL_GPIO_WritePin(GPIOI,GPIO_PIN_1,GPIO_PIN_RESET);	
			}
			else
			{
				HAL_GPIO_WritePin(GPIOI,GPIO_PIN_1,GPIO_PIN_SET);
			}
		}
		if(ch&0x04)
		{
			if(sw)
			{
				HAL_GPIO_WritePin(GPIOI,GPIO_PIN_2,GPIO_PIN_RESET);	
			}
			else
			{
				HAL_GPIO_WritePin(GPIOI,GPIO_PIN_2,GPIO_PIN_SET);
			}
		}
		if(ch&0x08)
		{
			if(sw)
			{
				HAL_GPIO_WritePin(GPIOI,GPIO_PIN_3,GPIO_PIN_RESET);	
			}
			else
			{
				HAL_GPIO_WritePin(GPIOI,GPIO_PIN_3,GPIO_PIN_SET);
			}
		}
		if(ch&0x10)
		{
			if(sw)
			{
				HAL_GPIO_WritePin(GPIOI,GPIO_PIN_4,GPIO_PIN_RESET);	
			}
			else
			{
				HAL_GPIO_WritePin(GPIOI,GPIO_PIN_4,GPIO_PIN_SET);
			}
		}
		if(ch&0x20)
		{
			if(sw)
			{
				HAL_GPIO_WritePin(GPIOI,GPIO_PIN_5,GPIO_PIN_RESET);	
			}
			else
			{
				HAL_GPIO_WritePin(GPIOI,GPIO_PIN_5,GPIO_PIN_SET);
			}
		}
		if(ch&0x40)
		{
			if(sw)
			{
				HAL_GPIO_WritePin(GPIOI,GPIO_PIN_6,GPIO_PIN_RESET);	
			}
			else
			{
				HAL_GPIO_WritePin(GPIOI,GPIO_PIN_6,GPIO_PIN_SET);
			}
		}
		if(ch&0x80)
		{
			if(sw)
			{
				HAL_GPIO_WritePin(GPIOI,GPIO_PIN_7,GPIO_PIN_RESET);	
			}
			else
			{
				HAL_GPIO_WritePin(GPIOI,GPIO_PIN_7,GPIO_PIN_SET);
			}
		}
}
//BIN脚置位
u8 Bad_flag=0;
void SetBin(int bin)
{
	switch(bin)
	{
		case 1: HAL_GPIO_WritePin(GPIOF,GPIO_PIN_0,GPIO_PIN_SET);break;//bin1
		case 2: HAL_GPIO_WritePin(GPIOF,GPIO_PIN_1,GPIO_PIN_SET);break;
		case 3: HAL_GPIO_WritePin(GPIOF,GPIO_PIN_2,GPIO_PIN_SET);break;
		case 4: HAL_GPIO_WritePin(GPIOF,GPIO_PIN_3,GPIO_PIN_SET);break;
		case 5: HAL_GPIO_WritePin(GPIOF,GPIO_PIN_4,GPIO_PIN_SET);break;
		case 6: HAL_GPIO_WritePin(GPIOF,GPIO_PIN_5,GPIO_PIN_SET);break;
		case 7: HAL_GPIO_WritePin(GPIOF,GPIO_PIN_6,GPIO_PIN_SET);break;
		case 8: HAL_GPIO_WritePin(GPIOF,GPIO_PIN_7,GPIO_PIN_SET);break;
	}
	if(bin!=1)
	{
		Bad_flag=1;
	}
}

//将IO口初始化为普通输出口，因为IO口是复用为定时器的
void PAInit_DeTIM1(void)
{
	GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;
  GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //输出
  GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
  GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;   //高速
  HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET);
}

void PCInit_DeTIM8(void)
{
	GPIO_Initure.Pin=GPIO_PIN_6;          
  GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //输出
  GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
  GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //高速
  HAL_GPIO_Init(GPIOC,&GPIO_Initure);
	
	GPIO_Initure.Pin=GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;          
  GPIO_Initure.Mode=GPIO_MODE_INPUT;  //输入
  GPIO_Initure.Pull=GPIO_NOPULL;          
  GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //高速
  HAL_GPIO_Init(GPIOC,&GPIO_Initure);
	
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET);
}

void Direct_MCUtoCH(int ch,u8 sw)//低:CHtoMCU 高：MCUtoCH
{
	if(sw)
	{	
    switch(ch)
		{
			case 1: HAL_GPIO_WritePin(GPIOG,GPIO_PIN_0,GPIO_PIN_SET);break;
			case 2: HAL_GPIO_WritePin(GPIOG,GPIO_PIN_1,GPIO_PIN_SET);break;
			case 3: HAL_GPIO_WritePin(GPIOG,GPIO_PIN_2,GPIO_PIN_SET);break;
			case 4: HAL_GPIO_WritePin(GPIOG,GPIO_PIN_3,GPIO_PIN_SET);break;
			case 5: HAL_GPIO_WritePin(GPIOG,GPIO_PIN_4,GPIO_PIN_SET);break;
			case 6: HAL_GPIO_WritePin(GPIOG,GPIO_PIN_5,GPIO_PIN_SET);break;
			case 7: HAL_GPIO_WritePin(GPIOG,GPIO_PIN_6,GPIO_PIN_SET);break;
			case 8: HAL_GPIO_WritePin(GPIOG,GPIO_PIN_7,GPIO_PIN_SET);break;
		}
	}
	else
	{
		switch(ch)
		{
			case 1: HAL_GPIO_WritePin(GPIOG,GPIO_PIN_0,GPIO_PIN_RESET);break;
			case 2: HAL_GPIO_WritePin(GPIOG,GPIO_PIN_1,GPIO_PIN_RESET);break;
			case 3: HAL_GPIO_WritePin(GPIOG,GPIO_PIN_2,GPIO_PIN_RESET);break;
			case 4: HAL_GPIO_WritePin(GPIOG,GPIO_PIN_3,GPIO_PIN_RESET);break;
			case 5: HAL_GPIO_WritePin(GPIOG,GPIO_PIN_4,GPIO_PIN_RESET);break;
			case 6: HAL_GPIO_WritePin(GPIOG,GPIO_PIN_5,GPIO_PIN_RESET);break;
			case 7: HAL_GPIO_WritePin(GPIOG,GPIO_PIN_6,GPIO_PIN_RESET);break;
			case 8: HAL_GPIO_WritePin(GPIOG,GPIO_PIN_7,GPIO_PIN_RESET);break;
		}
	}
}

void Fix(u8 cfn,u8 vol)//cfn:修调继电器选择0:全关 1/2/3:开 CF1、2、3 修调档位选择vol:1:3.3V 0:5V
{
	switch(cfn)
	{
		case 0: HAL_GPIO_WritePin(GPIOI,GPIO_PIN_9,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOI,GPIO_PIN_10,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOI,GPIO_PIN_11,GPIO_PIN_RESET);break;
		
		case 1: HAL_GPIO_WritePin(GPIOI,GPIO_PIN_9,GPIO_PIN_SET);break;
						
		case 2: HAL_GPIO_WritePin(GPIOI,GPIO_PIN_10,GPIO_PIN_SET);break;
						
		case 3:	HAL_GPIO_WritePin(GPIOI,GPIO_PIN_11,GPIO_PIN_SET);break;
	}
	if(vol)
	{
		HAL_GPIO_WritePin(GPIOI,GPIO_PIN_8,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOI,GPIO_PIN_8,GPIO_PIN_RESET);
	}
}

//上拉被测芯片管脚
void PullUp(int ch,u8 sw)
{
	if(sw)
	{
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1,GPIO_PIN_RESET);//使用电阻拉高时，必须置位
		delay_ms(2);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_RESET);
	  
	  switch(ch)
		{
			case 1:HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET);//开SENCE 1
			case 2:HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);//开SENCE 2 
			case 3:HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);
			case 4:HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_RESET);
			case 5:HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);
			case 6:HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET);
			case 7:HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
			case 8:HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);
		}	
	}
	else
	{
	  switch(ch)
		{
			case 1:HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);//关SENCE 1
			case 2:HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);//关SENCE 2
			case 3:HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);
			case 4:HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_SET);
			case 5:HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);
			case 6:HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET);
			case 7:HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
			case 8:HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET);
		}
		delay_ms(2);//避免还没来得及关闭SENSE
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_SET);//不需拉高时，关掉这两个脚
	  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1,GPIO_PIN_SET);
	}
}
