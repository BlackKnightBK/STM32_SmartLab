#include "Relay_Control.h"

void Relay_Control_Init()//����PC11-PC14���ŵ����  //���ģ�����Ƶ�main.c��
{
		GPIO_InitTypeDef GPIO_InitStructure;
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE); 
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				//���������ģʽ
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOC,&GPIO_InitStructure);
  	GPIO_ResetBits(GPIOB,GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14);//����͵�ƽ
}