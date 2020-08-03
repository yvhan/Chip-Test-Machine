#include "gpio.h"
#include "delay.h"
#include "timer.h"

GPIO_InitTypeDef GPIO_Initure;
//������ͨ���������ϵ��ʼ��
void GPIO_ALL_Init(void)
{

	  __HAL_RCC_GPIOA_CLK_ENABLE();           //����GPIOʱ��
	  __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE(); 
    __HAL_RCC_GPIOD_CLK_ENABLE();
	  __HAL_RCC_GPIOE_CLK_ENABLE();
	  __HAL_RCC_GPIOF_CLK_ENABLE();
	  __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();	

//A�Ÿ��ã��ڴ��� ADC DAC TIM1��ع����г�ʼ��
//PA8-11 �ϵ��ʼ��Ϊ ��ͨ����ڣ�������TIM1�г�ʼ��
//    GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;          
//    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //���
//    GPIO_Initure.Pull=GPIO_PULLUP;        //����
//    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //����
//    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
//		
//		GPIO_PinA_Reset();	
	
//PB0-7 CH1-7PMU����̵��� CH1F-CH8F �ϵ��ʼ��Ϊ�� ����Ч
//PB8-15 CH1-7PMU��������̵��� CH1S-CH8S  �ϵ��ʼ��Ϊ�� ����Ч		
	  GPIO_Initure.Pin=GPIO_PIN_All;          
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //���
    GPIO_Initure.Pull=GPIO_PULLUP;        //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
		
		GPIO_PinB_Reset();
 
//PC4-5 DPS ADC�����ģ���г�ʼ��
//PC6-9 TIM8 �ڶ�ʱ��ģ���г�ʼ��
//PC13 LEDָʾ�ƣ���ʼ���Ե����������ر�	
    GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_13;           
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //���
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //����
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
		
		GPIO_PinC_Reset();

//PD PMU FIMV FVMI
//PD0 PCL PMU����ڲ����ص��ݿ���λ���Ե��˲����� ����Ч
//PD1 FORCE-SENCE ���߶̽ӿ���λ��IO����ʱ�� ����Ч
//PD2-6 P5uA P50uA P500uA P5mA P50mA ��������������λ ����Ч
//PD7 -PDAC PMU����������źſ���λ ����Ч
//PD8-10 PL0.25 0.5 0.75 ��ǰ��λ������ѹ����λ ����Ч
//PD11 PFV ��ѹ����λ ����Ч
//PD12 PFI ��������λ ����Ч
//PD13 PMV ��ѹ����λ ����Ч
//PD14 PMI ��������λ ����Ч
//PD15 +PDAC PMU����������źſ���λ ����Ч
		GPIO_Initure.Pin=GPIO_PIN_All;   
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;//���
    GPIO_Initure.Pull=GPIO_PULLUP;        //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH; 
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);
		
		GPIO_PinD_Reset();
		

//PE DPS FV MI
//PE0-2 DL0.75 0.5 0.25 DPS��ǰ��λ��������λ ����Ч
//PE3-7 D5uA 50uA 500uA 5mA 50mA DPS������λ���� ����Ч
//PE8 -DDAC DPS���-�źſ���λ������Ч
//PE9 +DDAC DPS���+�źſ���λ������Ч
//PE10 DCL DPS�ڲ����ݸ��ؿ��ؿ���λ�� ����Ч
//PE11 DFORCE DPS������ؿ���λ������Ч
//PE12 DSENCE DPS����������ؿ���λ������Ч
//����û��ʹ��
		GPIO_Initure.Pin=GPIO_PIN_All;   
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;//���
    GPIO_Initure.Pull=GPIO_PULLUP;        //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH; 
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
		
		GPIO_PinE_Reset();

//PF0-7 BIN1-7���  ����Ч���ϵ��ʼ��Ϊ��
//PF8 EOT��� ����Ч���ϵ��ʼ��Ϊ��
		GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8;   
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;//���
    GPIO_Initure.Pull=GPIO_PULLUP;        //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH; 
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);
		
    GPIO_PinF_Reset();
		
//PF9 START�ź�����
		GPIO_Initure.Pin=GPIO_PIN_9;   
    GPIO_Initure.Mode=GPIO_MODE_INPUT;//����
    GPIO_Initure.Pull=GPIO_NOPULL;        //
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH; 
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);

//PG0-7 CH1-8ͨ���߼��źŷ�����ƽ�DIR1-8 H:MCU->CH L:CH->MCU �ϵ��ʼ��Ϊ��
		GPIO_Initure.Pin=GPIO_PIN_All;   
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;//���
    GPIO_Initure.Pull=GPIO_PULLUP;        //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH; 
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);
		
		GPIO_PinG_Reset();

