#include "spi.h"



void MySPI_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;//SCLK  MOSI
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;//输出模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//高速
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;//上拉输出
	GPIO_Init(GPIOD,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin   =  GPIO_Pin_11;//CS
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	CS = 1;//刚初始化完成  CS 拉高 关闭SPI通信
	SCLK = 1;//使用SPI Mode3  极性：1   相位：1    时钟默认是高电平
}




//Mode3
void MySPI_WriteByte(u8 WByte)
{
	int i;
	
	CS = 0;
	
	for(i=0;i<8;i++)
	{
		SCLK = 0;
			if(WByte & (0x80>>i))
				MOSI = 1;
			else 
				MOSI = 0;
			
		SCLK = 1;
			//上升沿不用接收
	}	
	
	CS = 1;
}














