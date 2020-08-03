#include "dac.h"

DAC_HandleTypeDef DAC1_Handler;			//DAC���
DAC_ChannelConfTypeDef DACCH1_Config;	//DAC1ͨ��1
DAC_ChannelConfTypeDef DACCH2_Config;	//DAC1ͨ��2
	
//��ʼ��DAC
void DAC1_Init(void)
{
    DAC1_Handler.Instance=DAC1;
    HAL_DAC_Init(&DAC1_Handler);               	//��ʼ��DAC
 //ͨ��1
    DACCH1_Config.DAC_Trigger=DAC_TRIGGER_NONE;             //��ʹ�ô�������
    DACCH1_Config.DAC_OutputBuffer=DAC_OUTPUTBUFFER_DISABLE;//DAC1�������ر�
    HAL_DAC_ConfigChannel(&DAC1_Handler,&DACCH1_Config,DAC_CHANNEL_1);//DACͨ��1����
 //ͨ��2
	  DACCH2_Config.DAC_Trigger=DAC_TRIGGER_NONE;             //��ʹ�ô�������
    DACCH2_Config.DAC_OutputBuffer=DAC_OUTPUTBUFFER_DISABLE;//DAC1�������ر�
    HAL_DAC_ConfigChannel(&DAC1_Handler,&DACCH2_Config,DAC_CHANNEL_2);//DACͨ��2����
    
    HAL_DAC_Start(&DAC1_Handler,DAC_CHANNEL_1);  //����DACͨ��1
	  HAL_DAC_Start(&DAC1_Handler,DAC_CHANNEL_2);  //����DACͨ��2
}

//DAC�ײ�������ʱ�����ã����� ����
//�˺����ᱻHAL_DAC_Init()����
//hdac:DAC���
void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac)
{      
    GPIO_InitTypeDef GPIO_Initure;
	
    __HAL_RCC_DAC12_CLK_ENABLE();           //ʹ��DACʱ��
    __HAL_RCC_GPIOA_CLK_ENABLE();			//����GPIOAʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_4|GPIO_PIN_5;            //PA4\5--ͨ��1��2
    GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //ģ��
    GPIO_Initure.Pull=GPIO_NOPULL;          //����������
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
}

//����ͨ��1�����ѹ
//vol:0~6,����0~6V
void DAC1CH1_Set_Vol(float vol)
{
	double temp1=vol;
	//temp1/=1000;
	temp1=temp1*4096/6;
	
	HAL_DAC_SetValue(&DAC1_Handler,DAC_CHANNEL_1,DAC_ALIGN_12B_R,temp1);//12λ�Ҷ������ݸ�ʽ����DACֵ
}
void DAC1CH1_Set_Cur(float cur)
{
	double temp1=cur;
	//temp1/=1000;
	temp1=temp1*2048;
	
	HAL_DAC_SetValue(&DAC1_Handler,DAC_CHANNEL_1,DAC_ALIGN_12B_R,temp1);//12λ�Ҷ������ݸ�ʽ����DACֵ
}
//����ͨ��2�����ѹ
//vol:0~6,����0~6V
void DAC1CH2_Set_Vol(float vol)
{
	double temp2=vol;
	//temp2/=1000;
	temp2=temp2*4096/6;
	
	HAL_DAC_SetValue(&DAC1_Handler,DAC_CHANNEL_2,DAC_ALIGN_12B_R,temp2);//12λ�Ҷ������ݸ�ʽ����DACֵ
}
