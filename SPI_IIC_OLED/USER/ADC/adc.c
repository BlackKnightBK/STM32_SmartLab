#include "adc.h"
#include <stm32f4xx.h>
#include "stm32f4xx_adc.h"

u16 tempLightData=0;

void ADC1IN5_Init(void)
{
	GPIO_InitTypeDef GPIO_IniteStruct;
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	ADC_InitTypeDef  ADC_InitStruct;
	//1��ʹ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	//2����ʼ��GPIOΪģ��ģʽ  PA5
	GPIO_IniteStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_IniteStruct.GPIO_Mode = GPIO_Mode_AN;//ģ������ģʽ
	GPIO_IniteStruct.GPIO_Speed = GPIO_Speed_100MHz;//����
	GPIO_IniteStruct.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_IniteStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//����  ��ҪӰ���ⲿ�����ģ���ѹ
	GPIO_Init(GPIOA,&GPIO_IniteStruct);//��ʼ��
	
	ADC_DeInit();//��λADC
	
	//2.5������ADC���е�ģ���ʼ��   ʱ�ӷ�Ƶ....
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div6;//168/6 = 28Mhz<36
	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//�������������˽�--DMA
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_15Cycles;
	
	//3����ʼ��ADC1
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	ADC_InitStruct.ADC_ContinuousConvMode  = DISABLE;
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//�������
	ADC_InitStruct.ADC_DataAlign             = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_NbrOfConversion       = 1;
	ADC_Init(ADC1,&ADC_InitStruct);
	
	
	//4������ADC ��ADC��ʼ���� ��= ��ʼת��
	ADC_Cmd(ADC1,ENABLE);
	
}


//�������� ADC3IN5   PF7
void ADC3IN5_Init(void)
{
	GPIO_InitTypeDef 			GPIO_InitStruct;
	ADC_InitTypeDef      		ADC_InitStruct;
	ADC_CommonInitTypeDef 		ADC_CommonInitStruct;
	
	//1��ʹ��ʱ��	ADC3 GPIOF
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

	//2����ʼ��PF7Ϊ����ģʽ AN
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_7;			//ָ������5
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AN;			//ģ��ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;			//�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		//����
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;		//����
	GPIO_Init(GPIOF,&GPIO_InitStruct);

	//3����ʼ��ADC3��������
	ADC_InitStruct.ADC_Resolution			=	ADC_Resolution_12b ;    //ת���ķֱ���        
	ADC_InitStruct.ADC_ScanConvMode			=	DISABLE;		//�ر�ɨ��--��ͨ��
	ADC_InitStruct.ADC_ContinuousConvMode	=	DISABLE;		//������
	ADC_InitStruct.ADC_ExternalTrigConvEdge	=	ADC_ExternalTrigConvEdge_None;	//�ޱ���
//	ADC_InitStruct.ADC_ExternalTrigConv							//ȱʡ
	ADC_InitStruct.ADC_DataAlign			=	ADC_DataAlign_Right;	//�����Ҷ���
	ADC_InitStruct.ADC_NbrOfConversion		=	1;				//�Ŷ���Ϊ1
	ADC_Init(ADC3,&ADC_InitStruct);
	
	//4������ͨ������
	ADC_RegularChannelConfig(ADC3,ADC_Channel_5,1,ADC_SampleTime_56Cycles);
	
	//5����ʼ��ADC��������
	ADC_CommonInitStruct.ADC_Mode				=	ADC_Mode_Independent  ;
	ADC_CommonInitStruct.ADC_Prescaler			=	ADC_Prescaler_Div4;
	ADC_CommonInitStruct.ADC_DMAAccessMode		=	ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStruct.ADC_TwoSamplingDelay	=	ADC_TwoSamplingDelay_13Cycles;
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	//6��ʹ��ADC3
	ADC_Cmd(ADC3,ENABLE);

}

unsigned short Get_ADCVal(ADC_TypeDef *ADCx)
{
	//unsigned short ADC_VAL;
	
	//���ù���ͨ��
	
	ADC_ClearFlag(ADCx,ADC_FLAG_EOC);
	ADC_RegularChannelConfig(ADCx,ADC_Channel_5,1,ADC_SampleTime_56Cycles);
	ADC_SoftwareStartConv(ADCx);//����ADC1��ת��
	while(0==ADC_GetFlagStatus(ADCx,ADC_FLAG_EOC));//�ȴ�ת������
	//ADC_ClearFlag(ADC1,ADC_FLAG_EOC);
	
	
	//��ȡת��ֵ
	return ADC_GetConversionValue(ADC3);

}



