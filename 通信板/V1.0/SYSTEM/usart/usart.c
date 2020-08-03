#include "usart.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��os,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//os ʹ��	  
#endif

//V1.0�޸�˵�� 
////////////////////////////////////////////////////////////////////////////////// 	  
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)	
#if 1
//#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART3->ISR&0X40)==0);//ѭ������,ֱ���������   
	USART3->TDR=(u8)ch;      
	return ch;
}
#endif 

#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART1_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
u8 USART1_TX_BUF[USART_TX_LEN];
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
int cnt1=0;
int length1=0;
u16 USART1_RX_STA=0;       //����״̬���	
int modebus_rec_flag1=0;  //������ɱ�־λ
u8 aRxBuffer[RXBUFFERSIZE];//HAL��ʹ�õĴ��ڽ��ջ���
UART_HandleTypeDef UART1_Handler; //UART���

//��ʼ��IO ����1
//bound:������
void uart1_init(u32 bound)
{	
	//UART ��ʼ������
	UART1_Handler.Instance=USART1;					    //USART1
	UART1_Handler.Init.BaudRate=bound;				    //������
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&UART1_Handler);					    //HAL_UART_Init()��ʹ��UART1
	
	HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
  
}

//����1�жϷ������
void USART1_IRQHandler(void)                	
{ 
	u32 timeout=0;
    u32 maxDelay=0x1FFFF;
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();    
#endif
	
	HAL_UART_IRQHandler(&UART1_Handler);	//����HAL���жϴ����ú���
	
	timeout=0;
  while (HAL_UART_GetState(&UART1_Handler)!=HAL_UART_STATE_READY)//�ȴ�����
	{
        timeout++;////��ʱ����
        if(timeout>maxDelay) break;		
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART1_Handler,(u8 *)aRxBuffer, RXBUFFERSIZE)!=HAL_OK)//һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
	{
        timeout++; //��ʱ����
        if(timeout>maxDelay) break;	
	}
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
#endif
} 
#endif	

#if EN_USART2_RX   //���ʹ���˽���
//����2�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART2_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
u8 USART2_TX_BUF[USART_TX_LEN];
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
int length2=0;
u16 USART2_RX_STA=0;       //����״̬���	
int modebus_rec_flag2=0;  //������ɱ�־λ
u8 aRxBuffer2[RXBUFFERSIZE];//HAL��ʹ�õĴ��ڽ��ջ���
UART_HandleTypeDef UART2_Handler; //UART���

//��ʼ��IO ����2
//bound:������
void uart2_init(u32 bound)
{	
	//UART ��ʼ������
	UART2_Handler.Instance=USART2;					    //USART2
	UART2_Handler.Init.BaudRate=bound;				    //������
	UART2_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART2_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART2_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART2_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	UART2_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&UART2_Handler);					    //HAL_UART_Init()��ʹ��UART2
	
	HAL_UART_Receive_IT(&UART2_Handler, (u8 *)aRxBuffer2, RXBUFFERSIZE);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
}

 
//����2�жϷ������
void USART2_IRQHandler(void)                	
{ 
	u32 timeout=0;
    u32 maxDelay=0x1FFFF;
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();    
#endif
	
	HAL_UART_IRQHandler(&UART2_Handler);	//����HAL���жϴ����ú���
	
	timeout=0;
    while (HAL_UART_GetState(&UART2_Handler)!=HAL_UART_STATE_READY)//�ȴ�����
	{
        timeout++;////��ʱ����
        if(timeout>maxDelay) break;		
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART2_Handler,(u8 *)aRxBuffer2, RXBUFFERSIZE)!=HAL_OK)//һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
	{
        timeout++; //��ʱ����
        if(timeout>maxDelay) break;	
	}
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
#endif
} 
#endif

#if EN_USART3_RX   //���ʹ���˽���
//����3�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART3_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
u8 USART3_TX_BUF[USART_TX_LEN];
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
int length3=0;
u16 USART3_RX_STA=0;       //����״̬���	
int modebus_rec_flag3=0;  //������ɱ�־λ
u8 aRxBuffer3[RXBUFFERSIZE];//HAL��ʹ�õĴ��ڽ��ջ���
UART_HandleTypeDef UART3_Handler; //UART���

//��ʼ��IO ����3
//bound:������
void uart3_init(u32 bound)
{	
	//UART ��ʼ������
	UART3_Handler.Instance=USART3;					    //USART3
	UART3_Handler.Init.BaudRate=bound;				    //������
	UART3_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART3_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART3_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART3_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	UART3_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&UART3_Handler);					    //HAL_UART_Init()��ʹ��UART3
	
	HAL_UART_Receive_IT(&UART3_Handler, (u8 *)aRxBuffer3, RXBUFFERSIZE);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
}

 
//����3�жϷ������
void USART3_IRQHandler(void)                	
{ 
	u32 timeout=0;
    u32 maxDelay=0x1FFFF;
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();    
#endif
	
	HAL_UART_IRQHandler(&UART3_Handler);	//����HAL���жϴ����ú���
	
	timeout=0;
    while (HAL_UART_GetState(&UART3_Handler)!=HAL_UART_STATE_READY)//�ȴ�����
	{
        timeout++;////��ʱ����
        if(timeout>maxDelay) break;		
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART3_Handler,(u8 *)aRxBuffer3, RXBUFFERSIZE)!=HAL_OK)//һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
	{
        timeout++; //��ʱ����
        if(timeout>maxDelay) break;	
	}
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
#endif
} 
#endif 
//UART�ײ��ʼ����ʱ��ʹ�ܣ��������ã��ж�����
//�˺����ᱻHAL_UART_Init()����
//huart:���ھ��

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO�˿�����
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART1)//����Ǵ���1�����д���1 MSP��ʼ��
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();			//ʹ��GPIOBʱ��
		__HAL_RCC_USART1_CLK_ENABLE();			//ʹ��USART1ʱ��
	
		GPIO_Initure.Pin=GPIO_PIN_14;			//PB14
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			//����
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//����
		GPIO_Initure.Alternate=GPIO_AF4_USART1;	//����ΪUSART1
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//��ʼ��PB

		GPIO_Initure.Pin=GPIO_PIN_15;			//PB15
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//��ʼ��PB15
		
#if EN_USART1_RX
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//ʹ��USART1�ж�ͨ��
		HAL_NVIC_SetPriority(USART1_IRQn,3,3);			//��ռ���ȼ�3�������ȼ�3
#endif	
	}
	
	if(huart->Instance==USART2)//����Ǵ���2�����д���2 MSP��ʼ��
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//ʹ��GPIOAʱ��
		__HAL_RCC_USART2_CLK_ENABLE();			//ʹ��USART2ʱ��
	
		GPIO_Initure.Pin=GPIO_PIN_2;			//PA2
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			//����
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//����
		GPIO_Initure.Alternate=GPIO_AF7_USART2;	//����ΪUSART2
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA2

		GPIO_Initure.Pin=GPIO_PIN_3;			//PA3
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA3
		
#if EN_USART2_RX
		HAL_NVIC_EnableIRQ(USART2_IRQn);				//ʹ��USART2�ж�ͨ��
		HAL_NVIC_SetPriority(USART2_IRQn,3,3);			//��ռ���ȼ�3�������ȼ�3
#endif	
	}
	
	if(huart->Instance==USART3)//����Ǵ���3�����д���3 MSP��ʼ��
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();			//ʹ��GPIOBʱ��
		__HAL_RCC_USART3_CLK_ENABLE();			//ʹ��USART3ʱ��
	
		GPIO_Initure.Pin=GPIO_PIN_10;			//PB10
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			//����
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//����
		GPIO_Initure.Alternate=GPIO_AF7_USART3;	//����ΪUSART3
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//��ʼ��PB

		GPIO_Initure.Pin=GPIO_PIN_11;			//PB11
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//��ʼ��PB11
		
