#include "stm32f10x.h"
#include "SR505.h"

void SR505_Init(void)//��������͵�ƽ ��Ӧ����������ߵ�ƽ,��A0�ӿ�
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 					//ʹ��GPIOA��ʱ�� 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
}