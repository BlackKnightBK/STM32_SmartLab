#include "delay.h"


void delay(int tm_s)
{
	int i;
	while(tm_s--)
	{
		for(i=38400;i>0;i--);
	}
}


//us级别延时
//参数1： 延时的长度 （0~798915us）
void delay_us(int tm_us)
{
	
	
	//21Mhz---1/21us  tm_us  
	//tm_us*21----延时这么多时间 对应要计的脉冲个数
	SysTick->LOAD = tm_us*21-1;
	SysTick->VAL  = 0;//加载新值  COUNTFLAG清零
	
	//开启定时器
	SysTick->CTRL |= 0x01;
	
	//卡住等待  等到标志位置为1  表示延时时间到  即可跳出循环结束阻塞
	while(1)
	{
		if(SysTick->CTRL & 0x01<<16)
			break;

	}
	
	//关闭定时器
	SysTick->CTRL &= ~0x01;
	SysTick->VAL  = 0;//加载新值  COUNTFLAG清零
}



//ms级别延时
//参数：延时的ms数 （0~798ms）
void delay_ms(int tm_ms)
{
	//21Mhz---1/21us  tm_us  
	//tm_us*21----延时这么多时间 对应要计的脉冲个数
	SysTick->LOAD = tm_ms*21000-1;
	SysTick->VAL  = 0;//加载新值  COUNTFLAG清零
	
	//开启定时器
	SysTick->CTRL |= 0x01;
	
	//卡住等待  等到标志位置为1  表示延时时间到  即可跳出循环结束阻塞
	while(1)
	{
		if(SysTick->CTRL & (0x01<<16))
			break;

	}
	
	//关闭定时器
	SysTick->CTRL &= ~0x01;
	SysTick->VAL  = 0;//加载新值  COUNTFLAG清零
	
}

void delay_s(int tm_s)
{
	while(tm_s--)
	{
		delay_ms(500);
		delay_ms(500);
	}
}



