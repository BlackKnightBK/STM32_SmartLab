#ifndef __SPI_H_
#define __SPI_H_

#include <stm32f4xx.h>
#include "sys.h"

#define SCLK PDout(6)
#define MOSI PDout(7)
#define CS   PCout(11)



void MySPI_Init(void);//SPI Òý½Å  ºÍOLED DC  RES½Å
void MySPI_WriteByte(u8 WByte);


#endif

