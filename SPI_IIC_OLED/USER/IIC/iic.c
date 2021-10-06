#include "iic.h"



//�����ʼ��IIC����  ������������ͷ����
void MyIIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;//���ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//����
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//����
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	SCL = 1;
	SDA_W = 1;//����״̬  SDAһֱΪ�ߵ�ƽ
}


//IICͨ�Ź�����  SDAֻ����SCL�ĵ͵�ƽ�ڼ�仯-------MyIIC_SelectMode()
//��SDAֻ����SCL�ĵ͵�ƽ�ڼ���У�������
static void MyIIC_SelectMode(GPIOMode_TypeDef Mode)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode  = Mode;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//����
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;//���ڲ�����
	GPIO_Init(GPIOB,&GPIO_InitStruct);
}


////////////////////////////////IICЭ��//////////////////////////////////////
void MyIIC_Start(void)
{
	//��Ϊ��ģ��IIC  ��ʼ����SDA��SCL��Ϊ�ߵ�ƽ
	MyIIC_SelectMode(GPIO_Mode_OUT);
	
	SCL = 1;
	SDA_W = 1;
	delay_us(5);
	
	//��SCL�ĸߵ�ƽ�ڼ� SDA���� �����½���
	SDA_W = 0;
	delay_us(5);//������>=4us�������һ��ͨ������ SCL = 0
	
	SCL = 0;
}

void MyIIC_Stop(void)
{
	//������SDAΪ���ģʽ
	MyIIC_SelectMode(GPIO_Mode_OUT);
	
	SCL = 0;
	SDA_W = 0;
	delay_us(5);

	SCL = 1;
	delay_us(5);
	SDA_W = 1;//��SCL�ߵ�ƽ�ڼ�  ����SDA---ֹͣ�ź�
	
}




//������Ϊ�����߷����ӻ�8λ����
void MyIIC_WriteByte(u8 WByte)
{
	int i;
	//�޸�SDAģʽ���ģʽ
	MyIIC_SelectMode(GPIO_Mode_OUT);
	
	for(i=0;i<8;i++)
	{
		SCL = 0;
			delay_us(2);//��5us�ĵ͵�ƽ�ڼ� ����WByte���λ����� �ı�SDA���͵�ǰ��һλ
			if(WByte & (0x80>>i)) 
				SDA_W = 1;
			else 
				SDA_W = 0;
			delay_us(3);
		
		

		SCL = 1;
			delay_us(5);//�������������� SCL�ߵ�ƽ�ڼ� �Ǹ��ӻ��������� �������ù�
	}
	SCL = 0;//�����ҵ�IICģ��ʱ��Ĵ���  ÿ���ֽ����󶼱�֤SCL Ϊ�͵�ƽ
}



//������ȡһ�� �ӻ����������ֽ�����
u8 MyIIC_ReadByte(void)
{
	u8 TemData = 0 ;
	int i;
	
	MyIIC_SelectMode(GPIO_Mode_IN);
		
	for(i=0;i<8;i++)
	{
		SCL = 0;
			delay_us(5);//���������ǽ�������  ���Բ�����SCL�ĵ͵�ƽ�ڼ�ı�SDA���SDA
		
		
		SCL = 1;
			delay_us(2);
			if(SDA_R == 1)
				TemData |= (0x80>>i);
			delay_us(3);
	}
	SCL = 0;
	
	return TemData;//���ض�ȡ��8Bit����
}



//�ӻ���������Ӧ��   �����Ǹոշ������ݸ��ӻ�
char MyIIC_ReadACK(void)
{
	char ACK;
	MyIIC_SelectMode(GPIO_Mode_IN);
	
	
	SCL = 0;
		delay_us(5);
	
	
	SCL = 1;
		delay_us(2);
		if(SDA_R == 0)
			ACK = 0;//��Ӧ��
		else 
			ACK = 1;//��Ӧ��
		delay_us(3);

	SCL = 0;
	
	return ACK;//���ض�ȡ����Ӧ����
}

//��������������ն���8bit����  �� �����ӻ���Ӧ���ź� 
//ACK:  0  ������Ӧ��   1����Ӧ��
void MyIIC_WriteAck(char ACK)
{
	MyIIC_SelectMode(GPIO_Mode_OUT);
	
	SCL = 0;
		delay_us(2);
		SDA_W = ACK;
		delay_us(3);
	
	SCL = 1;
		delay_us(5);
	
	SCL = 0;
}



