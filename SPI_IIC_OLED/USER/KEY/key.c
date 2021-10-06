#include "key.h"
#include "oled.h"
#include "usart.h"
#include "tim.h"

int DisplayMode=0;
int ChangeMode1=0;
int ChangeMode2=0;
int AlarmMode=0;
u16 tempYear;
u8 tempMon;
u8 tempDay;
u8 tempWeek;
u8 tempHour;
u8 tempMin;
u8 tempSec;

u8 tempUL=40;
u8 tempLL=90;

u8 tempDHTBuf[5];

int tm_sk=0;

void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//ʹ��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN;//����ģʽ
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//����
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	/**/
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN;//����ģʽ
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//����
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	/**/
}



void KEY_EXTIInit(void)
{
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//0��SYSCFGʱ��ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	
	//1����ʼ��GPIO 
	KEY_Init();
	
	//2�����õ���������Ϊ�ⲿ�ж�����   ѡ��PA0��EXTI0��������
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);
	
	/**/
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource2);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource3);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource4);
	/**/
	
	//3�������ⲿ�ж�ģ��
	EXTI_InitStruct.EXTI_Line = EXTI_Line0 ;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���--�������̴���
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	/**/
	EXTI_InitStruct.EXTI_Line = EXTI_Line2 ;
	EXTI_Init(&EXTI_InitStruct);
	EXTI_InitStruct.EXTI_Line = EXTI_Line3 ;
	EXTI_Init(&EXTI_InitStruct);
	EXTI_InitStruct.EXTI_Line = EXTI_Line4 ;
	EXTI_Init(&EXTI_InitStruct);
	/**/
	
	//4���жϿ�����NVIC��ʼ��-----�жϵ����ȼ�����---EXTI0
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;//6 ȫ���жϺ�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd  = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	/**/
	NVIC_InitStruct.NVIC_IRQChannel = EXTI2_IRQn;//6 ȫ���жϺ�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd  = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI3_IRQn;//6 ȫ���жϺ�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 2;
	NVIC_InitStruct.NVIC_IRQChannelCmd  = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn;//6 ȫ���жϺ�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 3;
	NVIC_InitStruct.NVIC_IRQChannelCmd  = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	/**/
	
}

void delayKey(int tm_ss)
{
	int i;
	tm_sk=tm_ss;
	while(tm_sk--)
	{
		for(i=38400;i>0;i--);
	}
}

//�ⲿ�ж�0 �жϷ�����
void EXTI0_IRQHandler(void)
{
	delayKey(200);
	if(ChangeMode1!=0||ChangeMode2!=0)
	{
		EXTI_ClearITPendingBit(EXTI_Line0);
		return;
	}
	else
	{
		DisplayMode=(DisplayMode+1)%3;
		OLED_Clear();
	}
	GPIO_ToggleBits(GPIOF,GPIO_Pin_9);
	//��ʱ����־λ �������
	EXTI_ClearITPendingBit(EXTI_Line0);
}

void EXTI2_IRQHandler(void)
{
	delayKey(200);//��ʱ����
	GPIO_ToggleBits(GPIOF,GPIO_Pin_10);
	//RCC->BDCR&=~(0x01<<15);
	//��ʱ����־λ �������
	if(DisplayMode==0&&ChangeMode1!=0)
	{
		switch(ChangeMode1)
		{
			case 1:
				tempYear++;
				break;
			case 2:
				addMonth();
				break;
			case 3:
				addDay();
				break;
			case 4:
				addHour();
				break;
			case 5:
				addMin();
				break;
			case 6:
				addSec();
				break;
			default:
				break;
		}
	}
	if(DisplayMode==1&&ChangeMode2!=0)
	{
		switch(ChangeMode2)
		{
			case 1:
				tempTUL++;
				break;
			case 2:
				tempTLL++;
				break;
			case 3:
				tempHUL++;
				break;
			case 4:
				tempHLL++;
				break;
			default:
				break;
		}
	}
	if(DisplayMode==1&&ChangeMode2==0)
	{
		AlarmMode=1;
	}
	EXTI_ClearITPendingBit(EXTI_Line2);
}

