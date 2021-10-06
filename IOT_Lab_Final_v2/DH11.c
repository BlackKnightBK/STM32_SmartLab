#include "DH11.h"
#include "delay.h"
//DH11��A7�ӿ�

int DH11INITED=0;

void DHT11_Rst(void)	   
{           
		GPIO_InitTypeDef  GPIO_InitStructure;	//DHT11_IO_OUT(); 	//SET OUTPUT
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//���ģʽ
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA,GPIO_Pin_7);//DHT11_DQ_OUT=0; 	//����DQ
    delay_mms(20);    	//��������18ms
    GPIO_SetBits(GPIOA,GPIO_Pin_7);//DHT11_DQ_OUT=1; 	//DQ=1 
	delay_us(30);     	//��������20~40us
}

//�ȴ�DHT11�Ļ�Ӧ
//����1:����
//����0:δ��⵽DHT11�Ĵ���
u8 DHT11_Check(void) 	   
{   
	u8 retry=0;
	GPIO_InitTypeDef  GPIO_InitStructure;	//DHT11_IO_OUT(); 	//SET OUTPUT
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING;//����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);//DHT11_IO_IN();//SET INPUT GPIO_Mode_IN_FLOATING;	 
  while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)&&retry<100)//while (DHT11_DQ_IN&&retry<100)//DHT11������40~80us
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=100)
	{
	return 0;
	}
	else
	{
	retry=0;
	}
  while(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)&&retry<100)//DHT11���ͺ���ٴ�����40~80us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)
	{return 0;}	    
	return 1;
}

//��DHT11��ȡһ��λ
//����ֵ��1/0
u8 DHT11_Read_Bit(void) 			 
{
 	u8 retry=0;
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)&&retry<100)//�ȴ���Ϊ�͵�ƽ
	{
		retry++;
		delay_us(1);
	}
	retry=0;
	while(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)&&retry<100)//�ȴ���ߵ�ƽ
	{
		retry++;
		delay_us(1);
	}
	delay_us(40);//�ȴ�40us
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7))
	{
		return 1;
	}
	else
	{
	return 0;
	}		   
}

//��DHT11��ȡһ���ֽ�
//����ֵ������������
u8 DHT11_Read_Byte(void)    
{        
    u8 i,dat;
    dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHT11_Read_Bit();
    }						    
    return dat;
}

//��DHT11��ȡһ������
//temp:�¶�ֵ(��Χ:0~50��)
//humi:ʪ��ֵ(��Χ:20%~90%)
//����ֵ��1,����;0,��ȡʧ��
u8 DHT11_Read_Data(u8 *temp,u8 *humi)    
{        
 	u8 buf[5];
	u8 i;
	DHT11_Rst();
	if(DHT11_Check()==1)
	{
		for(i=0;i<5;i++)//��ȡ40λ����
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];//10*buf[0]+buf[1];
			*temp=buf[2];//10*buf[2]+buf[3];
		}
		delay_s(1);
	}
	else
	{
		return 0;
	}
	return 1;	    
}
//��ʼ��DHT11��IO�� ͬʱ���DHT11�Ĵ���
//����1:������
//����0:����    	 
u8 DHT11_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//���ģʽ
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
		DHT11_Rst();
    return DHT11_Check();
}