#if EN_USART3_RX
		HAL_NVIC_EnableIRQ(USART3_IRQn);				//ʹ��USART3�ж�ͨ��
		HAL_NVIC_SetPriority(USART3_IRQn,3,3);			//��ռ���ȼ�3�������ȼ�3
#endif	
	}

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)//����Ǵ���1
	{
		if((aRxBuffer[0]==0xbe)&&(USART1_RX_BUF[0]!=0xbe))
		{
			USART1_RX_STA=0;
			USART1_RX_BUF[USART1_RX_STA]=aRxBuffer[0];
		}
		else if(USART1_RX_BUF[0]==0xbe)
		{
			USART1_RX_STA++;
			if(USART1_RX_STA==1)
			{
				USART1_RX_BUF[USART1_RX_STA]=aRxBuffer[0];
			  length1=aRxBuffer[0];
			}
			else
			{USART1_RX_BUF[USART1_RX_STA]=aRxBuffer[0];}
			if(USART1_RX_STA>length1)USART1_RX_STA=0;//�������ݴ���,���¿�ʼ����	 
			if(USART1_RX_STA==length1)modebus_rec_flag1=1;
		}
	}
	
	if(huart->Instance==USART2)//����Ǵ���2
	{
		if((aRxBuffer2[0]==0x9c)&&(USART2_RX_BUF[0]!=0x9c))
		{
			USART2_RX_STA=0;
			USART2_RX_BUF[USART2_RX_STA]=aRxBuffer2[0];
		}
		else if(USART2_RX_BUF[0]==0x9c)
		{
			USART2_RX_STA++;
			USART2_RX_BUF[USART2_RX_STA]=aRxBuffer2[0];
			if(USART2_RX_STA>88)USART2_RX_STA=0;//�������ݴ���,���¿�ʼ����	 
//			if((USART2_RX_BUF[6]==0xB0)&&(USART2_RX_STA>8))
//			{
				if(USART2_RX_STA==87)modebus_rec_flag2=1;
//				if(USART2_RX_STA>88)USART2_RX_STA=0;//�������ݴ���,���¿�ʼ����	 
//			}
//			else if((USART2_RX_BUF[6]==0xA8)&&(USART2_RX_STA>8))
//			{
//				if(USART2_RX_STA==83)modebus_rec_flag2=1;
//				if(USART2_RX_STA>83)USART2_RX_STA=0;//�������ݴ���,���¿�ʼ����	 
//			}
		}
		
	}
	
	if(huart->Instance==USART3)//����Ǵ���3
	{
		if((aRxBuffer3[0]==0xaf)&&(USART3_RX_BUF[0]!=0xaf))
		{
			USART3_RX_STA=0;
			USART3_RX_BUF[USART3_RX_STA]=aRxBuffer3[0];
		}
		else if(USART3_RX_BUF[0]==0xaf)
		{
			USART3_RX_STA++;
			if(USART3_RX_STA==1)//��ȥ��ʼλ�����ݳ���
			{
				USART3_RX_BUF[USART3_RX_STA]=aRxBuffer3[0];
				length3=aRxBuffer3[0];
			}
			else
			{USART3_RX_BUF[USART3_RX_STA]=aRxBuffer3[0];}
			if(USART3_RX_STA>length3)USART3_RX_STA=0;//�������ݴ���,���¿�ʼ����	 
			if(USART3_RX_STA==length3)modebus_rec_flag3=1;
		}
	}
}

