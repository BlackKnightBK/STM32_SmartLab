#include "tim.h"
#include "oled.h"
#include "key.h"
#include "dht11.h"
#include "usart.h"
#include "adc.h"

u8 posControl;
void TIM9_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	//1��ʱ��ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);

	//2����ʼ��TIM9
	TIM_InitStruct.TIM_Prescaler     = 16800-1;//168M/16800 = 10000Hz  100us/����
	TIM_InitStruct.TIM_Period        = 10000-1;//��ʱ1s
	TIM_InitStruct.TIM_CounterMode   = TIM_CounterMode_Up;//��������ģʽ
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//����Ƶ 
	TIM_TimeBaseInit(TIM9,&TIM_InitStruct);
	

	//3���������Ƕ�ʱ��������ж�  ʵ�ֶ�ʱʱ��һ�� �ܴ����ж�֪ͨCPUȥ���ĳ������
	TIM_ITConfig(TIM9,TIM_IT_Update,ENABLE);
	
	//4���жϹ�����NVIC  �����濪���ж����ȼ�������
	NVIC_InitStruct.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;//24 ȫ���жϺ�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 2;
	NVIC_InitStruct.NVIC_IRQChannelCmd  = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	//5��������ʱ��
	//TIM_Cmd(TIM9,ENABLE);

}

void TIM2_Init(void)//���ڷ�����
{
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	//1��ʱ��ʹ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//��������(?)
	TIM_DeInit(TIM2);
	
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//����ж�λ
	//2����ʼ��TIM2
	TIM_InitStruct.TIM_Prescaler     = 16800-1;//��APB1�У����������ʵ��Ч��Ϊ 200us/���� (168M/16800 = 10000Hz  100us/����)
	TIM_InitStruct.TIM_Period        = 2500-1;//��ʱ0.5s 
	TIM_InitStruct.TIM_CounterMode   = TIM_CounterMode_Up;//��������ģʽ
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//����Ƶ 
	TIM_TimeBaseInit(TIM2,&TIM_InitStruct);
	

	//3���������Ƕ�ʱ��������ж�  ʵ�ֶ�ʱʱ��һ�� �ܴ����ж�֪ͨCPUȥ���ĳ������
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	//4���жϹ�����NVIC  �����濪���ж����ȼ�������
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;//24 ȫ���жϺ�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 3;
	NVIC_InitStruct.NVIC_IRQChannelCmd  = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	//5��������ʱ��
	//TIM_Cmd(TIM2,ENABLE);

}

void TIM3_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	//1��ʱ��ʹ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//��������(?)
	TIM_DeInit(TIM3);
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);//����ж�λ
	//2����ʼ��TIM3
	TIM_InitStruct.TIM_Prescaler     = 16800-1;//��APB1�У����������ʵ��Ч��Ϊ 200us/���� (168M/16800 = 10000Hz  100us/����)
	TIM_InitStruct.TIM_Period        = 2500-1;//��ʱ0.5s
	TIM_InitStruct.TIM_CounterMode   = TIM_CounterMode_Up;//��������ģʽ
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//����Ƶ 
	TIM_TimeBaseInit(TIM3,&TIM_InitStruct);
	

	//3���������Ƕ�ʱ��������ж�  ʵ�ֶ�ʱʱ��һ�� �ܴ����ж�֪ͨCPUȥ���ĳ������
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	//4���жϹ�����NVIC  �����濪���ж����ȼ�������
	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;//24 ȫ���жϺ�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 4;
	NVIC_InitStruct.NVIC_IRQChannelCmd  = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	//5��������ʱ��
	//TIM_Cmd(TIM3,ENABLE);

}

void TIM4_Init(void)//��ȡDHT11
{
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	//1��ʱ��ʹ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//��������(?)
	TIM_DeInit(TIM4);
	
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);//����ж�λ
	//2����ʼ��TIM4
	TIM_InitStruct.TIM_Prescaler     = 16800-1;//��APB1�У����������ʵ��Ч��Ϊ 200us/���� (168M/16800 = 10000Hz  100us/����)
	TIM_InitStruct.TIM_Period        = 10000-1;//��ʱ2s
	TIM_InitStruct.TIM_CounterMode   = TIM_CounterMode_Up;//��������ģʽ
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//����Ƶ 
	TIM_TimeBaseInit(TIM4,&TIM_InitStruct);
	

	//3���������Ƕ�ʱ��������ж�  ʵ�ֶ�ʱʱ��һ�� �ܴ����ж�֪ͨCPUȥ���ĳ������
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	
	//4���жϹ�����NVIC  �����濪���ж����ȼ�������
	NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn;//24 ȫ���жϺ�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd  = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	//5��������ʱ��
	//TIM_Cmd(TIM4,ENABLE);

}

