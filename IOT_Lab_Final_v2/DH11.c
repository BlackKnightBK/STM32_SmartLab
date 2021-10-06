#include "DH11.h"
#include "delay.h"
//DH11接A7接口

int DH11INITED=0;

void DHT11_Rst(void)	   
{           
		GPIO_InitTypeDef  GPIO_InitStructure;	//DHT11_IO_OUT(); 	//SET OUTPUT
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//输出模式
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA,GPIO_Pin_7);//DHT11_DQ_OUT=0; 	//拉低DQ
    delay_mms(20);    	//拉低至少18ms
    GPIO_SetBits(GPIOA,GPIO_Pin_7);//DHT11_DQ_OUT=1; 	//DQ=1 
	delay_us(30);     	//主机拉高20~40us
}

//等待DHT11的回应
//返回1:存在
//返回0:未检测到DHT11的存在
u8 DHT11_Check(void) 	   
{   
	u8 retry=0;
	GPIO_InitTypeDef  GPIO_InitStructure;	//DHT11_IO_OUT(); 	//SET OUTPUT
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING;//输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);//DHT11_IO_IN();//SET INPUT GPIO_Mode_IN_FLOATING;	 
  while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)&&retry<100)//while (DHT11_DQ_IN&&retry<100)//DHT11会拉低40~80us
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=100)
	{
	return 0;
	}
	else
	{
	retry=0;
	}
  while(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)&&retry<100)//DHT11拉低后会再次拉高40~80us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)
	{return 0;}	    
	return 1;
}

//从DHT11读取一个位
//返回值：1/0
u8 DHT11_Read_Bit(void) 			 
{
 	u8 retry=0;
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)&&retry<100)//等待变为低电平
	{
		retry++;
		delay_us(1);
	}
	retry=0;
	while(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)&&retry<100)//等待变高电平
	{
		retry++;
		delay_us(1);
	}
	delay_us(40);//等待40us
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7))
	{
		return 1;
	}
	else
	{
	return 0;
	}		   
}

//从DHT11读取一个字节
//返回值：读到的数据
u8 DHT11_Read_Byte(void)    
{        
    u8 i,dat;
    dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHT11_Read_Bit();
    }						    
    return dat;
}

//从DHT11读取一次数据
//temp:温度值(范围:0~50°)
//humi:湿度值(范围:20%~90%)
//返回值：1,正常;0,读取失败
u8 DHT11_Read_Data(u8 *temp,u8 *humi)    
{        
 	u8 buf[5];
	u8 i;
	DHT11_Rst();
	if(DHT11_Check()==1)
	{
		for(i=0;i<5;i++)//读取40位数据
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];//10*buf[0]+buf[1];
			*temp=buf[2];//10*buf[2]+buf[3];
		}
		delay_s(1);
	}
	else
	{
		return 0;
	}
	return 1;	    
}
//初始化DHT11的IO口 同时检测DHT11的存在
//返回1:不存在
//返回0:存在    	 
u8 DHT11_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//输出模式
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
		DHT11_Rst();
    return DHT11_Check();
}