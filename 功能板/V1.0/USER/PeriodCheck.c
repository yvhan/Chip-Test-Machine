#include "PeriodCheck.h"
#include "main.h"
#include "usart.h" 

long HIGHCYCLE_11,HIGHCYCLE_12,HIGHCYCLE_13,HIGHCYCLE_14=0;
long PERIOD_11,PERIOD_12,PERIOD_13,PERIOD_14=0;  //定时器1通道高电平\周期变量
double DUTYCYCLE_11,DUTYCYCLE_12,DUTYCYCLE_13,DUTYCYCLE_14=0.0;//定时器1通道占空比


long HIGHCYCLE_81,HIGHCYCLE_82,HIGHCYCLE_83,HIGHCYCLE_84=0;
long PERIOD_81,PERIOD_82,PERIOD_83,PERIOD_84=0;  //定时器8通道高电平\周期变量
float DUTYCYCLE_81,DUTYCYCLE_82,DUTYCYCLE_83,DUTYCYCLE_84=0;//定时器8通道占空比


long TIM1_CH1_PeriodCheck(void)
{ 
	if(TIM1CH1_CAPTURE_STA&0X80) //TIM1捕获一个周期
	{
//		HIGHCYCLE_11=TIM1CH1_CAPTURE_STA&0X1F; 
//		HIGHCYCLE_11*=0xFFFF;		 	    //溢出时间总和
//		HIGHCYCLE_11+=TIM1CH1_CAPTURE_VAL;      //得到总的高电平时间
//		HIGHCYCLE_11*=5;               //每记一次数表示5ns,所以次数要乘5得到实际时间
//		printf("HIGH:%ld nsTIM1CH1\r\n",HIGHCYCLE_11);//打印总的高电平时间
		
		PERIOD_11=TIM1CH1_CAPTURE_STA&0X1F; 
		PERIOD_11*=0XFFFF;		 	    //溢出时间总和
//		PERIOD_11+=TIM1CH1_CAPTURE_PerioVAL;      //得到总的一个周期时间
		PERIOD_11*=5;               //每记一次数表示5ns,所以次数要乘5得到实际时间
		printf("Period:%ld nsTIM1CH1\r\n",PERIOD_11);//打印总的周期时间
//		DUTYCYCLE_11 =((((double)TIM1CH1_CAPTURE_VAL*100)/(double)TIM1CH1_CAPTURE_PerioVAL));
//		printf("Cycle:%3.0f%% \r\n",DUTYCYCLE_11);//打印占空比
		TIM1CH1_CAPTURE_STA=0;          //开启下一次捕获周期
		return PERIOD_11/8; 
	}	
}

long TIM1_CH2_PeriodCheck(void)
{ 
	if(TIM1CH2_CAPTURE_STA&0X80) //TIM1捕获一个周期
	{
//		HIGHCYCLE_12=TIM1CH2_CAPTURE_STA&0X1F; 
//		HIGHCYCLE_12*=0xFFFF;		 	    //溢出时间总和
//		HIGHCYCLE_12+=TIM1CH2_CAPTURE_VAL;      //得到总的高电平时间
//		HIGHCYCLE_12*=5;               //每记一次数表示5ns,所以次数要乘5得到实际时间
//		printf("HIGH:%ld nsTIM1CH2\r\n",HIGHCYCLE_12);//打印总的高电平时间
		
		PERIOD_12=TIM1CH2_CAPTURE_STA&0X1F; 
		PERIOD_12*=0XFFFF;		 	    //溢出时间总和
		PERIOD_12+=TIM1CH2_CAPTURE_PerioVAL;      //得到总的一个周期时间
		PERIOD_12*=5;               //每记一次数表示5ns,所以次数要乘5得到实际时间
		printf("Period:%ld nsTIM1CH2\r\n",PERIOD_12);//打印总的周期时间
//		DUTYCYCLE_12 =((((double)TIM1CH2_CAPTURE_VAL*100)/(double)TIM1CH2_CAPTURE_PerioVAL));
//		printf("Cycle:%3.0f%% \r\n",DUTYCYCLE_12);//打印占空比
		TIM1CH2_CAPTURE_STA=0;          //开启下一次捕获周期
		return PERIOD_12/8;
	}	
}

