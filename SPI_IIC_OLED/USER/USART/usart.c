#include "usart.h"
#include <stdio.h>

//重定义printf底层函数  fputc()
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
	
	//1使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

	//2、将PA9  PA10 复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
	//3、初始化串口对应引脚为AF模式
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;//输出模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//高速
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//浮空
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//4、初始化串口1
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
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;//37 全局中断号
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd  = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	//5、开启串口1
	USART_Cmd(USART1,ENABLE);

}




//发送一连串的字符
 void USART_SendStringData(USART_TypeDef* USARTx, char *Data)
{
 
    USART_ClearFlag(USART1,USART_FLAG_TC);
 
	/* Transmit Data */
	while(*Data!='\0')
	{
       
        USARTx->DR = ((*Data++) & (uint16_t)0x01FF);//TDR
		
         // delay_ms(1000);//的连续发送的问题   等刚刚的字符发送完成
         while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==0);//解决不能太快
         USART_ClearFlag(USART1,USART_FLAG_TC);
         
	}
}


//用来储存串口1接受的字符串----缓冲区/容器
char USART1_Rbuf[100];
char Recive_Flag=0;//标志位 0：还未收完  1：当前这轮收完

//A#'\0'
//B#    B#'\0'#'\0''\0'.....
//printf("%s\r\n")

void USART1_IRQHandler(void)
{
	char Dat=0;//临时存放单次接收的字符
	static int ReCNT=0;//定义一个用来记录当前接收字符个数的计数值
	
	if(USART_GetITStatus(USART1,USART_IT_RXNE) == 1)
	{
		//只有当前未收完  或是 收完之后用了数据标志位清零才能再进来
		if(Recive_Flag == 0)
		{
			Dat = USART_ReceiveData(USART1);
			if(Dat == '#' || ReCNT == 100  )
			{
				//只要满足以上两个条件之一 表示这轮接收结束  就算再有数据下来  也算下一轮
				Recive_Flag = 1;//当前收完
				
				//[0] [1]..... [98] [99]
				//               L    H  A  B  '#'  
				if(Dat == '#' && ReCNT != 100)	
					USART1_Rbuf[ReCNT] = '\0';//属于未收满，又遇到了#  将#替换为'\0'
				
				if(ReCNT == 100)
					USART1_Rbuf[99] = '\0';//如果是收满退出 则直接将缓冲数组的最后一个元素置为'\0'
											//保证后续使用这个数组 做字符串操作不会越界
				
				ReCNT = 0;//计数值要清零
			
			}
			else
			{
				USART1_Rbuf[ReCNT] = Dat;//逐个存放再缓冲数组中
				ReCNT++;
			}
		}
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}


//串口2的初始化和 接收中断服务函数的编写参考上面的逻辑


