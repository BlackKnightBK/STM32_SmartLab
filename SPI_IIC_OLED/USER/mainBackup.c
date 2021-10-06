#include <stm32f4xx.h>
#include "led.h"
#include "key.h"
#include "delay.h"
#include "tim.h"
#include "usart.h"
#include <stdio.h>
#include "sys.h"
#include <string.h>//�ַ���������
#include "hcsr04.h"
#include "dht11.h"
#include "oled.h"
#include "bmp.h"

void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  /*ʹ��RTC���ж�*/
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
	/*************************��ʼ��Ӳ������********************************/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//���ȼ�����
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//SysTickʱ��Դѡ�� 0:21Mhz   1:168Mhz
	LED_Init();
	KEY_EXTIInit();
	NVIC_Configuration();
	USART1_Init(115200);
	//HCSR04_Init();					
	//DHT11_Init();//��ʼ����ʪ�ȶ�Ӧ��PG9
	OLED_Init();//OLED��ʼ��
	
	OLED_Clear();//����
	
	//OLED_ShowChar(0,0,'A',16);//
	//OLED_ShowString(0,2,"hello world!",16);
	
	//OLED_DrawBMP(0,0,64,4,BMP1);
	while(1)
	{
	}
	
}




