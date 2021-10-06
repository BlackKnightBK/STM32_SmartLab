#include "hcsr04.h"

#define TRIG  PBout(10)
#define ECHO  PBin(11)

void HCSR04_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	
	//1、时钟使能 GPIOB  TIM2
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	//2、初始化PB10--TRIG  PB11--ECHO
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_10;//TRIG
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;//输出模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//高速
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//浮空
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_11;//ECHO
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN;//输入模式
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	TRIG = 0;//将TRIG置为默认空闲状态
	
	//3、TIM2初始化
	TIM_InitStruct.TIM_Prescaler     = 84-1;//84M/8400 = 1MHz  1us/脉冲   100us *0.34mm/us = 34mm
	TIM_InitStruct.TIM_Period        = 65536-1;//因为使用定时器2是用来计时--CNT计数器即可  不需要定时
	TIM_InitStruct.TIM_CounterMode   = TIM_CounterMode_Up;//递增计数模式32ms  34cm/ms 
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//不分频 
	TIM_TimeBaseInit(TIM2,&TIM_InitStruct);
	
	TIM_Cmd(TIM2,DISABLE);//先关闭TIM2计数器

}

float HCSR04_GetDistance(void)
{
	u32 GetCnt=0;
	
	TRIG = 1;
	delay_us(15);//拉高至少10us 启动
	TRIG = 0;

	//等待ECHO拉高
	while(ECHO == 0);
	
	TIM2->CNT = 0;//先清零计数器值
	TIM_Cmd(TIM2,ENABLE);//再开启定时器

	//等待ECHO拉低  结束测距
	while(ECHO == 1);

	TIM_Cmd(TIM2,DISABLE);//关闭定时器
	GetCnt = TIM2->CNT;//获取计数器值
	

	return GetCnt*0.017;//计算并返回测距结果  单位：CM
}




