#include "spi.h"



void MySPI_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;//SCLK  MOSI
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;//���ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//����
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;//�������
	GPIO_Init(GPIOD,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin   =  GPIO_Pin_11;//CS
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	CS = 1;//�ճ�ʼ�����  CS ���� �ر�SPIͨ��
	SCLK = 1;//ʹ��SPI Mode3  ���ԣ�1   ��λ��1    ʱ��Ĭ���Ǹߵ�ƽ
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
			//�����ز��ý���
	}	
	
	CS = 1;
}














