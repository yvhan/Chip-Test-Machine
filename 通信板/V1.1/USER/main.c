#include "sys.h"
#include "main.h"
#include "delay.h"
#include "usart.h" 
#include "gpio.h"
//#include "iwdg.h"
/************************************************
 T832MDͨ�Ű����
 ���ߣ����
 ʱ�䣺2020��5��8��
 �汾��V1.0
 ��Ȩ���У�����ؾ���
 Copyright(C) ��������΢���ӹɷ����޹�˾
 All rights reserved
************************************************/
int main(void)
{	
	Cache_Enable();               //��L1-Cache
	HAL_Init();				        		//��ʼ��HAL��
	Stm32_Clock_Init(160,5,2,4);  //����ϵͳʱ��,400Mhz 
	delay_init(400);						  //��ʱ��ʼ��
	
	GPIO_All_Init();              //��ͨ���ų�ʼ��
	uart1_init(115200);           //����P8����
	uart2_init(115200);				    //���԰�Ƭѡ����
	uart3_init(115200);          //����PC����
//	IWDG_Init(IWDG_PRESCALER_64,1000); 	//��Ƶ��Ϊ64,����ֵΪ2000,���ʱ��Ϊ4s	

  while(1)
  {
    int i=0,m=0,n=0,p=0,UnRec_flag=0,EOT_flag=0,TestReport=0;	

		if(Usart1_Rec_Start())//P8���Ϳ�ʼ32�����ָ������,�ظ�����ʼ������������
		{
			GPIO_All_Init();              //��ͨ���ų�ʼ״̬�ָ�
			Start_Test();
			while(EOT_Check()!=0x00000000)//û�м����ɾͼ����ȴ�һ��ʱ�� 
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
			else if(EOT_Check()!=0x00000000)//���һ��ʱ�����Ȼû��ȫ��������
			{
				EOT_flag=0;
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
			}
			TestReport=1;	//��ϸ�����ϱ����б�־λ��P8�п�ʼ���ԾͿ����ϴ���ϸ���ݣ���ʹû������յ����е�EOT
			SET_BIT(USART1->CR1, USART_CR1_RE);
			
		}
//		IWDG_Feed();
		if(Usart3_TData_Start()&&TestReport)//PCͨ������3������ϸ��������
		{
			for(m=0;m<32;m++)
			{
				//		IWDG_Feed();
				UnRec_flag=0;
				modebus_rec_flag2=0;//��������ź���λ
				Usart_select(m);//Ƭѡ���԰崮��
				delay_ms(1);        //��ƬѡоƬ�л�ʱ��
				//usart2���ںͲ��԰忨ͨ�ţ����ͻ�ȡ��ز��԰忨��Ϣ��ָ��0xbe 0x03 0xff 0xff
				USART2_TX_BUF[0]=0xbe;
				USART2_TX_BUF[1]=0x03;
				USART2_TX_BUF[2]=0xff;
				USART2_TX_BUF[3]=0xff;
				HAL_UART_Transmit(&UART2_Handler,(uint8_t*)USART2_TX_BUF,4,1000);	
				while(__HAL_UART_GET_FLAG(&UART2_Handler,UART_FLAG_TC)!=SET);		
				//�ȴ��ͽ��ղ��԰忨�ϴ�����Ϣ
				while(!Usart2_TData_Rec())//��һ��ʱ���ڵȴ���Ϣ����
				{
					n++;
					delay_ms(1);
					if(n>300)//���ֵ��Ҫ��Ϲ��ܰ���źţ���������źŶ�ʧ
					{
						n=0;
						UnRec_flag=1;
						//					IWDG_Feed();
						break;
					}
				}
				//ͨ������3�ϴ���ϸ������Ϣ
				if(m==0)//����
				{
					printf("HardBIN  OS_HV  OS_CS  OS_ROVP OS_DRN Iop_HV BVDSS IDSS HVDD_on HVDD_off IHVDD_op IHVDD_st VTH_CC TOFF_max\r\n");
					printf("LimitLo  -0.8V  -0.8V  -0.8V   -0.8V  -0.8V  0.3V  0.3V  241ns   294ns   588ns    65   32   65   600uA   9250uA_R  9250uA_G  9250uA_B  -1uA_R -1uA_G -1uA_B\r\n" );
					printf("LimitHi  -0.3V  -0.3V  -0.3V   -0.3V  -0.3V  0.8V  0.8V  366ns   366ns   732ns    68   34   68   950uA   16250uA_R 16250uA_G 16250uA_B  1uA_R  1uA_G  1uA_B\r\n" );
				}
				if(!UnRec_flag)//�յ��˲��԰忨����Ϣ
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
					else //if(USART2_RX_BUF[6]==0xb0)//��һ���޵�����ʱ
					{
						for(p=0;p<20;p++)
						{
							printf(" %4.2f",byte2Float(&USART2_RX_BUF[8+4*p]));
						}
					}
					printf(" %x",USART1_TX_BUF[m+4]);
					printf("\r\n");
				}
				else//ͨ�ų�ʱ������������ݶ�ʧ
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
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_All,GPIO_PIN_RESET);//���ͺ�����źţ�������԰淴������
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

void Bin_Select(uint16_t count)//countȡֵ 0-31
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

void Usart_select(uint16_t count)//countȡֵ 0-31
{
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9,GPIO_PIN_RESET);
	delay_ms(1);
	HAL_GPIO_WritePin(GPIOG,(count*32),GPIO_PIN_SET);
}

void SendBintoP8()
{
	int j,k=0;
	unsigned char Bin=0;
	for(j=0;j<32;j++)      //�ɼ�32����bin�ź�
	{
		Bin=0;
//		delay_ms(1);   
		Bin_Select(j);      //bin�ź�Ƭѡ
//		delay_ms(1);        //��ƬѡоƬ�л�ʱ��
		for(k=0;k<3;k++)
		{
			Bin=Bin_Collect();  //�ɼ�BIN�ź�
		}
		USART1_TX_BUF[j+4]=Bin;	//��BIN�źŷ���������
	}
	USART1_TX_BUF[0]=0xbe;
	USART1_TX_BUF[1]=0x25;
	USART1_TX_BUF[2]=0xff;
	USART1_TX_BUF[3]=0xff;
	USART1_TX_BUF[36]=CRC_Sum(&USART1_TX_BUF[1],35)>>8;//��λ
	USART1_TX_BUF[37]=CRC_Sum(&USART1_TX_BUF[1],35);//��λ
	HAL_UART_Transmit(&UART1_Handler,(uint8_t*)USART1_TX_BUF,38,2000);	//����BIN����
	while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);		//�ȴ����ͽ���
	SET_BIT(USART1->CR1, USART_CR1_RE);//ʹ�ܽ���
}