long TIM1_CH3_PeriodCheck(void)
{ 
	if(TIM1CH3_CAPTURE_STA&0X80) //TIM1捕获一个周期
	{
//		HIGHCYCLE_13=TIM1CH3_CAPTURE_STA&0X1F; 
//		HIGHCYCLE_13*=0xFFFF;		 	    //溢出时间总和
//		HIGHCYCLE_13+=TIM1CH3_CAPTURE_VAL;      //得到总的高电平时间
//		HIGHCYCLE_13*=5;               //每记一次数表示5ns,所以次数要乘5得到实际时间
//		printf("HIGH:%ld nsTIM1CH3\r\n",HIGHCYCLE_13);//打印总的高电平时间
		
		PERIOD_13=TIM1CH3_CAPTURE_STA&0X1F; 
		PERIOD_13*=0XFFFF;		 	    //溢出时间总和
		PERIOD_13+=TIM1CH3_CAPTURE_PerioVAL;      //得到总的一个周期时间
		PERIOD_13*=5;               //每记一次数表示5ns,所以次数要乘5得到实际时间
		printf("Period:%ld nsTIM1CH3\r\n",PERIOD_13);//打印总的周期时间
//		DUTYCYCLE_13 =((((double)TIM1CH3_CAPTURE_VAL*100)/(double)TIM1CH3_CAPTURE_PerioVAL));
//		printf("Cycle:%3.0f%% \r\n",DUTYCYCLE_13);//打印占空比
		TIM1CH3_CAPTURE_STA=0;          //开启下一次捕获周期
		return PERIOD_13/8;
	}	
}

long TIM1_CH4_PeriodCheck(void)
{ 
	if(TIM1CH4_CAPTURE_STA&0X80) //TIM1捕获一个周期
	{
//		HIGHCYCLE_14=TIM1CH4_CAPTURE_STA&0X1F; 
//		HIGHCYCLE_14*=0xFFFF;		 	    //溢出时间总和
//		HIGHCYCLE_14+=TIM1CH4_CAPTURE_VAL;      //得到总的高电平时间
//		HIGHCYCLE_14*=5;               //每记一次数表示5ns,所以次数要乘5得到实际时间
//		printf("HIGH:%ld nsTIM1CH4\r\n",HIGHCYCLE_14);//打印总的高电平时间
		
		PERIOD_14=TIM1CH4_CAPTURE_STA&0X1F; 
		PERIOD_14*=0XFFFF;		 	    //溢出时间总和
		PERIOD_14+=TIM1CH4_CAPTURE_PerioVAL;      //得到总的一个周期时间
		PERIOD_14*=5;               //每记一次数表示5ns,所以次数要乘5得到实际时间
		printf("Period:%ld nsTIM1CH4\r\n",PERIOD_14);//打印总的周期时间
//		DUTYCYCLE_14 =((((double)TIM1CH4_CAPTURE_VAL*100)/(double)TIM1CH4_CAPTURE_PerioVAL));
//		printf("Cycle:%3.0f%% \r\n",DUTYCYCLE_14);//打印占空比
		TIM1CH4_CAPTURE_STA=0;          //开启下一次捕获周期
		return PERIOD_14/8;
	}	
}
float TIM8_CH1_PeriodCheck(void)
{ 
	if(TIM8CH1_CAPTURE_STA&0X80) //TIM8捕获一个周期
	{
		HIGHCYCLE_81=TIM8CH1_CAPTURE_STA&0X1F; 
		HIGHCYCLE_81*=0xFFFF;		 	    //溢出时间总和
		HIGHCYCLE_81+=TIM8CH1_CAPTURE_VAL;      //得到总的高电平时间
		HIGHCYCLE_81*=5;               //每记一次数表示5ns,所以次数要乘5得到实际时间
		//printf("HIGH:%ld nsTIM8CH1\r\n",HIGHCYCLE_81);//打印总的高电平时间
		
		PERIOD_81=TIM8CH1_CAPTURE_STA&0X1F; 
		PERIOD_81*=0XFFFF;		 	    //溢出时间总和
		PERIOD_81+=TIM8CH1_CAPTURE_PerioVAL;      //得到总的一个周期时间
		PERIOD_81*=5;               //每记一次数表示5ns,所以次数要乘5得到实际时间
		//printf("Period:%ld nsTIM8CH1\r\n",PERIOD_81);//打印总的周期时间
		DUTYCYCLE_81 =((((float)TIM8CH1_CAPTURE_VAL*100)/(float)TIM8CH1_CAPTURE_PerioVAL));
		//printf("Cycle:%3.1f%% \r\n",DUTYCYCLE_81);//打印占空比
		delay_ms(1);
		TIM8CH1_CAPTURE_STA=0;          //开启下一次捕获周期
		return DUTYCYCLE_81;
	}	
}

