#include "sys.h"
#include "main.h"
#include "delay.h"
#include "usart.h" 
#include "gpio.h"
//#include "iwdg.h"
/************************************************
 T832MD通信板程序
 作者：刘宇航
 时间：2020年5月8日
 版本：V1.0
 版权所有，盗版必究。
 Copyright(C) 深圳市天微电子股份有限公司
 All rights reserved
************************************************/
int main(void)
{	
	Cache_Enable();               //打开L1-Cache
	HAL_Init();				        		//初始化HAL库
	Stm32_Clock_Init(160,5,2,4);  //设置系统时钟,400Mhz 
	delay_init(400);						  //延时初始化
	
	GPIO_All_Init();              //普通引脚初始化
	uart1_init(115200);           //连接P8串口
	uart2_init(115200);				    //测试板片选串口
	uart3_init(115200);          //连接PC串口
//	IWDG_Init(IWDG_PRESCALER_64,1000); 	//分频数为64,重载值为2000,溢出时间为4s	

  while(1)
  {
    int i=0,m=0,n=0,p=0,UnRec_flag=0,EOT_flag=0,TestReport=0;	

		if(Usart1_Rec_Start())//P8发送开始32测测试指令数据,回复并开始正常测试流程
		{
			GPIO_All_Init();              //普通引脚初始状态恢复
			Start_Test();
			while(EOT_Check()!=0x00000000)//没有检测完成就继续等待一段时间 
			{
				i++;
				delay_ms(1);
				if(i>2000)
				{
					i=0;
					//		IWDG_Feed();
					break;
				}
			}
			if(EOT_Check()==0x00000000)
			{ 
				SendBintoP8();
				EOT_flag=1;
			}
			else if(EOT_Check()!=0x00000000)//如果一段时间后仍然没有全部检测完成
			{
				EOT_flag=0;
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
			}
			TestReport=1;	//详细数据上报运行标志位，P8有开始测试就可以上传详细数据，即使没有完成收到所有的EOT
			SET_BIT(USART1->CR1, USART_CR1_RE);
			
		}
//		IWDG_Feed();
		if(Usart3_TData_Start()&&TestReport)//PC通过串口3请求详细测试数据
		{
			for(m=0;m<32;m++)
			{
				//		IWDG_Feed();
				UnRec_flag=0;
				modebus_rec_flag2=0;//避免干扰信号置位
				Usart_select(m);//片选测试板串口
				delay_ms(1);        //给片选芯片切换时间
				//usart2串口和测试板卡通信，发送获取相关测试板卡信息的指令0xbe 0x03 0xff 0xff
				USART2_TX_BUF[0]=0xbe;
				USART2_TX_BUF[1]=0x03;
				USART2_TX_BUF[2]=0xff;
				USART2_TX_BUF[3]=0xff;
				HAL_UART_Transmit(&UART2_Handler,(uint8_t*)USART2_TX_BUF,4,1000);	
				while(__HAL_UART_GET_FLAG(&UART2_Handler,UART_FLAG_TC)!=SET);		
				//等待和接收测试板卡上传的信息
				while(!Usart2_TData_Rec())//在一定时间内等待信息到来
				{
					n++;
					delay_ms(1);
					if(n>300)//这个值需要配合功能板的信号，否则造成信号丢失
					{
						n=0;
						UnRec_flag=1;
						//					IWDG_Feed();
						break;
					}
				}
				//通过串口3上传详细串口信息
				if(m==0)//标题
				{
					printf("HardBIN  OS_HV  OS_CS  OS_ROVP OS_DRN Iop_HV BVDSS IDSS HVDD_on HVDD_off IHVDD_op IHVDD_st VTH_CC TOFF_max\r\n");
					printf("LimitLo  -0.8V  -0.8V  -0.8V   -0.8V  -0.8V  0.3V  0.3V  241ns   294ns   588ns    65   32   65   600uA   9250uA_R  9250uA_G  9250uA_B  -1uA_R -1uA_G -1uA_B\r\n" );
					printf("LimitHi  -0.3V  -0.3V  -0.3V   -0.3V  -0.3V  0.8V  0.8V  366ns   366ns   732ns    68   34   68   950uA   16250uA_R 16250uA_G 16250uA_B  1uA_R  1uA_G  1uA_B\r\n" );
				}
				if(!UnRec_flag)//收到了测试板卡的信息
				{
					printf("TEST%d ",m+1);
					if((USART2_RX_BUF[38]==0x93&&USART2_RX_BUF[37]==0x80)||(USART2_RX_BUF[38]>0x93&&USART2_RX_BUF[38]<0xb7))//295~366
					{
						for(p=0;p<7;p++)
						{
							printf(" %4.2f",byte2Float(&USART2_RX_BUF[8+4*p]));
						}
						printf(" -----");
						for(p=7;p<19;p++)
						{
							printf(" %4.2f",byte2Float(&USART2_RX_BUF[8+4*p]));
						}
					}
					else //if(USART2_RX_BUF[6]==0xb0)//多一个修调数据时
					{
						for(p=0;p<20;p++)
						{
							printf(" %4.2f",byte2Float(&USART2_RX_BUF[8+4*p]));
						}
					}
					printf(" %x",USART1_TX_BUF[m+4]);
					printf("\r\n");
				}
				else//通信超时，传输错误，数据丢失
				{
					printf("TEST%d ResultIsNotReveiced,TryAgainOrReportItToRelativeDepartment\r\n",m+1);
				}
			}
		}
//	  IWDG_Feed();
	}
}

