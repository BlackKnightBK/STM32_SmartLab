#ifndef __DHT11_H
#define __DHT11_H   
#include "stm32f10x.h"

//IO方向设置
//#define DHT11_IO_IN()  {GPIOE->MODER&=~(3<<(9*2));GPIOE->MODER|=0<<9*2;}	//PE9输入模式
//#define DHT11_IO_OUT() {GPIOE->MODER&=~(3<<(9*2));GPIOE->MODER|=1<<9*2;} 	//PE9输出模式 
//IO操作函数											   
//#define	DHT11_DQ_OUT PEout(9) //数据端口	PE9 
//#define	DHT11_DQ_IN  PEin(9)  //数据端口	PE9 

u8 DHT11_Init(void);//初始化DHT11
u8 DHT11_Read_Data(u8 *temp,u8 *humi);//读取温湿度
u8 DHT11_Read_Byte(void);//读出一个字节
u8 DHT11_Read_Bit(void);//读出一个位
u8 DHT11_Check(void);//检测是否存在DHT11
void DHT11_Rst(void);//复位DHT11 

extern int DH11INITED;
#endif