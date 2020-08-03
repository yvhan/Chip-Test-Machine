#include "timer.h"
#include "gpio.h"


TIM_HandleTypeDef TIM8_Handler;      	//定时器句柄 
TIM_OC_InitTypeDef TIM8_CH1Handler;     //定时器通道句柄


//通用定时器3中断初始化,定时器3在APB1上，APB1的定时器时钟为200MHz
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!(定时器3挂在APB1上，时钟为HCLK/2)
//void TIM3_Init(u16 arr,u16 psc)
//{  
//    TIM3_Handler.Instance=TIM3;                          //通用定时器3
//    TIM3_Handler.Init.Prescaler=psc;                     //分频
//    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
//    TIM3_Handler.Init.Period=arr;                        //自动装载值
//    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
//    HAL_TIM_Base_Init(&TIM3_Handler);
//    
//    HAL_TIM_Base_Start_IT(&TIM3_Handler); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE    
//}

//TIM PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM8CH1_PWM_Init(u16 arr,u16 psc)
{ 
    TIM8_Handler.Instance=TIM8;            //定时器8
    TIM8_Handler.Init.Prescaler=psc;       //定时器分频
    TIM8_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
    TIM8_Handler.Init.Period=arr;          //自动重装载值
    TIM8_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM8_Handler);       //初始化PWM
    
    TIM8_CH1Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM8_CH1Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，
                                            //默认比较值为自动重装载值的一半,即占空比为50%
    TIM8_CH1Handler.OCPolarity=TIM_OCPOLARITY_LOW; //输出比较极性为低 
    HAL_TIM_PWM_ConfigChannel(&TIM8_Handler,&TIM8_CH1Handler,TIM_CHANNEL_1);//配置TIM8
    HAL_TIM_PWM_Start(&TIM8_Handler,TIM_CHANNEL_1);//开启PWM通道
}

//定时器底层驱动，时钟使能，引脚配置
//此函数会被HAL_TIM_PWM_Init()调用
//htim:定时器句柄
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
	  __HAL_RCC_TIM8_CLK_ENABLE();			//使能定时器时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();			//开启GPIOC时钟
	
    GPIO_Initure.Pin=GPIO_PIN_6;           	//PC6
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推完输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //高速
	  GPIO_Initure.Alternate=GPIO_AF3_TIM8;	//PC6复用为TIM8_CH1
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
	
	  Direct_MCUtoCH(1,1);//IO1输出
}

//定时器底册驱动，开启时钟，设置中断优先级
//此函数会被HAL_TIM_Base_Init()函数调用
//void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
//{
//  if(htim->Instance==TIM1)
//	{
//		__HAL_RCC_TIM1_CLK_ENABLE();            //使能TIM3时钟
//		HAL_NVIC_SetPriority(TIM1_CC_IRQn,1,3);    //设置中断优先级，抢占优先级1，子优先级3
//		HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);          //开启ITM3中断   
//	}  
//}

//定时器3中断服务函数
//void TIM3_IRQHandler(void)
//{
//    HAL_TIM_IRQHandler(&TIM3_Handler);
//}


//设置TIM通道1的占空比
//compare:比较值
void TIM_SetTIM8Compare1(u32 compare)
{
	TIM8->CCR1=compare; 
}

void TIM8CH1_PWM(u16 arr,u16 psc,u32 compare)
{
	TIM8CH1_PWM_Init(arr,psc); //PWM输出0XFF    		
	TIM_SetTIM8Compare1(compare);
}

//获取TIM捕获/比较寄存器值
u32 TIM_GetTIM8Capture1(void)
{
	return  HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_1);
}
/***************************************************************************
****************************************************************************
  TIM1通道捕获  
****************************************************************************
****************************************************************************/

