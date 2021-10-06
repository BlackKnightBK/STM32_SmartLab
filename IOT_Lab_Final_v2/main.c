/********************************************************************
//DMSTM-L型最小系统板板载的8个LED灯测试程序，流水显示
//程序采用ST官方外设固件库形式，版本为V3.50，外部晶振8.000MHZ
//调试环境：Keil MDK V4.60
//作者：www.avrgcc.com
//时间：2013.04.19
********************************************************************/

/********************************************************************
				包含头文件
********************************************************************/
#include "stm32f10x.h"                   //STM32器件寄存器定义头文件，必须包含
#include "user_Config.h"                  //用户配置头文件，用于配置硬件连接资源,位于项目文件夹下
#include "USART.h"
#include "delay.h"
#include "ESP8266.h"
#include "GY30.h"
#include "DH11.h"
#include "SR505.h"
#include "EXTI.h"
#include "TIM.h"
#include "Relay_Control.h"

/************************I2C代码点****************/

#define	  SlaveAddress   0x46 //从地址，用于控制GY-30芯片 ADDR接地未0x46 ADDR接电源为0x3A

unsigned int mcy;
static unsigned int counts=0;
//#define SDA_IN()  
//#define SDA_OUT() 
//#define IIC_SCL PBout(6) //SCL
//#define IIC_SDA PBout(7) //SDA	 
//#define READ_SDA PBin(7)  //输入SDA

/**********************************************************/
//精准延时函数//
//((1+arr )/72M)*(1+psc )=((1+1999)/72M)*(1+35999)=1s
//按照上面的设置，所有函数功能符合函数名


/************************************************************/
/************************************************************/
/************************************************************/
/**********************************************************/

/*********************************************/
/********************************************************************/

/********************************************************************
				普通延时函数
********************************************************************/





void GPIO_Config(void)
{
	  /* 配置LED灯使用的GPIO管脚模式，输出模式，灌电流驱动*/
  	RCC_APB2PeriphClockCmd(RCC_GPIO_LED8, ENABLE); 					//使能LED灯使用的GPIO时钟
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); 
  	GPIO_InitStructure.GPIO_Pin = DS1_PIN|DS2_PIN|DS3_PIN|DS4_PIN|DS5_PIN|DS6_PIN|DS7_PIN|DS8_PIN; //使用PC0~PC7
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				//GPIO工作在输出模式
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIO_LED8, &GPIO_InitStructure);  						//相关的GPIO口初始化
  	GPIO_SetBits(GPIO_LED8,DS1_PIN|DS2_PIN|DS3_PIN|DS4_PIN|DS5_PIN|DS6_PIN|DS7_PIN|DS8_PIN);//关闭所有的LED指示灯
		
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				//GPIO工作在输出模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);  
	GPIO_ResetBits(GPIOC,GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);	
}
/********************************************************************
				系统时钟RCC初始化函数
该配置方法为固件库2.0版本方法，目的是让用户了解配置过程
********************************************************************/
void RCC_Config(void)
{   
  RCC_DeInit();                                //RCC系统复位(用于调试)
  RCC_HSEConfig(RCC_HSE_ON);                   //使能HSE
  HSEStartUpStatus = RCC_WaitForHSEStartUp();  //等待HSE稳定

  if(HSEStartUpStatus == SUCCESS)
  {
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //使能预取缓冲
    FLASH_SetLatency(FLASH_Latency_2);         //Flash 2等待状态 	   
    RCC_HCLKConfig(RCC_SYSCLK_Div1);           // HCLK = SYSCLK       
    RCC_PCLK2Config(RCC_HCLK_Div1);            // PCLK2 = HCLK     
    RCC_PCLK1Config(RCC_HCLK_Div2);            // PCLK1 = HCLK/2     
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);//PLLCLK=8MHz*9=72 MHz，不同的倍数可设置不同的频率
    RCC_PLLCmd(ENABLE);                        //使能PLL
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) //等待PLL稳定
    {
    }
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);  //选择PLL作为系统时钟源
    while(RCC_GetSYSCLKSource() != 0x08)        //等待PLL被用作系统时钟源
    {
    }
  }

}
//////////////////////////////////////////////////////////////






