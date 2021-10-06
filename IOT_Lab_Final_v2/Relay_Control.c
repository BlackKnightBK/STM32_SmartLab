#include "Relay_Control.h"

void Relay_Control_Init()//控制PC11-PC14引脚的输出  //这个模块已移到main.c中
{
		GPIO_InitTypeDef GPIO_InitStructure;
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE); 
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				//工作在输出模式
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOC,&GPIO_InitStructure);
  	GPIO_ResetBits(GPIOB,GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14);//输出低电平
}