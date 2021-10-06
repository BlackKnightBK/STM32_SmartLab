#ifndef __GY30_H 
#define __GY30_H     
#define SlaveAddress 0x46
#define uchar unsigned char
#define uint unsigned int 
#include "stm32f10x.h"
#include "delay.h"




extern uchar BUF[8];  //�������ݻ�������
extern int dis_data;  //����
extern unsigned int mcy; //��λ

void GY30_IIC_Init(void);
void GPIOConfig(void);
void GY30_Start(void); //��ʼ�ź�
void GY30_Stop(void);  //ֹͣ�ź�
void GY30_SendACK(BitAction ack);
int GY30_RecvACK(void);
int GY30_SendByte(uchar dat);
uchar GY30_RecvByte(void);
int Single_Write_GY30(uchar REG_Address);
int Init_GY30(void);
void mread(void);

extern int GY30INITED;
#endif
