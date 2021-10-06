#ifndef __RTC_H_
#define __RTC_H_

#include <stm32f4xx.h>
#include "delay.h"

u8 BCD_encode(u8 data);
u8 BCD_decode(u8 data);
u8 RTC_Init(u8 hour,u8 min,u8 second);
u8 RTC_SetTime(u8 hour,u8 min,u8 second);
u8 RTC_SetDate(u16 year,u8 mon,u8 day,u8 week);
void readDate(u8* year,u8* mon,u8* day,u8* week);
void readTime(u8* hour,u8* min,u8* sec);
u8 getWeek(u32 year,u8 month,u8 day);

void addMonth();
void addDay();
void addHour();
void addMin();
void addSec();

void decMonth();
void decDay();
void decHour();
void decMin();
void decSec();

#endif