/********************************************************************
                      主函数
********************************************************************/
u8 msgByte=0;
uchar BUF[8];
unsigned int send=0;
int main(void)
{
	unsigned char datas;
	float LightData;
	char LightMsg[20];
	char GY30Data[20];
	char DH11Data[50];
	char SR505Data[20];
	char TotalSendDatas[50];
	char DH11Msg[50];
	int ESPCheck=0;
	u8 DH11_T=0;
	u8 DH11_H=0;
	///////////////
	char testMsg[100];
	///////////////
	Delay_init();//精准延时初始化
	RCC_Config();//系统时钟
	GPIO_Config();//LED引脚初始化
	GY30_IIC_Init();//IIC总线引脚初始化
	TIM2_Config(40-1,36000-1);//10ms TIM定时器2
	TIM3_Config(40-1,36000-1);//10ms TIM定时器3
	USART1_Init(115200);//串口初始化
	USART2_Init(115200);
	EXTI_InitConfig();//外部中断（按钮）初始化
	//Relay_Control_Init();//继电器引脚初始化
	DH11INITED=DHT11_Init();//DH11初始化
	GY30INITED=Init_GY30();//GY30初始化
	SR505_Init();//SR505初始化
	GPIO_SetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
	while(1)
	{
		/////////////ESP8266检测连接部分/////////////////    //按钮0 用于ESP8266在单片机供电前就连上了WIFI的情况，按下会手动检测是否已经连接AP
		if(ESP8266_CONNECTED==0)//检测是否连接
		{
			GPIO_SetBits(GPIOC,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);
			if(ESP8266_MsgNumbers>0)
			{
				ESP8266_CONNECTED=ESP8266_Check_CONNECT_Station();//检测初次连接/再次连接
			}
			GPIO_WriteBit(GPIOC,GPIO_Pin_0,(BitAction)(!GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_0)));
			delay_mms(1000);
			continue;
		}
		GPIO_ResetBits(GPIOC,GPIO_Pin_0);
		if(ESP8266_INITED==0&&ESP8266_CONNECTED==1)//WIFI连接成功后，开始尝试连接TCP服务器
		{
			delay_mms(1000);//1s尝试1次
			if(ESP8266_Init()==0)
			{
				GPIO_WriteBit(GPIOC,GPIO_Pin_1,(BitAction)(!GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_1)));
				continue;
			}
		}
		GPIO_ResetBits(GPIOC,GPIO_Pin_1);
		//////////ESP8266检测连接部分  END/////////////
		
		////////////[DH11部分]//////////////
		if(DH11INITED==0)//DH11初始化失败
		{
			DH11INITED=DHT11_Init();//继续尝试初始化
			//sprintf(DH11Data, "-1|-1");//发送错误信息
			strcpy(DH11Data,"-1|-1");
		}
		else
		{
			GPIO_ResetBits(GPIOC,GPIO_Pin_4);
			if(!DHT11_Read_Data(&DH11_T,&DH11_H))
			{
				DH11_T=(0);
				DH11_H=(0);
			}
			sprintf(DH11Data, "%d|%d",DH11_T,DH11_H);
			DH11_T=0;
			DH11_H=0;
		}
		////////[DH11部分  END]///////////
		
		//////////[GY30部分]///////////////
		if(GY30INITED==0)//GY30初始化失败
		{
			GY30INITED=Init_GY30();//继续尝试初始化
			//sprintf(GY30Data, "-1");//发送错误信息
			strcpy(GY30Data,"-1");
		}
		else
		{
			GPIO_ResetBits(GPIOC,GPIO_Pin_5);
			Single_Write_GY30(0x10);//连续H模式检测
			mread();//连续读取到BUF中
			datas=BUF[0];//16位的datas，先读取8位
			datas=(datas<<8)+BUF[1];//合成数据
			LightData=(float)datas/1.2;//光照数据公式，来自GY30说明书
			sprintf(GY30Data, "%.4f",LightData);//float转*char
		}
		//////////[GY30部分  END]///////////	
		
		////////////[SR505部分]/////////////
		sprintf(SR505Data, "%d",GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0));
		////////////////////////////////////
		
		/////////////////////信息合成部分/////////////////////
		sprintf(TotalSendDatas, "#%s|%s|%s!",GY30Data,SR505Data,DH11Data);
		//////////////////////////////////////////////////////
		
		ESP8266_SendMsg(TotalSendDatas);
		GPIO_WriteBit(GPIOC,GPIO_Pin_2,(BitAction)(!GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_2)));
		delay_s(1);//这里的延时函数不能和中断的延时函数一致，否则会导致函数被破坏而无法回到主函数
	}
}




