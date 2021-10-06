#include "stm32f10x.h"
#include "delay.h"
#include "ESP8266.h"
#include "USART.h"

int ESP8266_CONNECTED=0;
int ESP8266_INITED=0;
int ESP8266_READY=0;
int ESP8266_MsgNumbers=0;
int ESP8266_step=0;
char getCommand[100];

void ESP8266_SendMsg(char *msg)
{
	USART1_SendMessageStr(msg);
}

int ESP8266_Check_Ack()//用于检查"OK"或者已经建立的连接反馈
{
	int i,sta,end;
	char ESPMsg[50];
	int state=0;
	sta=(-1);
	end=(-1);
	delay_mms(500);//给芯片时间处理指令
		for(i=0;i<u1MsgLen-1;i++)
		{
			if(USART1_BUF[i]==0x0A||USART1_BUF[i]==0x0D)//跳过\r\n字符
			{
				continue;
			}
			if(sta==(-1))
			{
				sta=i;
			}
			if(USART1_BUF[i+1]==0x0D)//下一位为\r
			{
				end=i;
				strncpy(ESPMsg,USART1_BUF+sta,end-sta+1);//处理字符串
				//USART2_SendMessageStr("[");
				//USART2_SendMessageStr(ESPMsg);
				//USART2_SendMessageStr("]\r\n");
				if(strcmp(ESPMsg,"OK")==0 ||strcmp(ESPMsg,"link is builded")==0 ||strcmp(ESPMsg,"ALREADY CONNECTED")==0)//对比分析
				{
					state=1;
				}
				memset(ESPMsg,0,50);//清空暂存区
				sta=(-1);
				end=(-1);
			}
		}
	memset(USART1_BUF,0,500);//清空缓存
	u1MsgLen=0;							//信息长度清0
	ESP8266_MsgNumbers--;  //信息数-1
	return state;
}

int ESP8266_Check_CONNECT_Station()//用于检查是否连接成功
{
	int i,sta,end;
	char ESPMsg[50];
	int state=0;
	sta=(-1);
	end=(-1);
		for(i=0;i<u1MsgLen-1;i++)
		{
			if(USART1_BUF[i]==0x0A||USART1_BUF[i]==0x0D)
			{
				continue;
			}
			if(sta==(-1))
			{
				sta=i;
			}
			if(USART1_BUF[i+1]==0x0A)
			{
				end=i;
				strncpy(ESPMsg,USART1_BUF+sta,end-sta+1);
				USART2_SendMessageStr("[");
				USART2_SendMessageStr(ESPMsg);
				USART2_SendMessageStr("]\r\n");
				if(strstr(ESPMsg,"WIFI GOT IP"))
				{
					state=1;
				}
				memset(ESPMsg,0,50);
				sta=(-1);
				end=(-1);
			}
		}
	memset(USART1_BUF,0,500);
	u1MsgLen=0;
	return state;
}

int ESP8266_Check_CONNECT_AP()//检测是否连接上WIFI且已获得IP
{
	int i,sta,end;
	char ESPMsg[50];
	sta=(-1);
	end=(-1);
	USART1_SendMessageStr("+++");//如果正在透传模式，则退出
	Delay(0x200000);//给芯片处理时间
	USART1_SendMessageStr("AT+CWJAP?\r\n");
	Delay(0x200000);//给芯片处理时间
	if(ESP8266_MsgNumbers==0)
	{
		return 0;
	}
		for(i=0;i<u1MsgLen-1;i++)
		{
			if(USART1_BUF[i]==0x0A||USART1_BUF[i]==0x0D)
			{
				continue;
			}
			if(sta==(-1))
			{
				sta=i;
			}
			if(USART1_BUF[i+1]==0x0A)
			{
				end=i;
				strncpy(ESPMsg,USART1_BUF+sta,end-sta+1);
				if(strcmp(ESPMsg,"No AP")==0)
				{
					return 0;
				}
				memset(ESPMsg,0,50);
				sta=(-1);
				end=(-1);
			}
		}
	memset(USART1_BUF,0,500);
	u1MsgLen=0;
	ESP8266_MsgNumbers--;
	return 1;
}

