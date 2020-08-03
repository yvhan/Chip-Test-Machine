#include "timer.h"
#include "led.h"

TIM_HandleTypeDef TIM3_Handler;      	//∂® ±∆˜æ‰±˙ 
TIM_OC_InitTypeDef TIM3_CH4Handler;     //∂® ±∆˜3Õ®µ¿4æ‰±˙


//Õ®”√∂® ±∆˜3÷–∂œ≥ı ºªØ,∂® ±∆˜3‘⁄APB1…œ£¨APB1µƒ∂® ±∆˜ ±÷”Œ™200MHz
//arr£∫◊‘∂Ø÷ÿ◊∞÷µ°£
//psc£∫ ±÷”‘§∑÷∆µ ˝
//∂® ±∆˜“Á≥ˆ ±º‰º∆À„∑Ω∑®:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=∂® ±∆˜π§◊˜∆µ¬ ,µ•Œª:Mhz
//’‚¿Ô π”√µƒ «∂® ±∆˜3!(∂® ±∆˜3π“‘⁄APB1…œ£¨ ±÷”Œ™HCLK/2)
void TIM3_Init(u16 arr,u16 psc)
{  
    TIM3_Handler.Instance=TIM3;                          //Õ®”√∂® ±∆˜3
    TIM3_Handler.Init.Prescaler=psc;                     //∑÷∆µ
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //œÚ…œº∆ ˝∆˜
    TIM3_Handler.Init.Period=arr;                        //◊‘∂Ø◊∞‘ÿ÷µ
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;// ±÷”∑÷∆µ“Ú◊”
    HAL_TIM_Base_Init(&TIM3_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM3_Handler); // πƒ‹∂® ±∆˜3∫Õ∂® ±∆˜3∏¸–¬÷–∂œ£∫TIM_IT_UPDATE    
}

//TIM3 PWM≤ø∑÷≥ı ºªØ 
//PWM ‰≥ˆ≥ı ºªØ
//arr£∫◊‘∂Ø÷ÿ◊∞÷µ
//psc£∫ ±÷”‘§∑÷∆µ ˝
void TIM3_PWM_Init(u16 arr,u16 psc)
{ 
    TIM3_Handler.Instance=TIM3;            //∂® ±∆˜3
    TIM3_Handler.Init.Prescaler=psc;       //∂® ±∆˜∑÷∆µ
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//œÚ…œº∆ ˝ƒ£ Ω
    TIM3_Handler.Init.Period=arr;          //◊‘∂Ø÷ÿ◊∞‘ÿ÷µ
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM3_Handler);       //≥ı ºªØPWM
    
    TIM3_CH4Handler.OCMode=TIM_OCMODE_PWM1; //ƒ£ Ω—°‘ÒPWM1
    TIM3_CH4Handler.Pulse=arr/2;            //…Ë÷√±»Ωœ÷µ,¥À÷µ”√¿¥»∑∂®’ºø’±»£¨
                                            //ƒ¨»œ±»Ωœ÷µŒ™◊‘∂Ø÷ÿ◊∞‘ÿ÷µµƒ“ª∞Î,º¥’ºø’±»Œ™50%
    TIM3_CH4Handler.OCPolarity=TIM_OCPOLARITY_LOW; // ‰≥ˆ±»Ωœº´–‘Œ™µÕ 
    HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH4Handler,TIM_CHANNEL_4);//≈‰÷√TIM3Õ®µ¿4
    HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_4);//ø™∆ÙPWMÕ®µ¿4
}

//∂® ±∆˜µ◊≤„«˝∂Ø£¨ ±÷” πƒ‹£¨“˝Ω≈≈‰÷√
//¥À∫Ø ˝ª·±ªHAL_TIM_PWM_Init()µ˜”√
//htim:∂® ±∆˜æ‰±˙
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
	  __HAL_RCC_TIM3_CLK_ENABLE();			// πƒ‹∂® ±∆˜3
    __HAL_RCC_GPIOB_CLK_ENABLE();			//ø™∆ÙGPIOB ±÷”
	
    GPIO_Initure.Pin=GPIO_PIN_1;           	//PB1
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//∏¥”√Õ∆ÕÍ ‰≥ˆ
    GPIO_Initure.Pull=GPIO_PULLUP;          //…œ¿≠
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //∏ﬂÀŸ
	  GPIO_Initure.Alternate=GPIO_AF2_TIM3;	//PB1∏¥”√Œ™TIM3_CH4
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
}

//∂® ±∆˜µ◊≤·«˝∂Ø£¨ø™∆Ù ±÷”£¨…Ë÷√÷–∂œ”≈œ»º∂
//¥À∫Ø ˝ª·±ªHAL_TIM_Base_Init()∫Ø ˝µ˜”√
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();            // πƒ‹TIM3 ±÷”
		HAL_NVIC_SetPriority(TIM3_IRQn,1,3);    //…Ë÷√÷–∂œ”≈œ»º∂£¨«¿’º”≈œ»º∂1£¨◊””≈œ»º∂3
		HAL_NVIC_EnableIRQ(TIM3_IRQn);          //ø™∆ÙITM3÷–∂œ   
	}  
}

//∂® ±∆˜3÷–∂œ∑˛ŒÒ∫Ø ˝
void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM3_Handler);
}


//…Ë÷√TIMÕ®µ¿4µƒ’ºø’±»
//compare:±»Ωœ÷µ
void TIM_SetTIM3Compare4(u32 compare)
{
	TIM3->CCR4=compare; 
}

//ªÒ»°TIM≤∂ªÒ/±»Ωœºƒ¥Ê∆˜÷µ
u32 TIM_GetTIM3Capture4(void)
{
	return  HAL_TIM_ReadCapturedValue(&TIM3_Handler,TIM_CHANNEL_4);
}
/***************************************************************************
****************************************************************************
  TIM1Õ®µ¿≤∂ªÒ
****************************************************************************
****************************************************************************/

TIM_HandleTypeDef TIM1_Handler;         //∂® ±∆˜8æ‰±˙ Ω·ππÃÂ√¸√˚

