#include "adc.h"
#include "delay.h"


ADC_HandleTypeDef ADC1_Handler;//ADC���
ADC_HandleTypeDef ADC3_Handler;//ADC���

//��ʼ��ADC
//ch: ADC_channels 
//ͨ��ֵ 0~16ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
void ADC1_Init(void)
{ 
    ADC1_Handler.Instance=ADC1;
    ADC1_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4; 	//4��Ƶ��ADCCLK=PER_CK/4=64/4=16MHZ
    ADC1_Handler.Init.Resolution=ADC_RESOLUTION_16B;           	//16λģʽ
    ADC1_Handler.Init.ScanConvMode=DISABLE;                    	//��ɨ��ģʽ
    ADC1_Handler.Init.EOCSelection=ADC_EOC_SINGLE_CONV;       	//�ر�EOC�ж�
	  ADC1_Handler.Init.LowPowerAutoWait=DISABLE;					//�Զ��͹��Ĺر�				
    ADC1_Handler.Init.ContinuousConvMode=DISABLE;               //�ر�����ת��
    ADC1_Handler.Init.NbrOfConversion=1;                        //1��ת���ڹ��������� Ҳ����ֻת����������1 
    ADC1_Handler.Init.DiscontinuousConvMode=DISABLE;            //��ֹ����������ģʽ
    ADC1_Handler.Init.NbrOfDiscConversion=0;                    //����������ͨ����Ϊ0
    ADC1_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;      //�������
    ADC1_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;//ʹ���������
		ADC1_Handler.Init.BoostMode=ENABLE;							//BOOTģʽ�ر�
		ADC1_Handler.Init.Overrun=ADC_OVR_DATA_OVERWRITTEN;			//���µ����ݵ�����ֱ�Ӹ��ǵ�������
		ADC1_Handler.Init.OversamplingMode=DISABLE;					//�������ر�
		ADC1_Handler.Init.ConversionDataManagement=ADC_CONVERSIONDATA_DR;  //����ͨ�������ݽ���������DR�Ĵ�������
    HAL_ADC_Init(&ADC1_Handler);                                 //��ʼ�� 
	
	HAL_ADCEx_Calibration_Start(&ADC1_Handler,ADC_CALIB_OFFSET,ADC_SINGLE_ENDED); //ADCУ׼
}

//��ʼ��ADC3
//ch: ADC_channels 
//ͨ��ֵ 0~16ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
void MY_ADC3_Init(void)
{ 
    ADC3_Handler.Instance=ADC3;
    ADC3_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4; 	//4��Ƶ��ADCCLK=PER_CK/4=64/4=16MHZ
    ADC3_Handler.Init.Resolution=ADC_RESOLUTION_16B;           	//16λģʽ
    ADC3_Handler.Init.ScanConvMode=DISABLE;                    	//��ɨ��ģʽ
    ADC3_Handler.Init.EOCSelection=ADC_EOC_SINGLE_CONV;       	//�ر�EOC�ж�
	  ADC3_Handler.Init.LowPowerAutoWait=DISABLE;					//�Զ��͹��Ĺر�				
    ADC3_Handler.Init.ContinuousConvMode=DISABLE;               //�ر�����ת��
    ADC3_Handler.Init.NbrOfConversion=1;                        //1��ת���ڹ��������� Ҳ����ֻת����������1 
    ADC3_Handler.Init.DiscontinuousConvMode=DISABLE;            //��ֹ����������ģʽ
    ADC3_Handler.Init.NbrOfDiscConversion=0;                    //����������ͨ����Ϊ0
    ADC3_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;      //�������
    ADC3_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;//ʹ���������
		ADC3_Handler.Init.BoostMode=ENABLE;							//BOOTģʽ�ر�
		ADC3_Handler.Init.Overrun=ADC_OVR_DATA_OVERWRITTEN;			//���µ����ݵ�����ֱ�Ӹ��ǵ�������
		ADC3_Handler.Init.OversamplingMode=DISABLE;					//�������ر�
		ADC3_Handler.Init.ConversionDataManagement=ADC_CONVERSIONDATA_DR;  //����ͨ�������ݽ���������DR�Ĵ�������
    HAL_ADC_Init(&ADC3_Handler);                                 //��ʼ�� 
	
	HAL_ADCEx_Calibration_Start(&ADC3_Handler,ADC_CALIB_OFFSET,ADC_SINGLE_ENDED); //ADCУ׼
}

