#ifndef _USART_H
#define _USART_H
#include "sys.h"
#include "stdio.h"	
 
////////////////////////////////////////////////////////////////////////////////// 	
#define USART_REC_LEN  		20  	//定义最大接收字节数 200
#define USART_TC_LEN      40
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
#define EN_USART2_RX 			1		//使能（1）/禁止（0）串口2接收
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u8  USART2_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern float USART2_TX_BUF[USART_TC_LEN];

extern u16 USART_RX_STA;         		//接收状态标记	
extern u16 USART2_RX_STA;         	//接收状态标记	

extern UART_HandleTypeDef UART1_Handler; //UART句柄
extern UART_HandleTypeDef UART2_Handler; //UART句柄


#define RXBUFFERSIZE   1 //缓存大小
extern u8 aRxBuffer[RXBUFFERSIZE];//HAL库USART接收Buffer
extern u8 aRxBuffer2[RXBUFFERSIZE];//HAL库USART2接收Buffer2
//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);
void uart2_init(u32 bound);

int Usart2_Data_Requst(void);
extern unsigned char byteArry[90];
float Float2Byte(float floatNum, unsigned char *byteArry);
unsigned char* Data_Tranf(int num);
#endif