TIM_HandleTypeDef TIM1_Handler;         //定时器1句柄 结构体命名
TIM_SlaveConfigTypeDef  sSlaveConfig;
//定时1通道1输入捕获配置
//arr：自动重装值(TIM1,TIM8是16位的!!)
//psc：时钟预分频数
//ch: 捕获通道选择
void TIM1_Cap_Init(u16 arr,u16 psc,u8 ch)
{  
	TIM_IC_InitTypeDef TIM1_CH1Config; 
	TIM_IC_InitTypeDef TIM1_CH2Config;	
	TIM_IC_InitTypeDef TIM1_CH3Config; 
	TIM_IC_InitTypeDef TIM1_CH4Config;
	
	GPIO_InitTypeDef GPIO_Initure;

	TIM1_Handler.Instance=TIM1;                          //高级定时器1
	TIM1_Handler.Init.Prescaler=psc;                     //分频
	TIM1_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
	TIM1_Handler.Init.Period=arr;                        //自动装载值
	TIM1_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_IC_Init(&TIM1_Handler);
    
	if(ch&0x01)
	{
    GPIO_Initure.Pin=GPIO_PIN_8;//|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;            //PA8\9\10\11
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //复用推挽输出
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //高速
    GPIO_Initure.Alternate=GPIO_AF1_TIM1;   
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
		
    TIM1_CH1Config.ICPolarity=TIM_ICPOLARITY_RISING;    //上升沿捕获
    TIM1_CH1Config.ICSelection=TIM_ICSELECTION_DIRECTTI;//映射到TI1上
    TIM1_CH1Config.ICPrescaler=TIM_ICPSC_DIV1;          //配置输入分频，不分频
    TIM1_CH1Config.ICFilter=2;                          //配置输入滤波器，不滤波
    HAL_TIM_IC_ConfigChannel(&TIM1_Handler,&TIM1_CH1Config,TIM_CHANNEL_1);//配置TIM1通道1
		
		TIM1_CH2Config.ICPolarity=TIM_ICPOLARITY_FALLING;    //
    TIM1_CH2Config.ICSelection=TIM_ICSELECTION_INDIRECTTI;//映射到TI1上
    TIM1_CH2Config.ICPrescaler=TIM_ICPSC_DIV1;          //配置输入分频，不分频
    TIM1_CH2Config.ICFilter=2;                          //配置输入滤波器，不滤波
    HAL_TIM_IC_ConfigChannel(&TIM1_Handler,&TIM1_CH2Config,TIM_CHANNEL_2);//配置TIM1通道2
		
		sSlaveConfig.SlaveMode=TIM_SLAVEMODE_RESET;
		sSlaveConfig.InputTrigger=TIM_TS_TI1FP1;
		sSlaveConfig.TriggerPolarity=TIM_TRIGGERPOLARITY_RISING;
		sSlaveConfig.TriggerPrescaler=TIM_TRIGGERPRESCALER_DIV1;
		sSlaveConfig.TriggerFilter=2;
		HAL_TIM_SlaveConfigSynchronization(&TIM1_Handler, &sSlaveConfig);
		
		Direct_MCUtoCH(5,0);//低:CHtoMCU 高：MCUtoCH CH5设置为CHtoMCU
		
		HAL_TIM_IC_Start_IT(&TIM1_Handler,TIM_CHANNEL_1);   //开始捕获TIM1的通道1
		HAL_TIM_IC_Start_IT(&TIM1_Handler,TIM_CHANNEL_2);   //开始捕获TIM1的通道IC2
		
	}
	if(ch&0x02)
	{ 
    GPIO_Initure.Pin=GPIO_PIN_9;//|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;            //PA8\9\10\11
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //复用推挽输出
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //高速
    GPIO_Initure.Alternate=GPIO_AF1_TIM1;  
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
		
	  TIM1_CH2Config.ICPolarity=TIM_ICPOLARITY_RISING;    //上升沿捕获
    TIM1_CH2Config.ICSelection=TIM_ICSELECTION_DIRECTTI;//映射到TI2上
    TIM1_CH2Config.ICPrescaler=TIM_ICPSC_DIV8;          //配置输入分频，不分频
    TIM1_CH2Config.ICFilter=0;                          //配置输入滤波器，不滤波
    HAL_TIM_IC_ConfigChannel(&TIM1_Handler,&TIM1_CH2Config,TIM_CHANNEL_2);//配置TIM1通道2
		
		Direct_MCUtoCH(6,0);//低:CHtoMCU 高：MCUtoCH CH6设置为CHtoMCU
		HAL_TIM_IC_Start_IT(&TIM1_Handler,TIM_CHANNEL_2);   //开始捕获TIM1的通道2
		
	}
  if(ch&0x04)
	{	
    GPIO_Initure.Pin=GPIO_PIN_10;//|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;            //PA8\9\10\11
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //复用推挽输出
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //高速
    GPIO_Initure.Alternate=GPIO_AF1_TIM1;   
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
		
		TIM1_CH3Config.ICPolarity=TIM_ICPOLARITY_RISING;    //上升沿捕获
    TIM1_CH3Config.ICSelection=TIM_ICSELECTION_DIRECTTI;//映射到TI3上
    TIM1_CH3Config.ICPrescaler=TIM_ICPSC_DIV8;          //配置输入分频，不分频
    TIM1_CH3Config.ICFilter=0;                          //配置输入滤波器，不滤波
    HAL_TIM_IC_ConfigChannel(&TIM1_Handler,&TIM1_CH3Config,TIM_CHANNEL_3);//配置TIM1通道3
		
		Direct_MCUtoCH(7,0);//低:CHtoMCU 高：MCUtoCH CH7设置为CHtoMCU
		HAL_TIM_IC_Start_IT(&TIM1_Handler,TIM_CHANNEL_3);   //开始捕获TIM1的通道3
		
	}
	if(ch&0x08)
	{
    GPIO_Initure.Pin=GPIO_PIN_11;//|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;            //PA8\9\10\11
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //复用推挽输出
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //高速
    GPIO_Initure.Alternate=GPIO_AF1_TIM1;  
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
		
		TIM1_CH4Config.ICPolarity=TIM_ICPOLARITY_RISING;    //上升沿捕获
    TIM1_CH4Config.ICSelection=TIM_ICSELECTION_DIRECTTI;//映射到TI4上
    TIM1_CH4Config.ICPrescaler=TIM_ICPSC_DIV8;          //配置输入分频，不分频
    TIM1_CH4Config.ICFilter=0;                          //配置输入滤波器，不滤波
    HAL_TIM_IC_ConfigChannel(&TIM1_Handler,&TIM1_CH4Config,TIM_CHANNEL_4);//配置TIM1通道4
		
		Direct_MCUtoCH(8,0);//低:CHtoMCU 高：MCUtoCH CH8设置为CHtoMCU
		HAL_TIM_IC_Start_IT(&TIM1_Handler,TIM_CHANNEL_4);   //开始捕获TIM1的通道4
		
	}
    
    __HAL_TIM_ENABLE_IT(&TIM1_Handler,TIM_IT_UPDATE);   //使能更新中断
}

/***************************************************************************
****************************************************************************
  TIM8通道捕获 
****************************************************************************
****************************************************************************/

TIM_HandleTypeDef TIM8_Handler;         //定时器8句柄 结构体命名

