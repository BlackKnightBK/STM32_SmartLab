#include "stm32f10x.h"  
#include "EXTI.h"
#include "delay.h"
#include "ESP8266.h"
#include "USART.h"

void EXTI_InitConfig()
{
	GPIO_InitTypeDef GPIO_InitStructure; //似乎已经被定义了
	EXTI_InitTypeDef EXTI_InitStructrue;
	NVIC_InitTypeDef NVIC_InitStructrue;
	
	
	//此处为4个按钮的设置
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15; //使用PC0~PC7
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	//配置EXTI  EXTI0~EXTI22   EXTI0~EXTI15----和引脚号挂钩
	EXTI_InitStructrue.EXTI_Line  = EXTI_Line12;//PA0
	EXTI_InitStructrue.EXTI_Mode  = EXTI_Mode_Interrupt;//中断模式
	EXTI_InitStructrue.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
	EXTI_InitStructrue.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructrue);
	
	EXTI_InitStructrue.EXTI_Line  = EXTI_Line13;//PA0
	EXTI_InitStructrue.EXTI_Mode  = EXTI_Mode_Interrupt;//中断模式
	EXTI_InitStructrue.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
	EXTI_InitStructrue.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructrue);
	
	EXTI_InitStructrue.EXTI_Line  = EXTI_Line14;//PA0
	EXTI_InitStructrue.EXTI_Mode  = EXTI_Mode_Interrupt;//中断模式
	EXTI_InitStructrue.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
	EXTI_InitStructrue.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructrue);
	
	EXTI_InitStructrue.EXTI_Line  = EXTI_Line15;//PA0
	EXTI_InitStructrue.EXTI_Mode  = EXTI_Mode_Interrupt;//中断模式
	EXTI_InitStructrue.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
	EXTI_InitStructrue.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructrue);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource12);//此处本为EXTI_PinSource13 	//EXTI_PR_PR1
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource13);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource15);
	
	NVIC_InitStructrue.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructrue.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructrue.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructrue.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructrue);
	
	}

void EXTI15_10_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line12)!=RESET)
	{
		Delay(0x200000);//这里的延时函数不能和主函数的延时函数一致，否则会导致函数被破坏而无法回到主函数
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)==0);//等待松开按键
		Delay(0x200000);
		ESP8266_CONNECTED=ESP8266_Check_CONNECT_AP();
		EXTI_ClearITPendingBit(EXTI_Line12);
		return;
	}
	else if(EXTI_GetITStatus(EXTI_Line13)!=RESET)
	{
		Delay(0x200000);
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)==0);//等待松开按键
		Delay(0x200000);
		//USART2_SendMessageStr(USART1_BUF);
		GPIO_WriteBit(GPIOC,GPIO_Pin_5,(BitAction)(!GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_5)));
		EXTI_ClearITPendingBit(EXTI_Line13);
		return;
	}
	else if(EXTI_GetITStatus(EXTI_Line14)!=RESET)
	{
		Delay(0x200000);
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)==0);//等待松开按键
		Delay(0x200000);
		//ESP8266_Check_Ack();
		GPIO_WriteBit(GPIOC,GPIO_Pin_6,(BitAction)(!GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_6)));
		EXTI_ClearITPendingBit(EXTI_Line14);
		return;
	}
	else if(EXTI_GetITStatus(EXTI_Line15)!=RESET)
	{
		Delay(0x200000);
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==0);//等待松开按键
		Delay(0x200000);
		GPIO_WriteBit(GPIOC,GPIO_Pin_7,(BitAction)(!GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_7)));
		EXTI_ClearITPendingBit(EXTI_Line15);
		return;
	}
}