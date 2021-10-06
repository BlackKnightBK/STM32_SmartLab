#include "led.h"



void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;//���ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//����
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//����
	GPIO_Init(GPIOF,&GPIO_InitStruct);
	
	//LED3,4
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_13|GPIO_Pin_14;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;//���ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//����
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//����
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	
	GPIO_SetBits(GPIOF,GPIO_Pin_9 | GPIO_Pin_10);
	GPIO_ResetBits(GPIOF,GPIO_Pin_8);//�رշ�����
	GPIO_SetBits(GPIOE,GPIO_Pin_13 | GPIO_Pin_14);//����ȫ��
}