//定时器8通道1输入捕获配置
//arr：自动重装值(TIM1,TIM8是16位的!!)
//psc：时钟预分频数
//ch: 捕获通道选择，低4位每一位表示一个通道
void TIM8_Cap_Init(u16 arr,u16 psc,u8 ch)
{  
    TIM_IC_InitTypeDef TIM8_CH1Config;  
    TIM_IC_InitTypeDef TIM8_CH2Config;
	  TIM_IC_InitTypeDef TIM8_CH3Config;
	  TIM_IC_InitTypeDef TIM8_CH4Config;
	
	  GPIO_InitTypeDef GPIO_Initure;
   
  	TIM8_Handler.Instance=TIM8;                          //高级定时器8
    TIM8_Handler.Init.Prescaler=psc;                     //分频
    TIM8_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM8_Handler.Init.Period=arr;                        //自动装载值
    TIM8_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_IC_Init(&TIM8_Handler);
	
  if(ch&0x01)
	{
		GPIO_Initure.Pin=GPIO_PIN_6;            //PC6/7/8/9
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //复用推挽输
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //高速
    GPIO_Initure.Alternate=GPIO_AF3_TIM8;   //PC6复用为TIM8通道1...
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
		
		TIM8_CH1Config.ICPolarity=TIM_ICPOLARITY_RISING;    //上升沿捕获
    TIM8_CH1Config.ICSelection=TIM_ICSELECTION_DIRECTTI;//映射到TI1上
    TIM8_CH1Config.ICPrescaler=TIM_ICPSC_DIV1;          //配置输入分频，不分频
    TIM8_CH1Config.ICFilter=0x00;                          //配置输入滤波器，不滤波
    HAL_TIM_IC_ConfigChannel(&TIM8_Handler,&TIM8_CH1Config,TIM_CHANNEL_1);//配置TIM8通道1
		
		Direct_MCUtoCH(1,0);//低:CHtoMCU 高：MCUtoCH CH1设置为CHtoMCU
		HAL_TIM_IC_Start_IT(&TIM8_Handler,TIM_CHANNEL_1);   //开始捕获TIM8的通道1

	}
	if(ch&0x02)
	{
		GPIO_Initure.Pin=GPIO_PIN_7;            //PC6/7/8/9
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //复用推挽输出
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //高速
    GPIO_Initure.Alternate=GPIO_AF3_TIM8;   
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
		
	  TIM8_CH2Config.ICPolarity=TIM_ICPOLARITY_RISING;    //上升沿捕获
    TIM8_CH2Config.ICSelection=TIM_ICSELECTION_DIRECTTI;//映射到TI2上
    TIM8_CH2Config.ICPrescaler=TIM_ICPSC_DIV1;          //配置输入分频，不分频
    TIM8_CH2Config.ICFilter=3;                          //配置输入滤波器，不滤波
    HAL_TIM_IC_ConfigChannel(&TIM8_Handler,&TIM8_CH2Config,TIM_CHANNEL_2);//配置TIM8通道2
		
		Direct_MCUtoCH(2,0);//低:CHtoMCU 高：MCUtoCH CH2设置为CHtoMCU
		HAL_TIM_IC_Start_IT(&TIM8_Handler,TIM_CHANNEL_2);   //开始捕获TIM8的通道2
		
	}
  if(ch&0x04)
	{	
		GPIO_Initure.Pin=GPIO_PIN_8;            //PC6/7/8/9
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //复用推挽输出
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //高速
    GPIO_Initure.Alternate=GPIO_AF3_TIM8;   
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
		
		TIM8_CH3Config.ICPolarity=TIM_ICPOLARITY_RISING;    //上升沿捕获
    TIM8_CH3Config.ICSelection=TIM_ICSELECTION_DIRECTTI;//映射到TI3上
    TIM8_CH3Config.ICPrescaler=TIM_ICPSC_DIV1;          //配置输入分频，不分频
    TIM8_CH3Config.ICFilter=3;                          //配置输入滤波器，不滤波
    HAL_TIM_IC_ConfigChannel(&TIM8_Handler,&TIM8_CH3Config,TIM_CHANNEL_3);//配置TIM8通道3
		
		Direct_MCUtoCH(3,0);//低:CHtoMCU 高：MCUtoCH CH3设置为CHtoMCU
		HAL_TIM_IC_Start_IT(&TIM8_Handler,TIM_CHANNEL_3);   //开始捕获TIM8的通道3
		
	}
	if(ch&0x08)
	{	
		GPIO_Initure.Pin=GPIO_PIN_9;            //PC6/7/8/9
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //复用推挽输出
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //高速
    GPIO_Initure.Alternate=GPIO_AF3_TIM8;   
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
		
		TIM8_CH4Config.ICPolarity=TIM_ICPOLARITY_RISING;    //上升沿捕获
    TIM8_CH4Config.ICSelection=TIM_ICSELECTION_DIRECTTI;//映射到TI4上
    TIM8_CH4Config.ICPrescaler=TIM_ICPSC_DIV1;          //配置输入分频，不分频
    TIM8_CH4Config.ICFilter=3;                          //配置输入滤波器，不滤波
    HAL_TIM_IC_ConfigChannel(&TIM8_Handler,&TIM8_CH4Config,TIM_CHANNEL_4);//配置TIM8通道4
		
		Direct_MCUtoCH(4,0);//低:CHtoMCU 高：MCUtoCH CH4设置为CHtoMCU
		HAL_TIM_IC_Start_IT(&TIM8_Handler,TIM_CHANNEL_4);   //开始捕获TIM8的通道4
		
	}	
	
    __HAL_TIM_ENABLE_IT(&TIM8_Handler,TIM_IT_UPDATE);   //使能更新中断
}

//定时1/8底层驱动，时钟使能，引脚配置
//此函数会被HAL_TIM_IC_Init()调用
//htim:定时器句柄
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
  if(htim->Instance==TIM1)
	{
		
    __HAL_RCC_TIM1_CLK_ENABLE();            //使能TIM1时钟
		__HAL_RCC_GPIOA_CLK_ENABLE();
    HAL_NVIC_SetPriority(TIM1_CC_IRQn ,2,0);    //设置中断优先级，抢占优先级2，子优先级0
    HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);          //开启ITM1中断   
	}	
	
	if(htim->Instance==TIM8)
	{
	  
    __HAL_RCC_TIM8_CLK_ENABLE();            //使能TIM8时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();			//开启GPIOC时钟
    HAL_NVIC_SetPriority(TIM8_CC_IRQn ,2,0);    //设置中断优先级，抢占优先级2，子优先级0
    HAL_NVIC_EnableIRQ(TIM8_CC_IRQn);          //开启ITM8中断   
	}
}
//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到低电平;1,已经捕获到低电平了.
//[5:0]:捕获低电平后溢出的次数(对于16位定时器来说,1us计数器加1,溢出时间:65535us)
u8  TIM1CH1_CAPTURE_STA=0;	 //输入捕获状态		    				
u16	TIM1CH1_CAPTURE_VAL[8]={0};	   //输入捕获值(TIM1/TIM8是16位)
u16 TIM1CH1_CAPTURE_PerioVAL[8]={0};//输入捕获周期值

u8  TIM1CH2_CAPTURE_STA;		 //输入捕获状态		    				
u16	TIM1CH2_CAPTURE_VAL;	   //输入捕获值
u16 TIM1CH2_CAPTURE_PerioVAL;//输入捕获周期值