//∂® ±1Õ®µ¿1 ‰»Î≤∂ªÒ≈‰÷√
//arr£∫◊‘∂Ø÷ÿ◊∞÷µ(TIM1,TIM8 «16Œªµƒ!!)
//psc£∫ ±÷”‘§∑÷∆µ ˝
//ch: ≤∂ªÒÕ®µ¿—°‘Ò
void TIM1_Cap_Init(u16 arr,u16 psc,u8 ch)
{  
	TIM_IC_InitTypeDef TIM1_CH1Config; 
	TIM_IC_InitTypeDef TIM1_CH2Config;	
	TIM_IC_InitTypeDef TIM1_CH3Config; 
	TIM_IC_InitTypeDef TIM1_CH4Config;
	
	GPIO_InitTypeDef GPIO_Initure;

	TIM1_Handler.Instance=TIM1;                          //∏ﬂº∂∂® ±∆˜1
	TIM1_Handler.Init.Prescaler=psc;                     //∑÷∆µ
	TIM1_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //œÚ…œº∆ ˝∆˜
	TIM1_Handler.Init.Period=arr;                        //◊‘∂Ø◊∞‘ÿ÷µ
	TIM1_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_IC_Init(&TIM1_Handler);
    
	if(ch&0x01)
	{
    GPIO_Initure.Pin=GPIO_PIN_8;//|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;            //PA8\9\10\11
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //∏¥”√Õ∆ÕÏ ‰≥ˆ
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //œ¬¿≠
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //∏ﬂÀŸ
    GPIO_Initure.Alternate=GPIO_AF1_TIM1;   
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
		
    TIM1_CH1Config.ICPolarity=TIM_ICPOLARITY_RISING;    //…œ…˝—ÿ≤∂ªÒ
    TIM1_CH1Config.ICSelection=TIM_ICSELECTION_DIRECTTI;//”≥…‰µΩTI1…œ
    TIM1_CH1Config.ICPrescaler=TIM_ICPSC_DIV1;          //≈‰÷√ ‰»Î∑÷∆µ£¨≤ª∑÷∆µ
    TIM1_CH1Config.ICFilter=0;                          //≈‰÷√ ‰»Î¬À≤®∆˜£¨≤ª¬À≤®
    HAL_TIM_IC_ConfigChannel(&TIM1_Handler,&TIM1_CH1Config,TIM_CHANNEL_1);//≈‰÷√TIM1Õ®µ¿1
		
		HAL_TIM_IC_Start_IT(&TIM1_Handler,TIM_CHANNEL_1);   //ø™ º≤∂ªÒTIM1µƒÕ®µ¿1
	}
	if(ch&0x02)
	{ 
    GPIO_Initure.Pin=GPIO_PIN_9;//|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;            //PA8\9\10\11
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //∏¥”√Õ∆ÕÏ ‰≥ˆ
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //œ¬¿≠
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //∏ﬂÀŸ
    GPIO_Initure.Alternate=GPIO_AF1_TIM1;  
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
		
	  TIM1_CH2Config.ICPolarity=TIM_ICPOLARITY_RISING;    //…œ…˝—ÿ≤∂ªÒ
    TIM1_CH2Config.ICSelection=TIM_ICSELECTION_DIRECTTI;//”≥…‰µΩTI2…œ
    TIM1_CH2Config.ICPrescaler=TIM_ICPSC_DIV1;          //≈‰÷√ ‰»Î∑÷∆µ£¨≤ª∑÷∆µ
    TIM1_CH2Config.ICFilter=0;                          //≈‰÷√ ‰»Î¬À≤®∆˜£¨≤ª¬À≤®
    HAL_TIM_IC_ConfigChannel(&TIM1_Handler,&TIM1_CH2Config,TIM_CHANNEL_2);//≈‰÷√TIM1Õ®µ¿2
		
		HAL_TIM_IC_Start_IT(&TIM1_Handler,TIM_CHANNEL_2);   //ø™ º≤∂ªÒTIM1µƒÕ®µ¿2
	}
  if(ch&0x04)
	{	
    GPIO_Initure.Pin=GPIO_PIN_10;//|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;            //PA8\9\10\11
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //∏¥”√Õ∆ÕÏ ‰≥ˆ
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //œ¬¿≠
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //∏ﬂÀŸ
    GPIO_Initure.Alternate=GPIO_AF1_TIM1;   
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
		
		TIM1_CH3Config.ICPolarity=TIM_ICPOLARITY_RISING;    //…œ…˝—ÿ≤∂ªÒ
    TIM1_CH3Config.ICSelection=TIM_ICSELECTION_DIRECTTI;//”≥…‰µΩTI3…œ
    TIM1_CH3Config.ICPrescaler=TIM_ICPSC_DIV1;          //≈‰÷√ ‰»Î∑÷∆µ£¨≤ª∑÷∆µ
    TIM1_CH3Config.ICFilter=0;                          //≈‰÷√ ‰»Î¬À≤®∆˜£¨≤ª¬À≤®
    HAL_TIM_IC_ConfigChannel(&TIM1_Handler,&TIM1_CH3Config,TIM_CHANNEL_3);//≈‰÷√TIM1Õ®µ¿3
		
		HAL_TIM_IC_Start_IT(&TIM1_Handler,TIM_CHANNEL_3);   //ø™ º≤∂ªÒTIM1µƒÕ®µ¿3
	}
	if(ch&0x08)
	{
    GPIO_Initure.Pin=GPIO_PIN_11;//|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;            //PA8\9\10\11
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //∏¥”√Õ∆ÕÏ ‰≥ˆ
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //œ¬¿≠
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //∏ﬂÀŸ
    GPIO_Initure.Alternate=GPIO_AF1_TIM1;  
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
		
		TIM1_CH4Config.ICPolarity=TIM_ICPOLARITY_RISING;    //…œ…˝—ÿ≤∂ªÒ
    TIM1_CH4Config.ICSelection=TIM_ICSELECTION_DIRECTTI;//”≥…‰µΩTI4…œ
    TIM1_CH4Config.ICPrescaler=TIM_ICPSC_DIV1;          //≈‰÷√ ‰»Î∑÷∆µ£¨≤ª∑÷∆µ
    TIM1_CH4Config.ICFilter=0;                          //≈‰÷√ ‰»Î¬À≤®∆˜£¨≤ª¬À≤®
    HAL_TIM_IC_ConfigChannel(&TIM1_Handler,&TIM1_CH4Config,TIM_CHANNEL_4);//≈‰÷√TIM1Õ®µ¿4
		
		HAL_TIM_IC_Start_IT(&TIM1_Handler,TIM_CHANNEL_4);   //ø™ º≤∂ªÒTIM1µƒÕ®µ¿4
	}
    
    __HAL_TIM_ENABLE_IT(&TIM1_Handler,TIM_IT_UPDATE);   // πƒ‹∏¸–¬÷–∂œ
}

/***************************************************************************
****************************************************************************
  TIM8Õ®µ¿≤∂ªÒ
****************************************************************************
****************************************************************************/

TIM_HandleTypeDef TIM8_Handler;         //∂® ±∆˜8æ‰±˙ Ω·ππÃÂ√¸√˚

//∂® ±∆˜8Õ®µ¿1 ‰»Î≤∂ªÒ≈‰÷√
//arr£∫◊‘∂Ø÷ÿ◊∞÷µ(TIM1,TIM8 «16Œªµƒ!!)
//psc£∫ ±÷”‘§∑÷∆µ ˝
//ch: ≤∂ªÒÕ®µ¿—°‘Ò
void TIM8_Cap_Init(u16 arr,u16 psc,u8 ch)
{  
    TIM_IC_InitTypeDef TIM8_CH1Config;  
    TIM_IC_InitTypeDef TIM8_CH2Config;
	  TIM_IC_InitTypeDef TIM8_CH3Config;
	  TIM_IC_InitTypeDef TIM8_CH4Config;
	
	  GPIO_InitTypeDef GPIO_Initure;
   
  	TIM8_Handler.Instance=TIM8;                          //∏ﬂº∂∂® ±∆˜8
    TIM8_Handler.Init.Prescaler=psc;                     //∑÷∆µ
    TIM8_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //œÚ…œº∆ ˝∆˜
    TIM8_Handler.Init.Period=arr;                        //◊‘∂Ø◊∞‘ÿ÷µ
    TIM8_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_IC_Init(&TIM8_Handler);
	
  if(ch&0x01)
	{
		GPIO_Initure.Pin=GPIO_PIN_6;            //PC6/7/8/9
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //∏¥”√Õ∆ÕÏ ‰≥ˆ
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //œ¬¿≠
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //∏ﬂÀŸ
    GPIO_Initure.Alternate=GPIO_AF3_TIM8;   //PC6∏¥”√Œ™TIM8Õ®µ¿1...
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
		
    TIM8_CH1Config.ICPolarity=TIM_ICPOLARITY_RISING;    //…œ…˝—ÿ≤∂ªÒ
    TIM8_CH1Config.ICSelection=TIM_ICSELECTION_DIRECTTI;//”≥…‰µΩTI1…œ
    TIM8_CH1Config.ICPrescaler=TIM_ICPSC_DIV1;          //≈‰÷√ ‰»Î∑÷∆µ£¨≤ª∑÷∆µ
    TIM8_CH1Config.ICFilter=0;                          //≈‰÷√ ‰»Î¬À≤®∆˜£¨≤ª¬À≤®
    HAL_TIM_IC_ConfigChannel(&TIM8_Handler,&TIM8_CH1Config,TIM_CHANNEL_1);//≈‰÷√TIM8Õ®µ¿1
		
		HAL_TIM_IC_Start_IT(&TIM8_Handler,TIM_CHANNEL_1);   //ø™ º≤∂ªÒTIM8µƒÕ®µ¿1
	}
	if(ch&0x02)
	{
		GPIO_Initure.Pin=GPIO_PIN_7;            //PC6/7/8/9
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //∏¥”√Õ∆ÕÏ ‰≥ˆ
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //œ¬¿≠
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //∏ﬂÀŸ
    GPIO_Initure.Alternate=GPIO_AF3_TIM8;   //PC6∏¥”√Œ™TIM8Õ®µ¿1...
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
		
	  TIM8_CH2Config.ICPolarity=TIM_ICPOLARITY_RISING;    //…œ…˝—ÿ≤∂ªÒ
    TIM8_CH2Config.ICSelection=TIM_ICSELECTION_DIRECTTI;//”≥…‰µΩTI2…œ
    TIM8_CH2Config.ICPrescaler=TIM_ICPSC_DIV1;          //≈‰÷√ ‰»Î∑÷∆µ£¨≤ª∑÷∆µ
    TIM8_CH2Config.ICFilter=0;                          //≈‰÷√ ‰»Î¬À≤®∆˜£¨≤ª¬À≤®
    HAL_TIM_IC_ConfigChannel(&TIM8_Handler,&TIM8_CH2Config,TIM_CHANNEL_2);//≈‰÷√TIM8Õ®µ¿2
		
		HAL_TIM_IC_Start_IT(&TIM8_Handler,TIM_CHANNEL_2);   //ø™ º≤∂ªÒTIM8µƒÕ®µ¿2
	}
  if(ch&0x04)
	{	
		GPIO_Initure.Pin=GPIO_PIN_8;            //PC6/7/8/9
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //∏¥”√Õ∆ÕÏ ‰≥ˆ
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //œ¬¿≠
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //∏ﬂÀŸ
    GPIO_Initure.Alternate=GPIO_AF3_TIM8;   //PC6∏¥”√Œ™TIM8Õ®µ¿1...
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
		
		TIM8_CH3Config.ICPolarity=TIM_ICPOLARITY_RISING;    //…œ…˝—ÿ≤∂ªÒ
    TIM8_CH3Config.ICSelection=TIM_ICSELECTION_DIRECTTI;//”≥…‰µΩTI3…œ
    TIM8_CH3Config.ICPrescaler=TIM_ICPSC_DIV1;          //≈‰÷√ ‰»Î∑÷∆µ£¨≤ª∑÷∆µ
    TIM8_CH3Config.ICFilter=0;                          //≈‰÷√ ‰»Î¬À≤®∆˜£¨≤ª¬À≤®
    HAL_TIM_IC_ConfigChannel(&TIM8_Handler,&TIM8_CH3Config,TIM_CHANNEL_3);//≈‰÷√TIM8Õ®µ¿3
		
		HAL_TIM_IC_Start_IT(&TIM8_Handler,TIM_CHANNEL_3);   //ø™ º≤∂ªÒTIM8µƒÕ®µ¿3
	}
	if(ch&0x08)
	{	
		GPIO_Initure.Pin=GPIO_PIN_9;            //PC6/7/8/9
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //∏¥”√Õ∆ÕÏ ‰≥ˆ
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //œ¬¿≠
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //∏ﬂÀŸ
    GPIO_Initure.Alternate=GPIO_AF3_TIM8;   //PC6∏¥”√Œ™TIM8Õ®µ¿1...
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
		
		TIM8_CH4Config.ICPolarity=TIM_ICPOLARITY_RISING;    //…œ…˝—ÿ≤∂ªÒ
    TIM8_CH4Config.ICSelection=TIM_ICSELECTION_DIRECTTI;//”≥…‰µΩTI4…œ
    TIM8_CH4Config.ICPrescaler=TIM_ICPSC_DIV1;          //≈‰÷√ ‰»Î∑÷∆µ£¨≤ª∑÷∆µ
    TIM8_CH4Config.ICFilter=0;                          //≈‰÷√ ‰»Î¬À≤®∆˜£¨≤ª¬À≤®
    HAL_TIM_IC_ConfigChannel(&TIM8_Handler,&TIM8_CH4Config,TIM_CHANNEL_4);//≈‰÷√TIM8Õ®µ¿4
		
		HAL_TIM_IC_Start_IT(&TIM8_Handler,TIM_CHANNEL_4);   //ø™ º≤∂ªÒTIM8µƒÕ®µ¿4
	}	
	
    __HAL_TIM_ENABLE_IT(&TIM8_Handler,TIM_IT_UPDATE);   // πƒ‹∏¸–¬÷–∂œ
}

