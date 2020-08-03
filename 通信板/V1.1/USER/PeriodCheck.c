#include "PeriodCheck.h"
#include "timer.h"

extern u8  TIM1CH1_CAPTURE_STA;		//���벶��״̬		    				
extern u16	TIM1CH1_CAPTURE_VAL;	//���벶��ֵ
extern u16 TIM1CH1_CAPTURE_PerioVAL;//���벶������ֵ

extern u8  TIM1CH2_CAPTURE_STA;		//���벶��״̬		    				
extern u16	TIM1CH2_CAPTURE_VAL;	//���벶��ֵ
extern u16 TIM1CH2_CAPTURE_PerioVAL;//���벶������ֵ

extern u8  TIM1CH3_CAPTURE_STA;		//���벶��״̬		    				
extern u16	TIM1CH3_CAPTURE_VAL;	//���벶��ֵ
extern u16 TIM1CH3_CAPTURE_PerioVAL;//���벶������ֵ

extern u8  TIM1CH4_CAPTURE_STA;		//���벶��״̬		    				
extern u16	TIM1CH4_CAPTURE_VAL;	//���벶��ֵ
extern u16 TIM1CH4_CAPTURE_PerioVAL;//���벶������ֵ

long HIGHCYCLE_11,HIGHCYCLE_12,HIGHCYCLE_13,HIGHCYCLE_14=0;
long PERIOD_11,PERIOD_12,PERIOD_13,PERIOD_14=0;  //��ʱ��1ͨ���ߵ�ƽ\���ڱ���
double DUTYCYCLE_11,DUTYCYCLE_12,DUTYCYCLE_13,DUTYCYCLE_14=0.0;//��ʱ��1ͨ��ռ�ձ�

extern u8  TIM8CH1_CAPTURE_STA;		//���벶��״̬		    				
extern u16 TIM8CH1_CAPTURE_VAL;	//���벶��ֵ 
extern u16 TIM8CH1_CAPTURE_PerioVAL;//���벶������ֵ

extern u8  TIM8CH2_CAPTURE_STA;	//���벶��״̬		    				
extern u16	TIM8CH2_CAPTURE_VAL;	//���벶��ֵ
extern u16 TIM8CH2_CAPTURE_PerioVAL;//���벶������ֵ

extern u8  TIM8CH3_CAPTURE_STA;	//���벶��״̬		    				
extern u16	TIM8CH3_CAPTURE_VAL;	//���벶��ֵ
extern u16 TIM8CH3_CAPTURE_PerioVAL;//���벶������ֵ

extern u8  TIM8CH4_CAPTURE_STA;	//���벶��״̬		    				
extern u16	TIM8CH4_CAPTURE_VAL;	//���벶��
extern u16 TIM8CH4_CAPTURE_PerioVAL;//���벶������ֵ

long HIGHCYCLE_81,HIGHCYCLE_82,HIGHCYCLE_83,HIGHCYCLE_84=0;
long PERIOD_81,PERIOD_82,PERIOD_83,PERIOD_84=0;  //��ʱ��8ͨ���ߵ�ƽ\���ڱ���
double DUTYCYCLE_81,DUTYCYCLE_82,DUTYCYCLE_83,DUTYCYCLE_84=0.0;//��ʱ��8ͨ��ռ�ձ�


void TIM1_CH1_PeriodCheck(void)
{ 
	if(TIM1CH1_CAPTURE_STA&0X80) //TIM1����һ������
	{
		HIGHCYCLE_11=TIM1CH1_CAPTURE_STA&0X1F; 
		HIGHCYCLE_11*=0xFFFF;		 	    //���ʱ���ܺ�
		HIGHCYCLE_11+=TIM1CH1_CAPTURE_VAL;      //�õ��ܵĸߵ�ƽʱ��
		HIGHCYCLE_11*=5;               //ÿ��һ������ʾ5ns,���Դ���Ҫ��5�õ�ʵ��ʱ��
		printf("HIGH:%ld nsTIM1CH1\r\n",HIGHCYCLE_11);//��ӡ�ܵĸߵ�ƽʱ��
		
		PERIOD_11=TIM1CH1_CAPTURE_STA&0X1F; 
		PERIOD_11*=0XFFFF;		 	    //���ʱ���ܺ�
		PERIOD_11+=TIM1CH1_CAPTURE_PerioVAL;      //�õ��ܵ�һ������ʱ��
		PERIOD_11*=5;               //ÿ��һ������ʾ5ns,���Դ���Ҫ��5�õ�ʵ��ʱ��
		printf("Period:%ld nsTIM1CH1\r\n",PERIOD_11);//��ӡ�ܵ�����ʱ��
		DUTYCYCLE_11 =((((double)TIM1CH1_CAPTURE_VAL*100)/(double)TIM1CH1_CAPTURE_PerioVAL));
		printf("Cycle:%3.0f%% \r\n",DUTYCYCLE_11);//��ӡռ�ձ�
		TIM1CH1_CAPTURE_STA=0;          //������һ�β�������
	}	
}

