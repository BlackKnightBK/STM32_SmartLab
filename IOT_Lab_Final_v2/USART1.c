#include "stm32f10x.h"
#include "delay.h"
#include "USART.h"

char USART1_BUF[500];
int u1MsgLen=0;

void USART1_Init(u32 My_BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	// 外设使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	USART_DeInit(USART2);  //复位串口2 -> 可以没有
	
	// 初始化 串口对应IO口  TX-PA2  RX-PA3
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	// 初始化 串口模式状态
	USART_InitStructure.USART_BaudRate=My_BaudRate; // 波特率
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None; // 硬件流控制
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx; // 发送 接收 模式都使用
	USART_InitStructure.USART_Parity=USART_Parity_No; // 没有奇偶校验
	USART_InitStructure.USART_StopBits=USART_StopBits_1; // 一位停止位
	USART_InitStructure.USART_WordLength=USART_WordLength_8b; // 每次发送数据宽度为8位
	USART_Init(USART1,&USART_InitStructure);
	
	USART_Cmd(USART1,ENABLE);//使能串口
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//开启 接收中断
	//USART_ITConfig(USART2,USART_IT_TXE,ENABLE);//开启 发送中断
	/*一般与PC通信只需要接收中断，用于处理接收的指令*/
	
	// 初始化 中断优先级
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStructure);
}

void USART1_SendMessageStr(char *str)
{
	while(*str)
	{
		USART_SendData(USART1,*str++);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	}
}

void USART1_SendMessageU8(u8 *command)
{
	while(*command)
	{
		USART_SendData(USART1,*command++);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	}
}
void USART1_SendByte(char str)
{
	USART_SendData(USART1,str);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

void USART1_IRQHandler(void) // 串口1中断服务函数 接收中断 or 全局中断
{
	char res;//无符号8位整形	
	char msg[50];
	int i;
	if(USART_GetITStatus(USART1,USART_IT_RXNE))//检查中断是否发生(?)
	{
		
		res= USART_ReceiveData(USART1);
		if(u1MsgLen<500)
		{
			USART1_BUF[u1MsgLen]=res;
			u1MsgLen++;
		}
		if(u1MsgLen==500)
		{
			USART2_SendMessageStr("U1 BUF overflow\r\n");
			u1MsgLen=0;
			memset(USART1_BUF,0,500);//正常运行不会超出缓存，唯一的情况为ESP8266异常重启，故这里清空缓存
		}
		TIM_SetCounter(TIM2,0);//计数器清0 PS:数值尚未确定，此处为0可能计数时间大于10ms
		TIM_Cmd(TIM2,ENABLE);	
	}
}
