#include "stm32f10x.h"
#include "delay.h"
#include "USART.h"

char USART2_BUF[500];
int u2MsgLen;

void USART2_Init(u32 My_BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStrue;
	USART_InitTypeDef USART_InitStrue;
	NVIC_InitTypeDef NVIC_InitStrue;
	
	// 外设使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	USART_DeInit(USART2);  //复位串口2 -> 可以没有
	
	// 初始化 串口对应IO口  TX-PA2  RX-PA3
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStrue);
	
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_3;
  GPIO_Init(GPIOA,&GPIO_InitStrue);
	
	// 初始化 串口模式状态
	USART_InitStrue.USART_BaudRate=My_BaudRate; // 波特率
	USART_InitStrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None; // 硬件流控制
	USART_InitStrue.USART_Mode=USART_Mode_Tx|USART_Mode_Rx; // 发送 接收 模式都使用
	USART_InitStrue.USART_Parity=USART_Parity_No; // 没有奇偶校验
	USART_InitStrue.USART_StopBits=USART_StopBits_1; // 一位停止位
	USART_InitStrue.USART_WordLength=USART_WordLength_8b; // 每次发送数据宽度为8位
	USART_Init(USART2,&USART_InitStrue);
	
	USART_Cmd(USART2,ENABLE);//使能串口
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//开启 接收中断
	//USART_ITConfig(USART2,USART_IT_TXE,ENABLE);//开启 发送中断
	/*一般与PC通信只需要接收中断，用于处理接收的指令*/
	
	// 初始化 中断优先级
	NVIC_InitStrue.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStrue.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStrue);
}

void USART2_SendMessageStr(char *str)
{
	while(*str)
	{
		USART_SendData(USART2,*str++);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	}
}

void USART2_SendByte(char str)
{
	USART_SendData(USART2,str);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}

void USART2_IRQHandler(void) // 串口2中断服务函数 接收中断 or 全局中断
{
	char res;//无符号8位整形
	char msg[50];
		
	if(USART_GetITStatus(USART2,USART_IT_RXNE))//检查中断是否发生(?)
	{
		res= USART_ReceiveData(USART2);
		//接收后会自动清空接收中断位，不接收则会卡在接收中断函数中 USART_ClearFlag(USART2,USART_FLAG_RXNE)清除接收器内容，未实验是否能跳出中断		
		sprintf(msg,"%c",res);
		USART1_SendMessageStr(msg);
		//USART2_SendMessageStr(msg);
		GPIO_WriteBit(GPIOC,GPIO_Pin_7,(BitAction)(!GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_7)));
		//while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);//等待发送完毕
	}
	
	/*if(USART_GetITStatus(USART2,USART_IT_RXNE)) // 中断标志
 {
     res= USART_ReceiveData(USART2);  // 串口2 接收
	   while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);//等待发送完毕
  }*/
}
