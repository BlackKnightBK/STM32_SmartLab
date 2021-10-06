#ifndef __ADC_H_ //·ÀÖ¹ÖØ¸´°üº¬
#define __ADC_H_

#include <stm32f4xx.h>

void ADC1IN5_Init(void);
void ADC3IN5_Init(void);
unsigned short Get_ADCVal(ADC_TypeDef *ADCx);

extern u16 tempLightData;
#endif