//∂® ±1/8µ◊≤„«˝∂Ø£¨ ±÷” πƒ‹£¨“˝Ω≈≈‰÷√
//¥À∫Ø ˝ª·±ªHAL_TIM_IC_Init()µ˜”√
//htim:∂® ±∆˜æ‰±˙
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
  if(htim->Instance==TIM1)
	{
		
    __HAL_RCC_TIM1_CLK_ENABLE();            // πƒ‹TIM1 ±÷”
		__HAL_RCC_GPIOA_CLK_ENABLE();
    HAL_NVIC_SetPriority(TIM1_CC_IRQn ,2,0);    //…Ë÷√÷–∂œ”≈œ»º∂£¨«¿’º”≈œ»º∂2£¨◊””≈œ»º∂0
    HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);          //ø™∆ÙITM1÷–∂œ   
	}	
	
	if(htim->Instance==TIM8)
	{
	  
    __HAL_RCC_TIM8_CLK_ENABLE();            // πƒ‹TIM8 ±÷”
    __HAL_RCC_GPIOC_CLK_ENABLE();			//ø™∆ÙGPIOC ±÷”
    HAL_NVIC_SetPriority(TIM8_CC_IRQn ,2,0);    //…Ë÷√÷–∂œ”≈œ»º∂£¨«¿’º”≈œ»º∂2£¨◊””≈œ»º∂0
    HAL_NVIC_EnableIRQ(TIM8_CC_IRQn);          //ø™∆ÙITM8÷–∂œ   
	}
}
//≤∂ªÒ◊¥Ã¨
//[7]:0,√ª”–≥…π¶µƒ≤∂ªÒ;1,≥…π¶≤∂ªÒµΩ“ª¥Œ.
//[6]:0,ªπ√ª≤∂ªÒµΩµÕµÁ∆Ω;1,“—æ≠≤∂ªÒµΩµÕµÁ∆Ω¡À.
//[5:0]:≤∂ªÒµÕµÁ∆Ω∫Û“Á≥ˆµƒ¥Œ ˝(∂‘”⁄16Œª∂® ±∆˜¿¥Àµ,1usº∆ ˝∆˜º”1,“Á≥ˆ ±º‰:65535us)
u8  TIM1CH1_CAPTURE_STA=0;	// ‰»Î≤∂ªÒ◊¥Ã¨		    				
u16	TIM1CH1_CAPTURE_VAL;	// ‰»Î≤∂ªÒ÷µ(TIM1/TIM8 «16Œª)
u16 TIM1CH1_CAPTURE_PerioVAL;// ‰»Î≤∂ªÒ÷‹∆⁄÷µ

u8  TIM1CH2_CAPTURE_STA;		// ‰»Î≤∂ªÒ◊¥Ã¨		    				
u16	TIM1CH2_CAPTURE_VAL;	// ‰»Î≤∂ªÒ÷µ
u16 TIM1CH2_CAPTURE_PerioVAL;// ‰»Î≤∂ªÒ÷‹∆⁄÷µ

u8  TIM1CH3_CAPTURE_STA;		// ‰»Î≤∂ªÒ◊¥Ã¨		    				
u16	TIM1CH3_CAPTURE_VAL;	// ‰»Î≤∂ªÒ÷µ
u16 TIM1CH3_CAPTURE_PerioVAL;// ‰»Î≤∂ªÒ÷‹∆⁄÷µ

u8  TIM1CH4_CAPTURE_STA;		// ‰»Î≤∂ªÒ◊¥Ã¨		    				
u16	TIM1CH4_CAPTURE_VAL;	// ‰»Î≤∂ªÒ÷µ
u16 TIM1CH4_CAPTURE_PerioVAL;// ‰»Î≤∂ªÒ÷‹∆⁄÷µ

//∂® ±∆˜1÷–∂œ∑˛ŒÒ∫Ø ˝
void TIM1_CC_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM1_Handler);//∂® ±∆˜π≤”√¥¶¿Ì∫Ø ˝
}

//≤∂ªÒ◊¥Ã¨
//[7]:0,√ª”–≥…π¶µƒ≤∂ªÒ;1,≥…π¶≤∂ªÒµΩ“ª¥Œ.
//[6]:0,ªπ√ª≤∂ªÒµΩµÕµÁ∆Ω;1,“—æ≠≤∂ªÒµΩµÕµÁ∆Ω¡À.
//[5:0]:≤∂ªÒµÕµÁ∆Ω∫Û“Á≥ˆµƒ¥Œ ˝(∂‘”⁄16Œª∂® ±∆˜¿¥Àµ,1usº∆ ˝∆˜º”1,“Á≥ˆ ±º‰:65535us)
u8  TIM8CH1_CAPTURE_STA=0;	// ‰»Î≤∂ªÒ◊¥Ã¨		    				
u16	TIM8CH1_CAPTURE_VAL;	// ‰»Î≤∂ªÒ÷µ
u16 TIM8CH1_CAPTURE_PerioVAL;// ‰»Î≤∂ªÒ÷‹∆⁄÷µ

u8  TIM8CH2_CAPTURE_STA=0;	// ‰»Î≤∂ªÒ◊¥Ã¨		    				
u16	TIM8CH2_CAPTURE_VAL;	// ‰»Î≤∂ªÒ÷µ
u16 TIM8CH2_CAPTURE_PerioVAL;// ‰»Î≤∂ªÒ÷‹∆⁄