void TIM1_CH2_PeriodCheck(void)
{ 
	if(TIM1CH2_CAPTURE_STA&0X80) //TIM1����һ������
	{
		HIGHCYCLE_12=TIM1CH2_CAPTURE_STA&0X1F; 
		HIGHCYCLE_12*=0xFFFF;		 	    //���ʱ���ܺ�
		HIGHCYCLE_12+=TIM1CH2_CAPTURE_VAL;      //�õ��ܵĸߵ�ƽʱ��
		HIGHCYCLE_12*=5;               //ÿ��һ������ʾ5ns,���Դ���Ҫ��5�õ�ʵ��ʱ��
		printf("HIGH:%ld nsTIM1CH2\r\n",HIGHCYCLE_12);//��ӡ�ܵĸߵ�ƽʱ��
		
		PERIOD_12=TIM1CH2_CAPTURE_STA&0X1F; 
		PERIOD_12*=0XFFFF;		 	    //���ʱ���ܺ�
		PERIOD_12+=TIM1CH2_CAPTURE_PerioVAL;      //�õ��ܵ�һ������ʱ��
		PERIOD_12*=5;               //ÿ��һ������ʾ5ns,���Դ���Ҫ��5�õ�ʵ��ʱ��
		printf("Period:%ld nsTIM1CH2\r\n",PERIOD_12);//��ӡ�ܵ�����ʱ��
		DUTYCYCLE_12 =((((double)TIM1CH2_CAPTURE_VAL*100)/(double)TIM1CH2_CAPTURE_PerioVAL));
		printf("Cycle:%3.0f%% \r\n",DUTYCYCLE_12);//��ӡռ�ձ�
		TIM1CH2_CAPTURE_STA=0;          //������һ�β�������
	}	
}

void TIM1_CH3_PeriodCheck(void)
{ 
	if(TIM1CH3_CAPTURE_STA&0X80) //TIM1����һ������
	{
		HIGHCYCLE_13=TIM1CH3_CAPTURE_STA&0X1F; 
		HIGHCYCLE_13*=0xFFFF;		 	    //���ʱ���ܺ�
		HIGHCYCLE_13+=TIM1CH3_CAPTURE_VAL;      //�õ��ܵĸߵ�ƽʱ��
		HIGHCYCLE_13*=5;               //ÿ��һ������ʾ5ns,���Դ���Ҫ��5�õ�ʵ��ʱ��
		printf("HIGH:%ld nsTIM1CH3\r\n",HIGHCYCLE_13);//��ӡ�ܵĸߵ�ƽʱ��
		
		PERIOD_13=TIM1CH3_CAPTURE_STA&0X1F; 
		PERIOD_13*=0XFFFF;		 	    //���ʱ���ܺ�
		PERIOD_13+=TIM1CH3_CAPTURE_PerioVAL;      //�õ��ܵ�һ������ʱ��
		PERIOD_13*=5;               //ÿ��һ������ʾ5ns,���Դ���Ҫ��5�õ�ʵ��ʱ��
		printf("Period:%ld nsTIM1CH3\r\n",PERIOD_13);//��ӡ�ܵ�����ʱ��
		DUTYCYCLE_13 =((((double)TIM1CH3_CAPTURE_VAL*100)/(double)TIM1CH3_CAPTURE_PerioVAL));
		printf("Cycle:%3.0f%% \r\n",DUTYCYCLE_13);//��ӡռ�ձ�
		TIM1CH3_CAPTURE_STA=0;          //������һ�β�������
	}	
}

