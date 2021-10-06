#ifndef __TIM_H
#define __TIM_H   
#include "stm32f10x.h"

void TIM2_Config(u16 arr,u16 psc);
void TIM2_IRQHandler(void);

void TIM3_Config(u16 arr,u16 psc);
void TIM3_IRQHandler(void);

#endif