u8  TIM8CH3_CAPTURE_STA=0;	// ‰»Î≤∂ªÒ◊¥Ã¨		    				
u16	TIM8CH3_CAPTURE_VAL;	// ‰»Î≤∂ªÒ÷µ
u16 TIM8CH3_CAPTURE_PerioVAL;// ‰»Î≤∂ªÒ÷‹∆⁄÷µ

u8  TIM8CH4_CAPTURE_STA=0;	// ‰»Î≤∂ªÒ◊¥Ã¨		    				
u16	TIM8CH4_CAPTURE_VAL;	// ‰»Î≤∂ªÒ
u16 TIM8CH4_CAPTURE_PerioVAL;// ‰»Î≤∂ªÒ÷‹∆⁄÷µµ


//∂® ±∆˜8÷–∂œ∑˛ŒÒ∫Ø ˝
void TIM8_CC_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM8_Handler);//∂® ±∆˜π≤”√¥¶¿Ì∫Ø ˝
}

//∂® ±∆˜∏¸–¬÷–∂œ£®º∆ ˝“Á≥ˆ£©÷–∂œ¥¶¿Ìªÿµ˜∫Ø ˝£¨ ∏√∫Ø ˝‘⁄HAL_TIM_IRQHandler÷–ª·±ªµ˜”√
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//∏¸–¬÷–∂œ£®“Á≥ˆ£©∑¢…˙ ±÷¥––
{
	if(htim->Instance==TIM1)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
		  if((TIM1CH1_CAPTURE_STA&0X80)==0)//ªπŒ¥≥…π¶≤∂ªÒ“ª∏ˆ÷‹∆⁄
			{
				if(TIM1CH1_CAPTURE_STA&0X40)//“—æ≠≤∂ªÒµΩ∏ﬂµÁ∆Ω¡À
				{
					if((TIM1CH1_CAPTURE_STA&0X1F)==0X1F)//∏ﬂµÁ∆Ω÷Æ∫ÛµƒµÕµÁ∆Ω ±º‰Ã´æ√£¨º∆ ˝“Á≥ˆ≥¨π˝¡ø≥Ã
					{
						TIM1CH1_CAPTURE_STA|=0X80;		//±Íº«≥…π¶≤∂ªÒ¡À“ª¥Œ
						TIM1CH1_CAPTURE_VAL=0XFFFF;
					}else TIM1CH1_CAPTURE_STA++;//º∆ ˝“Á≥ˆµ´√ª”–¥Ìπ˝◊Ó¥Û¡ø≥Ã
				}	 
			}
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2){}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3){}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4){}
  }
	if(htim->Instance==TIM8)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
		  if((TIM8CH1_CAPTURE_STA&0X80)==0)//ªπŒ¥≥…π¶≤∂ªÒ
			{
				if(TIM8CH1_CAPTURE_STA&0X40)//“—æ≠≤∂ªÒµΩ∏ﬂµÁ∆Ω¡À
				{
					if((TIM8CH1_CAPTURE_STA&0X1F)==0X1F)//∏ﬂµÁ∆ΩÃ´≥§¡À
					{
						TIM8CH1_CAPTURE_STA|=0X80;		//±Íº«≥…π¶≤∂ªÒ¡À“ª¥Œ
						TIM8CH1_CAPTURE_VAL=0XFFFF;
					}else TIM8CH1_CAPTURE_STA++;
				}	 
			}
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
    {
		 if((TIM8CH2_CAPTURE_STA&0X80)==0)//ªπŒ¥≥…π¶≤∂ªÒ
			{
				if(TIM8CH2_CAPTURE_STA&0X40)//“—æ≠≤∂ªÒµΩ∏ﬂµÁ∆Ω¡À
				{
					if((TIM8CH2_CAPTURE_STA&0X1F)==0X1F)//∏ﬂµÁ∆ΩÃ´≥§¡À
					{
						TIM8CH2_CAPTURE_STA|=0X80;		//±Íº«≥…π¶≤∂ªÒ¡À“ª¥Œ
						TIM8CH2_CAPTURE_VAL=0XFFFF;
					}else TIM8CH2_CAPTURE_STA++;
				}	 
			}
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
		{
		 if((TIM8CH3_CAPTURE_STA&0X80)==0)//ªπŒ¥≥…π¶≤∂ªÒ
			{
				if(TIM8CH3_CAPTURE_STA&0X40)//“—æ≠≤∂ªÒµΩ∏ﬂµÁ∆Ω¡À
				{
					if((TIM8CH3_CAPTURE_STA&0X1F)==0X1F)//∏ﬂµÁ∆ΩÃ´≥§¡À
					{
						TIM8CH3_CAPTURE_STA|=0X80;		//±Íº«≥…π¶≤∂ªÒ¡À“ª¥Œ
						TIM8CH3_CAPTURE_VAL=0XFFFF;
					}else TIM8CH3_CAPTURE_STA++;
				}	 
			}
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
		{
		 if((TIM8CH4_CAPTURE_STA&0X80)==0)//ªπŒ¥≥…π¶≤∂ªÒ
			{
				if(TIM8CH4_CAPTURE_STA&0X40)//“—æ≠≤∂ªÒµΩ∏ﬂµÁ∆Ω¡À
				{
					if((TIM8CH4_CAPTURE_STA&0X1F)==0X1F)//∏ﬂµÁ∆ΩÃ´≥§¡À
					{
						TIM8CH4_CAPTURE_STA|=0X80;		//±Íº«≥…π¶≤∂ªÒ¡À“ª¥Œ
						TIM8CH4_CAPTURE_VAL=0XFFFF;
					}else TIM8CH4_CAPTURE_STA++;
				}	 
			}
		}
  }		
}

