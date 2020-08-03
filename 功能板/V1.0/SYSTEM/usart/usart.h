#ifndef _USART_H
#define _USART_H
#include "sys.h"
#include "stdio.h"	
 
////////////////////////////////////////////////////////////////////////////////// 	
#define USART_REC_LEN  		20  	//�����������ֽ��� 200
#define USART_TC_LEN      40
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
#define EN_USART2_RX 			1		//ʹ�ܣ�1��/��ֹ��0������2����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u8  USART2_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern float USART2_TX_BUF[USART_TC_LEN];

extern u16 USART_RX_STA;         		//����״̬���	
extern u16 USART2_RX_STA;         	//����״̬���	

extern UART_HandleTypeDef UART1_Handler; //UART���
extern UART_HandleTypeDef UART2_Handler; //UART���


#define RXBUFFERSIZE   1 //�����С
extern u8 aRxBuffer[RXBUFFERSIZE];//HAL��USART����Buffer
extern u8 aRxBuffer2[RXBUFFERSIZE];//HAL��USART2����Buffer2
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);
void uart2_init(u32 bound);

int Usart2_Data_Requst(void);
extern unsigned char byteArry[90];
float Float2Byte(float floatNum, unsigned char *byteArry);
unsigned char* Data_Tranf(int num);
#endif