void Start_Test()
{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_All,GPIO_PIN_SET);//1-16
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_All,GPIO_PIN_SET);//17-32
	  delay_ms(150);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_All,GPIO_PIN_RESET);//发送后清除信号，避免测试版反复进入
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_All,GPIO_PIN_RESET);//17-32
}

unsigned long int EOT_Check()
{
	int i;
	unsigned long int EOT_State=0xFFFFFFFF;
	for(i=0;i<16;i++)
	{
		if(HAL_GPIO_ReadPin(GPIOE, (0x0001<<i)))
		{
			if(HAL_GPIO_ReadPin(GPIOE, (0x0001<<i)))
			{
				EOT_State|=0x00000001<<i;
			}
			
		}
		else
		{
			if(!HAL_GPIO_ReadPin(GPIOE, (0x0001<<i)))
			{
				EOT_State&=  ~(0x00000001<<i);
			}
			
		}
	}
	for(i=0;i<16;i++)
	{
		if(HAL_GPIO_ReadPin(GPIOF, (0x0001<<i)))
		{
			if(HAL_GPIO_ReadPin(GPIOF, (0x0001<<i)))
			{
			 EOT_State|=0x00000001<<(i+16);
			} 
		}
		else
		{
			if(!HAL_GPIO_ReadPin(GPIOF, (0x0001<<i)))
			{
			  EOT_State&= ~(0x00000001<<(i+16));
			} 
		}
	}
	return EOT_State;
}

void Bin_Select(uint16_t count)//count取值 0-31
{
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOG,count,GPIO_PIN_SET);
}

unsigned char Bin_Collect()
{
	int i;
	unsigned char Bin_state=0xff;
	for(i=0;i<8;i++)
	{
		if(HAL_GPIO_ReadPin(GPIOI, (0x0001<<i)))
		{
//			delay_ms(1);
			if(HAL_GPIO_ReadPin(GPIOI, (0x0001<<i)))
			{
				Bin_state |= (0x01<<i);
			}
		}
		else
		{
			if(!HAL_GPIO_ReadPin(GPIOI, (0x0001<<i)))
			{
				Bin_state &=  ~(0x01<<i);
			}
			
		}
	}
	return Bin_state;
}

void Usart_select(uint16_t count)//count取值 0-31
{
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9,GPIO_PIN_RESET);
	delay_ms(1);
	HAL_GPIO_WritePin(GPIOG,(count*32),GPIO_PIN_SET);
}

void SendBintoP8()
{
	int j,k=0;
	unsigned char Bin=0;
	for(j=0;j<32;j++)      //采集32个的bin信号
	{
		Bin=0;
//		delay_ms(1);   
		Bin_Select(j);      //bin信号片选
//		delay_ms(1);        //给片选芯片切换时间
		for(k=0;k<3;k++)
		{
			Bin=Bin_Collect();  //采集BIN信号
		}
		USART1_TX_BUF[j+4]=Bin;	//将BIN信号放入数组中
	}
	USART1_TX_BUF[0]=0xbe;
	USART1_TX_BUF[1]=0x25;
	USART1_TX_BUF[2]=0xff;
	USART1_TX_BUF[3]=0xff;
	USART1_TX_BUF[36]=CRC_Sum(&USART1_TX_BUF[1],35)>>8;//高位
	USART1_TX_BUF[37]=CRC_Sum(&USART1_TX_BUF[1],35);//低位
	HAL_UART_Transmit(&UART1_Handler,(uint8_t*)USART1_TX_BUF,38,2000);	//发送BIN数据
	while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);		//等待发送结束
	SET_BIT(USART1->CR1, USART_CR1_RE);//使能接收
}
