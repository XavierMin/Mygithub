#ifndef __USART2_H
#define __USART2_H	 
#include "sys.h"  

#define USART2_MAX_RECV_LEN		1024				//最大接收缓存字节数
#define USART2_MAX_SEND_LEN		1024				//最大发送缓存字节数
#define USART2_RX_EN 			1					//0,不接收;1,接收.

extern u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; 		//接收缓冲,最大USART2_MAX_RECV_LEN字节
extern u8  USART2_TX_BUF[USART2_MAX_SEND_LEN]; 		//发送缓冲,最大USART2_MAX_SEND_LEN字节
extern u16 USART2_RX_STA;   						//接收数据状态

void USART2_Init(u32 bound);				//串口2初始化 
void TIM4_Set(u8 sta);
void TIM4_Init(u16 arr,u16 psc);
void UART_DMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr);
void UART_DMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr);
void uart2_send_char(u8 temp);
void uart2_send_buff(u8 buf[], u32 len);

//定义全局变量结构体用来存储接收数据
typedef struct {
    char USART_BUFF[800];
    int USART_Length;
    int flag;
}Usart_Struct;
extern  Usart_Struct struct_usart2;

#endif