//PH ����0\1��������ţ����඼û��ʹ��   
//    GPIO_Initure.Pin=GPIO_PIN_2|GPIO_PIN_3; //PH2,3 
//    HAL_GPIO_Init(GPIOH,&GPIO_Initure);

//PI0-7 CH1-8ͨ���߼����Ƽ̵��� L��Ч CH1D-CH8D����ͨ���Ƿ�͵�Ƭ��������
//PI8 L:5V�� H:3.3V��ѹѡ��λ 
//PI9-11 CF1/CF2/CF3 ����˿ѡ��λ H��Ч
    GPIO_Initure.Pin=GPIO_PIN_All;   
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;//���
    GPIO_Initure.Pull=GPIO_PULLUP;        //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH; 
    HAL_GPIO_Init(GPIOI,&GPIO_Initure);
		
		GPIO_PinI_Reset();
}

void GPIO_Init_ExcBin(void)
{

	  __HAL_RCC_GPIOA_CLK_ENABLE();           //����GPIOʱ��
	  __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE(); 
    __HAL_RCC_GPIOD_CLK_ENABLE();
	  __HAL_RCC_GPIOE_CLK_ENABLE();
	  __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();	
/***********�ر�PMU��DPS���ܣ���Ϊһ��ͨ��״̬**************/
		Pmu_FV(0,0,5000);
		delay_ms(3);
		Dps_FV(0,0,50000);
		delay_ms(3);
		SetDpsR(OFF);//��DPS
/***********�رն�ʱ������**************/	  
	  TIM1_Cap_DeInit();//�رն�ʱ��1���ܣ�ֹͣ��������
	  PAInit_DeTIM1();//��ʼ��TIM1��Ϊ��ͨIO��
	  HAL_TIM_PWM_DeInit(&TIM8_Handler);//�ر�PWM
	  PCInit_DeTIM8();//��ʼ��Ϊ��ͨIO��
	
//PB0-7 CH1-7PMU����̵��� CH1F-CH8F �ϵ��ʼ��Ϊ�� ����Ч
//PB8-15 CH1-7PMU��������̵��� CH1S-CH8S  �ϵ��ʼ��Ϊ�� ����Ч		
	  GPIO_Initure.Pin=GPIO_PIN_All;          
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //���
    GPIO_Initure.Pull=GPIO_PULLUP;        //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
		
		GPIO_PinB_Reset();
 
//PC4-5 DPS ADC�����ģ���г�ʼ��
//PC6-9 TIM8 �ڶ�ʱ��ģ���г�ʼ��
//PC13 LEDָʾ��,�����ﲻ�ó�ʼ��
    GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;           
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //���
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //����
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET);	

//PD PMU FIMV FVMI
//PD0 PCL PMU����ڲ����ص��ݿ���λ���Ե��˲����� ����Ч
//PD1 FORCE-SENCE ���߶̽ӿ���λ��IO����ʱ�� ����Ч
//PD2-6 P5uA P50uA P500uA P5mA P50mA ��������������λ ����Ч
//PD7 -PDAC PMU����������źſ���λ ����Ч
//PD8-10 PL0.25 0.5 0.75 ��ǰ��λ������ѹ����λ ����Ч
//PD11 PFV ��ѹ����λ ����Ч
//PD12 PFI ��������λ ����Ч
//PD13 PMV ��ѹ����λ ����Ч
//PD14 PMI ��������λ ����Ч
//PD15 +PDAC PMU����������źſ���λ ����Ч
		GPIO_Initure.Pin=GPIO_PIN_All;   
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;//���
    GPIO_Initure.Pull=GPIO_PULLUP;        //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH; 
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);
		
		GPIO_PinD_Reset();
		

//PE DPS FV MI
//PE0-2 DL0.75 0.5 0.25 DPS��ǰ��λ��������λ ����Ч
//PE3-7 D5uA 50uA 500uA 5mA 50mA DPS������λ���� ����Ч
//PE8 -DDAC DPS���-�źſ���λ������Ч
//PE9 +DDAC DPS���+�źſ���λ������Ч
//PE10 DCL DPS�ڲ����ݸ��ؿ��ؿ���λ�� ����Ч
//PE11 DFORCE DPS������ؿ���λ������Ч
//PE12 DSENCE DPS����������ؿ���λ������Ч
//����û��ʹ��
		GPIO_Initure.Pin=GPIO_PIN_All;   
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;//���
    GPIO_Initure.Pull=GPIO_PULLUP;        //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH; 
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
		
		GPIO_PinE_Reset();