float TIM8_CH2_PeriodCheck(void)
{ 
	if(TIM8CH2_CAPTURE_STA&0X80) //TIM8捕获一个周期
	{
		HIGHCYCLE_82=TIM8CH2_CAPTURE_STA&0X1F; 
		HIGHCYCLE_82*=0xFFFF;		 	    //溢出时间总和
		HIGHCYCLE_82+=TIM8CH2_CAPTURE_VAL;      //得到总的高电平时间
		//HIGHCYCLE_82*=5;               //每记一次数表示5ns,所以次数要乘5得到实际时间
		//printf("HIGH:%ld nsTIM8CH2\r\n",HIGHCYCLE_82);//打印总的高电平时间
		
		PERIOD_82=TIM8CH2_CAPTURE_STA&0X1F; 
		PERIOD_82*=0XFFFF;		 	    //溢出时间总和
		PERIOD_82+=TIM8CH2_CAPTURE_PerioVAL;      //得到总的一个周期时间
		//PERIOD_82*=5;               //每记一次数表示5ns,所以次数要乘5得到实际时间
		//printf("Period:%ld nsTIM8CH2\r\n",PERIOD_82);//打印总的周期时间
		DUTYCYCLE_82 =((((float)TIM8CH2_CAPTURE_VAL*100)/(float)TIM8CH2_CAPTURE_PerioVAL));
//		printf("Cycle:%3.1f%% \r\n",DUTYCYCLE_82);//打印占空比
		delay_ms(1);
		TIM8CH2_CAPTURE_STA=0;          //开启下一次捕获周期
		return DUTYCYCLE_82;
	}	
}

float TIM8_CH3_PeriodCheck(void)
{ 
	if(TIM8CH3_CAPTURE_STA&0X80) //TIM8捕获一个周期
	{
		HIGHCYCLE_83=TIM8CH3_CAPTURE_STA&0X1F; 
		HIGHCYCLE_83*=0xFFFF;		 	    //溢出时间总和
		HIGHCYCLE_83+=TIM8CH3_CAPTURE_VAL;      //得到总的高电平时间
		//HIGHCYCLE_83*=5;               //每记一次数表示5ns,所以次数要乘5得到实际时间
		//printf("HIGH:%ld nsTIM8CH3\r\n",HIGHCYCLE_83);//打印总的高电平时间
		
		PERIOD_83=TIM8CH3_CAPTURE_STA&0X1F; 
		PERIOD_83*=0XFFFF;		 	    //溢出时间总和
		PERIOD_83+=TIM8CH3_CAPTURE_PerioVAL;      //得到总的一个周期时间
		//PERIOD_83*=5;               //每记一次数表示5ns,所以次数要乘5得到实际时间
		//printf("Period:%ld nsTIM8CH3\r\n",PERIOD_83);//打印总的周期时间
		DUTYCYCLE_83 =((((float)TIM8CH3_CAPTURE_VAL*100)/(float)TIM8CH3_CAPTURE_PerioVAL));
//		printf("Cycle:%3.1f%% \r\n",DUTYCYCLE_83);//打印占空比
		delay_ms(1);
		TIM8CH3_CAPTURE_STA=0;          //开启下一次捕获周期
		return DUTYCYCLE_83;
	}	
}

float TIM8_CH4_PeriodCheck(void)
{ 
	if(TIM8CH4_CAPTURE_STA&0X80) //TIM8捕获一个周期
	{
		HIGHCYCLE_84=TIM8CH4_CAPTURE_STA&0X1F; 
		HIGHCYCLE_84*=0xFFFF;		 	    //溢出时间总和
		HIGHCYCLE_84+=TIM8CH4_CAPTURE_VAL;      //得到总的高电平时间
		//HIGHCYCLE_84*=5;               //每记一次数表示5ns,所以次数要乘5得到实际时间
		//printf("HIGH:%ld nsTIM8CH4\r\n",HIGHCYCLE_84);//打印总的高电平时间
		
		PERIOD_84=TIM8CH4_CAPTURE_STA&0X1F; 
		PERIOD_84*=0XFFFF;		 	    //溢出时间总和
		PERIOD_84+=TIM8CH4_CAPTURE_PerioVAL;      //得到总的一个周期时间
		//PERIOD_84*=5;               //每记一次数表示5ns,所以次数要乘5得到实际时间
		//printf("Period:%ld nsTIM8CH4\r\n",PERIOD_84);//打印总的周期时间
		DUTYCYCLE_84 =((((float)TIM8CH4_CAPTURE_VAL*100)/(float)TIM8CH4_CAPTURE_PerioVAL));
//		printf("Cycle:%3.1f%% \r\n",DUTYCYCLE_84);//打印占空比
		delay_ms(1);
		TIM8CH4_CAPTURE_STA=0;          //开启下一次捕获周期
		return DUTYCYCLE_84;
	}	
}
