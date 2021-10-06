#ifndef __DHT11_H_
#define __DHT11_H_


#include <stm32f4xx.h>
#include "sys.h"
#include "delay.h"


void DHT11_Init(void);
int DHT11_GetTR(u8 *TRbuf);

extern u8 tempT;
extern u8 tempH;

extern u8 tempTUL;
extern u8 tempTLL;

extern u8 tempHUL;
extern u8 tempHLL;
#endif

