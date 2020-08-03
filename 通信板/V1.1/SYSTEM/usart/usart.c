#include "usart.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用os,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//os 使用	  
#endif

//V1.0修改说明 
////////////////////////////////////////////////////////////////////////////////// 	  
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)	
#if 1
//#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART3->ISR&0X40)==0);//循环发送,直到发送完毕   
	USART3->TDR=(u8)ch;      
	return ch;
}
#endif 

#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART1_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
u8 USART1_TX_BUF[USART_TX_LEN];
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
int cnt1=0;
int length1=0;
u16 USART1_RX_STA=0;       //接收状态标记	
int modebus_rec_flag1=0;  //接收完成标志位
u8 aRxBuffer[RXBUFFERSIZE];//HAL库使用的串口接收缓冲
UART_HandleTypeDef UART1_Handler; //UART句柄

//初始化IO 串口1
//bound:波特率
void uart1_init(u32 bound)
{	
	//UART 初始化设置
	UART1_Handler.Instance=USART1;					    //USART1
	UART1_Handler.Init.BaudRate=bound;				    //波特率
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //无硬件流控
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&UART1_Handler);					    //HAL_UART_Init()会使能UART1
	
	HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
  
}

//串口1中断服务程序
void USART1_IRQHandler(void)                	
{ 
	u32 timeout=0;
    u32 maxDelay=0x1FFFF;
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntEnter();    
#endif
	
	HAL_UART_IRQHandler(&UART1_Handler);	//调用HAL库中断处理公用函数
	
	timeout=0;
  while (HAL_UART_GetState(&UART1_Handler)!=HAL_UART_STATE_READY)//等待就绪
	{
        timeout++;////超时处理
        if(timeout>maxDelay) break;		
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART1_Handler,(u8 *)aRxBuffer, RXBUFFERSIZE)!=HAL_OK)//一次处理完成之后，重新开启中断并设置RxXferCount为1
	{
        timeout++; //超时处理
        if(timeout>maxDelay) break;	
	}
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntExit();  											 
#endif
} 
#endif	

#if EN_USART2_RX   //如果使能了接收
//串口2中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART2_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
u8 USART2_TX_BUF[USART_TX_LEN];
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
int length2=0;
u16 USART2_RX_STA=0;       //接收状态标记	
int modebus_rec_flag2=0;  //接收完成标志位
u8 aRxBuffer2[RXBUFFERSIZE];//HAL库使用的串口接收缓冲
UART_HandleTypeDef UART2_Handler; //UART句柄

//初始化IO 串口2
//bound:波特率
void uart2_init(u32 bound)
{	
	//UART 初始化设置
	UART2_Handler.Instance=USART2;					    //USART2
	UART2_Handler.Init.BaudRate=bound;				    //波特率
	UART2_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	UART2_Handler.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
	UART2_Handler.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	UART2_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //无硬件流控
	UART2_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&UART2_Handler);					    //HAL_UART_Init()会使能UART2
	
	HAL_UART_Receive_IT(&UART2_Handler, (u8 *)aRxBuffer2, RXBUFFERSIZE);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
}

 
//串口2中断服务程序
void USART2_IRQHandler(void)                	
{ 
	u32 timeout=0;
    u32 maxDelay=0x1FFFF;
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntEnter();    
#endif
	
	HAL_UART_IRQHandler(&UART2_Handler);	//调用HAL库中断处理公用函数
	
	timeout=0;
    while (HAL_UART_GetState(&UART2_Handler)!=HAL_UART_STATE_READY)//等待就绪
	{
        timeout++;////超时处理
        if(timeout>maxDelay) break;		
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART2_Handler,(u8 *)aRxBuffer2, RXBUFFERSIZE)!=HAL_OK)//一次处理完成之后，重新开启中断并设置RxXferCount为1
	{
        timeout++; //超时处理
        if(timeout>maxDelay) break;	
	}
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntExit();  											 
#endif
} 
#endif

#if EN_USART3_RX   //如果使能了接收
//串口3中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART3_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
u8 USART3_TX_BUF[USART_TX_LEN];
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
int length3=0;
u16 USART3_RX_STA=0;       //接收状态标记	
int modebus_rec_flag3=0;  //接收完成标志位
u8 aRxBuffer3[RXBUFFERSIZE];//HAL库使用的串口接收缓冲
UART_HandleTypeDef UART3_Handler; //UART句柄

//初始化IO 串口3
//bound:波特率
void uart3_init(u32 bound)
{	
	//UART 初始化设置
	UART3_Handler.Instance=USART3;					    //USART3
	UART3_Handler.Init.BaudRate=bound;				    //波特率
	UART3_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	UART3_Handler.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
	UART3_Handler.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	UART3_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //无硬件流控
	UART3_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&UART3_Handler);					    //HAL_UART_Init()会使能UART3
	
	HAL_UART_Receive_IT(&UART3_Handler, (u8 *)aRxBuffer3, RXBUFFERSIZE);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
}

 
//串口3中断服务程序
void USART3_IRQHandler(void)                	
{ 
	u32 timeout=0;
    u32 maxDelay=0x1FFFF;
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntEnter();    
#endif
	
	HAL_UART_IRQHandler(&UART3_Handler);	//调用HAL库中断处理公用函数
	
	timeout=0;
    while (HAL_UART_GetState(&UART3_Handler)!=HAL_UART_STATE_READY)//等待就绪
	{
        timeout++;////超时处理
        if(timeout>maxDelay) break;		
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART3_Handler,(u8 *)aRxBuffer3, RXBUFFERSIZE)!=HAL_OK)//一次处理完成之后，重新开启中断并设置RxXferCount为1
	{
        timeout++; //超时处理
        if(timeout>maxDelay) break;	
	}
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntExit();  											 
#endif
} 
#endif 
//UART底层初始化，时钟使能，引脚配置，中断配置
//此函数会被HAL_UART_Init()调用
//huart:串口句柄

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART1)//如果是串口1，进行串口1 MSP初始化
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();			//使能GPIOB时钟
		__HAL_RCC_USART1_CLK_ENABLE();			//使能USART1时钟
	
		GPIO_Initure.Pin=GPIO_PIN_14;			//PB14
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
		GPIO_Initure.Alternate=GPIO_AF4_USART1;	//复用为USART1
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//初始化PB

		GPIO_Initure.Pin=GPIO_PIN_15;			//PB15
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//初始化PB15
		