void ESP8266_ReadCommand(char * getCommand)
{
	if(ESP8266_MsgNumbers>0)
	{
		/////////////测试代码区///////////////
		//USART2_SendMessageStr("[");				//用于测试分析后的指令
		//USART2_SendMessageStr(USART1_BUF);
		//USART2_SendMessageStr("]\r\n");
		//////////////////////////////////////
		strcpy(getCommand,USART1_BUF);  //复制缓存到暂存区
		memset(USART1_BUF,0,500);      //清空缓存
		u1MsgLen=0;
		ESP8266_MsgNumbers--;
	}
}

void ESP8266_HandleCommand(char * getCommand)//用于处理指令
{
	int i,pos;
	int controls[4];//最多接收4个开关指令
	char testMsg[50];
	i=0;
	pos=0;
	if(strcmp(getCommand,"WIFI DISCONNECT\r\n")==0)//带有\r\n的信息是ESP8266发送的，当断开网络时，重置初始化状态
	{
		ESP8266_CONNECTED=0;
		ESP8266_INITED=0;
		ESP8266_step=0;//重置初始化状态
	}
	else
	{
		while(getCommand[i]!='!')//数据格式以#开头，以!结尾，用|做分隔符，例如"#0|1|1|0!"
		{
			if(getCommand[i+1]=='|'||getCommand[i+1]=='!')
			{
				controls[pos]=getCommand[i]-'0';//char 转 int
				pos++;
			}
			i++;
		}
		//////此处放开关处理代码/////
		//sprintf(testMsg,"s1:%d s2:%d s3:%d s4:%d\n",controls[0],controls[1],controls[2],controls[3]);
		//USART2_SendMessageStr(testMsg);
		if(controls[0]==0)
		{
			GPIO_ResetBits(GPIOC,GPIO_Pin_9);
		}
		else
		{
			GPIO_SetBits(GPIOC,GPIO_Pin_9);
		}
		if(controls[1]==0)
		{
			GPIO_ResetBits(GPIOC,GPIO_Pin_10);
		}
		else
		{
			GPIO_SetBits(GPIOC,GPIO_Pin_10);
		}
		if(controls[2]==0)
		{
			GPIO_ResetBits(GPIOC,GPIO_Pin_11);
		}
		else
		{
			GPIO_SetBits(GPIOC,GPIO_Pin_11);
		}
		if(controls[3]==0)
		{
			GPIO_ResetBits(GPIOC,GPIO_Pin_12);
		}
		else
		{
			GPIO_SetBits(GPIOC,GPIO_Pin_12);
		}
		/////////////////////////////
	}
}

int ESP8266_Init()
{
	if(ESP8266_step==0)
	{
		USART1_SendMessageStr("+++");//退出透传模式
		if(ESP8266_Check_Ack()==0)
		{
			/*这一步用于芯片未断电但是网络突然断开的情况*/
		}
		ESP8266_step=1;
		delay_mms(100);
	}
	
	if(ESP8266_step==1)
	{
		USART1_SendMessageStr("AT+CIPMUX=0\r\n");//关闭多链接模式
		if(ESP8266_Check_Ack()==0)
		{
			return 0;
		}
		ESP8266_step=2;
		delay_mms(100);
	}
	
	if(ESP8266_step==2)
	{
		USART1_SendMessageStr("AT+CIPSTART=\"TCP\",\"192.168.137.1\",8080\r\n");//连接TCP服务器
		if(ESP8266_Check_Ack()==0)
		{
			return 0;
		}
		ESP8266_step=3;
		delay_mms(2000);
	}
	
	
	if(ESP8266_step==3)
	{
		USART1_SendMessageStr("AT+CIPMODE=1\r\n");//打开透传模式
		if(ESP8266_Check_Ack()==0)
		{
			return 0;
		}
		ESP8266_step=4;
		delay_mms(100);
	}
	ESP8266_step=4;
	delay_mms(100);
	
	if(ESP8266_step==4)
	{
		USART1_SendMessageStr("AT+CIPSEND\r\n");//进入透传模式
		if(ESP8266_Check_Ack()==0)
		{
			ESP8266_step=2;
			return 0;
		}
		ESP8266_step=5;
		delay_mms(100);
		//TIM_Cmd(TIM3,ENABLE);//(未启用)确认进入透传模式后，开始自动分析来自PC端的指令 现已将TIM2在完成初始化工作后用于此功能
	}
	
	ESP8266_INITED=1;
	ESP8266_READY=1;//未启用
	return 1;
	//delay_mms(100);
}