u8  TIM1CH3_CAPTURE_STA;		 //输入捕获状态		    				
u16	TIM1CH3_CAPTURE_VAL;	   //输入捕获值
u16 TIM1CH3_CAPTURE_PerioVAL;//输入捕获周期值

u8  TIM1CH4_CAPTURE_STA;		 //输入捕获状态		    				
u16	TIM1CH4_CAPTURE_VAL;	   //输入捕获值
u16 TIM1CH4_CAPTURE_PerioVAL;//输入捕获周期值

//定时器1中断服务函数
void TIM1_CC_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM1_Handler);//定时器共用处理函数
}

//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到低电平;1,已经捕获到低电平了.
//[5:0]:捕获低电平后溢出的次数(对于16位定时器来说,1us计数器加1,溢出时间:65535us)
u8  TIM8CH1_CAPTURE_STA=0;	 //输入捕获状态		    				
u16	TIM8CH1_CAPTURE_VAL=0;	   //输入捕获值
u16 TIM8CH1_CAPTURE_PerioVAL;//输入捕获周期值

u8  TIM8CH2_CAPTURE_STA=0;	 //输入捕获状态		    				
u16	TIM8CH2_CAPTURE_VAL;	   //输入捕获值
u16 TIM8CH2_CAPTURE_PerioVAL;//输入捕获周期

u8  TIM8CH3_CAPTURE_STA=0;	  //输入捕获状态		    				
u16	TIM8CH3_CAPTURE_VAL;	   //输入捕获值
u16 TIM8CH3_CAPTURE_PerioVAL;//输入捕获周期值

u8  TIM8CH4_CAPTURE_STA=0;	 //输入捕获状态		    				
u16	TIM8CH4_CAPTURE_VAL;	   //输入捕获
u16 TIM8CH4_CAPTURE_PerioVAL;//输入捕获周期值


//定时器8中断服务函数
void TIM8_CC_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM8_Handler);//定时器共用处理函数
}

