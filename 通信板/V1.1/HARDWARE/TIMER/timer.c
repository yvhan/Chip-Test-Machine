#include "timer.h"
#include "led.h"

TIM_HandleTypeDef TIM3_Handler;      	//��ʱ����� 
TIM_OC_InitTypeDef TIM3_CH4Handler;     //��ʱ��3ͨ��4���


//ͨ�ö�ʱ��3�жϳ�ʼ��,��ʱ��3��APB1�ϣ�APB1�Ķ�ʱ��ʱ��Ϊ200MHz
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!(��ʱ��3����APB1�ϣ�ʱ��ΪHCLK/2)
void TIM3_Init(u16 arr,u16 psc)
{  
    TIM3_Handler.Instance=TIM3;                          //ͨ�ö�ʱ��3
    TIM3_Handler.Init.Prescaler=psc;                     //��Ƶ
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM3_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
    HAL_TIM_Base_Init(&TIM3_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM3_Handler); //ʹ�ܶ�ʱ��3�Ͷ�ʱ��3�����жϣ�TIM_IT_UPDATE    
}

//TIM3 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM3_PWM_Init(u16 arr,u16 psc)
{ 
    TIM3_Handler.Instance=TIM3;            //��ʱ��3
    TIM3_Handler.Init.Prescaler=psc;       //��ʱ����Ƶ
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//���ϼ���ģʽ
    TIM3_Handler.Init.Period=arr;          //�Զ���װ��ֵ
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM3_Handler);       //��ʼ��PWM
    
    TIM3_CH4Handler.OCMode=TIM_OCMODE_PWM1; //ģʽѡ��PWM1
    TIM3_CH4Handler.Pulse=arr/2;            //���ñȽ�ֵ,��ֵ����ȷ��ռ�ձȣ�
                                            //Ĭ�ϱȽ�ֵΪ�Զ���װ��ֵ��һ��,��ռ�ձ�Ϊ50%
    TIM3_CH4Handler.OCPolarity=TIM_OCPOLARITY_LOW; //����Ƚϼ���Ϊ�� 
    HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH4Handler,TIM_CHANNEL_4);//����TIM3ͨ��4
    HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_4);//����PWMͨ��4
}

//��ʱ���ײ�������ʱ��ʹ�ܣ���������
//�˺����ᱻHAL_TIM_PWM_Init()����
//htim:��ʱ�����
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
	  __HAL_RCC_TIM3_CLK_ENABLE();			//ʹ�ܶ�ʱ��3
    __HAL_RCC_GPIOB_CLK_ENABLE();			//����GPIOBʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_1;           	//PB1
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//�����������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //����
	  GPIO_Initure.Alternate=GPIO_AF2_TIM3;	//PB1����ΪTIM3_CH4
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
}

//��ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
//�˺����ᱻHAL_TIM_Base_Init()��������
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();            //ʹ��TIM3ʱ��
		HAL_NVIC_SetPriority(TIM3_IRQn,1,3);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
		HAL_NVIC_EnableIRQ(TIM3_IRQn);          //����ITM3�ж�   
	}  
}

//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM3_Handler);
}


//����TIMͨ��4��ռ�ձ�
//compare:�Ƚ�ֵ
void TIM_SetTIM3Compare4(u32 compare)
{
	TIM3->CCR4=compare; 
}

//��ȡTIM����/�ȽϼĴ���ֵ
u32 TIM_GetTIM3Capture4(void)
{
	return  HAL_TIM_ReadCapturedValue(&TIM3_Handler,TIM_CHANNEL_4);
}
/***************************************************************************
****************************************************************************
  TIM1ͨ������
****************************************************************************
****************************************************************************/

TIM_HandleTypeDef TIM1_Handler;         //��ʱ��8��� �ṹ������

//��ʱ1ͨ��1���벶������
//arr���Զ���װֵ(TIM1,TIM8��16λ��!!)
//psc��ʱ��Ԥ��Ƶ��
//ch: ����ͨ��ѡ��
void TIM1_Cap_Init(u16 arr,u16 psc,u8 ch)
{  
	TIM_IC_InitTypeDef TIM1_CH1Config; 
	TIM_IC_InitTypeDef TIM1_CH2Config;	
	TIM_IC_InitTypeDef TIM1_CH3Config; 
	TIM_IC_InitTypeDef TIM1_CH4Config;
	
	GPIO_InitTypeDef GPIO_Initure;

	TIM1_Handler.Instance=TIM1;                          //�߼���ʱ��1
	TIM1_Handler.Init.Prescaler=psc;                     //��Ƶ
	TIM1_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
	TIM1_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
	TIM1_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_IC_Init(&TIM1_Handler);
    
	if(ch&0x01)
	{
    GPIO_Initure.Pin=GPIO_PIN_8;//|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;            //PA8\9\10\11
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //�����������
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //����
    GPIO_Initure.Alternate=GPIO_AF1_TIM1;   
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
		
    TIM1_CH1Config.ICPolarity=TIM_ICPOLARITY_RISING;    //�����ز���
    TIM1_CH1Config.ICSelection=TIM_ICSELECTION_DIRECTTI;//ӳ�䵽TI1��
    TIM1_CH1Config.ICPrescaler=TIM_ICPSC_DIV1;          //���������Ƶ������Ƶ
    TIM1_CH1Config.ICFilter=0;                          //���������˲��������˲�
    HAL_TIM_IC_ConfigChannel(&TIM1_Handler,&TIM1_CH1Config,TIM_CHANNEL_1);//����TIM1ͨ��1
		
		HAL_TIM_IC_Start_IT(&TIM1_Handler,TIM_CHANNEL_1);   //��ʼ����TIM1��ͨ��1
	}
	if(ch&0x02)
	{ 
    GPIO_Initure.Pin=GPIO_PIN_9;//|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;            //PA8\9\10\11
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //�����������
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //����
    GPIO_Initure.Alternate=GPIO_AF1_TIM1;  
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
		
	  TIM1_CH2Config.ICPolarity=TIM_ICPOLARITY_RISING;    //�����ز���
    TIM1_CH2Config.ICSelection=TIM_ICSELECTION_DIRECTTI;//ӳ�䵽TI2��
    TIM1_CH2Config.ICPrescaler=TIM_ICPSC_DIV1;          //���������Ƶ������Ƶ
    TIM1_CH2Config.ICFilter=0;                          //���������˲��������˲�
    HAL_TIM_IC_ConfigChannel(&TIM1_Handler,&TIM1_CH2Config,TIM_CHANNEL_2);//����TIM1ͨ��2
		
		HAL_TIM_IC_Start_IT(&TIM1_Handler,TIM_CHANNEL_2);   //��ʼ����TIM1��ͨ��2
	}
  if(ch&0x04)
	{	
    GPIO_Initure.Pin=GPIO_PIN_10;//|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;            //PA8\9\10\11
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //�����������
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //����
    GPIO_Initure.Alternate=GPIO_AF1_TIM1;   
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
		
		TIM1_CH3Config.ICPolarity=TIM_ICPOLARITY_RISING;    //�����ز���
    TIM1_CH3Config.ICSelection=TIM_ICSELECTION_DIRECTTI;//ӳ�䵽TI3��
    TIM1_CH3Config.ICPrescaler=TIM_ICPSC_DIV1;          //���������Ƶ������Ƶ
    TIM1_CH3Config.ICFilter=0;                          //���������˲��������˲�
    HAL_TIM_IC_ConfigChannel(&TIM1_Handler,&TIM1_CH3Config,TIM_CHANNEL_3);//����TIM1ͨ��3
		
		HAL_TIM_IC_Start_IT(&TIM1_Handler,TIM_CHANNEL_3);   //��ʼ����TIM1��ͨ��3
	}
	if(ch&0x08)
	{
    GPIO_Initure.Pin=GPIO_PIN_11;//|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;            //PA8\9\10\11
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //�����������
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //����
    GPIO_Initure.Alternate=GPIO_AF1_TIM1;  
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
		
		TIM1_CH4Config.ICPolarity=TIM_ICPOLARITY_RISING;    //�����ز���
    TIM1_CH4Config.ICSelection=TIM_ICSELECTION_DIRECTTI;//ӳ�䵽TI4��
    TIM1_CH4Config.ICPrescaler=TIM_ICPSC_DIV1;          //���������Ƶ������Ƶ
    TIM1_CH4Config.ICFilter=0;                          //���������˲��������˲�
    HAL_TIM_IC_ConfigChannel(&TIM1_Handler,&TIM1_CH4Config,TIM_CHANNEL_4);//����TIM1ͨ��4
		
		HAL_TIM_IC_Start_IT(&TIM1_Handler,TIM_CHANNEL_4);   //��ʼ����TIM1��ͨ��4
	}
    
    __HAL_TIM_ENABLE_IT(&TIM1_Handler,TIM_IT_UPDATE);   //ʹ�ܸ����ж�
}

/***************************************************************************
****************************************************************************
  TIM8ͨ������
****************************************************************************
****************************************************************************/

TIM_HandleTypeDef TIM8_Handler;         //��ʱ��8��� �ṹ������

//��ʱ��8ͨ��1���벶������
//arr���Զ���װֵ(TIM1,TIM8��16λ��!!)
//psc��ʱ��Ԥ��Ƶ��
//ch: ����ͨ��ѡ��
void TIM8_Cap_Init(u16 arr,u16 psc,u8 ch)
{  
    TIM_IC_InitTypeDef TIM8_CH1Config;  
    TIM_IC_InitTypeDef TIM8_CH2Config;
	  TIM_IC_InitTypeDef TIM8_CH3Config;
	  TIM_IC_InitTypeDef TIM8_CH4Config;
	
	  GPIO_InitTypeDef GPIO_Initure;
   
  	TIM8_Handler.Instance=TIM8;                          //�߼���ʱ��8
    TIM8_Handler.Init.Prescaler=psc;                     //��Ƶ
    TIM8_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM8_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    TIM8_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_IC_Init(&TIM8_Handler);
	
  if(ch&0x01)
	{
		GPIO_Initure.Pin=GPIO_PIN_6;            //PC6/7/8/9
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //�����������
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //����
    GPIO_Initure.Alternate=GPIO_AF3_TIM8;   //PC6����ΪTIM8ͨ��1...
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
		
    TIM8_CH1Config.ICPolarity=TIM_ICPOLARITY_RISING;    //�����ز���
    TIM8_CH1Config.ICSelection=TIM_ICSELECTION_DIRECTTI;//ӳ�䵽TI1��
    TIM8_CH1Config.ICPrescaler=TIM_ICPSC_DIV1;          //���������Ƶ������Ƶ
    TIM8_CH1Config.ICFilter=0;                          //���������˲��������˲�
    HAL_TIM_IC_ConfigChannel(&TIM8_Handler,&TIM8_CH1Config,TIM_CHANNEL_1);//����TIM8ͨ��1
		
		HAL_TIM_IC_Start_IT(&TIM8_Handler,TIM_CHANNEL_1);   //��ʼ����TIM8��ͨ��1
	}
	if(ch&0x02)
	{
		GPIO_Initure.Pin=GPIO_PIN_7;            //PC6/7/8/9
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //�����������
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //����
    GPIO_Initure.Alternate=GPIO_AF3_TIM8;   //PC6����ΪTIM8ͨ��1...
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
		
	  TIM8_CH2Config.ICPolarity=TIM_ICPOLARITY_RISING;    //�����ز���
    TIM8_CH2Config.ICSelection=TIM_ICSELECTION_DIRECTTI;//ӳ�䵽TI2��
    TIM8_CH2Config.ICPrescaler=TIM_ICPSC_DIV1;          //���������Ƶ������Ƶ
    TIM8_CH2Config.ICFilter=0;                          //���������˲��������˲�
    HAL_TIM_IC_ConfigChannel(&TIM8_Handler,&TIM8_CH2Config,TIM_CHANNEL_2);//����TIM8ͨ��2
		
		HAL_TIM_IC_Start_IT(&TIM8_Handler,TIM_CHANNEL_2);   //��ʼ����TIM8��ͨ��2
	}
  if(ch&0x04)
	{	
		GPIO_Initure.Pin=GPIO_PIN_8;            //PC6/7/8/9
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //�����������
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //����
    GPIO_Initure.Alternate=GPIO_AF3_TIM8;   //PC6����ΪTIM8ͨ��1...
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
		
		TIM8_CH3Config.ICPolarity=TIM_ICPOLARITY_RISING;    //�����ز���
    TIM8_CH3Config.ICSelection=TIM_ICSELECTION_DIRECTTI;//ӳ�䵽TI3��
    TIM8_CH3Config.ICPrescaler=TIM_ICPSC_DIV1;          //���������Ƶ������Ƶ
    TIM8_CH3Config.ICFilter=0;                          //���������˲��������˲�
    HAL_TIM_IC_ConfigChannel(&TIM8_Handler,&TIM8_CH3Config,TIM_CHANNEL_3);//����TIM8ͨ��3
		
		HAL_TIM_IC_Start_IT(&TIM8_Handler,TIM_CHANNEL_3);   //��ʼ����TIM8��ͨ��3
	}
	if(ch&0x08)
	{	
		GPIO_Initure.Pin=GPIO_PIN_9;            //PC6/7/8/9
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //�����������
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //����
    GPIO_Initure.Alternate=GPIO_AF3_TIM8;   //PC6����ΪTIM8ͨ��1...
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
		
		TIM8_CH4Config.ICPolarity=TIM_ICPOLARITY_RISING;    //�����ز���
    TIM8_CH4Config.ICSelection=TIM_ICSELECTION_DIRECTTI;//ӳ�䵽TI4��
    TIM8_CH4Config.ICPrescaler=TIM_ICPSC_DIV1;          //���������Ƶ������Ƶ
    TIM8_CH4Config.ICFilter=0;                          //���������˲��������˲�
    HAL_TIM_IC_ConfigChannel(&TIM8_Handler,&TIM8_CH4Config,TIM_CHANNEL_4);//����TIM8ͨ��4
		
		HAL_TIM_IC_Start_IT(&TIM8_Handler,TIM_CHANNEL_4);   //��ʼ����TIM8��ͨ��4
	}	
	
    __HAL_TIM_ENABLE_IT(&TIM8_Handler,TIM_IT_UPDATE);   //ʹ�ܸ����ж�
}

//��ʱ1/8�ײ�������ʱ��ʹ�ܣ���������
//�˺����ᱻHAL_TIM_IC_Init()����
//htim:��ʱ�����
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
  if(htim->Instance==TIM1)
	{
		
    __HAL_RCC_TIM1_CLK_ENABLE();            //ʹ��TIM1ʱ��
		__HAL_RCC_GPIOA_CLK_ENABLE();
    HAL_NVIC_SetPriority(TIM1_CC_IRQn ,2,0);    //�����ж����ȼ�����ռ���ȼ�2�������ȼ�0
    HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);          //����ITM1�ж�   
	}	
	
	if(htim->Instance==TIM8)
	{
	  
    __HAL_RCC_TIM8_CLK_ENABLE();            //ʹ��TIM8ʱ��
    __HAL_RCC_GPIOC_CLK_ENABLE();			//����GPIOCʱ��
    HAL_NVIC_SetPriority(TIM8_CC_IRQn ,2,0);    //�����ж����ȼ�����ռ���ȼ�2�������ȼ�0
    HAL_NVIC_EnableIRQ(TIM8_CC_IRQn);          //����ITM8�ж�   
	}
}
//����״̬
//[7]:0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
//[6]:0,��û���񵽵͵�ƽ;1,�Ѿ����񵽵͵�ƽ��.
//[5:0]:����͵�ƽ������Ĵ���(����16λ��ʱ����˵,1us��������1,���ʱ��:65535us)
u8  TIM1CH1_CAPTURE_STA=0;	//���벶��״̬		    				
u16	TIM1CH1_CAPTURE_VAL;	//���벶��ֵ(TIM1/TIM8��16λ)
u16 TIM1CH1_CAPTURE_PerioVAL;//���벶������ֵ

u8  TIM1CH2_CAPTURE_STA;		//���벶��״̬		    				
u16	TIM1CH2_CAPTURE_VAL;	//���벶��ֵ
u16 TIM1CH2_CAPTURE_PerioVAL;//���벶������ֵ

u8  TIM1CH3_CAPTURE_STA;		//���벶��״̬		    				
u16	TIM1CH3_CAPTURE_VAL;	//���벶��ֵ
u16 TIM1CH3_CAPTURE_PerioVAL;//���벶������ֵ

u8  TIM1CH4_CAPTURE_STA;		//���벶��״̬		    				
u16	TIM1CH4_CAPTURE_VAL;	//���벶��ֵ
u16 TIM1CH4_CAPTURE_PerioVAL;//���벶������ֵ

//��ʱ��1�жϷ�����
void TIM1_CC_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM1_Handler);//��ʱ�����ô�����
}

//����״̬
//[7]:0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
//[6]:0,��û���񵽵͵�ƽ;1,�Ѿ����񵽵͵�ƽ��.
//[5:0]:����͵�ƽ������Ĵ���(����16λ��ʱ����˵,1us��������1,���ʱ��:65535us)
u8  TIM8CH1_CAPTURE_STA=0;	//���벶��״̬		    				
u16	TIM8CH1_CAPTURE_VAL;	//���벶��ֵ
u16 TIM8CH1_CAPTURE_PerioVAL;//���벶������ֵ

