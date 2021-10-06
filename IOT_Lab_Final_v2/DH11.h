#ifndef __DHT11_H
#define __DHT11_H   
#include "stm32f10x.h"

//IO��������
//#define DHT11_IO_IN()  {GPIOE->MODER&=~(3<<(9*2));GPIOE->MODER|=0<<9*2;}	//PE9����ģʽ
//#define DHT11_IO_OUT() {GPIOE->MODER&=~(3<<(9*2));GPIOE->MODER|=1<<9*2;} 	//PE9���ģʽ 
//IO��������											   
//#define	DHT11_DQ_OUT PEout(9) //���ݶ˿�	PE9 
//#define	DHT11_DQ_IN  PEin(9)  //���ݶ˿�	PE9 

u8 DHT11_Init(void);//��ʼ��DHT11
u8 DHT11_Read_Data(u8 *temp,u8 *humi);//��ȡ��ʪ��
u8 DHT11_Read_Byte(void);//����һ���ֽ�
u8 DHT11_Read_Bit(void);//����һ��λ
u8 DHT11_Check(void);//����Ƿ����DHT11
void DHT11_Rst(void);//��λDHT11 

extern int DH11INITED;
#endif