//ADC�ײ��������������ã�ʱ��ʹ��
//�˺����ᱻHAL_ADC_Init()����
//hadc:ADC���
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    GPIO_InitTypeDef GPIO_Initure;
	  RCC_PeriphCLKInitTypeDef  ADCClkInitStruct;
	
    __HAL_RCC_ADC12_CLK_ENABLE();           //ʹ��ADC1/2ʱ��
	 // __HAL_RCC_ADC3_CLK_ENABLE();			//ʹ��ADC3ʱ��
    __HAL_RCC_GPIOA_CLK_ENABLE();			//����GPIOAʱ��--------------!!!!!!!!����Ӳ���޸�
    __HAL_RCC_GPIOC_CLK_ENABLE();			//����GPIOCʱ--------------!!!!!!!!����Ӳ���޸�
		ADCClkInitStruct.PeriphClockSelection=RCC_PERIPHCLK_ADC; 
		ADCClkInitStruct.AdcClockSelection=RCC_ADCCLKSOURCE_CLKP; 
		HAL_RCCEx_PeriphCLKConfig(&ADCClkInitStruct);
	
    GPIO_Initure.Pin=GPIO_PIN_6;            //PA6--------------!!!!!!!!����Ӳ���޸�
    GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //ģ��
    GPIO_Initure.Pull=GPIO_NOPULL;          //����������
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);//--------------!!!!!!!!����Ӳ���޸�
	
	  GPIO_Initure.Pin=GPIO_PIN_4;            //PC4--------------!!!!!!!!����Ӳ���޸�
    GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //ģ��
    GPIO_Initure.Pull=GPIO_NOPULL;          //����������
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);//--------------!!!!!!!!����Ӳ���޸�
}

//���ADCֵ
//ch: ͨ��ֵ 0~16��ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
//����ֵ:ת�����
u16 Get_Adc(ADC_HandleTypeDef ADCx,u32 ch)   
{
    ADC_ChannelConfTypeDef ADC_ChanConf;
    
    ADC_ChanConf.Channel=ch;                                   //ͨ��
    ADC_ChanConf.Rank=ADC_REGULAR_RANK_1;                  	//1������
    ADC_ChanConf.SamplingTime=ADC_SAMPLETIME_810CYCLES_5;     	//����ʱ��       
		ADC_ChanConf.SingleDiff=ADC_SINGLE_ENDED;  				//���߲ɼ�          		
		ADC_ChanConf.OffsetNumber=ADC_OFFSET_NONE;             	
		ADC_ChanConf.Offset=0;   
    HAL_ADC_ConfigChannel(&ADCx,&ADC_ChanConf);        //ͨ������

    HAL_ADC_Start(&ADCx);                               		//����ADC
	
    HAL_ADC_PollForConversion(&ADCx,10);                		//��ѯת��
	return (u16)HAL_ADC_GetValue(&ADCx);	            		//�������һ��ADC�������ת�����
}
//��ȡָ��ͨ����ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
u16 Get_Adc_Average(ADC_HandleTypeDef ADCx,u32 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ADCx,ch);
		delay_ms(5);
	}
	return temp_val/times;
}

//�õ��¶�ֵ
//����ֵ:�¶�ֵ(������100��,��λ:��.)
short Get_Temprate(void)
{
		u32 adcx;
		short result;
		double temperate;
    float temp=0;
    u16 ts_cal1,ts_cal2;
    
    ts_cal1=*(vu16*)(0X1FF1E820);
    ts_cal2=*(vu16*)(0X1FF1E840);
    temp=(float)((110.0f-30.0f)/(ts_cal2-ts_cal1));
	  adcx=Get_Adc_Average(ADC3_Handler,ADC_CHANNEL_TEMPSENSOR,10);//��ȡ�ڲ��¶ȴ�����ͨ��,10��ȡƽ��
    temperate=(float)(temp*(adcx-ts_cal1)+30);
		result=temperate*=100;					//����100��.
		return result;

}
