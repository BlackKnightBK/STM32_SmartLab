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
	
	// ����ʹ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	USART_DeInit(USART2);  //��λ����2 -> ����û��
	
	// ��ʼ�� ���ڶ�ӦIO��  TX-PA2  RX-PA3
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	// ��ʼ�� ����ģʽ״̬
	USART_InitStructure.USART_BaudRate=My_BaudRate; // ������
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None; // Ӳ��������
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx; // ���� ���� ģʽ��ʹ��
	USART_InitStructure.USART_Parity=USART_Parity_No; // û����żУ��
	USART_InitStructure.USART_StopBits=USART_StopBits_1; // һλֹͣλ
	USART_InitStructure.USART_WordLength=USART_WordLength_8b; // ÿ�η������ݿ��Ϊ8λ
	USART_Init(USART1,&USART_InitStructure);
	
	USART_Cmd(USART1,ENABLE);//ʹ�ܴ���
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//���� �����ж�
	//USART_ITConfig(USART2,USART_IT_TXE,ENABLE);//���� �����ж�
	/*һ����PCͨ��ֻ��Ҫ�����жϣ����ڴ�����յ�ָ��*/
	
	// ��ʼ�� �ж����ȼ�
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

void USART1_IRQHandler(void) // ����1�жϷ����� �����ж� or ȫ���ж�
{
	char res;//�޷���8λ����	
	char msg[50];
	int i;
	if(USART_GetITStatus(USART1,USART_IT_RXNE))//����ж��Ƿ���(?)
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
			memset(USART1_BUF,0,500);//�������в��ᳬ�����棬Ψһ�����ΪESP8266�쳣��������������ջ���
		}
		TIM_SetCounter(TIM2,0);//��������0 PS:��ֵ��δȷ�����˴�Ϊ0���ܼ���ʱ�����10ms
		TIM_Cmd(TIM2,ENABLE);	
	}
}
