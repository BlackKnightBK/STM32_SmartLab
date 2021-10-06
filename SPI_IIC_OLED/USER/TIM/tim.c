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

	//1、时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);

	//2、初始化TIM9
	TIM_InitStruct.TIM_Prescaler     = 16800-1;//168M/16800 = 10000Hz  100us/脉冲
	TIM_InitStruct.TIM_Period        = 10000-1;//定时1s
	TIM_InitStruct.TIM_CounterMode   = TIM_CounterMode_Up;//递增计数模式
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//不分频 
	TIM_TimeBaseInit(TIM9,&TIM_InitStruct);
	

	//3、开启我们定时器的相关中断  实现定时时间一到 能触发中断通知CPU去完成某个功能
	TIM_ITConfig(TIM9,TIM_IT_Update,ENABLE);
	
	//4、中断管理器NVIC  对上面开启中断优先级的配置
	NVIC_InitStruct.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;//24 全局中断号
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 2;
	NVIC_InitStruct.NVIC_IRQChannelCmd  = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	//5、开启定时器
	//TIM_Cmd(TIM9,ENABLE);

}

void TIM2_Init(void)//用于蜂鸣器
{
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	//1、时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//低速总线(?)
	TIM_DeInit(TIM2);
	
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//清除中断位
	//2、初始化TIM2
	TIM_InitStruct.TIM_Prescaler     = 16800-1;//在APB1中，这个参数的实际效果为 200us/脉冲 (168M/16800 = 10000Hz  100us/脉冲)
	TIM_InitStruct.TIM_Period        = 2500-1;//定时0.5s 
	TIM_InitStruct.TIM_CounterMode   = TIM_CounterMode_Up;//递增计数模式
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//不分频 
	TIM_TimeBaseInit(TIM2,&TIM_InitStruct);
	

	//3、开启我们定时器的相关中断  实现定时时间一到 能触发中断通知CPU去完成某个功能
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	//4、中断管理器NVIC  对上面开启中断优先级的配置
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;//24 全局中断号
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 3;
	NVIC_InitStruct.NVIC_IRQChannelCmd  = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	//5、开启定时器
	//TIM_Cmd(TIM2,ENABLE);

}

void TIM3_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	//1、时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//低速总线(?)
	TIM_DeInit(TIM3);
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);//清除中断位
	//2、初始化TIM3
	TIM_InitStruct.TIM_Prescaler     = 16800-1;//在APB1中，这个参数的实际效果为 200us/脉冲 (168M/16800 = 10000Hz  100us/脉冲)
	TIM_InitStruct.TIM_Period        = 2500-1;//定时0.5s
	TIM_InitStruct.TIM_CounterMode   = TIM_CounterMode_Up;//递增计数模式
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//不分频 
	TIM_TimeBaseInit(TIM3,&TIM_InitStruct);
	

	//3、开启我们定时器的相关中断  实现定时时间一到 能触发中断通知CPU去完成某个功能
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	//4、中断管理器NVIC  对上面开启中断优先级的配置
	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;//24 全局中断号
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 4;
	NVIC_InitStruct.NVIC_IRQChannelCmd  = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	//5、开启定时器
	//TIM_Cmd(TIM3,ENABLE);

}

void TIM4_Init(void)//读取DHT11
{
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	//1、时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//低速总线(?)
	TIM_DeInit(TIM4);
	
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);//清除中断位
	//2、初始化TIM4
	TIM_InitStruct.TIM_Prescaler     = 16800-1;//在APB1中，这个参数的实际效果为 200us/脉冲 (168M/16800 = 10000Hz  100us/脉冲)
	TIM_InitStruct.TIM_Period        = 10000-1;//定时2s
	TIM_InitStruct.TIM_CounterMode   = TIM_CounterMode_Up;//递增计数模式
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//不分频 
	TIM_TimeBaseInit(TIM4,&TIM_InitStruct);
	

	//3、开启我们定时器的相关中断  实现定时时间一到 能触发中断通知CPU去完成某个功能
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	
	//4、中断管理器NVIC  对上面开启中断优先级的配置
	NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn;//24 全局中断号
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd  = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	//5、开启定时器
	//TIM_Cmd(TIM4,ENABLE);

}

void TIM5_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	//1、时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);//低速总线(?)
	TIM_DeInit(TIM5);
	
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);//清除中断位
	//2、初始化TIM5
	TIM_InitStruct.TIM_Prescaler     = 16800-1;//在APB1中，这个参数的实际效果为 200us/脉冲 (168M/16800 = 10000Hz  100us/脉冲)
	TIM_InitStruct.TIM_Period        = 2500-1;//定时0.5s
	TIM_InitStruct.TIM_CounterMode   = TIM_CounterMode_Up;//递增计数模式
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//不分频 
	TIM_TimeBaseInit(TIM5,&TIM_InitStruct);
	

	//3、开启我们定时器的相关中断  实现定时时间一到 能触发中断通知CPU去完成某个功能
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
	
	//4、中断管理器NVIC  对上面开启中断优先级的配置
	NVIC_InitStruct.NVIC_IRQChannel = TIM5_IRQn;//24 全局中断号
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd  = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	//5、开启定时器
	//TIM_Cmd(TIM5,ENABLE);

}

void TIM1_BRK_TIM9_IRQHandler(void)//显示时间日期 检测强度
{
	if(TIM_GetITStatus(TIM9,TIM_IT_Update) == 1)
	{
		//能进来 表示确实定时时间到了 触发更新中断进来
		if(DisplayMode==0)
		{
			OLED_DisplayDates();//显示时间
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
		//清除标志位
		TIM_ClearITPendingBit(TIM9,TIM_IT_Update);
	}
	TIM_ClearITPendingBit(TIM9,TIM_IT_Update);
}

void TIM2_IRQHandler(void)//显示DHT11
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == 1)
	{
		//能进来 表示确实定时时间到了 触发更新中断进来
		GPIO_ToggleBits(GPIOF,GPIO_Pin_8);
		//清除标志位
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}

void TIM3_IRQHandler(void)//显示修改状态
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) == 1)
	{
		if(DisplayMode==0)
		{
			posControl=posControl>0?0:ChangeMode1;
			//能进来 表示确实定时时间到了 触发更新中断进来
			OLED_DisPlayChangeDates(tempYear,tempMon,tempDay,tempWeek,tempHour,tempMin,tempSec,posControl);
		}
		else
		{
			posControl=posControl>0?0:ChangeMode2;
			OLED_DisPlayChangeDHT(tempTUL,tempTLL,tempHUL,tempHLL,posControl);
		}
		//清除标志位
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}
}

void TIM4_IRQHandler(void)//读取DHT11
{
	
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) == 1)
	{
		GPIO_ToggleBits(GPIOE,GPIO_Pin_14);
		u8 DHT11Buf[5];
		//能进来 表示确实定时时间到了 触发更新中断进来
		if(!DHT11_GetTR(DHT11Buf))
		{
			tempT=DHT11Buf[2];
			tempH=DHT11Buf[0];
		}		
		//清除标志位
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	}
}

void TIM5_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM5,TIM_IT_Update) == 1)
	{
		//能进来 表示确实定时时间到了 触发更新中断进来
		GPIO_ToggleBits(GPIOF,GPIO_Pin_8);
		//清除标志位
		TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
	}
}














