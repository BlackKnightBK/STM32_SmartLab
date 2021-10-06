#include "stm32f10x.h"                   //STM32器件寄存器定义头文件，必须包含
#include "delay.h"
      
//SysTick
//static u8  fac_us=0;//us延时倍乘数
//static u16 fac_ms=0;//ms延时倍乘数
//初始化延迟函数
void Delay_init()
{
 SysTick->CTRL&=0xfffffffb;//选择内部时钟 不分频
}            

void delay_us(u32 us)		//uS微秒级延时程序（参考值即是延时数，72MHz时最大值233015）
{ 	
	u32 temp;
	SysTick->LOAD=72*us;      //重装计数初值（主频是72MHz，不分频，72次为1微秒）
	SysTick->VAL=0x00;        		 //清空定时器的计数器
	SysTick->CTRL=0x00000005;		 //内部时钟FCLK，打开定时器
	do
	{
		temp=SysTick->CTRL;
	}
	while( temp&0x01 && !(temp&(1<<16)) );		//等待时间到达 0-16位    
	/*
	temp&0x01:检测0号
	!(temp&(1<<16)):检测16号位
	SysTick->CTRL
	位16：当前值寄存器递减到0，位16置1
	位2：时钟源选择位
              0：使用外部参考时钟
              1：使用内核时钟
	位1：使能SysTick中断，当前值寄存器递减到0时产生中断
	位0：SysTick时钟使能
	*/
	SysTick->CTRL=0x00000004;				//关闭定时器(操控位0)
	SysTick->VAL=0x00;        				//清空定时器的计数器
}

void delay_mms(u16 ms)			//mS毫秒级延时程序（参考值即是延时数，最大值65535）
{ 	 		  	  
	while( ms-- != 0)
	{
		delay_us(1000);			//调用1000微秒的延时
	}
}

void delay_s(u16 s)				//S秒级延时程序（参考值即是延时数，最大值65535）
{ 	 		  	  
	while( s-- != 0)
	{
		delay_mms(1000);			//调用1000毫秒的延时
	}
}

/*粗延时*/
void Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}