void TIM5_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	//1��ʱ��ʹ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);//��������(?)
	TIM_DeInit(TIM5);
	
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);//����ж�λ
	//2����ʼ��TIM5
	TIM_InitStruct.TIM_Prescaler     = 16800-1;//��APB1�У����������ʵ��Ч��Ϊ 200us/���� (168M/16800 = 10000Hz  100us/����)
	TIM_InitStruct.TIM_Period        = 2500-1;//��ʱ0.5s
	TIM_InitStruct.TIM_CounterMode   = TIM_CounterMode_Up;//��������ģʽ
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//����Ƶ 
	TIM_TimeBaseInit(TIM5,&TIM_InitStruct);
	

	//3���������Ƕ�ʱ��������ж�  ʵ�ֶ�ʱʱ��һ�� �ܴ����ж�֪ͨCPUȥ���ĳ������
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
	
	//4���жϹ�����NVIC  �����濪���ж����ȼ�������
	NVIC_InitStruct.NVIC_IRQChannel = TIM5_IRQn;//24 ȫ���жϺ�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd  = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	//5��������ʱ��
	//TIM_Cmd(TIM5,ENABLE);

}

void TIM1_BRK_TIM9_IRQHandler(void)//��ʾʱ������ ���ǿ��
{
	if(TIM_GetITStatus(TIM9,TIM_IT_Update) == 1)
	{
		//�ܽ��� ��ʾȷʵ��ʱʱ�䵽�� ���������жϽ���
		if(DisplayMode==0)
		{
			OLED_DisplayDates();//��ʾʱ��
		}
		else if(DisplayMode==1)
		{
			OLED_DisplayDH11(tempT,tempH);
		}
		else if(DisplayMode==2)
		{
			tempLightData=Get_ADCVal(ADC3);
			OLED_DisplayLight(tempLightData);
		}
		if(ChangeMode1==0&&ChangeMode2==0&&AlarmMode==1)
		{
			if(tempH<tempHLL||tempH>tempHUL||tempT<tempTLL||tempT>tempTUL)
			{
				TIM_Cmd(TIM2,ENABLE);
			}
			else
			{
				TIM_Cmd(TIM2,DISABLE);
				GPIO_ResetBits(GPIOF,GPIO_Pin_8);
				GPIO_ResetBits(GPIOF,GPIO_Pin_8);
				GPIO_ResetBits(GPIOF,GPIO_Pin_8);
			}
		}
		if(AlarmMode==0)
		{
			TIM_Cmd(TIM2,DISABLE);
			TIM_Cmd(TIM2,DISABLE);
			TIM_Cmd(TIM2,DISABLE);
			GPIO_ResetBits(GPIOF,GPIO_Pin_8);
			GPIO_ResetBits(GPIOF,GPIO_Pin_8);
			GPIO_ResetBits(GPIOF,GPIO_Pin_8);
		}
		printf("AD:%d\r\n",Get_ADCVal(ADC3));
		//�����־λ
		TIM_ClearITPendingBit(TIM9,TIM_IT_Update);
	}
	TIM_ClearITPendingBit(TIM9,TIM_IT_Update);
}

void TIM2_IRQHandler(void)//��ʾDHT11
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == 1)
	{
		//�ܽ��� ��ʾȷʵ��ʱʱ�䵽�� ���������жϽ���
		GPIO_ToggleBits(GPIOF,GPIO_Pin_8);
		//�����־λ
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}

void TIM3_IRQHandler(void)//��ʾ�޸�״̬
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) == 1)
	{
		if(DisplayMode==0)
		{
			posControl=posControl>0?0:ChangeMode1;
			//�ܽ��� ��ʾȷʵ��ʱʱ�䵽�� ���������жϽ���
			OLED_DisPlayChangeDates(tempYear,tempMon,tempDay,tempWeek,tempHour,tempMin,tempSec,posControl);
		}
		else
		{
			posControl=posControl>0?0:ChangeMode2;
			OLED_DisPlayChangeDHT(tempTUL,tempTLL,tempHUL,tempHLL,posControl);
		}
		//�����־λ
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}
}

void TIM4_IRQHandler(void)//��ȡDHT11
{
	
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) == 1)
	{
		GPIO_ToggleBits(GPIOE,GPIO_Pin_14);
		u8 DHT11Buf[5];
		//�ܽ��� ��ʾȷʵ��ʱʱ�䵽�� ���������жϽ���
		if(!DHT11_GetTR(DHT11Buf))
		{
			tempT=DHT11Buf[2];
			tempH=DHT11Buf[0];
		}		
		//�����־λ
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	}
}

void TIM5_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM5,TIM_IT_Update) == 1)
	{
		//�ܽ��� ��ʾȷʵ��ʱʱ�䵽�� ���������жϽ���
		GPIO_ToggleBits(GPIOF,GPIO_Pin_8);
		//�����־λ
		TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
	}
}