void TIM1_CH4_PeriodCheck(void)
{ 
	if(TIM1CH4_CAPTURE_STA&0X80) //TIM1����һ������
	{
		HIGHCYCLE_14=TIM1CH4_CAPTURE_STA&0X1F; 
		HIGHCYCLE_14*=0xFFFF;		 	    //���ʱ���ܺ�
		HIGHCYCLE_14+=TIM1CH4_CAPTURE_VAL;      //�õ��ܵĸߵ�ƽʱ��
		HIGHCYCLE_14*=5;               //ÿ��һ������ʾ5ns,���Դ���Ҫ��5�õ�ʵ��ʱ��
		printf("HIGH:%ld nsTIM1CH4\r\n",HIGHCYCLE_14);//��ӡ�ܵĸߵ�ƽʱ��
		
		PERIOD_14=TIM1CH4_CAPTURE_STA&0X1F; 
		PERIOD_14*=0XFFFF;		 	    //���ʱ���ܺ�
		PERIOD_14+=TIM1CH4_CAPTURE_PerioVAL;      //�õ��ܵ�һ������ʱ��
		PERIOD_14*=5;               //ÿ��һ������ʾ5ns,���Դ���Ҫ��5�õ�ʵ��ʱ��
		printf("Period:%ld nsTIM1CH4\r\n",PERIOD_14);//��ӡ�ܵ�����ʱ��
		DUTYCYCLE_14 =((((double)TIM1CH4_CAPTURE_VAL*100)/(double)TIM1CH4_CAPTURE_PerioVAL));
		printf("Cycle:%3.0f%% \r\n",DUTYCYCLE_14);//��ӡռ�ձ�
		TIM1CH4_CAPTURE_STA=0;          //������һ�β�������
	}	
}
void TIM8_CH1_PeriodCheck(void)
{ 
	if(TIM8CH1_CAPTURE_STA&0X80) //TIM8����һ������
	{
		HIGHCYCLE_81=TIM8CH1_CAPTURE_STA&0X1F; 
		HIGHCYCLE_81*=0xFFFF;		 	    //���ʱ���ܺ�
		HIGHCYCLE_81+=TIM8CH1_CAPTURE_VAL;      //�õ��ܵĸߵ�ƽʱ��
		HIGHCYCLE_81*=5;               //ÿ��һ������ʾ5ns,���Դ���Ҫ��5�õ�ʵ��ʱ��
		printf("HIGH:%ld nsTIM8CH1\r\n",HIGHCYCLE_81);//��ӡ�ܵĸߵ�ƽʱ��
		
		PERIOD_81=TIM8CH1_CAPTURE_STA&0X1F; 
		PERIOD_81*=0XFFFF;		 	    //���ʱ���ܺ�
		PERIOD_81+=TIM8CH1_CAPTURE_PerioVAL;      //�õ��ܵ�һ������ʱ��
		PERIOD_81*=5;               //ÿ��һ������ʾ5ns,���Դ���Ҫ��5�õ�ʵ��ʱ��
		printf("Period:%ld nsTIM8CH1\r\n",PERIOD_81);//��ӡ�ܵ�����ʱ��
		DUTYCYCLE_81 =((((double)TIM8CH1_CAPTURE_VAL*100)/(double)TIM8CH1_CAPTURE_PerioVAL));
		printf("Cycle:%3.1f%% \r\n",DUTYCYCLE_81);//��ӡռ�ձ�
		TIM8CH1_CAPTURE_STA=0;          //������һ�β�������
	}	
}

void TIM8_CH2_PeriodCheck(void)
{ 
	if(TIM8CH2_CAPTURE_STA&0X80) //TIM8����һ������
	{
		HIGHCYCLE_82=TIM8CH2_CAPTURE_STA&0X1F; 
		HIGHCYCLE_82*=0xFFFF;		 	    //���ʱ���ܺ�
		HIGHCYCLE_82+=TIM8CH2_CAPTURE_VAL;      //�õ��ܵĸߵ�ƽʱ��
		HIGHCYCLE_82*=5;               //ÿ��һ������ʾ5ns,���Դ���Ҫ��5�õ�ʵ��ʱ��
		printf("HIGH:%ld nsTIM8CH2\r\n",HIGHCYCLE_82);//��ӡ�ܵĸߵ�ƽʱ��
		
		PERIOD_82=TIM8CH2_CAPTURE_STA&0X1F; 
		PERIOD_82*=0XFFFF;		 	    //���ʱ���ܺ�
		PERIOD_82+=TIM8CH2_CAPTURE_PerioVAL;      //�õ��ܵ�һ������ʱ��
		PERIOD_82*=5;               //ÿ��һ������ʾ5ns,���Դ���Ҫ��5�õ�ʵ��ʱ��
		printf("Period:%ld nsTIM8CH2\r\n",PERIOD_82);//��ӡ�ܵ�����ʱ��
		DUTYCYCLE_82 =((((double)TIM8CH2_CAPTURE_VAL*100)/(double)TIM8CH2_CAPTURE_PerioVAL));
		printf("Cycle:%3.1f%% \r\n",DUTYCYCLE_82);//��ӡռ�ձ�
		TIM8CH2_CAPTURE_STA=0;          //������һ�β�������
	}	
}