//定时器更新中断（计数溢出）中断处理回调函数， 该函数在HAL_TIM_IRQHandler中会被调用
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//更新中断（溢出）发生时执行
{
	if(htim->Instance==TIM1)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
		  if((TIM1CH1_CAPTURE_STA&0X80)==0)//还未成功捕获一个周期
			{
//				if(TIM1CH1_CAPTURE_STA&0X40)//已经捕获到高电平了
//				{
//					if((TIM1CH1_CAPTURE_STA&0X1F)==0X1F)//高电平之后的低电平时间太久，计数溢出超过量程
//					{
						TIM1CH1_CAPTURE_STA|=0X80;		//标记成功捕获了一次
//						TIM1CH1_CAPTURE_VAL=0XFFFF;
//					}
//			else TIM1CH1_CAPTURE_STA++;//计数溢出但没有错过最大量程
//				}	 
			}
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
    {
		  if((TIM1CH2_CAPTURE_STA&0X80)==0)//还未成功捕获一个周期
			{
				if(TIM1CH2_CAPTURE_STA&0X40)//已经捕获到高电平了
				{
					if((TIM1CH2_CAPTURE_STA&0X1F)==0X1F)//高电平之后的低电平时间太久，计数溢出超过量程
					{
						TIM1CH2_CAPTURE_STA|=0X80;		//标记成功捕获了一次
						TIM1CH2_CAPTURE_VAL=0XFFFF;
					}else TIM1CH2_CAPTURE_STA++;//计数溢出但没有错过最大量程
				}	 
			}
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
		{
			if((TIM1CH3_CAPTURE_STA&0X80)==0)//还未成功捕获一个周期
			{
				if(TIM1CH3_CAPTURE_STA&0X40)//已经捕获到高电平了
				{
					if((TIM1CH3_CAPTURE_STA&0X1F)==0X1F)//高电平之后的低电平时间太久，计数溢出超过量程
					{
						TIM1CH3_CAPTURE_STA|=0X80;		//标记成功捕获了一次
						TIM1CH3_CAPTURE_VAL=0XFFFF;
					}else TIM1CH3_CAPTURE_STA++;//计数溢出但没有错过最大量程
				}	 
			}
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
		{
			if((TIM1CH4_CAPTURE_STA&0X80)==0)//还未成功捕获一个周期
			{
				if(TIM1CH4_CAPTURE_STA&0X40)//已经捕获到高电平了
				{
					if((TIM1CH4_CAPTURE_STA&0X1F)==0X1F)//高电平之后的低电平时间太久，计数溢出超过量程
					{
						TIM1CH4_CAPTURE_STA|=0X80;		//标记成功捕获了一次
						TIM1CH4_CAPTURE_VAL=0XFFFF;
					}else TIM1CH4_CAPTURE_STA++;//计数溢出但没有错过最大量程
				}	 
			}
		}
  }
	if(htim->Instance==TIM8)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
		  if((TIM8CH1_CAPTURE_STA&0X80)==0)//还未成功捕获
			{
				if(TIM8CH1_CAPTURE_STA&0X40)//已经捕获到高电平了
				{
					if((TIM8CH1_CAPTURE_STA&0X1F)==0X1F)//高电平太长了
					{
						TIM8CH1_CAPTURE_STA|=0X80;		//标记成功捕获了一次
						TIM8CH1_CAPTURE_VAL=0XFFFF;
					}else TIM8CH1_CAPTURE_STA++;
				}	 
			}
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
    {
		 if((TIM8CH2_CAPTURE_STA&0X80)==0)//还未成功捕获
			{
				if(TIM8CH2_CAPTURE_STA&0X40)//已经捕获到高电平了
				{
					if((TIM8CH2_CAPTURE_STA&0X1F)==0X1F)//高电平太长了
					{
						TIM8CH2_CAPTURE_STA|=0X80;		//标记成功捕获了一次
						TIM8CH2_CAPTURE_VAL=0XFFFF;
					}else TIM8CH2_CAPTURE_STA++;
				}	 
			}
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
		{
		 if((TIM8CH3_CAPTURE_STA&0X80)==0)//还未成功捕获
			{
				if(TIM8CH3_CAPTURE_STA&0X40)//已经捕获到高电平了
				{
					if((TIM8CH3_CAPTURE_STA&0X1F)==0X1F)//高电平太长了
					{
						TIM8CH3_CAPTURE_STA|=0X80;		//标记成功捕获了一次
						TIM8CH3_CAPTURE_VAL=0XFFFF;
					}else TIM8CH3_CAPTURE_STA++;
				}	 
			}
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
		{
		 if((TIM8CH4_CAPTURE_STA&0X80)==0)//还未成功捕获
			{
				if(TIM8CH4_CAPTURE_STA&0X40)//已经捕获到高电平了
				{
					if((TIM8CH4_CAPTURE_STA&0X1F)==0X1F)//高电平太长了
					{
						TIM8CH4_CAPTURE_STA|=0X80;		//标记成功捕获了一次
						TIM8CH4_CAPTURE_VAL=0XFFFF;
					}else TIM8CH4_CAPTURE_STA++;
				}	 
			}
		}
  }		
}

int cnt=0;
u8 data=0x00;
//定时器输入捕获中断处理回调函数，该函数在HAL_TIM_IRQHandler中会被调用
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//捕获中断发生时执行
{
	if(htim->Instance==TIM1)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			if((TIM1CH1_CAPTURE_STA&0X80)==0)//还未成功捕获
			{
				if(cnt<8)
				{
					TIM1CH1_CAPTURE_VAL[cnt]=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_2)+1;
					TIM1CH1_CAPTURE_PerioVAL[cnt]=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_1)+1;
					cnt++;
				}
				else
				{
					TIM1CH1_CAPTURE_STA|=0X80;
					__HAL_TIM_DISABLE(&TIM1_Handler);        //关闭定时器1
	        __HAL_TIM_DISABLE_IT(&TIM1_Handler,TIM_IT_UPDATE); //失能更新中断
	        __HAL_TIM_SET_COUNTER(&TIM1_Handler,0);  //定时器计数清零
					cnt=0;
				}
	    }
			else
			{
				TIM1_Cap_DeInit();//关闭定时器1功能，停止复用引脚
				PAInit_DeTIM1();//初始化TIM1口为普通IO口
        HAL_TIM_PWM_DeInit(&TIM8_Handler);//关闭PWM
				PCInit_DeTIM8();//初始化为普通IO口
			}
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
		{
			if((TIM1CH2_CAPTURE_STA&0X80)==0)//还未成功捕获一个周期
			{
				if(TIM1CH2_CAPTURE_STA&0X40)//第二次捕获上升沿，一个周期计数完
				{
					TIM1CH2_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_2)-TIM1CH2_CAPTURE_PerioVAL;//获取当前的捕获值.
					TIM1CH2_CAPTURE_STA|=0X80;		//标记捕获第二个上升沿
					__HAL_TIM_DISABLE_IT(&TIM1_Handler,TIM_IT_UPDATE);
					__HAL_TIM_DISABLE(&TIM1_Handler);        //关闭定时器1
					__HAL_TIM_SET_COUNTER(&TIM1_Handler,0);  //让定时器从零开始计数
//					TIM_RESET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_2);   //一定要先清除原来的设置！！
//					TIM_SET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_2,TIM_ICPOLARITY_RISING);//定时器1通道1设置恢复初始的上升沿捕获
					__HAL_TIM_ENABLE(&TIM1_Handler);//使能定时器1
					__HAL_TIM_ENABLE_IT(&TIM1_Handler,TIM_IT_UPDATE);
				}
//				else if((TIM1CH2_CAPTURE_STA&0X20))//&&((TIM1CH1_CAPTURE_STA&0X40)==0))		//捕获下降沿 ，且不会应为第二次捕获上升沿进入这个判断		
//				{	  			
//					TIM1CH2_CAPTURE_STA|=0X40;		//标记成功捕获到一次高电平脉宽
//					TIM1CH2_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_2)-TIM1CH2_CAPTURE_VAL;//获取当前的捕获值.
//					__HAL_TIM_DISABLE(&TIM1_Handler);        //关闭定时器1
//					TIM_RESET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_2);   //一定要先清除原来的设置！！
//					TIM_SET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_2,TIM_ICPOLARITY_RISING);//配置TIM8通道1上升沿捕获
//					__HAL_TIM_ENABLE(&TIM1_Handler);//使能定时器1
//					
//				}
				else if(TIM1CH2_CAPTURE_STA==0)        //第一次捕获上升沿
				{
					TIM1CH2_CAPTURE_STA=0;			//清空
//					TIM1CH2_CAPTURE_VAL=0;
					TIM1CH2_CAPTURE_PerioVAL=0;
					TIM1CH2_CAPTURE_STA|=0X40;		//标记捕获到了第一个上升沿
					//TIM1CH2_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_2);//获取当前的捕获值.
					TIM1CH2_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_2);
//					__HAL_TIM_DISABLE(&TIM1_Handler);        //关闭定时器1
//		      __HAL_TIM_SET_COUNTER(&TIM1_Handler,0);  //让定时器从零开始计数
//					TIM_RESET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_2);   //一定要先清除原来的设置！！
//					TIM_SET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_2,TIM_ICPOLARITY_FALLING);//定时器1通道1设置为下降沿捕获
//					__HAL_TIM_ENABLE(&TIM1_Handler);//使能定时器1
				}		    
			}
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
		{
			if((TIM1CH3_CAPTURE_STA&0X80)==0)//还未成功捕获一个周期
			{
				if(TIM1CH3_CAPTURE_STA&0X40)//第二次捕获上升沿，一个周期计数完
				{
					TIM1CH3_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_3)-TIM1CH3_CAPTURE_PerioVAL;//获取当前的捕获值.
					TIM1CH3_CAPTURE_STA|=0X80;		//标记捕获第二个上升沿
					__HAL_TIM_DISABLE_IT(&TIM1_Handler,TIM_IT_UPDATE);
					__HAL_TIM_DISABLE(&TIM1_Handler);        //关闭定时器1
					__HAL_TIM_SET_COUNTER(&TIM1_Handler,0);  //让定时器从零开始计数
//					TIM_RESET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_3);   //一定要先清除原来的设置！！
//					TIM_SET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_3,TIM_ICPOLARITY_RISING);//定时器1通道1设置恢复初始的上升沿捕获
					__HAL_TIM_ENABLE(&TIM1_Handler);//使能定时器1
					__HAL_TIM_ENABLE_IT(&TIM1_Handler,TIM_IT_UPDATE);
				}
//				else if((TIM1CH3_CAPTURE_STA&0X20))//&&((TIM1CH1_CAPTURE_STA&0X40)==0))		//捕获下降沿 ，且不会应为第二次捕获上升沿进入这个判断		
//				{	  			
//					TIM1CH3_CAPTURE_STA|=0X40;		//标记成功捕获到一次高电平脉宽
//					TIM1CH3_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_1)-TIM1CH3_CAPTURE_VAL;//获取当前的捕获值.
//					__HAL_TIM_DISABLE(&TIM1_Handler);        //关闭定时器1
//					TIM_RESET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_3);   //一定要先清除原来的设置！！
//					TIM_SET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_3,TIM_ICPOLARITY_RISING);//配置TIM8通道1上升沿捕获
//					__HAL_TIM_ENABLE(&TIM1_Handler);//使能定时器1
//					
//				}
				else if(TIM1CH3_CAPTURE_STA==0)        //第一次捕获上升沿
				{
					TIM1CH3_CAPTURE_STA=0;			//清空
//					TIM1CH3_CAPTURE_VAL=0;
					TIM1CH3_CAPTURE_PerioVAL=0;
					TIM1CH3_CAPTURE_STA|=0X40;		//标记捕获到了第一个上升沿
//					TIM1CH3_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_3);//获取当前的捕获值.
					TIM1CH3_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_3);
//					__HAL_TIM_DISABLE(&TIM1_Handler);        //关闭定时器1
					//__HAL_TIM_SET_COUNTER(&TIM1_Handler,0);  //让定时器从零开始计数
//					TIM_RESET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_3);   //一定要先清除原来的设置！！
//					TIM_SET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_3,TIM_ICPOLARITY_FALLING);//定时器1通道1设置为下降沿捕获
//					__HAL_TIM_ENABLE(&TIM1_Handler);//使能定时器1
				}		    
			}
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
    {
			if((TIM1CH4_CAPTURE_STA&0X80)==0)//还未成功捕获一个周期
			{
				if(TIM1CH4_CAPTURE_STA&0X40)//第二次捕获上升沿，一个周期计数完
				{
					TIM1CH4_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_4)-TIM1CH4_CAPTURE_PerioVAL;//获取当前的捕获值.
					TIM1CH4_CAPTURE_STA|=0X80;		//标记捕获第二个上升沿
					__HAL_TIM_DISABLE_IT(&TIM1_Handler,TIM_IT_UPDATE);
					__HAL_TIM_DISABLE(&TIM1_Handler);        //关闭定时器1
					__HAL_TIM_SET_COUNTER(&TIM1_Handler,0);  //让定时器从零开始计数
//					TIM_RESET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_4);   //一定要先清除原来的设置！！
//					TIM_SET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_4,TIM_ICPOLARITY_RISING);//定时器1通道1设置恢复初始的上升沿捕获
					__HAL_TIM_ENABLE(&TIM1_Handler);//使能定时器1
					__HAL_TIM_ENABLE_IT(&TIM1_Handler,TIM_IT_UPDATE);
				}
//				else if((TIM1CH4_CAPTURE_STA&0X20))//&&((TIM1CH1_CAPTURE_STA&0X40)==0))		//捕获下降沿 ，且不会应为第二次捕获上升沿进入这个判断		
//				{	  			
//					TIM1CH4_CAPTURE_STA|=0X40;		//标记成功捕获到一次高电平脉宽
//					TIM1CH4_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_4)-TIM1CH4_CAPTURE_VAL;//获取当前的捕获值.
//					__HAL_TIM_DISABLE(&TIM1_Handler);        //关闭定时器1
//					TIM_RESET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_4);   //一定要先清除原来的设置！！
//					TIM_SET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_4,TIM_ICPOLARITY_RISING);//配置TIM8通道1上升沿捕获
//					__HAL_TIM_ENABLE(&TIM1_Handler);//使能定时器1
//					
//				}
				else if(TIM1CH4_CAPTURE_STA==0)        //第一次捕获上升沿
				{
					TIM1CH4_CAPTURE_STA=0;			//清空
//					TIM1CH4_CAPTURE_VAL=0;
					TIM1CH4_CAPTURE_PerioVAL=0;
					TIM1CH4_CAPTURE_STA|=0X40;		//标记捕获到了第一个上升沿
//					TIM1CH4_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_4);//获取当前的捕获值.
					TIM1CH4_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_4);
//					__HAL_TIM_DISABLE(&TIM1_Handler);        //关闭定时器1
//					//__HAL_TIM_SET_COUNTER(&TIM1_Handler,0);  //让定时器从零开始计数
//					TIM_RESET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_4);   //一定要先清除原来的设置！！
//					TIM_SET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_4,TIM_ICPOLARITY_FALLING);//定时器1通道1设置为下降沿捕获
//					__HAL_TIM_ENABLE(&TIM1_Handler);//使能定时器1
				}		    
			}
		}			
  }
	
	if(htim->Instance==TIM8)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			if((TIM8CH1_CAPTURE_STA&0X80)==0)//还未成功捕获
			{
   			if(TIM8CH1_CAPTURE_STA&0X40)//第二次捕获上升沿，一个周期计数完
				{
					TIM8CH1_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_1)-TIM8CH1_CAPTURE_PerioVAL;//获取当前的捕获值.
					TIM8CH1_CAPTURE_STA|=0X80;		//标记捕获第二个上升沿
					__HAL_TIM_DISABLE(&TIM8_Handler);        //关闭定时器8
					__HAL_TIM_SET_COUNTER(&TIM8_Handler,0);  //让定时器从零开始计数
					TIM_RESET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_1);   //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);//定时器8通道2设置恢复初始的上升沿捕获
					__HAL_TIM_ENABLE(&TIM8_Handler);//使能定时器8
				}
				else if(TIM8CH1_CAPTURE_STA&0X20)  								//还未开始,第一次捕获上升沿
				{
					TIM8CH1_CAPTURE_STA|=0X40;		//标记成功捕获到一次高电平脉宽
					TIM8CH1_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_1)-TIM8CH1_CAPTURE_VAL;//获取当前的捕获值.
					__HAL_TIM_DISABLE(&TIM8_Handler);        //关闭定时器8
					TIM_RESET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_1);   //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);//配置TIM8通道2上升沿捕获
					__HAL_TIM_ENABLE(&TIM8_Handler);//使能定时器8
				}
        else if(TIM8CH1_CAPTURE_STA==0)        //第一次捕获上升沿
				{
					TIM8CH1_CAPTURE_STA=0;			//清空
					TIM8CH1_CAPTURE_VAL=0;
					TIM8CH1_CAPTURE_PerioVAL=0;
					TIM8CH1_CAPTURE_STA|=0X20;		//标记捕获到了第一个上升沿
					TIM8CH1_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_1);
					TIM8CH1_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_1);
					__HAL_TIM_DISABLE(&TIM8_Handler);        //关闭定时器1
					//__HAL_TIM_SET_COUNTER(&TIM8_Handler,0);  //让定时器从零开始计数
					TIM_RESET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_1);   //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);//定时器8通道2设置为下降沿捕获
					__HAL_TIM_ENABLE(&TIM8_Handler);//使能定时器8
				}	
      }				
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
    {
		 if((TIM8CH2_CAPTURE_STA&0X80)==0)//还未成功捕获
			{
				if(TIM8CH2_CAPTURE_STA&0X40)//第二次捕获上升沿，一个周期计数完
				{
					TIM8CH2_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_2)-TIM8CH2_CAPTURE_PerioVAL;//获取当前的捕获值.
					TIM8CH2_CAPTURE_STA|=0X80;		//标记捕获第二个上升沿
					__HAL_TIM_DISABLE(&TIM8_Handler);        //关闭定时器8
					__HAL_TIM_SET_COUNTER(&TIM8_Handler,0);  //让定时器从零开始计数
					TIM_RESET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_2);   //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_2,TIM_ICPOLARITY_RISING);//定时器8通道2设置恢复初始的上升沿捕获
					__HAL_TIM_ENABLE(&TIM8_Handler);//使能定时器8
				}
				else if(TIM8CH2_CAPTURE_STA&0X20)  								//还未开始,第一次捕获上升沿
				{
					TIM8CH2_CAPTURE_STA|=0X40;		//标记成功捕获到一次高电平脉宽
					TIM8CH2_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_2)-TIM8CH2_CAPTURE_VAL;//获取当前的捕获值.
					__HAL_TIM_DISABLE(&TIM8_Handler);        //关闭定时器8
					TIM_RESET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_2);   //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_2,TIM_ICPOLARITY_RISING);//配置TIM8通道2上升沿捕获
					__HAL_TIM_ENABLE(&TIM8_Handler);//使能定时器8
				}
        else if(TIM8CH2_CAPTURE_STA==0)        //第一次捕获上升沿
				{
					TIM8CH2_CAPTURE_STA=0;			//清空
					TIM8CH2_CAPTURE_VAL=0;
					TIM8CH2_CAPTURE_PerioVAL=0;
					TIM8CH2_CAPTURE_STA|=0X20;		//标记捕获到了第一个上升沿
					TIM8CH2_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_2);
					TIM8CH2_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_2);
					__HAL_TIM_DISABLE(&TIM8_Handler);        //关闭定时器1
					//__HAL_TIM_SET_COUNTER(&TIM8_Handler,0);  //让定时器从零开始计数
					TIM_RESET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_2);   //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_2,TIM_ICPOLARITY_FALLING);//定时器8通道2设置为下降沿捕获
					__HAL_TIM_ENABLE(&TIM8_Handler);//使能定时器8
				}		    
			}	
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
		{
		 if((TIM8CH3_CAPTURE_STA&0X80)==0)//还未成功捕获
			{
				if(TIM8CH3_CAPTURE_STA&0X40)//第二次捕获上升沿，一个周期计数完
				{
					TIM8CH3_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_3)-TIM8CH3_CAPTURE_PerioVAL;//获取当前的捕获值.
					TIM8CH3_CAPTURE_STA|=0X80;		//标记捕获第二个上升沿
					__HAL_TIM_DISABLE(&TIM8_Handler);        //关闭定时器8
					__HAL_TIM_SET_COUNTER(&TIM8_Handler,0);  //让定时器从零开始计数
					TIM_RESET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_3);   //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_3,TIM_ICPOLARITY_RISING);//定时器8通道3设置恢复初始的上升沿捕获
					__HAL_TIM_ENABLE(&TIM8_Handler);//使能定时器8
				}
				else if(TIM8CH3_CAPTURE_STA&0X20)  								//还未开始,第一次捕获上升沿
				{
					TIM8CH3_CAPTURE_STA|=0X40;		//标记成功捕获到一次高电平脉宽
					TIM8CH3_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_3)-TIM8CH3_CAPTURE_VAL;//获取当前的捕获值.
					__HAL_TIM_DISABLE(&TIM8_Handler);        //关闭定时器8
					TIM_RESET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_3);   //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_3,TIM_ICPOLARITY_RISING);//配置TIM8通道3上升沿捕获
					__HAL_TIM_ENABLE(&TIM8_Handler);//使能定时器8
				}
        else if(TIM8CH3_CAPTURE_STA==0)        //第一次捕获上升沿
				{
					TIM8CH3_CAPTURE_STA=0;			//清空
					TIM8CH3_CAPTURE_VAL=0;
					TIM8CH3_CAPTURE_PerioVAL=0;
					TIM8CH3_CAPTURE_STA|=0X20;		//标记捕获到了第一个上升沿
					TIM8CH3_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_3);
					TIM8CH3_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_3);
					__HAL_TIM_DISABLE(&TIM8_Handler);        //关闭定时器8
					//__HAL_TIM_SET_COUNTER(&TIM8_Handler,0);  //让定时器从零开始计数
					TIM_RESET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_3);   //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_3,TIM_ICPOLARITY_FALLING);//定时器8通道2设置为下降沿捕获
					__HAL_TIM_ENABLE(&TIM8_Handler);//使能定时器8
				}		    
			}	
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
		{
		 if((TIM8CH4_CAPTURE_STA&0X80)==0)//还未成功捕获
			{
				if(TIM8CH4_CAPTURE_STA&0X40)//第二次捕获上升沿，一个周期计数完
				{
					TIM8CH4_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_4)-TIM8CH4_CAPTURE_PerioVAL;//获取当前的捕获值.
					TIM8CH4_CAPTURE_STA|=0X80;		//标记捕获第二个上升沿
					__HAL_TIM_DISABLE(&TIM8_Handler);        //关闭定时器8
					__HAL_TIM_SET_COUNTER(&TIM8_Handler,0);  //让定时器从零开始计数
					TIM_RESET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_4);   //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_4,TIM_ICPOLARITY_RISING);//定时器8通道4设置恢复初始的上升沿捕获
					__HAL_TIM_ENABLE(&TIM8_Handler);//使能定时器8
				}
				else if(TIM8CH4_CAPTURE_STA&0X20)  								//还未开始,第一次捕获上升沿
				{
					TIM8CH4_CAPTURE_STA|=0X40;		//标记成功捕获到一次高电平脉宽
					TIM8CH4_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_4)-TIM8CH4_CAPTURE_VAL;//获取当前的捕获值.
					__HAL_TIM_DISABLE(&TIM8_Handler);        //关闭定时器8
					TIM_RESET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_4);   //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_4,TIM_ICPOLARITY_RISING);//配置TIM8通道4上升沿捕获
					__HAL_TIM_ENABLE(&TIM8_Handler);//使能定时器8
				}
        else if(TIM8CH4_CAPTURE_STA==0)        //第一次捕获上升沿
				{
					TIM8CH4_CAPTURE_STA=0;			//清空
					TIM8CH4_CAPTURE_VAL=0;
					TIM8CH4_CAPTURE_PerioVAL=0;
					TIM8CH4_CAPTURE_STA|=0X20;		//标记捕获到了第一个上升沿
					TIM8CH4_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_4);
					TIM8CH4_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_4);
					__HAL_TIM_DISABLE(&TIM8_Handler);        //关闭定时器
					//__HAL_TIM_SET_COUNTER(&TIM8_Handler,0);  //让定时器从零开始计数，会有500ns的计数误差
					TIM_RESET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_4);   //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_4,TIM_ICPOLARITY_FALLING);//定时器8通道2设置为下降沿捕获
					__HAL_TIM_ENABLE(&TIM8_Handler);//使能定时器8
				}		    
			}	
		}		
	}
}

