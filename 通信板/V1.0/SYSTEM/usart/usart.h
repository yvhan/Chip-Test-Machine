#ifndef _USART_H
#define _USART_H
#include "sys.h"
#include "stdio.h"	
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32H7开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.csom
//修改日期:2017/6/8
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
//********************************************************************************
//V1.0修改说明 
////////////////////////////////////////////////////////////////////////////////// 	
#define USART_REC_LEN  		90  	//定义最大接收字节数 200
#define USART_TX_LEN      40
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
#define EN_USART2_RX 			1		//使能（1）/禁止（0）串口2接收
#define EN_USART3_RX 			1

extern u8  USART1_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u8  USART2_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u8  USART3_RX_BUF[USART_REC_LEN];

extern u8  USART1_TX_BUF[USART_TX_LEN];
extern u8  USART2_TX_BUF[USART_TX_LEN]; 
extern u8  USART3_TX_BUF[USART_TX_LEN]; 

extern u16 USART1_RX_STA;         		//接收状态标记	
extern u16 USART2_RX_STA;         	//接收状态标记	
extern u16 USART3_RX_STA; 

extern int modebus_rec_flag2;
extern UART_HandleTypeDef UART1_Handler; //UART句柄
extern UART_HandleTypeDef UART2_Handler; //UART句柄
extern UART_HandleTypeDef UART3_Handler; //UART句柄

#define RXBUFFERSIZE   1 //缓存大小
extern u8 aRxBuffer[RXBUFFERSIZE];//HAL库USART接收Buffer
extern u8 aRxBuffer2[RXBUFFERSIZE];//HAL库USART2接收Buffer2
extern u8 aRxBuffer3[RXBUFFERSIZE];
//如果想串口中断接收，请不要注释以下宏定义
void uart1_init(u32 bound);
void uart2_init(u32 bound);
void uart3_init(u32 bound);

int Usart1_Rec_Start(void);
int Usart2_TData_Rec(void);
int Usart3_TData_Start(void);
float byte2Float(unsigned char *byteArry);
unsigned int CRC_Sum(unsigned char *Pushdata,unsigned int length);
#endif
