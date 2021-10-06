#include "stm32f10x.h"                   //STM32�����Ĵ�������ͷ�ļ����������
#include "delay.h"
      
//SysTick
//static u8  fac_us=0;//us��ʱ������
//static u16 fac_ms=0;//ms��ʱ������
//��ʼ���ӳٺ���
void Delay_init()
{
 SysTick->CTRL&=0xfffffffb;//ѡ���ڲ�ʱ�� ����Ƶ
}            

void delay_us(u32 us)		//uS΢�뼶��ʱ���򣨲ο�ֵ������ʱ����72MHzʱ���ֵ233015��
{ 	
	u32 temp;
	SysTick->LOAD=72*us;      //��װ������ֵ����Ƶ��72MHz������Ƶ��72��Ϊ1΢�룩
	SysTick->VAL=0x00;        		 //��ն�ʱ���ļ�����
	SysTick->CTRL=0x00000005;		 //�ڲ�ʱ��FCLK���򿪶�ʱ��
	do
	{
		temp=SysTick->CTRL;
	}
	while( temp&0x01 && !(temp&(1<<16)) );		//�ȴ�ʱ�䵽�� 0-16λ    
	/*
	temp&0x01:���0��
	!(temp&(1<<16)):���16��λ
	SysTick->CTRL
	λ16����ǰֵ�Ĵ����ݼ���0��λ16��1
	λ2��ʱ��Դѡ��λ
              0��ʹ���ⲿ�ο�ʱ��
              1��ʹ���ں�ʱ��
	λ1��ʹ��SysTick�жϣ���ǰֵ�Ĵ����ݼ���0ʱ�����ж�
	λ0��SysTickʱ��ʹ��
	*/
	SysTick->CTRL=0x00000004;				//�رն�ʱ��(�ٿ�λ0)
	SysTick->VAL=0x00;        				//��ն�ʱ���ļ�����
}

void delay_mms(u16 ms)			//mS���뼶��ʱ���򣨲ο�ֵ������ʱ�������ֵ65535��
{ 	 		  	  
	while( ms-- != 0)
	{
		delay_us(1000);			//����1000΢�����ʱ
	}
}

void delay_s(u16 s)				//S�뼶��ʱ���򣨲ο�ֵ������ʱ�������ֵ65535��
{ 	 		  	  
	while( s-- != 0)
	{
		delay_mms(1000);			//����1000�������ʱ
	}
}

/*����ʱ*/
void Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}
