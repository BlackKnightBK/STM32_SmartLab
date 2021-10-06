#ifndef __HCSR04_H_
#define __HCSR04_H_


#include <stm32f4xx.h>
#include "sys.h"
#include "delay.h"

void HCSR04_Init(void);
float HCSR04_GetDistance(void);

#endif
