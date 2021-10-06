#include "usart.h"
#include <stdio.h>

//�ض���printf�ײ㺯��  fputc()
int fputc(int ch , FILE *f)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==0);
	USART_SendData(USART1,(char)ch);
	
	return ch;
}



void USART1_Init(int Baud)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//1ʹ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

	//2����PA9  PA10 ����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
	//3����ʼ�����ڶ�Ӧ����ΪAFģʽ
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;//���ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//����
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//����
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//4����ʼ������1
	USART_InitStruct.USART_BaudRate = Baud;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//
	USART_InitStruct.USART_StopBits   = USART_StopBits_1;
	USART_InitStruct.USART_Parity     = USART_Parity_No;
	USART_InitStruct.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1,&USART_InitStruct);
	
	
	//
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	
	//NVIC
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;//37 ȫ���жϺ�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd  = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	//5����������1
	USART_Cmd(USART1,ENABLE);

}




//����һ�������ַ�
 void USART_SendStringData(USART_TypeDef* USARTx, char *Data)
{
 
    USART_ClearFlag(USART1,USART_FLAG_TC);
 
	/* Transmit Data */
	while(*Data!='\0')
	{
       
        USARTx->DR = ((*Data++) & (uint16_t)0x01FF);//TDR
		
         // delay_ms(1000);//���������͵�����   �ȸոյ��ַ��������
         while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==0);//�������̫��
         USART_ClearFlag(USART1,USART_FLAG_TC);
         
	}
}


//�������洮��1���ܵ��ַ���----������/����
char USART1_Rbuf[100];
char Recive_Flag=0;//��־λ 0����δ����  1����ǰ��������

//A#'\0'
//B#    B#'\0'#'\0''\0'.....
//printf("%s\r\n")

void USART1_IRQHandler(void)
{
	char Dat=0;//��ʱ��ŵ��ν��յ��ַ�
	static int ReCNT=0;//����һ��������¼��ǰ�����ַ������ļ���ֵ
	
	if(USART_GetITStatus(USART1,USART_IT_RXNE) == 1)
	{
		//ֻ�е�ǰδ����  ���� ����֮���������ݱ�־λ��������ٽ���
		if(Recive_Flag == 0)
		{
			Dat = USART_ReceiveData(USART1);
			if(Dat == '#' || ReCNT == 100  )
			{
				//ֻҪ����������������֮һ ��ʾ���ֽ��ս���  ����������������  Ҳ����һ��
				Recive_Flag = 1;//��ǰ����
				
				//[0] [1]..... [98] [99]
				//               L    H  A  B  '#'  
				if(Dat == '#' && ReCNT != 100)	
					USART1_Rbuf[ReCNT] = '\0';//����δ��������������#  ��#�滻Ϊ'\0'
				
				if(ReCNT == 100)
					USART1_Rbuf[99] = '\0';//����������˳� ��ֱ�ӽ�������������һ��Ԫ����Ϊ'\0'
											//��֤����ʹ��������� ���ַ�����������Խ��
				
				ReCNT = 0;//����ֵҪ����
			
			}
			else
			{
				USART1_Rbuf[ReCNT] = Dat;//�������ٻ���������
				ReCNT++;
			}
		}
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}


//����2�ĳ�ʼ���� �����жϷ������ı�д�ο�������߼�


