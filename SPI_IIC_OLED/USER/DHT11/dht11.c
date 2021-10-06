#include "dht11.h"

#define Dat    PGout(9)
#define Dat_R  PGin(9)

u8 tempT=20;
u8 tempH=40;

u8 tempTUL=60;
u8 tempTLL=10;

u8 tempHUL=99;
u8 tempHLL=10;

void DHT11_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;//���ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//����
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//����
	GPIO_Init(GPIOG,&GPIO_InitStruct);
	
	Dat = 1;//�տ�ʼ PG9�����  DHT11������Ҳ��Ĭ�Ͽ��иߵ�ƽ
}

void DHT11_SelectMode(GPIOMode_TypeDef Mode)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode  = Mode;//���ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//����
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//����
	GPIO_Init(GPIOG,&GPIO_InitStruct);
}


/*
DHT11����Э�����ʵ��
*/

//1�������ź�
void DHT11_Start(void)
{
	//��������PG9����Ϊ���ģʽ
	DHT11_SelectMode(GPIO_Mode_OUT);
	
	Dat = 0;
	delay_ms(20);//��������18ms
	Dat = 1;
	
}


//2��DHT11�ӻ�Ӧ�𲿷�
int DHT11_Resp(void)
{
	int i=0;
	
	//��32��������λ����ģʽ
	DHT11_SelectMode(GPIO_Mode_IN);
	
	//�ȴ�Ӧ���ź�
	while(Dat_R==1)
	{
		//��ʱ����
		delay_us(1);
		i++;
		if(i>1000000)
			return -1;//��ʱʱ������ԼΪ10ms  ���10ms��ȥ DHT11��δ��Ӧ  �㳬ʱ ֱ�ӽ�������  
	}
	
	//�ȴ�DHT11�� Dat������80+us��ȥ----�ȴ��͵�ƽ����
	while(Dat_R==0);
	
	//�ȴ��ߵ�ƽ����
	while(Dat_R==1);
	
	return 0;//�����˳�  DHT11��Ӧ��
}

//3��DHT11�����ݴ���--��һ���ֽ�8BIt���� ---- 40bit=8+8+8+8+8  ��λ�ȴ�
u8 DHT11_ReadByte(void)
{
	u8 TmpData=0;//����һ��������ŵ�ǰ�����ֽڵ�����
	int  i ;
	
	//�������л�Ϊ����ģʽ
	DHT11_SelectMode(GPIO_Mode_IN);
	
	for(i=0;i<8;i++)
	{
		//�ȴ�54us�ĵ͵�ƽ��ȥ
		while(Dat_R == 0);
		
		delay_us(35);//ֱ���ڱ�Ϊ�ߵ�ƽ��˲����ʱ35us
		if(Dat_R == 1)
		{
			//��ǰ�յ���������λ 1
			TmpData |= 0x01<<(7-i);
			while(Dat_R == 1);//��Ϊ������ʱֻ��35us  �������1 ��ô�������е���  Dat���Ǹߵ�ƽ
		}
		
	}
	
	//ѭ�������� ������Dat���Ǳ�DHT11���͵�
	return TmpData;

}



//4��DHT11��ʪ�ȶ�ȡ����ʱ�Ĵ���
void DHT11_End(void)
{
	//��������Ϊ���ģʽ
	DHT11_SelectMode(GPIO_Mode_OUT);
	
	Dat = 1;
	
	delay_ms(10);//���ߺ� ֹͣ����һ��ʱ��
}





//������ʪ�������ĸ�������д�� �������������������ȡ
int DHT11_GetTR(u8 *TRbuf)
{
	DHT11_Start();
	if(0 != DHT11_Resp())
	{
		//��ʾ��ʱ
		return -1;
	}
	TRbuf[0] = DHT11_ReadByte();//ʪ������
	TRbuf[1] = DHT11_ReadByte();//ʪ��С�� Ĭ��Ϊ0
	TRbuf[2] = DHT11_ReadByte();//�¶�����
	TRbuf[3] = DHT11_ReadByte();//�¶�С��  С��ֵ*0.1 
	TRbuf[4] = DHT11_ReadByte();//У���
	
	DHT11_End();
	
	//������У��
	if((u8)(TRbuf[0] + TRbuf[1] + TRbuf[2] + TRbuf[3]) == TRbuf[4])
	{
		//������ȷ
		return 0;//��ʾ���ݿ���
	}
	else
	{
		return -2;//У��ʧ��  �������� ������ֵ������
	}

}





















