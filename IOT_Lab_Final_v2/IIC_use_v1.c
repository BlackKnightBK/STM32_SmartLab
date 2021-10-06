#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "delay.h"
#include "GY30.h"

//uchar BUF[8];
//unsigned int mcy;
//void delay_mms(u16 nms);
//工程使用iic通信
//SDA接的管脚为  PB6
//SCL接的管脚为   PB7

int GY30INITED=0;

void GY30_IIC_Init()//接PB6 PB7，6为数据线，7为时钟线
{
		GPIO_InitTypeDef  GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	  GPIO_Init(GPIOB, &GPIO_InitStructure);  
}

void GY30_Start() //开始信号
{
    GPIO_SetBits(GPIOB,GPIO_Pin_6);          //拉高数据线
    GPIO_SetBits(GPIOB,GPIO_Pin_7);         //拉高时钟线
    delay_us(5);
    GPIO_ResetBits(GPIOB,GPIO_Pin_6);     //产生下降沿
    delay_us(5);
    GPIO_ResetBits(GPIOB,GPIO_Pin_7);   //拉低时钟线
}

void GY30_Stop()  //停止信号
{
    GPIO_ResetBits(GPIOB,GPIO_Pin_6);      //拉低数据线
    GPIO_SetBits(GPIOB,GPIO_Pin_7);       //拉高时钟线
    delay_us(5);
    GPIO_SetBits(GPIOB,GPIO_Pin_6);     //产生上升沿
    delay_us(5);
}

void GY30_SendACK(BitAction ack)                //发送应答信号
{
   GPIO_InitTypeDef  GPIO_InitStructure;        
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                    //SDA
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;            //推挽输出
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOB, &GPIO_InitStructure);
   if(ack==1)
   {
      GPIO_SetBits(GPIOB,GPIO_Pin_6);//置1
   }
   else if(ack==0)
   {
      GPIO_ResetBits(GPIOB,GPIO_Pin_6);//置0
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

/*
每当主机向从机发送完一个字节的数据，主机总是需要等待从机给出一个应答信号，
以确认从机是否成功接收到了数据，从机应答主机所需要的时钟仍是主机提供的，
应答出现在每一次主机完成8个数据位传输后紧跟着的时钟周期，低电平0表示应答，1表示非应答
*/

int GY30_RecvACK()//接受应答信号
{
   GPIO_InitTypeDef  GPIO_InitStructure;        
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;              
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//输入上拉  GY30存在会拉低引脚(?)
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOB, &GPIO_InitStructure);
        
   GPIO_SetBits(GPIOB,GPIO_Pin_7);
   delay_us(5);//Delay_5us();
        
   if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6) == 1)//接收应答,接收到1为非应答，返回0，
	{																								//接收到0为应答，返回1，接收成功
		mcy = 0;//1
	}
   else
	{
		mcy =1;//0
	}
        
  GPIO_ResetBits(GPIOB,GPIO_Pin_7);                                //拉低时钟线
  delay_us(5);//Delay_5us();
        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;              //推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  return mcy;
}

int GY30_SendByte(uchar dat)
{
	uchar i;
  for(i=0;i<8;i++)
  {
		if(0x80 & dat)//取最高位 1000 0000
		{
			GPIO_SetBits(GPIOB,GPIO_Pin_6);
		}
    else
    {
			GPIO_ResetBits(GPIOB,GPIO_Pin_6);
		}
    dat <<= 1; //数据左移一位，丢掉最高位
		GPIO_SetBits(GPIOB,GPIO_Pin_7);				//拉高时钟线
    delay_us(5);
    GPIO_ResetBits(GPIOB,GPIO_Pin_7); 	//拉低时钟线
    delay_us(5);         
  }
  return GY30_RecvACK();
}

uchar GY30_RecvByte()
{
		uchar i;
    uchar dat=0;
    uchar bit;
    GPIO_InitTypeDef  GPIO_InitStructure;        
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//输入上拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
        
    GPIO_SetBits(GPIOB,GPIO_Pin_6);//使能内部上拉,准备好接收数据
    for(i=0; i<8;i++)
    {
			dat <<= 1;
      GPIO_SetBits(GPIOB,GPIO_Pin_7);//拉高时钟线
      delay_us(5);       
      if(SET == GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6))
			{
				bit = 0x01;
			}
      else
			{
				bit = 0x00;
			}
      dat |= bit;//读数据
      GPIO_ResetBits(GPIOB,GPIO_Pin_7);
      delay_us(5);        
    }
        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//输入上拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    return dat;           
}

int Single_Write_GY30(uchar REG_Address)
{
	int state=0;
  GY30_Start();                                  //起使信号
	if(GY30_SendByte(SlaveAddress))								//发送设备地址+写信号
  {
		if(GY30_SendByte(REG_Address))            //内部寄存器地址
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
  if(Single_Write_GY30(0x01))//0x01为供电指令
	{
		state=1;
	}
	return state;
}

/*
2C总线上传送的数据信号是广义的，既包括地址信号，又包括真正的数据信号。
在起始信号后必须传送一个从机的地址（7位），
第8位是数据的传送方向位（R/T），用“0”表示主机发送数据（T），“1”表示主机接收数据（R）
GY30_SendByte(SlaveAddress+1);+1的含义是从从机接收信息
*/
void mread(void)
{
		uchar i;
    GY30_Start();
	GY30_SendByte(SlaveAddress+1);                                //发送设备地址+读信号 SlaveAddress+1为从GY30读取信息(命令GY30发送信息)(?)
    for(i=0; i<3;i++)																					 //只发送SlaveAddress为写指令去GY30(命令GY30等待接收指令)(?)
    {
			BUF[i] = GY30_RecvByte();
			GPIO_ResetBits(GPIOC,GPIO_Pin_3);
      if(i ==3)
      {
				GY30_SendACK(1);                                //最后一个回应NOACK
      }
      else
      {
        GY30_SendACK(0);
      }
    }        
    GY30_Stop();
    delay_mms(5);//Delay_mms(5);
}