//关闭、清除初始化设置
void TIM1_Cap_DeInit(void)
{
//	HAL_TIM_IC_DeInit(&TIM1_Handler);//会导致再次测量时，后面的功能测试数据出现错误，可能是再次初始化用时较长
	
	HAL_GPIO_DeInit(GPIOA,GPIO_PIN_8);
	HAL_GPIO_DeInit(GPIOA,GPIO_PIN_9);
	HAL_GPIO_DeInit(GPIOA,GPIO_PIN_10);
	HAL_GPIO_DeInit(GPIOA,GPIO_PIN_11);
	
	HAL_TIM_IC_Stop_IT(&TIM1_Handler,TIM_CHANNEL_1);   //停止捕获TIM1的通道
	HAL_TIM_IC_Stop_IT(&TIM1_Handler,TIM_CHANNEL_2);   //停止捕获TIM1的通道
	HAL_TIM_IC_Stop_IT(&TIM1_Handler,TIM_CHANNEL_3);   //停止捕获TIM1的通道
	HAL_TIM_IC_Stop_IT(&TIM1_Handler,TIM_CHANNEL_4);   //停止捕获TIM1的通道
	
	__HAL_TIM_DISABLE(&TIM1_Handler);        //关闭定时器1
	__HAL_TIM_DISABLE_IT(&TIM1_Handler,TIM_IT_UPDATE); //失能更新中断
	__HAL_TIM_SET_COUNTER(&TIM1_Handler,0);  //定时器计数清零
}

