#ifndef __KEY_H_
#define __KEY_H_


#include <stm32f4xx.h>


//函数声明列表
void KEY_Init(void);
void KEY_EXTIInit(void);
void EXTI0_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);

void delayKey(int tm_s);

extern int DisplayMode;
extern u16 tempYear;
extern u8 tempMon;
extern u8 tempDay;
extern u8 tempWeek;
extern u8 tempHour;
extern u8 tempMin;
extern u8 tempSec;

extern int ChangeMode1;
extern int ChangeMode2;
extern int AlarmMode;
#endif
