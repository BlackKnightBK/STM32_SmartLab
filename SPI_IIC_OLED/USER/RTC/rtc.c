#include "rtc.h"
#include "key.h"

u8 BCD_encode(u8 data)//十进制转BCD编码 BCD编码，每4位表达一个10进制数字(0~9)，会浪费一些空间
{
	return (((data/10<<4)&0xf0) | ((data%10)&0x0f));
}

u8 BCD_decode(u8 data)
{
	u8 ten=(data>>4)*10;
	u8 ge=(data&0x0f);
	return ten+ge;
}
	
u8 getWeek(u32 year,u8 month,u8 day)
{
	u8 setWeek,c;
	c=(year/100);
	if(month<3)
	{
		month=month+12;
	}
	setWeek=year+(year/4)+(c/4)-(2*c)+(26*(month+1)/10)+day-1;//0-6 1-7
	setWeek=setWeek%7;
	setWeek=((setWeek+6)%7)+1;
	return setWeek;
}


u8 RTC_Init(u8 hour,u8 min,u8 second)
{
	u16 timeout=0;
	u32 temp=0;
	RCC->AHB1ENR|=0x01<<28;//使能电源接口
	PWR->CR|=0x01<<8;//使能 RTC备份寄存器 备份SRAM的访问
	RCC->BDCR|=0x01<<0;//开启LSE
	while(!(RCC->BDCR&(0x01<<1)))//等待LSE时钟就绪
	{
		delay_ms(1);
		if(timeout++>500)
		{
			return -1;
		}
	}
	timeout=0;
	RCC->BDCR&=~(0x03<<8);
	RCC->BDCR|=(0x01<<8);//LSE振荡器 作 RTC时钟源
	RCC->BDCR|=(0x01<<15);//使能RTC时钟
	
	//取消RTC寄存器的写保护
	RTC->WPR=0xCA;
	RTC->WPR=0x53;
	
	RTC->ISR|=0x01<<7;//进入初始化模式
	while(!(RTC->ISR&(0x01<<6)))//等待进入初始化模式
	{
		delay_ms(1);
		if(timeout++>500)
		{
			return -2;
		}
	}
	
	RTC->PRER|=0;
	RTC->PRER|=0xFF;//同步分频系数 127+1分频
	RTC->PRER|=0x7F<<16; //异步分频系数 255+1分频
	
	//初始化时间和日期
	temp|=(BCD_encode(hour)&0x3F)<<16;
	temp|=(BCD_encode(min)&0x7F)<<8;
	temp|=(BCD_encode(second)&0x7F);
	RTC->TR=temp;
	//退出初始化模式
	RTC->ISR&=~(0x01<<7);
	//激活RTC寄存器写保护
	RTC->WPR=0xFF;
	return 1;
}

u8 RTC_SetTime(u8 hour,u8 min,u8 second)
{
	u16 timeout=0;
	u32 temp=0;
	//取消RTC寄存器的写保护
	RTC->WPR=0xCA;
	RTC->WPR=0x53;
	
	RTC->ISR|=0x01<<7;//进入初始化模式
	while(!(RTC->ISR&(0x01<<6)))//等待进入初始化模式
	{
		delay_ms(1);
		if(timeout++>500)
		{
			return 0;
		}
	}
	
	//初始化时间和日期
	temp|=(BCD_encode(hour)&0x3F)<<16;//6位，2位--十位,4位--个位
	temp|=(BCD_encode(min)&0x7F)<<8;
	temp|=(BCD_encode(second)&0x7F);
	RTC->TR=temp;//TR寄存器 存储时间数据
	
	//退出初始化模式
	RTC->ISR&=~(0x01<<7);
	//激活RTC寄存器写保护
	RTC->WPR=0xFF;
	return 1;
}

u8 RTC_SetDate(u16 year,u8 mon,u8 day,u8 week)
{
	u16 timeout=0;
	u32 temp=0;
	u8 syear;
	//取消RTC寄存器的写保护
	RTC->WPR=0xCA;
	RTC->WPR=0x53;
	
	RTC->ISR|=0x01<<7;//进入初始化模式
	while(!(RTC->ISR&(0x01<<6)))//等待进入初始化模式
	{
		delay_ms(1);
		if(timeout++>500)
		{
			return 0;
		}
	}
	
	//初始化时间和日期
	syear=year-2000;
	temp|=(BCD_encode(syear)&0xFF)<<16;
	temp|=(BCD_encode(week)&0x07)<<13;
	temp|=(BCD_encode(mon)&0x1F)<<8;
	temp|=(BCD_encode(day)&0x3F);
	RTC->DR=temp;//DR寄存器 存储日期数据
	delay_ms(100);
	//退出初始化模式
	RTC->ISR&=~(0x01<<7);
	//激活RTC寄存器写保护
	RTC->WPR=0xFF;
	return 1;
}

void readDate(u8* year,u8* mon,u8* day,u8* week)
{
	*year=BCD_decode((RTC->DR>>16)&0xFF);
	*mon=BCD_decode((RTC->DR>>8)&0x1F);
	*day=BCD_decode(RTC->DR&0x3F);
	*week=BCD_decode((RTC->DR>>13)&0x07);
}

void readTime(u8* hour,u8* min,u8* sec)
{
	*hour=BCD_decode((RTC->TR>>16)&0x3f);
	*min=BCD_decode((RTC->TR>>8&0x7f));
	*sec=BCD_decode((RTC->TR&0x7f));
}

void addMonth()
{
	tempMon++;
	tempDay=1;
	if(tempMon>12)
	{
		tempYear++;
		tempMon=1;
	}
}

void decMonth()
{
	tempMon--;
	tempDay=1;
	if(tempMon<1)
	{
		tempMon=12;
	}
}

void addDay()
{
	tempDay++;
	if(tempMon==2)
	{
		if(tempDay>28)
		{
			//addMonth();
			tempDay=1;
		}
	}
	else if(tempMon==1||tempMon==3||tempMon==5||tempMon==7||tempMon==8||tempMon==10||tempMon==12)
	{
		if(tempDay>31)
		{
			//addMonth();
			tempDay=1;
		}
	}
	else
	{
		if(tempDay>30)
		{
			//addMonth();
			tempDay=1;
		}
	}
}

void decDay()
{
	tempDay--;
	if(tempDay<1)
	{
		if(tempMon==2)
		{
			tempDay=28;
		}
		else if(tempMon==1||tempMon==3||tempMon==5||tempMon==7||tempMon==8||tempMon==10||tempMon==12)
		{
			tempDay=31;
		}
		else
		{
			tempDay=30;
		}
	}
}
	
void addHour()
{
	tempHour++;
	if(tempHour>24)
	{
		//addDay();
		tempHour=0;
	}
}

void decHour()
{
	tempHour--;
	if(tempHour<0)
	{
		tempHour=24;
	}
}
	
void addMin()
{
	tempMin++;
	if(tempMin>59)
	{
		//addHour();
		tempMin=0;
	}
}

void decMin()
{
	tempMin--;
	if(tempMin<0)
	{
		tempMin=59;
	}
}
	
void addSec()
{
	tempSec++;
	if(tempMin>59)
	{
		//addMin();
		tempSec=0;
	}
}

void decSec()
{
	tempSec--;
	if(tempSec<0)
	{
		tempSec=59;
	}
}