void TIM8_CH3_PeriodCheck(void)
{ 
	if(TIM8CH3_CAPTURE_STA&0X80) //TIM8����һ������
	{
		HIGHCYCLE_83=TIM8CH3_CAPTURE_STA&0X1F; 
		HIGHCYCLE_83*=0xFFFF;		 	    //���ʱ���ܺ�
		HIGHCYCLE_83+=TIM8CH3_CAPTURE_VAL;      //�õ��ܵĸߵ�ƽʱ��
		HIGHCYCLE_83*=5;               //ÿ��һ������ʾ5ns,���Դ���Ҫ��5�õ�ʵ��ʱ��
		printf("HIGH:%ld nsTIM8CH3\r\n",HIGHCYCLE_83);//��ӡ�ܵĸߵ�ƽʱ��
		
		PERIOD_83=TIM8CH3_CAPTURE_STA&0X1F; 
		PERIOD_83*=0XFFFF;		 	    //���ʱ���ܺ�
		PERIOD_83+=TIM8CH3_CAPTURE_PerioVAL;      //�õ��ܵ�һ������ʱ��
		PERIOD_83*=5;               //ÿ��һ������ʾ5ns,���Դ���Ҫ��5�õ�ʵ��ʱ��
		printf("Period:%ld nsTIM8CH3\r\n",PERIOD_83);//��ӡ�ܵ�����ʱ��
		DUTYCYCLE_83 =((((double)TIM8CH3_CAPTURE_VAL*100)/(double)TIM8CH3_CAPTURE_PerioVAL));
		printf("Cycle:%3.1f%% \r\n",DUTYCYCLE_83);//��ӡռ�ձ�
		TIM8CH3_CAPTURE_STA=0;          //������һ�β�������
	}	
}

void TIM8_CH4_PeriodCheck(void)
{ 
	if(TIM8CH4_CAPTURE_STA&0X80) //TIM8����һ������
	{
		HIGHCYCLE_84=TIM8CH4_CAPTURE_STA&0X1F; 
		HIGHCYCLE_84*=0xFFFF;		 	    //���ʱ���ܺ�
		HIGHCYCLE_84+=TIM8CH4_CAPTURE_VAL;      //�õ��ܵĸߵ�ƽʱ��
		HIGHCYCLE_84*=5;               //ÿ��һ������ʾ5ns,���Դ���Ҫ��5�õ�ʵ��ʱ��
		printf("HIGH:%ld nsTIM8CH4\r\n",HIGHCYCLE_84);//��ӡ�ܵĸߵ�ƽʱ��
		
		PERIOD_84=TIM8CH4_CAPTURE_STA&0X1F; 
		PERIOD_84*=0XFFFF;		 	    //���ʱ���ܺ�
		PERIOD_84+=TIM8CH4_CAPTURE_PerioVAL;      //�õ��ܵ�һ������ʱ��
		PERIOD_84*=5;               //ÿ��һ������ʾ5ns,���Դ���Ҫ��5�õ�ʵ��ʱ��
		printf("Period:%ld nsTIM8CH4\r\n",PERIOD_84);//��ӡ�ܵ�����ʱ��
		DUTYCYCLE_84 =((((double)TIM8CH4_CAPTURE_VAL*100)/(double)TIM8CH4_CAPTURE_PerioVAL));
		printf("Cycle:%3.1f%% \r\n",DUTYCYCLE_84);//��ӡռ�ձ�
		TIM8CH4_CAPTURE_STA=0;          //������һ�β�������
	}	
}