//∂® ±∆˜ ‰»Î≤∂ªÒ÷–∂œ¥¶¿Ìªÿµ˜∫Ø ˝£¨∏√∫Ø ˝‘⁄HAL_TIM_IRQHandler÷–ª·±ªµ˜”√
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//≤∂ªÒ÷–∂œ∑¢…˙ ±÷¥––
{
	if(htim->Instance==TIM1)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			if((TIM1CH1_CAPTURE_STA&0X80)==0)//ªπŒ¥≥…π¶≤∂ªÒ“ª∏ˆ÷‹∆⁄
			{
				if(TIM1CH1_CAPTURE_STA&0X40)//µ⁄∂˛¥Œ≤∂ªÒ…œ…˝—ÿ£¨“ª∏ˆ÷‹∆⁄º∆ ˝ÕÍ
				{
					TIM1CH1_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_1)-TIM1CH1_CAPTURE_PerioVAL;//ªÒ»°µ±«∞µƒ≤∂ªÒ÷µ.
					TIM1CH1_CAPTURE_STA|=0X80;		//±Íº«≤∂ªÒµ⁄∂˛∏ˆ…œ…˝—ÿ
					__HAL_TIM_DISABLE(&TIM1_Handler);        //πÿ±’∂® ±∆˜1
					__HAL_TIM_SET_COUNTER(&TIM1_Handler,0);  //»√∂® ±∆˜¥”¡„ø™ ºº∆ ˝
					TIM_RESET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_1);   //“ª∂®“™œ»«Â≥˝‘≠¿¥µƒ…Ë÷√£°£°
					TIM_SET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);//∂® ±∆˜1Õ®µ¿1…Ë÷√ª÷∏¥≥ı ºµƒ…œ…˝—ÿ≤∂ªÒ
					__HAL_TIM_ENABLE(&TIM1_Handler);// πƒ‹∂® ±∆˜1
				}
				else if((TIM1CH1_CAPTURE_STA&0X20))//&&((TIM1CH1_CAPTURE_STA&0X40)==0))		//≤∂ªÒœ¬Ωµ—ÿ £¨«“≤ªª·”¶Œ™µ⁄∂˛¥Œ≤∂ªÒ…œ…˝—ÿΩ¯»Î’‚∏ˆ≈–∂œ		
				{	  			
					TIM1CH1_CAPTURE_STA|=0X40;		//±Íº«≥…π¶≤∂ªÒµΩ“ª¥Œ∏ﬂµÁ∆Ω¬ˆøÌ
					TIM1CH1_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_1)-TIM1CH1_CAPTURE_VAL;//ªÒ»°µ±«∞µƒ≤∂ªÒ÷µ.
					__HAL_TIM_DISABLE(&TIM1_Handler);        //πÿ±’∂® ±∆˜1
					TIM_RESET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_1);   //“ª∂®“™œ»«Â≥˝‘≠¿¥µƒ…Ë÷√£°£°
					TIM_SET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);//≈‰÷√TIM8Õ®µ¿1…œ…˝—ÿ≤∂ªÒ
					__HAL_TIM_ENABLE(&TIM1_Handler);// πƒ‹∂® ±∆˜1
					
				}
				else if(TIM1CH1_CAPTURE_STA==0)        //µ⁄“ª¥Œ≤∂ªÒ…œ…˝—ÿ
				{
					TIM1CH1_CAPTURE_STA=0;			//«Âø’
					TIM1CH1_CAPTURE_VAL=0;
					TIM1CH1_CAPTURE_PerioVAL=0;
					TIM1CH1_CAPTURE_STA|=0X20;		//±Íº«≤∂ªÒµΩ¡Àµ⁄“ª∏ˆ…œ…˝—ÿ
					TIM1CH1_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_1);//ªÒ»°µ±«∞µƒ≤∂ªÒ÷µ.
					TIM1CH1_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_1);
					__HAL_TIM_DISABLE(&TIM1_Handler);        //πÿ±’∂® ±∆˜1
					//__HAL_TIM_SET_COUNTER(&TIM1_Handler,0);  //»√∂® ±∆˜¥”¡„ø™ ºº∆ ˝
					TIM_RESET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_1);   //“ª∂®“™œ»«Â≥˝‘≠¿¥µƒ…Ë÷√£°£°
					TIM_SET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);//∂® ±∆˜1Õ®µ¿1…Ë÷√Œ™œ¬Ωµ—ÿ≤∂ªÒ
					__HAL_TIM_ENABLE(&TIM1_Handler);// πƒ‹∂® ±∆˜1
				}		    
			}
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
		{
			if((TIM1CH2_CAPTURE_STA&0X80)==0)//ªπŒ¥≥…π¶≤∂ªÒ“ª∏ˆ÷‹∆⁄
			{
				if(TIM1CH2_CAPTURE_STA&0X40)//µ⁄∂˛¥Œ≤∂ªÒ…œ…˝—ÿ£¨“ª∏ˆ÷‹∆⁄º∆ ˝ÕÍ
				{
					TIM1CH2_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_2)-TIM1CH2_CAPTURE_PerioVAL;//ªÒ»°µ±«∞µƒ≤∂ªÒ÷µ.
					TIM1CH2_CAPTURE_STA|=0X80;		//±Íº«≤∂ªÒµ⁄∂˛∏ˆ…œ…˝—ÿ
					__HAL_TIM_DISABLE(&TIM1_Handler);        //πÿ±’∂® ±∆˜1
					__HAL_TIM_SET_COUNTER(&TIM1_Handler,0);  //»√∂® ±∆˜¥”¡„ø™ ºº∆ ˝
					TIM_RESET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_2);   //“ª∂®“™œ»«Â≥˝‘≠¿¥µƒ…Ë÷√£°£°
					TIM_SET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_2,TIM_ICPOLARITY_RISING);//∂® ±∆˜1Õ®µ¿1…Ë÷√ª÷∏¥≥ı ºµƒ…œ…˝—ÿ≤∂ªÒ
					__HAL_TIM_ENABLE(&TIM1_Handler);// πƒ‹∂® ±∆˜1
				}
				else if((TIM1CH2_CAPTURE_STA&0X20))//&&((TIM1CH1_CAPTURE_STA&0X40)==0))		//≤∂ªÒœ¬Ωµ—ÿ £¨«“≤ªª·”¶Œ™µ⁄∂˛¥Œ≤∂ªÒ…œ…˝—ÿΩ¯»Î’‚∏ˆ≈–∂œ		
				{	  			
					TIM1CH2_CAPTURE_STA|=0X40;		//±Íº«≥…π¶≤∂ªÒµΩ“ª¥Œ∏ﬂµÁ∆Ω¬ˆøÌ
					TIM1CH2_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_2)-TIM1CH2_CAPTURE_VAL;//ªÒ»°µ±«∞µƒ≤∂ªÒ÷µ.
					__HAL_TIM_DISABLE(&TIM1_Handler);        //πÿ±’∂® ±∆˜1
					TIM_RESET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_2);   //“ª∂®“™œ»«Â≥˝‘≠¿¥µƒ…Ë÷√£°£°
					TIM_SET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_2,TIM_ICPOLARITY_RISING);//≈‰÷√TIM8Õ®µ¿1…œ…˝—ÿ≤∂ªÒ
					__HAL_TIM_ENABLE(&TIM1_Handler);// πƒ‹∂® ±∆˜1
					
				}
				else if(TIM1CH2_CAPTURE_STA==0)        //µ⁄“ª¥Œ≤∂ªÒ…œ…˝—ÿ
				{
					TIM1CH2_CAPTURE_STA=0;			//«Âø’
					TIM1CH2_CAPTURE_VAL=0;
					TIM1CH2_CAPTURE_PerioVAL=0;
					TIM1CH2_CAPTURE_STA|=0X20;		//±Íº«≤∂ªÒµΩ¡Àµ⁄“ª∏ˆ…œ…˝—ÿ
					TIM1CH2_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_2);//ªÒ»°µ±«∞µƒ≤∂ªÒ÷µ.
					TIM1CH2_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_2);
					__HAL_TIM_DISABLE(&TIM1_Handler);        //πÿ±’∂® ±∆˜1
					//__HAL_TIM_SET_COUNTER(&TIM1_Handler,0);  //»√∂® ±∆˜¥”¡„ø™ ºº∆ ˝
					TIM_RESET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_2);   //“ª∂®“™œ»«Â≥˝‘≠¿¥µƒ…Ë÷√£°£°
					TIM_SET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_2,TIM_ICPOLARITY_FALLING);//∂® ±∆˜1Õ®µ¿1…Ë÷√Œ™œ¬Ωµ—ÿ≤∂ªÒ
					__HAL_TIM_ENABLE(&TIM1_Handler);// πƒ‹∂® ±∆˜1
				}		    
			}
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
		{
			if((TIM1CH3_CAPTURE_STA&0X80)==0)//ªπŒ¥≥…π¶≤∂ªÒ“ª∏ˆ÷‹∆⁄
			{
				if(TIM1CH3_CAPTURE_STA&0X40)//µ⁄∂˛¥Œ≤∂ªÒ…œ…˝—ÿ£¨“ª∏ˆ÷‹∆⁄º∆ ˝ÕÍ
				{
					TIM1CH3_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_3)-TIM1CH3_CAPTURE_PerioVAL;//ªÒ»°µ±«∞µƒ≤∂ªÒ÷µ.
					TIM1CH3_CAPTURE_STA|=0X80;		//±Íº«≤∂ªÒµ⁄∂˛∏ˆ…œ…˝—ÿ
					__HAL_TIM_DISABLE(&TIM1_Handler);        //πÿ±’∂® ±∆˜1
					__HAL_TIM_SET_COUNTER(&TIM1_Handler,0);  //»√∂® ±∆˜¥”¡„ø™ ºº∆ ˝
					TIM_RESET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_3);   //“ª∂®“™œ»«Â≥˝‘≠¿¥µƒ…Ë÷√£°£°
					TIM_SET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_3,TIM_ICPOLARITY_RISING);//∂® ±∆˜1Õ®µ¿1…Ë÷√ª÷∏¥≥ı ºµƒ…œ…˝—ÿ≤∂ªÒ
					__HAL_TIM_ENABLE(&TIM1_Handler);// πƒ‹∂® ±∆˜1
				}
				else if((TIM1CH3_CAPTURE_STA&0X20))//&&((TIM1CH1_CAPTURE_STA&0X40)==0))		//≤∂ªÒœ¬Ωµ—ÿ £¨«“≤ªª·”¶Œ™µ⁄∂˛¥Œ≤∂ªÒ…œ…˝—ÿΩ¯»Î’‚∏ˆ≈–∂œ		
				{	  			
					TIM1CH3_CAPTURE_STA|=0X40;		//±Íº«≥…π¶≤∂ªÒµΩ“ª¥Œ∏ﬂµÁ∆Ω¬ˆøÌ
					TIM1CH3_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_1)-TIM1CH3_CAPTURE_VAL;//ªÒ»°µ±«∞µƒ≤∂ªÒ÷µ.
					__HAL_TIM_DISABLE(&TIM1_Handler);        //πÿ±’∂® ±∆˜1
					TIM_RESET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_3);   //“ª∂®“™œ»«Â≥˝‘≠¿¥µƒ…Ë÷√£°£°
					TIM_SET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_3,TIM_ICPOLARITY_RISING);//≈‰÷√TIM8Õ®µ¿1…œ…˝—ÿ≤∂ªÒ
					__HAL_TIM_ENABLE(&TIM1_Handler);// πƒ‹∂® ±∆˜1
					
				}
				else if(TIM1CH3_CAPTURE_STA==0)        //µ⁄“ª¥Œ≤∂ªÒ…œ…˝—ÿ
				{
					TIM1CH3_CAPTURE_STA=0;			//«Âø’
					TIM1CH3_CAPTURE_VAL=0;
					TIM1CH3_CAPTURE_PerioVAL=0;
					TIM1CH3_CAPTURE_STA|=0X20;		//±Íº«≤∂ªÒµΩ¡Àµ⁄“ª∏ˆ…œ…˝—ÿ
					TIM1CH3_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_3);//ªÒ»°µ±«∞µƒ≤∂ªÒ÷µ.
					TIM1CH3_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_3);
					__HAL_TIM_DISABLE(&TIM1_Handler);        //πÿ±’∂® ±∆˜1
					//__HAL_TIM_SET_COUNTER(&TIM1_Handler,0);  //»√∂® ±∆˜¥”¡„ø™ ºº∆ ˝
					TIM_RESET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_3);   //“ª∂®“™œ»«Â≥˝‘≠¿¥µƒ…Ë÷√£°£°
					TIM_SET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_3,TIM_ICPOLARITY_FALLING);//∂® ±∆˜1Õ®µ¿1…Ë÷√Œ™œ¬Ωµ—ÿ≤∂ªÒ
					__HAL_TIM_ENABLE(&TIM1_Handler);// πƒ‹∂® ±∆˜1
				}		    
			}
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
    {
			if((TIM1CH4_CAPTURE_STA&0X80)==0)//ªπŒ¥≥…π¶≤∂ªÒ“ª∏ˆ÷‹∆⁄
			{
				if(TIM1CH4_CAPTURE_STA&0X40)//µ⁄∂˛¥Œ≤∂ªÒ…œ…˝—ÿ£¨“ª∏ˆ÷‹∆⁄º∆ ˝ÕÍ
				{
					TIM1CH4_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_4)-TIM1CH4_CAPTURE_PerioVAL;//ªÒ»°µ±«∞µƒ≤∂ªÒ÷µ.
					TIM1CH4_CAPTURE_STA|=0X80;		//±Íº«≤∂ªÒµ⁄∂˛∏ˆ…œ…˝—ÿ
					__HAL_TIM_DISABLE(&TIM1_Handler);        //πÿ±’∂® ±∆˜1
					__HAL_TIM_SET_COUNTER(&TIM1_Handler,0);  //»√∂® ±∆˜¥”¡„ø™ ºº∆ ˝
					TIM_RESET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_4);   //“ª∂®“™œ»«Â≥˝‘≠¿¥µƒ…Ë÷√£°£°
					TIM_SET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_4,TIM_ICPOLARITY_RISING);//∂® ±∆˜1Õ®µ¿1…Ë÷√ª÷∏¥≥ı ºµƒ…œ…˝—ÿ≤∂ªÒ
					__HAL_TIM_ENABLE(&TIM1_Handler);// πƒ‹∂® ±∆˜1
				}
				else if((TIM1CH4_CAPTURE_STA&0X20))//&&((TIM1CH1_CAPTURE_STA&0X40)==0))		//≤∂ªÒœ¬Ωµ—ÿ £¨«“≤ªª·”¶Œ™µ⁄∂˛¥Œ≤∂ªÒ…œ…˝—ÿΩ¯»Î’‚∏ˆ≈–∂œ		
				{	  			
					TIM1CH4_CAPTURE_STA|=0X40;		//±Íº«≥…π¶≤∂ªÒµΩ“ª¥Œ∏ﬂµÁ∆Ω¬ˆøÌ
					TIM1CH4_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_4)-TIM1CH4_CAPTURE_VAL;//ªÒ»°µ±«∞µƒ≤∂ªÒ÷µ.
					__HAL_TIM_DISABLE(&TIM1_Handler);        //πÿ±’∂® ±∆˜1
					TIM_RESET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_4);   //“ª∂®“™œ»«Â≥˝‘≠¿¥µƒ…Ë÷√£°£°
					TIM_SET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_4,TIM_ICPOLARITY_RISING);//≈‰÷√TIM8Õ®µ¿1…œ…˝—ÿ≤∂ªÒ
					__HAL_TIM_ENABLE(&TIM1_Handler);// πƒ‹∂® ±∆˜1
					
				}
				else if(TIM1CH4_CAPTURE_STA==0)        //µ⁄“ª¥Œ≤∂ªÒ…œ…˝—ÿ
				{
					TIM1CH4_CAPTURE_STA=0;			//«Âø’
					TIM1CH4_CAPTURE_VAL=0;
					TIM1CH4_CAPTURE_PerioVAL=0;
					TIM1CH4_CAPTURE_STA|=0X20;		//±Íº«≤∂ªÒµΩ¡Àµ⁄“ª∏ˆ…œ…˝—ÿ
					TIM1CH4_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_4);//ªÒ»°µ±«∞µƒ≤∂ªÒ÷µ.
					TIM1CH4_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_4);
					__HAL_TIM_DISABLE(&TIM1_Handler);        //πÿ±’∂® ±∆˜1
					//__HAL_TIM_SET_COUNTER(&TIM1_Handler,0);  //»√∂® ±∆˜¥”¡„ø™ ºº∆ ˝
					TIM_RESET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_4);   //“ª∂®“™œ»«Â≥˝‘≠¿¥µƒ…Ë÷√£°£°
					TIM_SET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_4,TIM_ICPOLARITY_FALLING);//∂® ±∆˜1Õ®µ¿1…Ë÷√Œ™œ¬Ωµ—ÿ≤∂ªÒ
					__HAL_TIM_ENABLE(&TIM1_Handler);// πƒ‹∂® ±∆˜1
				}		    
			}
		}			
  }
	
	if(htim->Instance==TIM8)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
		 if((TIM8CH1_CAPTURE_STA&0X80)==0)//ªπŒ¥≥…π¶≤∂ªÒ
			{
				if(TIM8CH1_CAPTURE_STA&0X40)//µ⁄∂˛¥Œ≤∂ªÒ…œ…˝—ÿ£¨“ª∏ˆ÷‹∆⁄º∆ ˝ÕÍ
				{
					TIM8CH1_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_1)-TIM8CH1_CAPTURE_PerioVAL;//ªÒ»°µ±«∞µƒ≤∂ªÒ÷µ.
					TIM8CH1_CAPTURE_STA|=0X80;		//±Íº«≤∂ªÒµ⁄∂˛∏ˆ…œ…˝—ÿ
					__HAL_TIM_DISABLE(&TIM8_Handler);        //πÿ±’∂® ±∆˜1
					__HAL_TIM_SET_COUNTER(&TIM8_Handler,0);  //»√∂® ±∆˜¥”¡„ø™ ºº∆ ˝
					TIM_RESET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_1);   //“ª∂®“™œ»«Â≥˝‘≠¿¥µƒ…Ë÷√£°£°
					TIM_SET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);//∂® ±∆˜8Õ®µ¿1…Ë÷√ª÷∏¥≥ı ºµƒ…œ…˝—ÿ≤∂ªÒ
					__HAL_TIM_ENABLE(&TIM8_Handler);// πƒ‹∂® ±∆˜8
				}
				else if(TIM8CH1_CAPTURE_STA&0X20)  								//ªπŒ¥ø™ º,µ⁄“ª¥Œ≤∂ªÒ…œ…˝—ÿ
				{
					TIM8CH1_CAPTURE_STA|=0X40;		//±Íº«≥…π¶≤∂ªÒµΩ“ª¥Œ∏ﬂµÁ∆Ω¬ˆøÌ
					TIM8CH1_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_1)-TIM8CH1_CAPTURE_VAL;//ªÒ»°µ±«∞µƒ≤∂ªÒ÷µ.
					__HAL_TIM_DISABLE(&TIM8_Handler);        //πÿ±’∂® ±∆˜1
					TIM_RESET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_1);   //“ª∂®“™œ»«Â≥˝‘≠¿¥µƒ…Ë÷√£°£°
					TIM_SET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);//≈‰÷√TIM8Õ®µ¿1…œ…˝—ÿ≤∂ªÒ
					__HAL_TIM_ENABLE(&TIM8_Handler);// πƒ‹∂® ±∆˜8
				}
        else if(TIM8CH1_CAPTURE_STA==0)        //µ⁄“ª¥Œ≤∂ªÒ…œ…˝—ÿ
				{
					TIM8CH1_CAPTURE_STA=0;			//«Âø’
					TIM8CH1_CAPTURE_VAL=0;
					TIM8CH1_CAPTURE_PerioVAL=0;
					TIM8CH1_CAPTURE_STA|=0X20;		//±Íº«≤∂ªÒµΩ¡Àµ⁄“ª∏ˆ…œ…˝—ÿ
					TIM8CH1_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_1);
					TIM8CH1_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_1);
					__HAL_TIM_DISABLE(&TIM8_Handler);        //πÿ±’∂® ±∆˜1
					//__HAL_TIM_SET_COUNTER(&TIM8_Handler,0);  //»√∂® ±∆˜¥”¡„ø™ ºº∆ ˝
					TIM_RESET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_1);   //“ª∂®“™œ»«Â≥˝‘≠¿¥µƒ…Ë÷√£°£°
					TIM_SET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);//∂® ±∆˜8Õ®µ¿1…Ë÷√Œ™œ¬Ωµ—ÿ≤∂ªÒ
					__HAL_TIM_ENABLE(&TIM8_Handler);// πƒ‹∂® ±∆˜8
				}		    
			}	
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
    {
		 if((TIM8CH2_CAPTURE_STA&0X80)==0)//ªπŒ¥≥…π¶≤∂ªÒ
			{
				if(TIM8CH2_CAPTURE_STA&0X40)//µ⁄∂˛¥Œ≤∂ªÒ…œ…˝—ÿ£¨“ª∏ˆ÷‹∆⁄º∆ ˝ÕÍ
				{
					TIM8CH2_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_2)-TIM8CH2_CAPTURE_PerioVAL;//ªÒ»°µ±«∞µƒ≤∂ªÒ÷µ.
					TIM8CH2_CAPTURE_STA|=0X80;		//±Íº«≤∂ªÒµ⁄∂˛∏ˆ…œ…˝—ÿ
					__HAL_TIM_DISABLE(&TIM8_Handler);        //πÿ±’∂® ±∆˜8
					__HAL_TIM_SET_COUNTER(&TIM8_Handler,0);  //»√∂® ±∆˜¥”¡„ø™ ºº∆ ˝
					TIM_RESET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_2);   //“ª∂®“™œ»«Â≥˝‘≠¿¥µƒ…Ë÷√£°£°
					TIM_SET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_2,TIM_ICPOLARITY_RISING);//∂® ±∆˜8Õ®µ¿2…Ë÷√ª÷∏¥≥ı ºµƒ…œ…˝—ÿ≤∂ªÒ
					__HAL_TIM_ENABLE(&TIM8_Handler);// πƒ‹∂® ±∆˜8
				}
				else if(TIM8CH2_CAPTURE_STA&0X20)  								//ªπŒ¥ø™ º,µ⁄“ª¥Œ≤∂ªÒ…œ…˝—ÿ
				{
					TIM8CH2_CAPTURE_STA|=0X40;		//±Íº«≥…π¶≤∂ªÒµΩ“ª¥Œ∏ﬂµÁ∆Ω¬ˆøÌ
					TIM8CH2_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_2)-TIM8CH2_CAPTURE_VAL;//ªÒ»°µ±«∞µƒ≤∂ªÒ÷µ.
					__HAL_TIM_DISABLE(&TIM8_Handler);        //πÿ±’∂® ±∆˜8
					TIM_RESET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_2);   //“ª∂®“™œ»«Â≥˝‘≠¿¥µƒ…Ë÷√£°£°
					TIM_SET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_2,TIM_ICPOLARITY_RISING);//≈‰÷√TIM8Õ®µ¿2…œ…˝—ÿ≤∂ªÒ
					__HAL_TIM_ENABLE(&TIM8_Handler);// πƒ‹∂® ±∆˜8
				}
        else if(TIM8CH2_CAPTURE_STA==0)        //µ⁄“ª¥Œ≤∂ªÒ…œ…˝—ÿ
				{
					TIM8CH2_CAPTURE_STA=0;			//«Âø’
					TIM8CH2_CAPTURE_VAL=0;
					TIM8CH2_CAPTURE_PerioVAL=0;
					TIM8CH2_CAPTURE_STA|=0X20;		//±Íº«≤∂ªÒµΩ¡Àµ⁄“ª∏ˆ…œ…˝—ÿ
					TIM8CH2_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_2);
					TIM8CH2_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_2);
					__HAL_TIM_DISABLE(&TIM8_Handler);        //πÿ±’∂® ±∆˜1
					//__HAL_TIM_SET_COUNTER(&TIM8_Handler,0);  //»√∂® ±∆˜¥”¡„ø™ ºº∆ ˝
					TIM_RESET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_2);   //“ª∂®“™œ»«Â≥˝‘≠¿¥µƒ…Ë÷√£°£°
					TIM_SET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_2,TIM_ICPOLARITY_FALLING);//∂® ±∆˜8Õ®µ¿2…Ë÷√Œ™œ¬Ωµ—ÿ≤∂ªÒ
					__HAL_TIM_ENABLE(&TIM8_Handler);// πƒ‹∂® ±∆˜8
				}		    
			}	
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
		{
		 if((TIM8CH3_CAPTURE_STA&0X80)==0)//ªπŒ¥≥…π¶≤∂ªÒ
			{
				if(TIM8CH3_CAPTURE_STA&0X40)//µ⁄∂˛¥Œ≤∂ªÒ…œ…˝—ÿ£¨“ª∏ˆ÷‹∆⁄º∆ ˝ÕÍ
				{
					TIM8CH3_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_3)-TIM8CH3_CAPTURE_PerioVAL;//ªÒ»°µ±«∞µƒ≤∂ªÒ÷µ.
					TIM8CH3_CAPTURE_STA|=0X80;		//±Íº«≤∂ªÒµ⁄∂˛∏ˆ…œ…˝—ÿ
					__HAL_TIM_DISABLE(&TIM8_Handler);        //πÿ±’∂® ±∆˜8
					__HAL_TIM_SET_COUNTER(&TIM8_Handler,0);  //»√∂® ±∆˜¥”¡„ø™ ºº∆ ˝
					TIM_RESET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_3);   //“ª∂®“™œ»«Â≥˝‘≠¿¥µƒ…Ë÷√£°£°
					TIM_SET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_3,TIM_ICPOLARITY_RISING);//∂® ±∆˜8Õ®µ¿3…Ë÷√ª÷∏¥≥ı ºµƒ…œ…˝—ÿ≤∂ªÒ
					__HAL_TIM_ENABLE(&TIM8_Handler);// πƒ‹∂® ±∆˜8
				}
				else if(TIM8CH3_CAPTURE_STA&0X20)  								//ªπŒ¥ø™ º,µ⁄“ª¥Œ≤∂ªÒ…œ…˝—ÿ
				{
					TIM8CH3_CAPTURE_STA|=0X40;		//±Íº«≥…π¶≤∂ªÒµΩ“ª¥Œ∏ﬂµÁ∆Ω¬ˆøÌ
					TIM8CH3_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_3)-TIM8CH3_CAPTURE_VAL;//ªÒ»°µ±«∞µƒ≤∂ªÒ÷µ.
					__HAL_TIM_DISABLE(&TIM8_Handler);        //πÿ±’∂® ±∆˜8
					TIM_RESET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_3);   //“ª∂®“™œ»«Â≥˝‘≠¿¥µƒ…Ë÷√£°£°
					TIM_SET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_3,TIM_ICPOLARITY_RISING);//≈‰÷√TIM8Õ®µ¿3…œ…˝—ÿ≤∂ªÒ
					__HAL_TIM_ENABLE(&TIM8_Handler);// πƒ‹∂® ±∆˜8
				}
        else if(TIM8CH3_CAPTURE_STA==0)        //µ⁄“ª¥Œ≤∂ªÒ…œ…˝—ÿ
				{
					TIM8CH3_CAPTURE_STA=0;			//«Âø’
					TIM8CH3_CAPTURE_VAL=0;
					TIM8CH3_CAPTURE_PerioVAL=0;
					TIM8CH3_CAPTURE_STA|=0X20;		//±Íº«≤∂ªÒµΩ¡Àµ⁄“ª∏ˆ…œ…˝—ÿ
					TIM8CH3_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_3);
					TIM8CH3_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_3);
					__HAL_TIM_DISABLE(&TIM8_Handler);        //πÿ±’∂® ±∆˜8
					//__HAL_TIM_SET_COUNTER(&TIM8_Handler,0);  //»√∂® ±∆˜¥”¡„ø™ ºº∆ ˝
					TIM_RESET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_3);   //“ª∂®“™œ»«Â≥˝‘≠¿¥µƒ…Ë÷√£°£°
					TIM_SET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_3,TIM_ICPOLARITY_FALLING);//∂® ±∆˜8Õ®µ¿2…Ë÷√Œ™œ¬Ωµ—ÿ≤∂ªÒ
					__HAL_TIM_ENABLE(&TIM8_Handler);// πƒ‹∂® ±∆˜8
				}		    
			}	
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
		{
		 if((TIM8CH4_CAPTURE_STA&0X80)==0)//ªπŒ¥≥…π¶≤∂ªÒ
			{
				if(TIM8CH4_CAPTURE_STA&0X40)//µ⁄∂˛¥Œ≤∂ªÒ…œ…˝—ÿ£¨“ª∏ˆ÷‹∆⁄º∆ ˝ÕÍ
				{
					TIM8CH4_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_4)-TIM8CH4_CAPTURE_PerioVAL;//ªÒ»°µ±«∞µƒ≤∂ªÒ÷µ.
					TIM8CH4_CAPTURE_STA|=0X80;		//±Íº«≤∂ªÒµ⁄∂˛∏ˆ…œ…˝—ÿ
					__HAL_TIM_DISABLE(&TIM8_Handler);        //πÿ±’∂® ±∆˜8
					__HAL_TIM_SET_COUNTER(&TIM8_Handler,0);  //»√∂® ±∆˜¥”¡„ø™ ºº∆ ˝
					TIM_RESET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_4);   //“ª∂®“™œ»«Â≥˝‘≠¿¥µƒ…Ë÷√£°£°
					TIM_SET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_4,TIM_ICPOLARITY_RISING);//∂® ±∆˜8Õ®µ¿4…Ë÷√ª÷∏¥≥ı ºµƒ…œ…˝—ÿ≤∂ªÒ
					__HAL_TIM_ENABLE(&TIM8_Handler);// πƒ‹∂® ±∆˜8
				}
				else if(TIM8CH4_CAPTURE_STA&0X20)  								//ªπŒ¥ø™ º,µ⁄“ª¥Œ≤∂ªÒ…œ…˝—ÿ
				{
					TIM8CH4_CAPTURE_STA|=0X40;		//±Íº«≥…π¶≤∂ªÒµΩ“ª¥Œ∏ﬂµÁ∆Ω¬ˆøÌ
					TIM8CH4_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_4)-TIM8CH4_CAPTURE_VAL;//ªÒ»°µ±«∞µƒ≤∂ªÒ÷µ.
					__HAL_TIM_DISABLE(&TIM8_Handler);        //πÿ±’∂® ±∆˜8
					TIM_RESET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_4);   //“ª∂®“™œ»«Â≥˝‘≠¿¥µƒ…Ë÷√£°£°
					TIM_SET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_4,TIM_ICPOLARITY_RISING);//≈‰÷√TIM8Õ®µ¿4…œ…˝—ÿ≤∂ªÒ
					__HAL_TIM_ENABLE(&TIM8_Handler);// πƒ‹∂® ±∆˜8
				}
        else if(TIM8CH4_CAPTURE_STA==0)        //µ⁄“ª¥Œ≤∂ªÒ…œ…˝—ÿ
				{
					TIM8CH4_CAPTURE_STA=0;			//«Âø’
					TIM8CH4_CAPTURE_VAL=0;
					TIM8CH4_CAPTURE_PerioVAL=0;
					TIM8CH4_CAPTURE_STA|=0X20;		//±Íº«≤∂ªÒµΩ¡Àµ⁄“ª∏ˆ…œ…˝—ÿ
					TIM8CH4_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_4);
					TIM8CH4_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_4);
					__HAL_TIM_DISABLE(&TIM8_Handler);        //πÿ±’∂® ±∆˜
					//__HAL_TIM_SET_COUNTER(&TIM8_Handler,0);  //»√∂® ±∆˜¥”¡„ø™ ºº∆ ˝£¨ª·”–500nsµƒº∆ ˝ŒÛ≤Ó
					TIM_RESET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_4);   //“ª∂®“™œ»«Â≥˝‘≠¿¥µƒ…Ë÷√£°£°
					TIM_SET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_4,TIM_ICPOLARITY_FALLING);//∂® ±∆˜8Õ®µ¿2…Ë÷√Œ™œ¬Ωµ—ÿ≤∂ªÒ
					__HAL_TIM_ENABLE(&TIM8_Handler);// πƒ‹∂® ±∆˜8
				}		    
			}	
		}		
	}
}

