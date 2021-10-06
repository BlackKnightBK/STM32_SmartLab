#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "delay.h"
#include "GY30.h"

//uchar BUF[8];
//unsigned int mcy;
//void delay_mms(u16 nms);
//����ʹ��iicͨ��
//SDA�ӵĹܽ�Ϊ  PB6
//SCL�ӵĹܽ�Ϊ   PB7

int GY30INITED=0;

void GY30_IIC_Init()//��PB6 PB7��6Ϊ�����ߣ�7Ϊʱ����
{
		GPIO_InitTypeDef  GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	  GPIO_Init(GPIOB, &GPIO_InitStructure);  
}

void GY30_Start() //��ʼ�ź�
{
    GPIO_SetBits(GPIOB,GPIO_Pin_6);          //����������
    GPIO_SetBits(GPIOB,GPIO_Pin_7);         //����ʱ����
    delay_us(5);
    GPIO_ResetBits(GPIOB,GPIO_Pin_6);     //�����½���
    delay_us(5);
    GPIO_ResetBits(GPIOB,GPIO_Pin_7);   //����ʱ����
}

void GY30_Stop()  //ֹͣ�ź�
{
    GPIO_ResetBits(GPIOB,GPIO_Pin_6);      //����������
    GPIO_SetBits(GPIOB,GPIO_Pin_7);       //����ʱ����
    delay_us(5);
    GPIO_SetBits(GPIOB,GPIO_Pin_6);     //����������
    delay_us(5);
}

void GY30_SendACK(BitAction ack)                //����Ӧ���ź�
{
   GPIO_InitTypeDef  GPIO_InitStructure;        
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                    //SDA
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;            //�������
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOB, &GPIO_InitStructure);
   if(ack==1)
   {
      GPIO_SetBits(GPIOB,GPIO_Pin_6);//��1
   }
   else if(ack==0)
   {
      GPIO_ResetBits(GPIOB,GPIO_Pin_6);//��0
   }
   else
	 {
			return;//(?)
	 }
   GPIO_SetBits(GPIOB,GPIO_Pin_7);
   delay_us(5);
   GPIO_ResetBits(GPIOB,GPIO_Pin_7);
   delay_us(5);
}

int GY30_RecvACK()//����Ӧ���ź�
{
   GPIO_InitTypeDef  GPIO_InitStructure;        
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;              
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������  GY30���ڻ���������(?)
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOB, &GPIO_InitStructure);
        
   GPIO_SetBits(GPIOB,GPIO_Pin_7);
   delay_us(5);//Delay_5us();
        
   if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6) == 1)//����Ӧ��
	{
		mcy = 1;//1
	}
   else
	{
		mcy =0;//0
	}
        
  GPIO_ResetBits(GPIOB,GPIO_Pin_7);                                //����ʱ����
  delay_us(5);//Delay_5us();
        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;              //�������
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  return mcy;
}

int GY30_SendByte(uchar dat)
{
	uchar i;
  for(i=0;i<8;i++)
  {
		if(0x80 & dat)//ȡ���λ 1000 0000
		{
			GPIO_SetBits(GPIOB,GPIO_Pin_6);
		}
    else
    {
			GPIO_ResetBits(GPIOB,GPIO_Pin_6);
		}
    dat <<= 1; //��������һλ���������λ
		GPIO_SetBits(GPIOB,GPIO_Pin_7);				//����ʱ����
    delay_us(5);
    GPIO_ResetBits(GPIOB,GPIO_Pin_7); 	//����ʱ����
    delay_us(5);         
  }
	GY30_RecvACK();
  //return GY30_RecvACK();
	return 1;
}

uchar GY30_RecvByte()
{
		uchar i;
    uchar dat=0;
    uchar bit;
    GPIO_InitTypeDef  GPIO_InitStructure;        
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
        
    GPIO_SetBits(GPIOB,GPIO_Pin_6);//ʹ���ڲ�����,׼���ý�������
    for(i=0; i<8;i++)
    {
			dat <<= 1;
      GPIO_SetBits(GPIOB,GPIO_Pin_7);//����ʱ����
      delay_us(5);       
      if(SET == GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6))
			{
				bit = 0x01;
			}
      else
			{
				bit = 0x00;
			}
      dat |= bit;//������
      GPIO_ResetBits(GPIOB,GPIO_Pin_7);
      delay_us(5);        
    }
        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    return dat;           
}

int Single_Write_GY30(uchar REG_Address)
{
	int state=0;
  GY30_Start();                                  //��ʹ�ź�
	if(GY30_SendByte(SlaveAddress))								//�����豸��ַ+д�ź�
  {
		if(GY30_SendByte(REG_Address))            //�ڲ��Ĵ�����ַ
		{
			state=1;
		}
	}                
  GY30_Stop();
	return state;
}

int Init_GY30()
{
	int state=0;
  if(Single_Write_GY30(0x01))//0x01Ϊ����ָ��
	{
		state=1;
	}
	return state;
}

void mread(void)
{
		uchar i;
    GY30_Start();
	GY30_SendByte(SlaveAddress+1);                                //�����豸��ַ+���ź� SlaveAddress+1Ϊ��GY30��ȡ��Ϣ(����GY30������Ϣ)(?)
    for(i=0; i<3;i++)																					 //ֻ����SlaveAddressΪдָ��ȥGY30(����GY30�ȴ�����ָ��)(?)
    {
			BUF[i] = GY30_RecvByte();
			GPIO_ResetBits(GPIOC,GPIO_Pin_3);
      if(i ==3)
      {
				GY30_SendACK(1);                                //���һ����ӦNOACK
      }
      else
      {
        GY30_SendACK(0);
      }
    }        
    GY30_Stop();
    delay_mms(5);//Delay_mms(5);
}