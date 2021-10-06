#ifndef __DELAY_H   
#define __DELAY_H 
#include "stm32f10x.h" 

 void SysTick_Configuration(void);
 void delay_mms(u16 ms);
 void Delay_init(void);
 void TimingDelay_Decrement(void);
 void delay_us(u32 us);
 void delay_s(u16 s);
 void Delay(vu32 nCount);

/*延时函数相关变量定义*/
static vu32 TimingDelay;

#endif
