#ifndef __DELAY_H_
#define __DELAY_H_


#include <stm32f4xx.h>
//函数声明列表
//粗延时
void delay(int tm_s);

//SysTick的精准延时
void delay_us(int tm_us);
void delay_ms(int tm_ms);
void delay_s(int tm_s);


#endif