//πÿ±’°¢«Â≥˝≥ı ºªØ…Ë÷√
void TIM1_Cap_DeInit(void)
{
	HAL_TIM_IC_DeInit(&TIM1_Handler);
	
	HAL_GPIO_DeInit(GPIOA,GPIO_PIN_8);
	HAL_GPIO_DeInit(GPIOA,GPIO_PIN_9);
	HAL_GPIO_DeInit(GPIOA,GPIO_PIN_10);
	HAL_GPIO_DeInit(GPIOA,GPIO_PIN_11);
	
	HAL_TIM_IC_Stop_IT(&TIM1_Handler,TIM_CHANNEL_1);   //Õ£÷π≤∂ªÒTIM1µƒÕ®µ¿
	HAL_TIM_IC_Stop_IT(&TIM1_Handler,TIM_CHANNEL_2);   //Õ£÷π≤∂ªÒTIM1µƒÕ®µ¿
	HAL_TIM_IC_Stop_IT(&TIM1_Handler,TIM_CHANNEL_3);   //Õ£÷π≤∂ªÒTIM1µƒÕ®µ¿
	HAL_TIM_IC_Stop_IT(&TIM1_Handler,TIM_CHANNEL_4);   //Õ£÷π≤∂ªÒTIM1µƒÕ®µ¿
	
	__HAL_TIM_DISABLE_IT(&TIM1_Handler,TIM_IT_UPDATE); // ßƒ‹∏¸–¬÷–∂œ
}

