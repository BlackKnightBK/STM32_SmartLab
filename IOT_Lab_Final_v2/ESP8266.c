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

int ESP8266_Check_Ack()//���ڼ��"OK"�����Ѿ����������ӷ���
{
	int i,sta,end;
	char ESPMsg[50];
	int state=0;
	sta=(-1);
	end=(-1);
	delay_mms(500);//��оƬʱ�䴦��ָ��
		for(i=0;i<u1MsgLen-1;i++)
		{
			if(USART1_BUF[i]==0x0A||USART1_BUF[i]==0x0D)//����\r\n�ַ�
			{
				continue;
			}
			if(sta==(-1))
			{
				sta=i;
			}
			if(USART1_BUF[i+1]==0x0D)//��һλΪ\r
			{
				end=i;
				strncpy(ESPMsg,USART1_BUF+sta,end-sta+1);//�����ַ���
				//USART2_SendMessageStr("[");
				//USART2_SendMessageStr(ESPMsg);
				//USART2_SendMessageStr("]\r\n");
				if(strcmp(ESPMsg,"OK")==0 ||strcmp(ESPMsg,"link is builded")==0 ||strcmp(ESPMsg,"ALREADY CONNECTED")==0)//�Աȷ���
				{
					state=1;
				}
				memset(ESPMsg,0,50);//����ݴ���
				sta=(-1);
				end=(-1);
			}
		}
	memset(USART1_BUF,0,500);//��ջ���
	u1MsgLen=0;							//��Ϣ������0
	ESP8266_MsgNumbers--;  //��Ϣ��-1
	return state;
}

int ESP8266_Check_CONNECT_Station()//���ڼ���Ƿ����ӳɹ�
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

int ESP8266_Check_CONNECT_AP()//����Ƿ�������WIFI���ѻ��IP
{
	int i,sta,end;
	char ESPMsg[50];
	sta=(-1);
	end=(-1);
	USART1_SendMessageStr("+++");//�������͸��ģʽ�����˳�
	Delay(0x200000);//��оƬ����ʱ��
	USART1_SendMessageStr("AT+CWJAP?\r\n");
	Delay(0x200000);//��оƬ����ʱ��
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
		/////////////���Դ�����///////////////
		//USART2_SendMessageStr("[");				//���ڲ��Է������ָ��
		//USART2_SendMessageStr(USART1_BUF);
		//USART2_SendMessageStr("]\r\n");
		//////////////////////////////////////
		strcpy(getCommand,USART1_BUF);  //���ƻ��浽�ݴ���
		memset(USART1_BUF,0,500);      //��ջ���
		u1MsgLen=0;
		ESP8266_MsgNumbers--;
	}
}

void ESP8266_HandleCommand(char * getCommand)//���ڴ���ָ��
{
	int i,pos;
	int controls[4];//������4������ָ��
	char testMsg[50];
	i=0;
	pos=0;
	if(strcmp(getCommand,"WIFI DISCONNECT\r\n")==0)//����\r\n����Ϣ��ESP8266���͵ģ����Ͽ�����ʱ�����ó�ʼ��״̬
	{
		ESP8266_CONNECTED=0;
		ESP8266_INITED=0;
		ESP8266_step=0;//���ó�ʼ��״̬
	}
	else
	{
		while(getCommand[i]!='!')//���ݸ�ʽ��#��ͷ����!��β����|���ָ���������"#0|1|1|0!"
		{
			if(getCommand[i+1]=='|'||getCommand[i+1]=='!')
			{
				controls[pos]=getCommand[i]-'0';//char ת int
				pos++;
			}
			i++;
		}
		//////�˴��ſ��ش������/////
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
		USART1_SendMessageStr("+++");//�˳�͸��ģʽ
		if(ESP8266_Check_Ack()==0)
		{
			/*��һ������оƬδ�ϵ絫������ͻȻ�Ͽ������*/
		}
		ESP8266_step=1;
		delay_mms(100);
	}
	
	if(ESP8266_step==1)
	{
		USART1_SendMessageStr("AT+CIPMUX=0\r\n");//�رն�����ģʽ
		if(ESP8266_Check_Ack()==0)
		{
			return 0;
		}
		ESP8266_step=2;
		delay_mms(100);
	}
	
	if(ESP8266_step==2)
	{
		USART1_SendMessageStr("AT+CIPSTART=\"TCP\",\"192.168.137.1\",8080\r\n");//����TCP������
		if(ESP8266_Check_Ack()==0)
		{
			return 0;
		}
		ESP8266_step=3;
		delay_mms(2000);
	}
	
	
	if(ESP8266_step==3)
	{
		USART1_SendMessageStr("AT+CIPMODE=1\r\n");//��͸��ģʽ
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
		USART1_SendMessageStr("AT+CIPSEND\r\n");//����͸��ģʽ
		if(ESP8266_Check_Ack()==0)
		{
			ESP8266_step=2;
			return 0;
		}
		ESP8266_step=5;
		delay_mms(100);
		//TIM_Cmd(TIM3,ENABLE);//(δ����)ȷ�Ͻ���͸��ģʽ�󣬿�ʼ�Զ���������PC�˵�ָ�� ���ѽ�TIM2����ɳ�ʼ�����������ڴ˹���
	}
	
	ESP8266_INITED=1;
	ESP8266_READY=1;//δ����
	return 1;
	//delay_mms(100);
}