void TIM8_Cap_DeInit(void)
{
	HAL_TIM_IC_DeInit(&TIM8_Handler);
	
	HAL_GPIO_DeInit(GPIOC,GPIO_PIN_6);
	HAL_GPIO_DeInit(GPIOC,GPIO_PIN_7);
	HAL_GPIO_DeInit(GPIOC,GPIO_PIN_8);
	HAL_GPIO_DeInit(GPIOC,GPIO_PIN_9);
	
	HAL_TIM_IC_Stop_IT(&TIM8_Handler,TIM_CHANNEL_1);   //停止捕获TIM的通道
	HAL_TIM_IC_Stop_IT(&TIM8_Handler,TIM_CHANNEL_2);  
	HAL_TIM_IC_Stop_IT(&TIM8_Handler,TIM_CHANNEL_3);   
	HAL_TIM_IC_Stop_IT(&TIM8_Handler,TIM_CHANNEL_4);  
	
	__HAL_TIM_DISABLE_IT(&TIM8_Handler,TIM_IT_UPDATE); //失能更新中断
	
	PCInit_DeTIM8();//初始化为普通IO口
}
//定时器底层驱动，时钟失能，关闭中断
//此函数会被HAL_TIM_IC_DeInit()调用
//htim:定时器句柄
void HAL_TIM_IC_MspDeInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM1)
	{
		__HAL_RCC_TIM1_CLK_DISABLE();            //失能时钟
    HAL_NVIC_DisableIRQ(TIM1_CC_IRQn);          //关闭TIM1中断   
	}
	if(htim->Instance==TIM8)
	{
		__HAL_RCC_TIM8_CLK_DISABLE();            //失能时钟
    HAL_NVIC_DisableIRQ(TIM8_CC_IRQn);          //关闭TIM8中断
	}
}
