#ifndef __IIC_H_
#define __IIC_H_

#include <stm32f4xx.h>
#include "sys.h"
#include "delay.h"


#define SCL  	PBout(8)
#define SDA_W   PBout(9)
#define SDA_R	PBin(9)


//函数声明列表
void MyIIC_Init(void);

//IIC 6个协议函数
void MyIIC_Start(void);
void MyIIC_Stop(void);
void MyIIC_WriteByte(u8 WByte);
u8   MyIIC_ReadByte(void);
char MyIIC_ReadACK(void);
void MyIIC_WriteAck(char ACK);

#endif