void TIM8_Cap_DeInit(void)
{
	HAL_TIM_IC_DeInit(&TIM8_Handler);
	
	HAL_GPIO_DeInit(GPIOC,GPIO_PIN_6);
	HAL_GPIO_DeInit(GPIOC,GPIO_PIN_7);
	HAL_GPIO_DeInit(GPIOC,GPIO_PIN_8);
	HAL_GPIO_DeInit(GPIOC,GPIO_PIN_9);
	
	HAL_TIM_IC_Stop_IT(&TIM8_Handler,TIM_CHANNEL_1);   //Õ£÷π≤∂ªÒTIM1µƒÕ®µ¿
	HAL_TIM_IC_Stop_IT(&TIM8_Handler,TIM_CHANNEL_2);   //Õ£÷π≤∂ªÒTIM1µƒÕ®µ¿
	HAL_TIM_IC_Stop_IT(&TIM8_Handler,TIM_CHANNEL_3);   //Õ£÷π≤∂ªÒTIM1µƒÕ®µ¿
	HAL_TIM_IC_Stop_IT(&TIM8_Handler,TIM_CHANNEL_4);   //Õ£÷π≤∂ªÒTIM1µƒÕ®µ¿
	
	__HAL_TIM_DISABLE_IT(&TIM8_Handler,TIM_IT_UPDATE); // ßƒ‹∏¸–¬÷–∂œ
}
//∂® ±∆˜µ◊≤„«˝∂Ø£¨ ±÷” ßƒ‹£¨πÿ±’÷–∂œ
//¥À∫Ø ˝ª·±ªHAL_TIM_IC_DeInit()µ˜”√
//htim:∂® ±∆˜æ‰±˙
void HAL_TIM_IC_MspDeInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM1)
	{
		__HAL_RCC_TIM1_CLK_DISABLE();            // ßƒ‹ ±÷”
	//	__HAL_RCC_GPIOA_CLK_DISABLE();  ≤ªƒ‹”∞œÏ±µƒ“˝Ω≈
    HAL_NVIC_DisableIRQ(TIM1_CC_IRQn);          //πÿ±’TIM1÷–∂œ   
	}
	if(htim->Instance==TIM8)
	{
		__HAL_RCC_TIM8_CLK_DISABLE();            // ßƒ‹ ±÷”
	//	__HAL_RCC_GPIOC_CLK_DISABLE();
    HAL_NVIC_DisableIRQ(TIM8_CC_IRQn);          //πÿ±’TIM8÷–∂œ
	}
}
