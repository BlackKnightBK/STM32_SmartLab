#include "led.h"



void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;//输出模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//高速
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//浮空
	GPIO_Init(GPIOF,&GPIO_InitStruct);
	
	//LED3,4
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_13|GPIO_Pin_14;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;//输出模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//高速
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//浮空
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	
	GPIO_SetBits(GPIOF,GPIO_Pin_9 | GPIO_Pin_10);
	GPIO_ResetBits(GPIOF,GPIO_Pin_8);//关闭蜂鸣器
	GPIO_SetBits(GPIOE,GPIO_Pin_13 | GPIO_Pin_14);//灯先全灭
}














