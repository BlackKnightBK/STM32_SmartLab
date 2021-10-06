#include <stm32f4xx.h>
#include "led.h"
#include "key.h"
#include "delay.h"
#include "tim.h"
#include "usart.h"
#include <stdio.h>
#include "sys.h"
#include <string.h>//字符串处理函数
#include "hcsr04.h"
#include "dht11.h"
#include "oled.h"
#include "bmp.h"

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

void RTC_Config()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	//PWR_BackupAccessCmd();
	RCC_BackupResetCmd(ENABLE);
	RCC_BackupResetCmd(DISABLE);
	RCC_LSEConfig(RCC_LSE_ON);
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET);
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	RCC_RTCCLKCmd(ENABLE);
	RTC_WaitForSynchro();
}

int main()
{
	/*************************初始化硬件部分********************************/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//优先级分组
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//SysTick时钟源选择 0:21Mhz   1:168Mhz
	LED_Init();
	KEY_EXTIInit();
	NVIC_Configuration();
	USART1_Init(115200);
	//HCSR04_Init();					
	//DHT11_Init();//初始化温湿度对应的PG9
	OLED_Init();//OLED初始化
	
	OLED_Clear();//清屏
	
	//OLED_ShowChar(0,0,'A',16);//
	//OLED_ShowString(0,2,"hello world!",16);
	
	//OLED_DrawBMP(0,0,64,4,BMP1);
	while(1)
	{
	}
	
}




