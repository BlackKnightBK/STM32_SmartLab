#include <stm32f4xx.h>
#include "led.h"
#include "key.h"
#include "delay.h"
#include "tim.h"
#include "usart.h"
#include <stdio.h>
#include "sys.h"
#include "rtc.h"
#include <string.h>//字符串处理函数
#include "hcsr04.h"
#include "dht11.h"
#include "oled.h"
#include "bmp.h"
#include "adc.h"

int tm_s;

void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  /*使能RTC秒中断*/
  NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;//RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void mydelay(int tm_ss)
{
	int i;
	tm_s=tm_ss;
	while(tm_s--)
	{
		for(i=38400;i>0;i--);
	}
}


int main()
{
	u8 setWeek=0;//2021+505+（20/4）-2*20+26*（6+1）/10）+8-1
	u16 year=2021;//2021+505+5-40+18+8-1
	u8 month=6;
	u8 day=21;
	u8 c=0;
	u8 RTC_Inited=0;
	/*************************初始化硬件部分********************************/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//优先级分组
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//SysTick时钟源选择 0:21Mhz   1:168Mhz
	LED_Init();
	KEY_EXTIInit();
	ADC1IN5_Init();//ADC1 滑动变阻器初始化
	ADC3IN5_Init();//ADC3 光敏电阻初始化
	//NVIC_Configuration();
	USART1_Init(115200);
	RTC_Inited=(RTC->ISR>>4)&0x01;
	RTC_Init(21,9,0);
	RTC_SetTime(17,2,10);
	RTC_SetDate(year,month,day,getWeek(year,month,day));
	//////////////////////////////////////////
	
	//////////////////////////////////////////
	TIM2_Init();
	TIM3_Init();
	TIM4_Init();
	TIM9_Init();
	//HCSR04_Init();					
	DHT11_Init();//初始化温湿度对应的PG9
	TIM_Cmd(TIM4,ENABLE);//开始读取DHT11
	OLED_Init();//OLED初始化
	OLED_Clear();//清屏
	TIM_Cmd(TIM9,ENABLE);//开始显示
	GPIO_ResetBits(GPIOF,GPIO_Pin_8);
	//TIM_Cmd(TIM2,ENABLE);
	//OLED_ShowChar(0,0,'A',16);//
	//OLED_ShowString(0,2,"hello world!",16);
	//OLED_DrawBMP(0,0,64,4,BMP1);
	while(1)
	{
	}
}