u8  TIM8CH2_CAPTURE_STA=0;	//���벶��״̬		    				
u16	TIM8CH2_CAPTURE_VAL;	//���벶��ֵ
u16 TIM8CH2_CAPTURE_PerioVAL;//���벶������

u8  TIM8CH3_CAPTURE_STA=0;	//���벶��״̬		    				
u16	TIM8CH3_CAPTURE_VAL;	//���벶��ֵ
u16 TIM8CH3_CAPTURE_PerioVAL;//���벶������ֵ

u8  TIM8CH4_CAPTURE_STA=0;	//���벶��״̬		    				
u16	TIM8CH4_CAPTURE_VAL;	//���벶��
u16 TIM8CH4_CAPTURE_PerioVAL;//���벶������ֵ�


//��ʱ��8�жϷ�����
void TIM8_CC_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM8_Handler);//��ʱ�����ô�����
}

//��ʱ�������жϣ�����������жϴ���ص������� �ú�����HAL_TIM_IRQHandler�лᱻ����
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//�����жϣ����������ʱִ��
{
	if(htim->Instance==TIM1)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
		  if((TIM1CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����һ������
			{
				if(TIM1CH1_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
				{
					if((TIM1CH1_CAPTURE_STA&0X1F)==0X1F)//�ߵ�ƽ֮��ĵ͵�ƽʱ��̫�ã����������������
					{
						TIM1CH1_CAPTURE_STA|=0X80;		//��ǳɹ�������һ��
						TIM1CH1_CAPTURE_VAL=0XFFFF;
					}else TIM1CH1_CAPTURE_STA++;//���������û�д���������
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
		  if((TIM8CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
			{
				if(TIM8CH1_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
				{
					if((TIM8CH1_CAPTURE_STA&0X1F)==0X1F)//�ߵ�ƽ̫����
					{
						TIM8CH1_CAPTURE_STA|=0X80;		//��ǳɹ�������һ��
						TIM8CH1_CAPTURE_VAL=0XFFFF;
					}else TIM8CH1_CAPTURE_STA++;
				}	 
			}
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
    {
		 if((TIM8CH2_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
			{
				if(TIM8CH2_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
				{
					if((TIM8CH2_CAPTURE_STA&0X1F)==0X1F)//�ߵ�ƽ̫����
					{
						TIM8CH2_CAPTURE_STA|=0X80;		//��ǳɹ�������һ��
						TIM8CH2_CAPTURE_VAL=0XFFFF;
					}else TIM8CH2_CAPTURE_STA++;
				}	 
			}
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
		{
		 if((TIM8CH3_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
			{
				if(TIM8CH3_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
				{
					if((TIM8CH3_CAPTURE_STA&0X1F)==0X1F)//�ߵ�ƽ̫����
					{
						TIM8CH3_CAPTURE_STA|=0X80;		//��ǳɹ�������һ��
						TIM8CH3_CAPTURE_VAL=0XFFFF;
					}else TIM8CH3_CAPTURE_STA++;
				}	 
			}
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
		{
		 if((TIM8CH4_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
			{
				if(TIM8CH4_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
				{
					if((TIM8CH4_CAPTURE_STA&0X1F)==0X1F)//�ߵ�ƽ̫����
					{
						TIM8CH4_CAPTURE_STA|=0X80;		//��ǳɹ�������һ��
						TIM8CH4_CAPTURE_VAL=0XFFFF;
					}else TIM8CH4_CAPTURE_STA++;
				}	 
			}
		}
  }		
}

//��ʱ�����벶���жϴ���ص��������ú�����HAL_TIM_IRQHandler�лᱻ����
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//�����жϷ���ʱִ��
{
	if(htim->Instance==TIM1)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			if((TIM1CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����һ������
			{
				if(TIM1CH1_CAPTURE_STA&0X40)//�ڶ��β��������أ�һ�����ڼ�����
				{
					TIM1CH1_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_1)-TIM1CH1_CAPTURE_PerioVAL;//��ȡ��ǰ�Ĳ���ֵ.
					TIM1CH1_CAPTURE_STA|=0X80;		//��ǲ���ڶ���������
					__HAL_TIM_DISABLE(&TIM1_Handler);        //�رն�ʱ��1
					__HAL_TIM_SET_COUNTER(&TIM1_Handler,0);  //�ö�ʱ�����㿪ʼ����
					TIM_RESET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_1);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);//��ʱ��1ͨ��1���ûָ���ʼ�������ز���
					__HAL_TIM_ENABLE(&TIM1_Handler);//ʹ�ܶ�ʱ��1
				}
				else if((TIM1CH1_CAPTURE_STA&0X20))//&&((TIM1CH1_CAPTURE_STA&0X40)==0))		//�����½��� ���Ҳ���ӦΪ�ڶ��β��������ؽ�������ж�		
				{	  			
					TIM1CH1_CAPTURE_STA|=0X40;		//��ǳɹ�����һ�θߵ�ƽ����
					TIM1CH1_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_1)-TIM1CH1_CAPTURE_VAL;//��ȡ��ǰ�Ĳ���ֵ.
					__HAL_TIM_DISABLE(&TIM1_Handler);        //�رն�ʱ��1
					TIM_RESET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_1);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);//����TIM8ͨ��1�����ز���
					__HAL_TIM_ENABLE(&TIM1_Handler);//ʹ�ܶ�ʱ��1
					
				}
				else if(TIM1CH1_CAPTURE_STA==0)        //��һ�β���������
				{
					TIM1CH1_CAPTURE_STA=0;			//���
					TIM1CH1_CAPTURE_VAL=0;
					TIM1CH1_CAPTURE_PerioVAL=0;
					TIM1CH1_CAPTURE_STA|=0X20;		//��ǲ����˵�һ��������
					TIM1CH1_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_1);//��ȡ��ǰ�Ĳ���ֵ.
					TIM1CH1_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_1);
					__HAL_TIM_DISABLE(&TIM1_Handler);        //�رն�ʱ��1
					//__HAL_TIM_SET_COUNTER(&TIM1_Handler,0);  //�ö�ʱ�����㿪ʼ����
					TIM_RESET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_1);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);//��ʱ��1ͨ��1����Ϊ�½��ز���
					__HAL_TIM_ENABLE(&TIM1_Handler);//ʹ�ܶ�ʱ��1
				}		    
			}
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
		{
			if((TIM1CH2_CAPTURE_STA&0X80)==0)//��δ�ɹ�����һ������
			{
				if(TIM1CH2_CAPTURE_STA&0X40)//�ڶ��β��������أ�һ�����ڼ�����
				{
					TIM1CH2_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_2)-TIM1CH2_CAPTURE_PerioVAL;//��ȡ��ǰ�Ĳ���ֵ.
					TIM1CH2_CAPTURE_STA|=0X80;		//��ǲ���ڶ���������
					__HAL_TIM_DISABLE(&TIM1_Handler);        //�رն�ʱ��1
					__HAL_TIM_SET_COUNTER(&TIM1_Handler,0);  //�ö�ʱ�����㿪ʼ����
					TIM_RESET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_2);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_2,TIM_ICPOLARITY_RISING);//��ʱ��1ͨ��1���ûָ���ʼ�������ز���
					__HAL_TIM_ENABLE(&TIM1_Handler);//ʹ�ܶ�ʱ��1
				}
				else if((TIM1CH2_CAPTURE_STA&0X20))//&&((TIM1CH1_CAPTURE_STA&0X40)==0))		//�����½��� ���Ҳ���ӦΪ�ڶ��β��������ؽ�������ж�		
				{	  			
					TIM1CH2_CAPTURE_STA|=0X40;		//��ǳɹ�����һ�θߵ�ƽ����
					TIM1CH2_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_2)-TIM1CH2_CAPTURE_VAL;//��ȡ��ǰ�Ĳ���ֵ.
					__HAL_TIM_DISABLE(&TIM1_Handler);        //�رն�ʱ��1
					TIM_RESET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_2);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_2,TIM_ICPOLARITY_RISING);//����TIM8ͨ��1�����ز���
					__HAL_TIM_ENABLE(&TIM1_Handler);//ʹ�ܶ�ʱ��1
					
				}
				else if(TIM1CH2_CAPTURE_STA==0)        //��һ�β���������
				{
					TIM1CH2_CAPTURE_STA=0;			//���
					TIM1CH2_CAPTURE_VAL=0;
					TIM1CH2_CAPTURE_PerioVAL=0;
					TIM1CH2_CAPTURE_STA|=0X20;		//��ǲ����˵�һ��������
					TIM1CH2_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_2);//��ȡ��ǰ�Ĳ���ֵ.
					TIM1CH2_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_2);
					__HAL_TIM_DISABLE(&TIM1_Handler);        //�رն�ʱ��1
					//__HAL_TIM_SET_COUNTER(&TIM1_Handler,0);  //�ö�ʱ�����㿪ʼ����
					TIM_RESET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_2);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_2,TIM_ICPOLARITY_FALLING);//��ʱ��1ͨ��1����Ϊ�½��ز���
					__HAL_TIM_ENABLE(&TIM1_Handler);//ʹ�ܶ�ʱ��1
				}		    
			}
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
		{
			if((TIM1CH3_CAPTURE_STA&0X80)==0)//��δ�ɹ�����һ������
			{
				if(TIM1CH3_CAPTURE_STA&0X40)//�ڶ��β��������أ�һ�����ڼ�����
				{
					TIM1CH3_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_3)-TIM1CH3_CAPTURE_PerioVAL;//��ȡ��ǰ�Ĳ���ֵ.
					TIM1CH3_CAPTURE_STA|=0X80;		//��ǲ���ڶ���������
					__HAL_TIM_DISABLE(&TIM1_Handler);        //�رն�ʱ��1
					__HAL_TIM_SET_COUNTER(&TIM1_Handler,0);  //�ö�ʱ�����㿪ʼ����
					TIM_RESET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_3);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_3,TIM_ICPOLARITY_RISING);//��ʱ��1ͨ��1���ûָ���ʼ�������ز���
					__HAL_TIM_ENABLE(&TIM1_Handler);//ʹ�ܶ�ʱ��1
				}
				else if((TIM1CH3_CAPTURE_STA&0X20))//&&((TIM1CH1_CAPTURE_STA&0X40)==0))		//�����½��� ���Ҳ���ӦΪ�ڶ��β��������ؽ�������ж�		
				{	  			
					TIM1CH3_CAPTURE_STA|=0X40;		//��ǳɹ�����һ�θߵ�ƽ����
					TIM1CH3_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_1)-TIM1CH3_CAPTURE_VAL;//��ȡ��ǰ�Ĳ���ֵ.
					__HAL_TIM_DISABLE(&TIM1_Handler);        //�رն�ʱ��1
					TIM_RESET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_3);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_3,TIM_ICPOLARITY_RISING);//����TIM8ͨ��1�����ز���
					__HAL_TIM_ENABLE(&TIM1_Handler);//ʹ�ܶ�ʱ��1
					
				}
				else if(TIM1CH3_CAPTURE_STA==0)        //��һ�β���������
				{
					TIM1CH3_CAPTURE_STA=0;			//���
					TIM1CH3_CAPTURE_VAL=0;
					TIM1CH3_CAPTURE_PerioVAL=0;
					TIM1CH3_CAPTURE_STA|=0X20;		//��ǲ����˵�һ��������
					TIM1CH3_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_3);//��ȡ��ǰ�Ĳ���ֵ.
					TIM1CH3_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_3);
					__HAL_TIM_DISABLE(&TIM1_Handler);        //�رն�ʱ��1
					//__HAL_TIM_SET_COUNTER(&TIM1_Handler,0);  //�ö�ʱ�����㿪ʼ����
					TIM_RESET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_3);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_3,TIM_ICPOLARITY_FALLING);//��ʱ��1ͨ��1����Ϊ�½��ز���
					__HAL_TIM_ENABLE(&TIM1_Handler);//ʹ�ܶ�ʱ��1
				}		    
			}
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
    {
			if((TIM1CH4_CAPTURE_STA&0X80)==0)//��δ�ɹ�����һ������
			{
				if(TIM1CH4_CAPTURE_STA&0X40)//�ڶ��β��������أ�һ�����ڼ�����
				{
					TIM1CH4_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_4)-TIM1CH4_CAPTURE_PerioVAL;//��ȡ��ǰ�Ĳ���ֵ.
					TIM1CH4_CAPTURE_STA|=0X80;		//��ǲ���ڶ���������
					__HAL_TIM_DISABLE(&TIM1_Handler);        //�رն�ʱ��1
					__HAL_TIM_SET_COUNTER(&TIM1_Handler,0);  //�ö�ʱ�����㿪ʼ����
					TIM_RESET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_4);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_4,TIM_ICPOLARITY_RISING);//��ʱ��1ͨ��1���ûָ���ʼ�������ز���
					__HAL_TIM_ENABLE(&TIM1_Handler);//ʹ�ܶ�ʱ��1
				}
				else if((TIM1CH4_CAPTURE_STA&0X20))//&&((TIM1CH1_CAPTURE_STA&0X40)==0))		//�����½��� ���Ҳ���ӦΪ�ڶ��β��������ؽ�������ж�		
				{	  			
					TIM1CH4_CAPTURE_STA|=0X40;		//��ǳɹ�����һ�θߵ�ƽ����
					TIM1CH4_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_4)-TIM1CH4_CAPTURE_VAL;//��ȡ��ǰ�Ĳ���ֵ.
					__HAL_TIM_DISABLE(&TIM1_Handler);        //�رն�ʱ��1
					TIM_RESET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_4);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_4,TIM_ICPOLARITY_RISING);//����TIM8ͨ��1�����ز���
					__HAL_TIM_ENABLE(&TIM1_Handler);//ʹ�ܶ�ʱ��1
					
				}
				else if(TIM1CH4_CAPTURE_STA==0)        //��һ�β���������
				{
					TIM1CH4_CAPTURE_STA=0;			//���
					TIM1CH4_CAPTURE_VAL=0;
					TIM1CH4_CAPTURE_PerioVAL=0;
					TIM1CH4_CAPTURE_STA|=0X20;		//��ǲ����˵�һ��������
					TIM1CH4_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_4);//��ȡ��ǰ�Ĳ���ֵ.
					TIM1CH4_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_4);
					__HAL_TIM_DISABLE(&TIM1_Handler);        //�رն�ʱ��1
					//__HAL_TIM_SET_COUNTER(&TIM1_Handler,0);  //�ö�ʱ�����㿪ʼ����
					TIM_RESET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_4);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&TIM1_Handler,TIM_CHANNEL_4,TIM_ICPOLARITY_FALLING);//��ʱ��1ͨ��1����Ϊ�½��ز���
					__HAL_TIM_ENABLE(&TIM1_Handler);//ʹ�ܶ�ʱ��1
				}		    
			}
		}			
  }
	
	if(htim->Instance==TIM8)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
		 if((TIM8CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
			{
				if(TIM8CH1_CAPTURE_STA&0X40)//�ڶ��β��������أ�һ�����ڼ�����
				{
					TIM8CH1_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_1)-TIM8CH1_CAPTURE_PerioVAL;//��ȡ��ǰ�Ĳ���ֵ.
					TIM8CH1_CAPTURE_STA|=0X80;		//��ǲ���ڶ���������
					__HAL_TIM_DISABLE(&TIM8_Handler);        //�رն�ʱ��1
					__HAL_TIM_SET_COUNTER(&TIM8_Handler,0);  //�ö�ʱ�����㿪ʼ����
					TIM_RESET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_1);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);//��ʱ��8ͨ��1���ûָ���ʼ�������ز���
					__HAL_TIM_ENABLE(&TIM8_Handler);//ʹ�ܶ�ʱ��8
				}
				else if(TIM8CH1_CAPTURE_STA&0X20)  								//��δ��ʼ,��һ�β���������
				{
					TIM8CH1_CAPTURE_STA|=0X40;		//��ǳɹ�����һ�θߵ�ƽ����
					TIM8CH1_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_1)-TIM8CH1_CAPTURE_VAL;//��ȡ��ǰ�Ĳ���ֵ.
					__HAL_TIM_DISABLE(&TIM8_Handler);        //�رն�ʱ��1
					TIM_RESET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_1);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);//����TIM8ͨ��1�����ز���
					__HAL_TIM_ENABLE(&TIM8_Handler);//ʹ�ܶ�ʱ��8
				}
        else if(TIM8CH1_CAPTURE_STA==0)        //��һ�β���������
				{
					TIM8CH1_CAPTURE_STA=0;			//���
					TIM8CH1_CAPTURE_VAL=0;
					TIM8CH1_CAPTURE_PerioVAL=0;
					TIM8CH1_CAPTURE_STA|=0X20;		//��ǲ����˵�һ��������
					TIM8CH1_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_1);
					TIM8CH1_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_1);
					__HAL_TIM_DISABLE(&TIM8_Handler);        //�رն�ʱ��1
					//__HAL_TIM_SET_COUNTER(&TIM8_Handler,0);  //�ö�ʱ�����㿪ʼ����
					TIM_RESET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_1);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);//��ʱ��8ͨ��1����Ϊ�½��ز���
					__HAL_TIM_ENABLE(&TIM8_Handler);//ʹ�ܶ�ʱ��8
				}		    
			}	
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
    {
		 if((TIM8CH2_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
			{
				if(TIM8CH2_CAPTURE_STA&0X40)//�ڶ��β��������أ�һ�����ڼ�����
				{
					TIM8CH2_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_2)-TIM8CH2_CAPTURE_PerioVAL;//��ȡ��ǰ�Ĳ���ֵ.
					TIM8CH2_CAPTURE_STA|=0X80;		//��ǲ���ڶ���������
					__HAL_TIM_DISABLE(&TIM8_Handler);        //�رն�ʱ��8
					__HAL_TIM_SET_COUNTER(&TIM8_Handler,0);  //�ö�ʱ�����㿪ʼ����
					TIM_RESET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_2);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_2,TIM_ICPOLARITY_RISING);//��ʱ��8ͨ��2���ûָ���ʼ�������ز���
					__HAL_TIM_ENABLE(&TIM8_Handler);//ʹ�ܶ�ʱ��8
				}
				else if(TIM8CH2_CAPTURE_STA&0X20)  								//��δ��ʼ,��һ�β���������
				{
					TIM8CH2_CAPTURE_STA|=0X40;		//��ǳɹ�����һ�θߵ�ƽ����
					TIM8CH2_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_2)-TIM8CH2_CAPTURE_VAL;//��ȡ��ǰ�Ĳ���ֵ.
					__HAL_TIM_DISABLE(&TIM8_Handler);        //�رն�ʱ��8
					TIM_RESET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_2);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_2,TIM_ICPOLARITY_RISING);//����TIM8ͨ��2�����ز���
					__HAL_TIM_ENABLE(&TIM8_Handler);//ʹ�ܶ�ʱ��8
				}
        else if(TIM8CH2_CAPTURE_STA==0)        //��һ�β���������
				{
					TIM8CH2_CAPTURE_STA=0;			//���
					TIM8CH2_CAPTURE_VAL=0;
					TIM8CH2_CAPTURE_PerioVAL=0;
					TIM8CH2_CAPTURE_STA|=0X20;		//��ǲ����˵�һ��������
					TIM8CH2_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_2);
					TIM8CH2_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_2);
					__HAL_TIM_DISABLE(&TIM8_Handler);        //�رն�ʱ��1
					//__HAL_TIM_SET_COUNTER(&TIM8_Handler,0);  //�ö�ʱ�����㿪ʼ����
					TIM_RESET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_2);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_2,TIM_ICPOLARITY_FALLING);//��ʱ��8ͨ��2����Ϊ�½��ز���
					__HAL_TIM_ENABLE(&TIM8_Handler);//ʹ�ܶ�ʱ��8
				}		    
			}	
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
		{
		 if((TIM8CH3_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
			{
				if(TIM8CH3_CAPTURE_STA&0X40)//�ڶ��β��������أ�һ�����ڼ�����
				{
					TIM8CH3_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_3)-TIM8CH3_CAPTURE_PerioVAL;//��ȡ��ǰ�Ĳ���ֵ.
					TIM8CH3_CAPTURE_STA|=0X80;		//��ǲ���ڶ���������
					__HAL_TIM_DISABLE(&TIM8_Handler);        //�رն�ʱ��8
					__HAL_TIM_SET_COUNTER(&TIM8_Handler,0);  //�ö�ʱ�����㿪ʼ����
					TIM_RESET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_3);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_3,TIM_ICPOLARITY_RISING);//��ʱ��8ͨ��3���ûָ���ʼ�������ز���
					__HAL_TIM_ENABLE(&TIM8_Handler);//ʹ�ܶ�ʱ��8
				}
				else if(TIM8CH3_CAPTURE_STA&0X20)  								//��δ��ʼ,��һ�β���������
				{
					TIM8CH3_CAPTURE_STA|=0X40;		//��ǳɹ�����һ�θߵ�ƽ����
					TIM8CH3_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_3)-TIM8CH3_CAPTURE_VAL;//��ȡ��ǰ�Ĳ���ֵ.
					__HAL_TIM_DISABLE(&TIM8_Handler);        //�رն�ʱ��8
					TIM_RESET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_3);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_3,TIM_ICPOLARITY_RISING);//����TIM8ͨ��3�����ز���
					__HAL_TIM_ENABLE(&TIM8_Handler);//ʹ�ܶ�ʱ��8
				}
        else if(TIM8CH3_CAPTURE_STA==0)        //��һ�β���������
				{
					TIM8CH3_CAPTURE_STA=0;			//���
					TIM8CH3_CAPTURE_VAL=0;
					TIM8CH3_CAPTURE_PerioVAL=0;
					TIM8CH3_CAPTURE_STA|=0X20;		//��ǲ����˵�һ��������
					TIM8CH3_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_3);
					TIM8CH3_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_3);
					__HAL_TIM_DISABLE(&TIM8_Handler);        //�رն�ʱ��8
					//__HAL_TIM_SET_COUNTER(&TIM8_Handler,0);  //�ö�ʱ�����㿪ʼ����
					TIM_RESET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_3);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_3,TIM_ICPOLARITY_FALLING);//��ʱ��8ͨ��2����Ϊ�½��ز���
					__HAL_TIM_ENABLE(&TIM8_Handler);//ʹ�ܶ�ʱ��8
				}		    
			}	
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
		{
		 if((TIM8CH4_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
			{
				if(TIM8CH4_CAPTURE_STA&0X40)//�ڶ��β��������أ�һ�����ڼ�����
				{
					TIM8CH4_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_4)-TIM8CH4_CAPTURE_PerioVAL;//��ȡ��ǰ�Ĳ���ֵ.
					TIM8CH4_CAPTURE_STA|=0X80;		//��ǲ���ڶ���������
					__HAL_TIM_DISABLE(&TIM8_Handler);        //�رն�ʱ��8
					__HAL_TIM_SET_COUNTER(&TIM8_Handler,0);  //�ö�ʱ�����㿪ʼ����
					TIM_RESET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_4);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_4,TIM_ICPOLARITY_RISING);//��ʱ��8ͨ��4���ûָ���ʼ�������ز���
					__HAL_TIM_ENABLE(&TIM8_Handler);//ʹ�ܶ�ʱ��8
				}
				else if(TIM8CH4_CAPTURE_STA&0X20)  								//��δ��ʼ,��һ�β���������
				{
					TIM8CH4_CAPTURE_STA|=0X40;		//��ǳɹ�����һ�θߵ�ƽ����
					TIM8CH4_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_4)-TIM8CH4_CAPTURE_VAL;//��ȡ��ǰ�Ĳ���ֵ.
					__HAL_TIM_DISABLE(&TIM8_Handler);        //�رն�ʱ��8
					TIM_RESET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_4);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_4,TIM_ICPOLARITY_RISING);//����TIM8ͨ��4�����ز���
					__HAL_TIM_ENABLE(&TIM8_Handler);//ʹ�ܶ�ʱ��8
				}
        else if(TIM8CH4_CAPTURE_STA==0)        //��һ�β���������
				{
					TIM8CH4_CAPTURE_STA=0;			//���
					TIM8CH4_CAPTURE_VAL=0;
					TIM8CH4_CAPTURE_PerioVAL=0;
					TIM8CH4_CAPTURE_STA|=0X20;		//��ǲ����˵�һ��������
					TIM8CH4_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_4);
					TIM8CH4_CAPTURE_PerioVAL=HAL_TIM_ReadCapturedValue(&TIM8_Handler,TIM_CHANNEL_4);
					__HAL_TIM_DISABLE(&TIM8_Handler);        //�رն�ʱ��
					//__HAL_TIM_SET_COUNTER(&TIM8_Handler,0);  //�ö�ʱ�����㿪ʼ����������500ns�ļ������
					TIM_RESET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_4);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&TIM8_Handler,TIM_CHANNEL_4,TIM_ICPOLARITY_FALLING);//��ʱ��8ͨ��2����Ϊ�½��ز���
					__HAL_TIM_ENABLE(&TIM8_Handler);//ʹ�ܶ�ʱ��8
				}		    
			}	
		}		
	}
}

