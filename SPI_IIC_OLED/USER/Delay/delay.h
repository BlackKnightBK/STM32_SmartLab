#ifndef __DELAY_H_
#define __DELAY_H_


#include <stm32f4xx.h>
//���������б�
//����ʱ
void delay(int tm_s);

//SysTick�ľ�׼��ʱ
void delay_us(int tm_us);
void delay_ms(int tm_ms);
void delay_s(int tm_s);


#endif