void EXTI3_IRQHandler(void)
{
	delayKey(200);//��ʱ����
	//GPIO_ToggleBits(GPIOE,GPIO_Pin_13);
	if(DisplayMode==0&&ChangeMode1!=0)
	{
		switch(ChangeMode1)
		{
			case 1:
				tempYear--;
				break;
			case 2:
				decMonth();
				break;
			case 3:
				decDay();
				break;
			case 4:
				decHour();
				break;
			case 5:
				decMin();
				break;
			case 6:
				decSec();
				break;
			default:
				break;
		}
	}
	if(DisplayMode==1&&ChangeMode2!=0)
	{
		switch(ChangeMode2)
		{
			case 1:
				tempTUL--;
				break;
			case 2:
				tempTLL--;
				break;
			case 3:
				tempHUL--;
				break;
			case 4:
				tempHLL--;
				break;
			default:
				break;
		}
	}
	if(DisplayMode==1&&ChangeMode2==0)
	{
		AlarmMode=0;
		TIM_Cmd(TIM2,DISABLE);
		GPIO_ResetBits(GPIOF,GPIO_Pin_8);
		GPIO_ResetBits(GPIOF,GPIO_Pin_8);
		GPIO_ResetBits(GPIOF,GPIO_Pin_8);
	}
	//RCC->BDCR|=(0x01<<15);
	//��ʱ����־λ �������
	EXTI_ClearITPendingBit(EXTI_Line3);
}

void EXTI4_IRQHandler(void)
{
	int DM0C=0;
	int DM1C=0;
	u8 tempSYear;
	u8 d1,d2,d3,d4,d5,d6,d7;
	GPIO_ToggleBits(GPIOE,GPIO_Pin_13);
	delayKey(200);//��ʱ����
	if(DisplayMode!=2)
	{
		TIM_Cmd(TIM9,DISABLE);
		TIM_Cmd(TIM2,DISABLE);
	}
	if(DisplayMode==0)
	{
		ChangeMode1=(ChangeMode1+1)%7;
		if(ChangeMode1==0)
		{
			posControl=0;
			TIM_Cmd(TIM3,DISABLE);
			OLED_Clear();
			RCC->BDCR|=(0x01<<15);//����RTC
			RTC_SetTime(tempHour,tempMin,tempSec);
			RTC_SetDate(tempYear,tempMon,tempDay,getWeek(tempYear,tempMon,tempDay));
			//delay_s(1);
			readTime(&d1,&d2,&d3);//�ȶ�һ�Σ��ܹ�������ʾδ���ĵ�����
			readDate(&d4,&d5,&d6,&d7);
			TIM_Cmd(TIM9,ENABLE);//��������
			TIM_Cmd(TIM9,ENABLE);//��������
			TIM_Cmd(TIM9,ENABLE);//��������
			EXTI_ClearITPendingBit(EXTI_Line4);
		}
		else
		{
			if(ChangeMode1==1)
			{
				RCC->BDCR&=~(0x01<<15);//��ͣRTC
				TIM_Cmd(TIM9,DISABLE);//��ͣ����
				readDate(&tempSYear,&tempMon,&tempDay,&tempWeek);//��ȡ��ʾ,д��ȫ��
				tempYear=tempSYear+2000;
				readTime(&tempHour,&tempMin,&tempSec);
			}
			//�����޸�ģʽ
			TIM_Cmd(TIM3,ENABLE);//����TIM3��ʾ
			EXTI_ClearITPendingBit(EXTI_Line4);
		}
	}
	else if(DisplayMode==1)
	{
		ChangeMode2=(ChangeMode2+1)%5;
		if(ChangeMode2==0)
		{
			posControl=0;
			TIM_Cmd(TIM3,DISABLE);
			TIM_Cmd(TIM3,DISABLE);
			TIM_Cmd(TIM3,DISABLE);
			OLED_Clear();
			OLED_Clear();
			OLED_Clear();
			TIM_Cmd(TIM9,ENABLE);//��ʼ����
			OLED_Clear();
			//TIM_Cmd(TIM2,ENABLE);//��ʼ����
			//EXTI_ClearITPendingBit(EXTI_Line4);
		}
		else
		{
			if(ChangeMode2==1)
			{
				TIM_Cmd(TIM9,DISABLE);//��ͣ���� ��ͣ���
				TIM_Cmd(TIM2,DISABLE);//�رշ�����
				GPIO_ResetBits(GPIOF,GPIO_Pin_8);
				GPIO_ResetBits(GPIOF,GPIO_Pin_8);
				GPIO_ResetBits(GPIOF,GPIO_Pin_8);
				GPIO_ResetBits(GPIOF,GPIO_Pin_8);
				//TIM_Cmd(TIM4,DISABLE);
				TIM_Cmd(TIM3,ENABLE);//����TIM3��ʾ
			}
			EXTI_ClearITPendingBit(EXTI_Line4);
		}
	}
	//��ʱ����־λ �������
	EXTI_ClearITPendingBit(EXTI_Line4);
}




