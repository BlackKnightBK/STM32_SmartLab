#include "hcsr04.h"

#define TRIG  PBout(10)
#define ECHO  PBin(11)

void HCSR04_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	
	//1��ʱ��ʹ�� GPIOB  TIM2
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	//2����ʼ��PB10--TRIG  PB11--ECHO
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_10;//TRIG
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;//���ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//����
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//����
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_11;//ECHO
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN;//����ģʽ
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	TRIG = 0;//��TRIG��ΪĬ�Ͽ���״̬
	
	//3��TIM2��ʼ��
	TIM_InitStruct.TIM_Prescaler     = 84-1;//84M/8400 = 1MHz  1us/����   100us *0.34mm/us = 34mm
	TIM_InitStruct.TIM_Period        = 65536-1;//��Ϊʹ�ö�ʱ��2��������ʱ--CNT����������  ����Ҫ��ʱ
	TIM_InitStruct.TIM_CounterMode   = TIM_CounterMode_Up;//��������ģʽ32ms  34cm/ms 
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//����Ƶ 
	TIM_TimeBaseInit(TIM2,&TIM_InitStruct);
	
	TIM_Cmd(TIM2,DISABLE);//�ȹر�TIM2������

}

float HCSR04_GetDistance(void)
{
	u32 GetCnt=0;
	
	TRIG = 1;
	delay_us(15);//��������10us ����
	TRIG = 0;

	//�ȴ�ECHO����
	while(ECHO == 0);
	
	TIM2->CNT = 0;//�����������ֵ
	TIM_Cmd(TIM2,ENABLE);//�ٿ�����ʱ��

	//�ȴ�ECHO����  �������
	while(ECHO == 1);

	TIM_Cmd(TIM2,DISABLE);//�رն�ʱ��
	GetCnt = TIM2->CNT;//��ȡ������ֵ
	

	return GetCnt*0.017;//���㲢���ز����  ��λ��CM
}




