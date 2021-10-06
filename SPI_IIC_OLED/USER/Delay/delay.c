#include "delay.h"


void delay(int tm_s)
{
	int i;
	while(tm_s--)
	{
		for(i=38400;i>0;i--);
	}
}


//us������ʱ
//����1�� ��ʱ�ĳ��� ��0~798915us��
void delay_us(int tm_us)
{
	
	
	//21Mhz---1/21us  tm_us  
	//tm_us*21----��ʱ��ô��ʱ�� ��ӦҪ�Ƶ��������
	SysTick->LOAD = tm_us*21-1;
	SysTick->VAL  = 0;//������ֵ  COUNTFLAG����
	
	//������ʱ��
	SysTick->CTRL |= 0x01;
	
	//��ס�ȴ�  �ȵ���־λ��Ϊ1  ��ʾ��ʱʱ�䵽  ��������ѭ����������
	while(1)
	{
		if(SysTick->CTRL & 0x01<<16)
			break;

	}
	
	//�رն�ʱ��
	SysTick->CTRL &= ~0x01;
	SysTick->VAL  = 0;//������ֵ  COUNTFLAG����
}



//ms������ʱ
//��������ʱ��ms�� ��0~798ms��
void delay_ms(int tm_ms)
{
	//21Mhz---1/21us  tm_us  
	//tm_us*21----��ʱ��ô��ʱ�� ��ӦҪ�Ƶ��������
	SysTick->LOAD = tm_ms*21000-1;
	SysTick->VAL  = 0;//������ֵ  COUNTFLAG����
	
	//������ʱ��
	SysTick->CTRL |= 0x01;
	
	//��ס�ȴ�  �ȵ���־λ��Ϊ1  ��ʾ��ʱʱ�䵽  ��������ѭ����������
	while(1)
	{
		if(SysTick->CTRL & (0x01<<16))
			break;

	}
	
	//�رն�ʱ��
	SysTick->CTRL &= ~0x01;
	SysTick->VAL  = 0;//������ֵ  COUNTFLAG����
	
}

void delay_s(int tm_s)
{
	while(tm_s--)
	{
		delay_ms(500);
		delay_ms(500);
	}
}



