#include "adc.h"
#include <stm32f4xx.h>
#include "stm32f4xx_adc.h"

u16 tempLightData=0;

void ADC1IN5_Init(void)
{
	GPIO_InitTypeDef GPIO_IniteStruct;
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	ADC_InitTypeDef  ADC_InitStruct;
	//1、使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	//2、初始化GPIO为模拟模式  PA5
	GPIO_IniteStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_IniteStruct.GPIO_Mode = GPIO_Mode_AN;//模拟输入模式
	GPIO_IniteStruct.GPIO_Speed = GPIO_Speed_100MHz;//高速
	GPIO_IniteStruct.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_IniteStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//浮空  不要影响外部输入的模拟电压
	GPIO_Init(GPIOA,&GPIO_IniteStruct);//初始化
	
	ADC_DeInit();//复位ADC
	
	//2.5、三个ADC共有的模块初始化   时钟分频....
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div6;//168/6 = 28Mhz<36
	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//留给你们自行了解--DMA
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_15Cycles;
	
	//3、初始化ADC1
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	ADC_InitStruct.ADC_ContinuousConvMode  = DISABLE;
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//软件启动
	ADC_InitStruct.ADC_DataAlign             = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_NbrOfConversion       = 1;
	ADC_Init(ADC1,&ADC_InitStruct);
	
	
	//4、开启ADC 让ADC开始工作 ！= 开始转换
	ADC_Cmd(ADC1,ENABLE);
	
}


//光敏电阻 ADC3IN5   PF7
void ADC3IN5_Init(void)
{
	GPIO_InitTypeDef 			GPIO_InitStruct;
	ADC_InitTypeDef      		ADC_InitStruct;
	ADC_CommonInitTypeDef 		ADC_CommonInitStruct;
	
	//1、使能时钟	ADC3 GPIOF
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

	//2、初始化PF7为输入模式 AN
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_7;			//指定引脚5
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AN;			//模拟模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;			//推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		//高速
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;		//浮空
	GPIO_Init(GPIOF,&GPIO_InitStruct);

	//3、初始化ADC3单独部分
	ADC_InitStruct.ADC_Resolution			=	ADC_Resolution_12b ;    //转换的分辨率        
	ADC_InitStruct.ADC_ScanConvMode			=	DISABLE;		//关闭扫描--单通道
	ADC_InitStruct.ADC_ContinuousConvMode	=	DISABLE;		//非连续
	ADC_InitStruct.ADC_ExternalTrigConvEdge	=	ADC_ExternalTrigConvEdge_None;	//无边沿
//	ADC_InitStruct.ADC_ExternalTrigConv							//缺省
	ADC_InitStruct.ADC_DataAlign			=	ADC_DataAlign_Right;	//数据右对齐
	ADC_InitStruct.ADC_NbrOfConversion		=	1;				//排队数为1
	ADC_Init(ADC3,&ADC_InitStruct);
	
	//4、规则通道配置
	ADC_RegularChannelConfig(ADC3,ADC_Channel_5,1,ADC_SampleTime_56Cycles);
	
	//5、初始化ADC公共部分
	ADC_CommonInitStruct.ADC_Mode				=	ADC_Mode_Independent  ;
	ADC_CommonInitStruct.ADC_Prescaler			=	ADC_Prescaler_Div4;
	ADC_CommonInitStruct.ADC_DMAAccessMode		=	ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStruct.ADC_TwoSamplingDelay	=	ADC_TwoSamplingDelay_13Cycles;
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	//6、使能ADC3
	ADC_Cmd(ADC3,ENABLE);

}

unsigned short Get_ADCVal(ADC_TypeDef *ADCx)
{
	//unsigned short ADC_VAL;
	
	//设置规则通道
	
	ADC_ClearFlag(ADCx,ADC_FLAG_EOC);
	ADC_RegularChannelConfig(ADCx,ADC_Channel_5,1,ADC_SampleTime_56Cycles);
	ADC_SoftwareStartConv(ADCx);//启动ADC1的转换
	while(0==ADC_GetFlagStatus(ADCx,ADC_FLAG_EOC));//等待转换结束
	//ADC_ClearFlag(ADC1,ADC_FLAG_EOC);
	
	
	//获取转换值
	return ADC_GetConversionValue(ADC3);

}