//PF0-7 BIN1-7���  ����Ч���ϵ��ʼ��Ϊ��
//PF8 EOT��� ����Ч���ϵ��ʼ��Ϊ��
//����������ڱ���BIN�źź�EOT��������Բ���ʼ���������
		
//PF9 START�ź�����
//START��ʼ�������������Ҳ�Ƕ���

//PG0-7 CH1-8ͨ���߼��źŷ�����ƽ�DIR1-8 H:MCU->CH L:CH->MCU �ϵ��ʼ��Ϊ��
		GPIO_Initure.Pin=GPIO_PIN_All;   
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;//���
    GPIO_Initure.Pull=GPIO_PULLUP;        //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH; 
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);
		
		GPIO_PinG_Reset();

//PH ����0\1��������ţ����඼û��ʹ��   


//PI0-7 CH1-8ͨ���߼����Ƽ̵��� L��Ч CH1D-CH8D����ͨ���Ƿ�͵�Ƭ��������
//PI8 L:5V�� H:3.3V��ѹѡ��λ 
//PI9-11 CF1/CF2/CF3 ����˿ѡ��λ H��Ч
    GPIO_Initure.Pin=GPIO_PIN_All;   
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;//���
    GPIO_Initure.Pull=GPIO_PULLUP;        //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH; 
    HAL_GPIO_Init(GPIOI,&GPIO_Initure);
		
		GPIO_PinI_Reset();
}

void BinEOT_Init(void)
{
		__HAL_RCC_GPIOF_CLK_ENABLE();
	
//PF0-7 BIN1-7���  ����Ч���ϵ��ʼ��Ϊ��
//PF8 EOT��� ����Ч���ϵ��ʼ��Ϊ��
		GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8;   
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;//���
    GPIO_Initure.Pull=GPIO_PULLUP;        //����
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
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);	//PB0��1
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
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_0,GPIO_PIN_RESET);	//PE0��0
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
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_0,GPIO_PIN_RESET);	//PF0��0
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_1,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_2,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_3,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_4,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_5,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_6,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_7,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_8,GPIO_PIN_SET); //EOT  �ϵ�Ϊ1
}

void GPIO_PinG_Reset(void)
{
    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_0,GPIO_PIN_RESET);	//PG0��0
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
    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_0,GPIO_PIN_SET);	//PI0��1
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

//PMU����
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

//��ͨ��CH1-CH8��������оƬIO������ͨ��ѡ��ʽ��8λ�ӵ͵��߷ֱ����CH1-CH8
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
//BIN����λ
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

//��IO�ڳ�ʼ��Ϊ��ͨ����ڣ���ΪIO���Ǹ���Ϊ��ʱ����
void PAInit_DeTIM1(void)
{
	GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;
  GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //���
  GPIO_Initure.Pull=GPIO_PULLUP;          //����
  GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;   //����
  HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET);
}

void PCInit_DeTIM8(void)
{
	GPIO_Initure.Pin=GPIO_PIN_6;          
  GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //���
  GPIO_Initure.Pull=GPIO_PULLUP;          //����
  GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //����
  HAL_GPIO_Init(GPIOC,&GPIO_Initure);
	
	GPIO_Initure.Pin=GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;          
  GPIO_Initure.Mode=GPIO_MODE_INPUT;  //����
  GPIO_Initure.Pull=GPIO_NOPULL;          
  GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //����
  HAL_GPIO_Init(GPIOC,&GPIO_Initure);
	
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET);
}

void Direct_MCUtoCH(int ch,u8 sw)//��:CHtoMCU �ߣ�MCUtoCH
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

void Fix(u8 cfn,u8 vol)//cfn:�޵��̵���ѡ��0:ȫ�� 1/2/3:�� CF1��2��3 �޵���λѡ��vol:1:3.3V 0:5V
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

//��������оƬ�ܽ�
void PullUp(int ch,u8 sw)
{
	if(sw)
	{
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1,GPIO_PIN_RESET);//ʹ�õ�������ʱ��������λ
		delay_ms(2);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_RESET);
	  
	  switch(ch)
		{
			case 1:HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET);//��SENCE 1
			case 2:HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);//��SENCE 2 
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
			case 1:HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);//��SENCE 1
			case 2:HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);//��SENCE 2
			case 3:HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);
			case 4:HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_SET);
			case 5:HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);
			case 6:HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET);
			case 7:HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
			case 8:HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET);
		}
		delay_ms(2);//���⻹û���ü��ر�SENSE
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_SET);//��������ʱ���ص���������
	  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1,GPIO_PIN_SET);
	}
}
