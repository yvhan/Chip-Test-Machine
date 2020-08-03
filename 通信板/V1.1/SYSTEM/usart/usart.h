#ifndef _USART_H
#define _USART_H
#include "sys.h"
#include "stdio.h"	
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32H7������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.csom
//�޸�����:2017/6/8
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************
//V1.0�޸�˵�� 
////////////////////////////////////////////////////////////////////////////////// 	
#define USART_REC_LEN  		90  	//�����������ֽ��� 200
#define USART_TX_LEN      40
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
#define EN_USART2_RX 			1		//ʹ�ܣ�1��/��ֹ��0������2����
#define EN_USART3_RX 			1

extern u8  USART1_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u8  USART2_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u8  USART3_RX_BUF[USART_REC_LEN];

extern u8  USART1_TX_BUF[USART_TX_LEN];
extern u8  USART2_TX_BUF[USART_TX_LEN]; 
extern u8  USART3_TX_BUF[USART_TX_LEN]; 

extern u16 USART1_RX_STA;         		//����״̬���	
extern u16 USART2_RX_STA;         	//����״̬���	
extern u16 USART3_RX_STA; 

extern int modebus_rec_flag2;
extern UART_HandleTypeDef UART1_Handler; //UART���
extern UART_HandleTypeDef UART2_Handler; //UART���
extern UART_HandleTypeDef UART3_Handler; //UART���

#define RXBUFFERSIZE   1 //�����С
extern u8 aRxBuffer[RXBUFFERSIZE];//HAL��USART����Buffer
extern u8 aRxBuffer2[RXBUFFERSIZE];//HAL��USART2����Buffer2
extern u8 aRxBuffer3[RXBUFFERSIZE];
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart1_init(u32 bound);
void uart2_init(u32 bound);
void uart3_init(u32 bound);

int Usart1_Rec_Start(void);
int Usart2_TData_Rec(void);
int Usart3_TData_Start(void);
float byte2Float(unsigned char *byteArry);
unsigned int CRC_Sum(unsigned char *Pushdata,unsigned int length);
#endif