int Usart1_Rec_Start(void)//������P8�����Ŀ�ʼ���Ե������ź�
{
	if(modebus_rec_flag1)
	{
		if((USART1_RX_BUF[0]==0xbe)&&(USART1_RX_BUF[1]==0x05)&&(USART1_RX_BUF[2]==0xff)&&(USART1_RX_BUF[3]==0xff)&&(USART1_RX_BUF[4]==0x02)&&(USART1_RX_BUF[5]==0x03))
		{
		  cnt1=0;
			USART1_RX_BUF[0]=0;
			USART1_RX_BUF[1]=0;
			USART1_RX_BUF[2]=0;
			USART1_RX_BUF[3]=0;
			USART1_RX_BUF[4]=0;
			USART1_RX_BUF[5]=0;
			modebus_rec_flag1=0;
			CLEAR_BIT(USART1->CR1, USART_CR1_RE);
			return 1;
		}
		else if((USART1_RX_BUF[0]==0xbe)||(USART1_RX_BUF[1]==0x05)||(USART1_RX_BUF[2]==0xff)||(USART1_RX_BUF[3]==0xff)||(USART1_RX_BUF[4]!=(CRC_Sum(&USART1_RX_BUF[1],5)>>8))||(USART1_RX_BUF[5]!=CRC_Sum(&USART1_RX_BUF[1],5)))//У�鲻ͨ��
		{
			USART1_RX_BUF[0]=0xbe;
			USART1_RX_BUF[1]=0x05;
			USART1_RX_BUF[2]=0x00;
			USART1_RX_BUF[3]=0x57;
			USART1_RX_BUF[4]=0x00;
			USART1_RX_BUF[5]=0x5C;	
			if((cnt1++)<3)
			{
				HAL_UART_Transmit(&UART1_Handler,(uint8_t*)USART1_RX_BUF,6,1000);	
			  while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);		//�ȴ����ͽ���
			}
			else
			{cnt1=0;}
			USART1_RX_BUF[0]=0;
			USART1_RX_BUF[1]=0;
			USART1_RX_BUF[2]=0;
			USART1_RX_BUF[3]=0;
			USART1_RX_BUF[4]=0;
			USART1_RX_BUF[5]=0;
			modebus_rec_flag1=0;
			return 0;
		}
		else
		{
			USART1_RX_BUF[0]=0;
			USART1_RX_BUF[1]=0;
			USART1_RX_BUF[2]=0;
			USART1_RX_BUF[3]=0;
			USART1_RX_BUF[4]=0;
			USART1_RX_BUF[5]=0;
			modebus_rec_flag1=0;
			return 0;
		}
	}
	else
	{return 0;}
}

int Usart2_TData_Rec(void)//ͨ�Ű巢�����ݸ����ܰ�������ϸ���ݣ����յ��Ĺ��ܰ���ϸ����
{
	if(modebus_rec_flag2)
	{
		if((USART2_RX_BUF[0]==0x9c)&&(USART2_RX_BUF[1]==0x08))//
		{
			
			USART2_RX_BUF[0]=0;
			USART2_RX_BUF[1]=0;
			USART2_RX_BUF[2]=0;
			USART2_RX_BUF[3]=0;
			modebus_rec_flag2=0;
			return 1;
		}
		else
		{
			USART2_RX_BUF[0]=0;
			USART2_RX_BUF[1]=0;
			USART2_RX_BUF[2]=0;
			USART2_RX_BUF[3]=0;
			modebus_rec_flag2=0;
			return 0;
		}
	}
	else
	{
	  modebus_rec_flag2=0;
		return 0;
	}
}
int Usart3_TData_Start(void)//PC����������Ե���ϸ���ݽ��������
{
  if(modebus_rec_flag3)
	{
		if((USART3_RX_BUF[0]==0xaf)&&(USART3_RX_BUF[1]==0x03)&&(USART3_RX_BUF[2]==0x09)&&(USART3_RX_BUF[3]==0xf6))
		{
			return 1;
		}
		else
		{
			USART3_RX_BUF[0]=0;
			USART3_RX_BUF[1]=0;
			USART3_RX_BUF[2]=0;
			USART3_RX_BUF[3]=0;
			modebus_rec_flag3=0;
			return 0;
		}
	}
	else
	{return 0;}
}

float byte2Float(unsigned char *byteArry)//���յ����ֽ�����ת��Ϊ�ĸ��ֽ���ɵĸ��������
{
	return *((float*)byteArry);
}
unsigned int CRC_Sum(unsigned char *Pushdata,unsigned int length)//�����ݽ����ۼ�У��
{
	int CRC_add=0;
	while(length--)
	{
		CRC_add+=(*Pushdata++);
	}
	return CRC_add;
}
