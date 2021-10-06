#include "dht11.h"

#define Dat    PGout(9)
#define Dat_R  PGin(9)

u8 tempT=20;
u8 tempH=40;

u8 tempTUL=60;
u8 tempTLL=10;

u8 tempHUL=99;
u8 tempHLL=10;

void DHT11_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;//输出模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//高速
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//浮空
	GPIO_Init(GPIOG,&GPIO_InitStruct);
	
	Dat = 1;//刚开始 PG9是输出  DHT11单总线也是默认空闲高电平
}

void DHT11_SelectMode(GPIOMode_TypeDef Mode)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode  = Mode;//输出模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//高速
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//浮空
	GPIO_Init(GPIOG,&GPIO_InitStruct);
}


/*
DHT11传输协议代码实现
*/

//1、启动信号
void DHT11_Start(void)
{
	//将单总线PG9设置为输出模式
	DHT11_SelectMode(GPIO_Mode_OUT);
	
	Dat = 0;
	delay_ms(20);//拉低至少18ms
	Dat = 1;
	
}


//2、DHT11从机应答部分
int DHT11_Resp(void)
{
	int i=0;
	
	//将32引脚配置位输入模式
	DHT11_SelectMode(GPIO_Mode_IN);
	
	//等待应答信号
	while(Dat_R==1)
	{
		//超时处理
		delay_us(1);
		i++;
		if(i>1000000)
			return -1;//超时时间设置约为10ms  如果10ms过去 DHT11还未响应  算超时 直接结束函数  
	}
	
	//等待DHT11将 Dat线拉低80+us过去----等待低电平结束
	while(Dat_R==0);
	
	//等待高电平结束
	while(Dat_R==1);
	
	return 0;//正常退出  DHT11有应答
}

//3、DHT11的数据传输--读一个字节8BIt函数 ---- 40bit=8+8+8+8+8  高位先传
u8 DHT11_ReadByte(void)
{
	u8 TmpData=0;//定义一个用来存放当前读出字节的容器
	int  i ;
	
	//引脚线切换为输入模式
	DHT11_SelectMode(GPIO_Mode_IN);
	
	for(i=0;i<8;i++)
	{
		//等待54us的低电平过去
		while(Dat_R == 0);
		
		delay_us(35);//直接在变为高电平的瞬间延时35us
		if(Dat_R == 1)
		{
			//当前收到的是数字位 1
			TmpData |= 0x01<<(7-i);
			while(Dat_R == 1);//因为上面延时只是35us  如果数字1 那么程序运行到这  Dat还是高电平
		}
		
	}
	
	//循环结束后 到这里Dat都是被DHT11拉低的
	return TmpData;

}



//4、DHT11温湿度读取结束时的处理
void DHT11_End(void)
{
	//引脚设置为输出模式
	DHT11_SelectMode(GPIO_Mode_OUT);
	
	Dat = 1;
	
	delay_ms(10);//拉高后 停止空闲一段时间
}





//以上温湿度驱动的各个部分写好 下面可以组合完成完整读取
int DHT11_GetTR(u8 *TRbuf)
{
	DHT11_Start();
	if(0 != DHT11_Resp())
	{
		//表示超时
		return -1;
	}
	TRbuf[0] = DHT11_ReadByte();//湿度整数
	TRbuf[1] = DHT11_ReadByte();//湿度小数 默认为0
	TRbuf[2] = DHT11_ReadByte();//温度整数
	TRbuf[3] = DHT11_ReadByte();//温度小数  小数值*0.1 
	TRbuf[4] = DHT11_ReadByte();//校验和
	
	DHT11_End();
	
	//做数据校验
	if((u8)(TRbuf[0] + TRbuf[1] + TRbuf[2] + TRbuf[3]) == TRbuf[4])
	{
		//数据正确
		return 0;//表示数据可用
	}
	else
	{
		return -2;//校验失败  告诉外面 数组内值不可用
	}

}





















