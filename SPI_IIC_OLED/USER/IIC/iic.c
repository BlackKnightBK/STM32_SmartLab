#include "iic.h"



//这个初始化IIC引脚  放在主函数开头调用
void MyIIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;//输出模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//高速
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//浮空
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	SCL = 1;
	SDA_W = 1;//空闲状态  SDA一直为高电平
}


//IIC通信过程中  SDA只能在SCL的低电平期间变化-------MyIIC_SelectMode()
//改SDA只能在SCL的低电平期间进行！！！！
static void MyIIC_SelectMode(GPIOMode_TypeDef Mode)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode  = Mode;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//高速
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;//接内部上拉
	GPIO_Init(GPIOB,&GPIO_InitStruct);
}


////////////////////////////////IIC协议//////////////////////////////////////
void MyIIC_Start(void)
{
	//因为是模拟IIC  开始建议SDA和SCL都为高电平
	MyIIC_SelectMode(GPIO_Mode_OUT);
	
	SCL = 1;
	SDA_W = 1;
	delay_us(5);
	
	//在SCL的高电平期间 SDA拉低 产生下降沿
	SDA_W = 0;
	delay_us(5);//在拉低>=4us后产生第一个通信脉冲 SCL = 0
	
	SCL = 0;
}

void MyIIC_Stop(void)
{
	//先设置SDA为输出模式
	MyIIC_SelectMode(GPIO_Mode_OUT);
	
	SCL = 0;
	SDA_W = 0;
	delay_us(5);

	SCL = 1;
	delay_us(5);
	SDA_W = 1;//在SCL高电平期间  拉高SDA---停止信号
	
}




//主机作为发送者发给从机8位数据
void MyIIC_WriteByte(u8 WByte)
{
	int i;
	//修改SDA模式输出模式
	MyIIC_SelectMode(GPIO_Mode_OUT);
	
	for(i=0;i<8;i++)
	{
		SCL = 0;
			delay_us(2);//在5us的低电平期间 根据WByte最高位的情况 改变SDA发送当前第一位
			if(WByte & (0x80>>i)) 
				SDA_W = 1;
			else 
				SDA_W = 0;
			delay_us(3);
		
		

		SCL = 1;
			delay_us(5);//在主机发送数据 SCL高电平期间 是给从机接收数据 主机不用管
	}
	SCL = 0;//建议大家的IIC模拟时序的代码  每部分结束后都保证SCL 为低电平
}



//主机读取一个 从机发过来的字节数据
u8 MyIIC_ReadByte(void)
{
	u8 TemData = 0 ;
	int i;
	
	MyIIC_SelectMode(GPIO_Mode_IN);
		
	for(i=0;i<8;i++)
	{
		SCL = 0;
			delay_us(5);//现在主机是接收数据  所以不能在SCL的低电平期间改变SDA或读SDA
		
		
		SCL = 1;
			delay_us(2);
			if(SDA_R == 1)
				TemData |= (0x80>>i);
			delay_us(3);
	}
	SCL = 0;
	
	return TemData;//返回读取的8Bit数据
}



//从机发给主机应答   主机是刚刚发完数据给从机
char MyIIC_ReadACK(void)
{
	char ACK;
	MyIIC_SelectMode(GPIO_Mode_IN);
	
	
	SCL = 0;
		delay_us(5);
	
	
	SCL = 1;
		delay_us(2);
		if(SDA_R == 0)
			ACK = 0;//有应答
		else 
			ACK = 1;//无应答
		delay_us(3);

	SCL = 0;
	
	return ACK;//返回读取到的应答结果
}

//这个函数是主机刚读完8bit数据  后 发给从机的应答信号 
//ACK:  0  发送有应答   1：无应答
void MyIIC_WriteAck(char ACK)
{
	MyIIC_SelectMode(GPIO_Mode_OUT);
	
	SCL = 0;
		delay_us(2);
		SDA_W = ACK;
		delay_us(3);
	
	SCL = 1;
		delay_us(5);
	
	SCL = 0;
}