//�رա������ʼ������
void TIM1_Cap_DeInit(void)
{
	HAL_TIM_IC_DeInit(&TIM1_Handler);
	
	HAL_GPIO_DeInit(GPIOA,GPIO_PIN_8);
	HAL_GPIO_DeInit(GPIOA,GPIO_PIN_9);
	HAL_GPIO_DeInit(GPIOA,GPIO_PIN_10);
	HAL_GPIO_DeInit(GPIOA,GPIO_PIN_11);
	
	HAL_TIM_IC_Stop_IT(&TIM1_Handler,TIM_CHANNEL_1);   //ֹͣ����TIM1��ͨ��
	HAL_TIM_IC_Stop_IT(&TIM1_Handler,TIM_CHANNEL_2);   //ֹͣ����TIM1��ͨ��
	HAL_TIM_IC_Stop_IT(&TIM1_Handler,TIM_CHANNEL_3);   //ֹͣ����TIM1��ͨ��
	HAL_TIM_IC_Stop_IT(&TIM1_Handler,TIM_CHANNEL_4);   //ֹͣ����TIM1��ͨ��
	
	__HAL_TIM_DISABLE_IT(&TIM1_Handler,TIM_IT_UPDATE); //ʧ�ܸ����ж�
}

void TIM8_Cap_DeInit(void)
{
	HAL_TIM_IC_DeInit(&TIM8_Handler);
	
	HAL_GPIO_DeInit(GPIOC,GPIO_PIN_6);
	HAL_GPIO_DeInit(GPIOC,GPIO_PIN_7);
	HAL_GPIO_DeInit(GPIOC,GPIO_PIN_8);
	HAL_GPIO_DeInit(GPIOC,GPIO_PIN_9);
	
	HAL_TIM_IC_Stop_IT(&TIM8_Handler,TIM_CHANNEL_1);   //ֹͣ����TIM1��ͨ��
	HAL_TIM_IC_Stop_IT(&TIM8_Handler,TIM_CHANNEL_2);   //ֹͣ����TIM1��ͨ��
	HAL_TIM_IC_Stop_IT(&TIM8_Handler,TIM_CHANNEL_3);   //ֹͣ����TIM1��ͨ��
	HAL_TIM_IC_Stop_IT(&TIM8_Handler,TIM_CHANNEL_4);   //ֹͣ����TIM1��ͨ��
	
	__HAL_TIM_DISABLE_IT(&TIM8_Handler,TIM_IT_UPDATE); //ʧ�ܸ����ж�
}
//��ʱ���ײ�������ʱ��ʧ�ܣ��ر��ж�
//�˺����ᱻHAL_TIM_IC_DeInit()����
//htim:��ʱ�����
void HAL_TIM_IC_MspDeInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM1)
	{
		__HAL_RCC_TIM1_CLK_DISABLE();            //ʧ��ʱ��
	//	__HAL_RCC_GPIOA_CLK_DISABLE();  ����Ӱ��������
    HAL_NVIC_DisableIRQ(TIM1_CC_IRQn);          //�ر�TIM1�ж�   
	}
	if(htim->Instance==TIM8)
	{
		__HAL_RCC_TIM8_CLK_DISABLE();            //ʧ��ʱ��
	//	__HAL_RCC_GPIOC_CLK_DISABLE();
    HAL_NVIC_DisableIRQ(TIM8_CC_IRQn);          //�ر�TIM8�ж�
	}
}
