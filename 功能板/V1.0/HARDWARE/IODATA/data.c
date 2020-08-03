#include "data.h"

GPIO_TypeDef *GPIOx;	
uint16_t GPIO_Pin;

void IO_Data(u8 ch,u8 *data,int ONE_HT,int ONE_LT,int ZERO_HT,int ZERO_LT)
{
  int Data_length,Data_bit,ONE_HIGH,ONE_LOW,ZERO_HIGH,ZERO_LOW;
	
	ONE_HT=ONE_HT/45;//1码高电平时间转化为循环次数，循环一次大约50ns
	ONE_LT=ONE_LT/50;
	ZERO_HT=ZERO_HT/55;
	ZERO_LT=ZERO_LT/55;
  if(ch==1)
	{
		GPIOx=GPIOC;
		GPIO_Pin=GPIO_PIN_6;
		SetDutR(0x01,ON);//开CH1 
		Direct_MCUtoCH(1,1);//低:CHtoMCU 高：MCUtoCH CH1设置为MCUtoCH
	}
	if(ch==2)
	{
		GPIOx=GPIOC;
		GPIO_Pin=GPIO_PIN_7;
		SetDutR(0x02,ON);//开CH2
		Direct_MCUtoCH(2,1);//低:CHtoMCU 高：MCUtoCH CH1设置为MCUtoCH
	}
	if(ch==3)
	{
		GPIOx=GPIOC;
		GPIO_Pin=GPIO_PIN_8;
		SetDutR(0x04,ON);//开CH3
		Direct_MCUtoCH(3,1);//低:CHtoMCU 高：MCUtoCH CH1设置为MCUtoCH
	}
	if(ch==4)
	{
		GPIOx=GPIOC;
		GPIO_Pin=GPIO_PIN_9;
		SetDutR(0x08,ON);//开CH4
		Direct_MCUtoCH(4,1);//低:CHtoMCU 高：MCUtoCH CH1设置为MCUtoCH
	}
	if(ch==5)
	{
		GPIOx=GPIOA;
		GPIO_Pin=GPIO_PIN_8;
		SetDutR(0x10,ON);//开CH5
		Direct_MCUtoCH(5,1);//低:CHtoMCU 高：MCUtoCH CH1设置为MCUtoCH
	}
	if(ch==6)
	{
		GPIOx=GPIOA;
		GPIO_Pin=GPIO_PIN_9;
		SetDutR(0x20,ON);//开CH6
		Direct_MCUtoCH(6,1);//低:CHtoMCU 高：MCUtoCH CH1设置为MCUtoCH
	}
	if(ch==7)
	{
		GPIOx=GPIOA;
		GPIO_Pin=GPIO_PIN_10;
		SetDutR(0x40,ON);//开CH7
		Direct_MCUtoCH(7,1);//低:CHtoMCU 高：MCUtoCH CH1设置为MCUtoCH
	}
	if(ch==8)
	{
		GPIOx=GPIOA;
		GPIO_Pin=GPIO_PIN_11;
		SetDutR(0x80,ON);//开CH8
		Direct_MCUtoCH(8,1);//低:CHtoMCU 高：MCUtoCH CH1设置为MCUtoCH
	}
//	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET);
	//PC6口输出3个以上字节数据
	for(Data_length=0;Data_length<3;Data_length++)
	{
		for(Data_bit=0;Data_bit<8;Data_bit++)
		{
		  if((data[Data_length]<<Data_bit)&0x80)
			{
				for(ONE_HIGH=0;ONE_HIGH<ONE_HT;ONE_HIGH++)
				{
					HAL_GPIO_WritePin(GPIOx,GPIO_Pin,GPIO_PIN_SET);
				}		
				for(ONE_LOW=0;ONE_LOW<ONE_LT;ONE_LOW++)
				{
					HAL_GPIO_WritePin(GPIOx,GPIO_Pin,GPIO_PIN_RESET);
				}
			}
			else
			{
				for(ZERO_HIGH=0;ZERO_HIGH<ZERO_HT;ZERO_HIGH++)
				{
					HAL_GPIO_WritePin(GPIOx,GPIO_Pin,GPIO_PIN_SET);
				}			
				for(ZERO_LOW=0;ZERO_LOW<ZERO_LT;ZERO_LOW++)
				{
					HAL_GPIO_WritePin(GPIOx,GPIO_Pin,GPIO_PIN_RESET);
				}
			}
		}
	}
}

