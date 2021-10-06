#ifndef __USART_H 
#define __USART_H     
#include "stm32f10x.h"
#include "delay.h"

//USART1
void USART1_Init(u32 My_BaudRate);
void USART1_SendMessageStr(char *str);
void USART1_SendMessageU8(u8 *command);
void USART1_SendByte(char str);
void USART1_IRQHandler(void);
extern char USART1_BUF[500];
extern int u1MsgLen;
//USART2
void USART2_Init(u32 My_BaudRate);
void USART2_SendMessageStr(char *str);
void USART2_SendByte(char str);
void USART2_IRQHandler(void);
extern char USART2_BUF[500];
extern int u2MsgLen;
#endif