#if EN_USART1_RX
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//使能USART1中断通道
		HAL_NVIC_SetPriority(USART1_IRQn,3,3);			//抢占优先级3，子优先级3
#endif	
	}
	
	if(huart->Instance==USART2)//如果是串口2，进行串口2 MSP初始化
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//使能GPIOA时钟
		__HAL_RCC_USART2_CLK_ENABLE();			//使能USART2时钟
	
		GPIO_Initure.Pin=GPIO_PIN_2;			//PA2
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
		GPIO_Initure.Alternate=GPIO_AF7_USART2;	//复用为USART2
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA2

		GPIO_Initure.Pin=GPIO_PIN_3;			//PA3
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA3
		
#if EN_USART2_RX
		HAL_NVIC_EnableIRQ(USART2_IRQn);				//使能USART2中断通道
		HAL_NVIC_SetPriority(USART2_IRQn,3,3);			//抢占优先级3，子优先级3
#endif	
	}
	
	if(huart->Instance==USART3)//如果是串口3，进行串口3 MSP初始化
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();			//使能GPIOB时钟
		__HAL_RCC_USART3_CLK_ENABLE();			//使能USART3时钟
	
		GPIO_Initure.Pin=GPIO_PIN_10;			//PB10
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
		GPIO_Initure.Alternate=GPIO_AF7_USART3;	//复用为USART3
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//初始化PB

		GPIO_Initure.Pin=GPIO_PIN_11;			//PB11
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//初始化PB11
		
#if EN_USART3_RX
		HAL_NVIC_EnableIRQ(USART3_IRQn);				//使能USART3中断通道
		HAL_NVIC_SetPriority(USART3_IRQn,3,3);			//抢占优先级3，子优先级3
#endif	
	}

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)//如果是串口1
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
			if(USART1_RX_STA>length1)USART1_RX_STA=0;//接收数据错误,重新开始接收	 
			if(USART1_RX_STA==length1)modebus_rec_flag1=1;
		}
	}
	
	if(huart->Instance==USART2)//如果是串口2
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
			if(USART2_RX_STA>88)USART2_RX_STA=0;//接收数据错误,重新开始接收	 
//			if((USART2_RX_BUF[6]==0xB0)&&(USART2_RX_STA>8))
//			{
				if(USART2_RX_STA==87)modebus_rec_flag2=1;
//				if(USART2_RX_STA>88)USART2_RX_STA=0;//接收数据错误,重新开始接收	 
//			}
//			else if((USART2_RX_BUF[6]==0xA8)&&(USART2_RX_STA>8))
//			{
//				if(USART2_RX_STA==83)modebus_rec_flag2=1;
//				if(USART2_RX_STA>83)USART2_RX_STA=0;//接收数据错误,重新开始接收	 
//			}
		}
		
	}
	
	if(huart->Instance==USART3)//如果是串口3
	{
		if((aRxBuffer3[0]==0xaf)&&(USART3_RX_BUF[0]!=0xaf))
		{
			USART3_RX_STA=0;
			USART3_RX_BUF[USART3_RX_STA]=aRxBuffer3[0];
		}
		else if(USART3_RX_BUF[0]==0xaf)
		{
			USART3_RX_STA++;
			if(USART3_RX_STA==1)//除去起始位的数据长度
			{
				USART3_RX_BUF[USART3_RX_STA]=aRxBuffer3[0];
				length3=aRxBuffer3[0];
			}
			else
			{USART3_RX_BUF[USART3_RX_STA]=aRxBuffer3[0];}
			if(USART3_RX_STA>length3)USART3_RX_STA=0;//接收数据错误,重新开始接收	 
			if(USART3_RX_STA==length3)modebus_rec_flag3=1;
		}
	}
}

int Usart1_Rec_Start(void)//接收来P8发来的开始测试的数据信号
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
		else if((USART1_RX_BUF[0]==0xbe)||(USART1_RX_BUF[1]==0x05)||(USART1_RX_BUF[2]==0xff)||(USART1_RX_BUF[3]==0xff)||(USART1_RX_BUF[4]!=(CRC_Sum(&USART1_RX_BUF[1],5)>>8))||(USART1_RX_BUF[5]!=CRC_Sum(&USART1_RX_BUF[1],5)))//校验不通过
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
			  while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);		//等待发送结束
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

int Usart2_TData_Rec(void)//通信板发送数据给功能板请求详细数据，接收到的功能板详细数据
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
int Usart3_TData_Start(void)//PC发送请求测试的详细数据结果的数据
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

float byte2Float(unsigned char *byteArry)//将收到的字节数组转换为四个字节组成的浮点数输出
{
	return *((float*)byteArry);
}
unsigned int CRC_Sum(unsigned char *Pushdata,unsigned int length)//对数据进行累加校验
{
	int CRC_add=0;
	while(length--)
	{
		CRC_add+=(*Pushdata++);
	}
	return CRC_add;
}
