#ifndef __USART_H_
#define __USART_H_


#include <stm32f4xx.h>

extern char USART1_Rbuf[100];
extern char Recive_Flag;


void USART1_Init(int Baud);
void USART_SendStringData(USART_TypeDef* USARTx, char *Data);
#endif


