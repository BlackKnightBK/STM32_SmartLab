/********************************************************************
//DMSTM-L����Сϵͳ����ص�8��LED�Ʋ��Գ�����ˮ��ʾ
//�������ST�ٷ�����̼�����ʽ���汾ΪV3.50���ⲿ����8.000MHZ
//���Ի�����Keil MDK V4.60
//���ߣ�www.avrgcc.com
//ʱ�䣺2013.04.19
********************************************************************/

/********************************************************************
				����ͷ�ļ�
********************************************************************/
#include "stm32f10x.h"                   //STM32�����Ĵ�������ͷ�ļ����������
#include "user_Config.h"                  //�û�����ͷ�ļ�����������Ӳ��������Դ,λ����Ŀ�ļ�����
#include "USART.h"
#include "delay.h"
#include "ESP8266.h"
#include "GY30.h"
#include "DH11.h"
#include "SR505.h"
#include "EXTI.h"
#include "TIM.h"
#include "Relay_Control.h"

/************************I2C�����****************/

#define	  SlaveAddress   0x46 //�ӵ�ַ�����ڿ���GY-30оƬ ADDR�ӵ�δ0x46 ADDR�ӵ�ԴΪ0x3A

unsigned int mcy;
static unsigned int counts=0;
//#define SDA_IN()  
//#define SDA_OUT() 
//#define IIC_SCL PBout(6) //SCL
//#define IIC_SDA PBout(7) //SDA	 
//#define READ_SDA PBin(7)  //����SDA

/**********************************************************/
//��׼��ʱ����//
//((1+arr )/72M)*(1+psc )=((1+1999)/72M)*(1+35999)=1s
//������������ã����к������ܷ��Ϻ�����


/************************************************************/
/************************************************************/
/************************************************************/
/**********************************************************/

/*********************************************/
/********************************************************************/

/********************************************************************
				��ͨ��ʱ����
********************************************************************/





void GPIO_Config(void)
{
	  /* ����LED��ʹ�õ�GPIO�ܽ�ģʽ�����ģʽ�����������*/
  	RCC_APB2PeriphClockCmd(RCC_GPIO_LED8, ENABLE); 					//ʹ��LED��ʹ�õ�GPIOʱ��
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); 
  	GPIO_InitStructure.GPIO_Pin = DS1_PIN|DS2_PIN|DS3_PIN|DS4_PIN|DS5_PIN|DS6_PIN|DS7_PIN|DS8_PIN; //ʹ��PC0~PC7
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				//GPIO���������ģʽ
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIO_LED8, &GPIO_InitStructure);  						//��ص�GPIO�ڳ�ʼ��
  	GPIO_SetBits(GPIO_LED8,DS1_PIN|DS2_PIN|DS3_PIN|DS4_PIN|DS5_PIN|DS6_PIN|DS7_PIN|DS8_PIN);//�ر����е�LEDָʾ��
		
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				//GPIO���������ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);  
	GPIO_ResetBits(GPIOC,GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);	
}
/********************************************************************
				ϵͳʱ��RCC��ʼ������
�����÷���Ϊ�̼���2.0�汾������Ŀ�������û��˽����ù���
********************************************************************/
void RCC_Config(void)
{   
  RCC_DeInit();                                //RCCϵͳ��λ(���ڵ���)
  RCC_HSEConfig(RCC_HSE_ON);                   //ʹ��HSE
  HSEStartUpStatus = RCC_WaitForHSEStartUp();  //�ȴ�HSE�ȶ�

  if(HSEStartUpStatus == SUCCESS)
  {
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //ʹ��Ԥȡ����
    FLASH_SetLatency(FLASH_Latency_2);         //Flash 2�ȴ�״̬ 	   
    RCC_HCLKConfig(RCC_SYSCLK_Div1);           // HCLK = SYSCLK       
    RCC_PCLK2Config(RCC_HCLK_Div1);            // PCLK2 = HCLK     
    RCC_PCLK1Config(RCC_HCLK_Div2);            // PCLK1 = HCLK/2     
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);//PLLCLK=8MHz*9=72 MHz����ͬ�ı��������ò�ͬ��Ƶ��
    RCC_PLLCmd(ENABLE);                        //ʹ��PLL
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) //�ȴ�PLL�ȶ�
    {
    }
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);  //ѡ��PLL��Ϊϵͳʱ��Դ
    while(RCC_GetSYSCLKSource() != 0x08)        //�ȴ�PLL������ϵͳʱ��Դ
    {
    }
  }

}
//////////////////////////////////////////////////////////////






/********************************************************************
                      ������
********************************************************************/
u8 msgByte=0;
uchar BUF[8];
unsigned int send=0;
int main(void)
{
	unsigned char datas;
	float LightData;
	char LightMsg[20];
	char GY30Data[20];
	char DH11Data[50];
	char SR505Data[20];
	char TotalSendDatas[50];
	char DH11Msg[50];
	int ESPCheck=0;
	u8 DH11_T=0;
	u8 DH11_H=0;
	///////////////
	char testMsg[100];
	///////////////
	Delay_init();//��׼��ʱ��ʼ��
	RCC_Config();//ϵͳʱ��
	GPIO_Config();//LED���ų�ʼ��
	GY30_IIC_Init();//IIC�������ų�ʼ��
	TIM2_Config(40-1,36000-1);//10ms TIM��ʱ��2
	TIM3_Config(40-1,36000-1);//10ms TIM��ʱ��3
	USART1_Init(115200);//���ڳ�ʼ��
	USART2_Init(115200);
	EXTI_InitConfig();//�ⲿ�жϣ���ť����ʼ��
	//Relay_Control_Init();//�̵������ų�ʼ��
	DH11INITED=DHT11_Init();//DH11��ʼ��
	GY30INITED=Init_GY30();//GY30��ʼ��
	SR505_Init();//SR505��ʼ��
	GPIO_SetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
	while(1)
	{
		/////////////ESP8266������Ӳ���/////////////////    //��ť0 ����ESP8266�ڵ�Ƭ������ǰ��������WIFI����������»��ֶ�����Ƿ��Ѿ�����AP
		if(ESP8266_CONNECTED==0)//����Ƿ�����
		{
			GPIO_SetBits(GPIOC,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);
			if(ESP8266_MsgNumbers>0)
			{
				ESP8266_CONNECTED=ESP8266_Check_CONNECT_Station();//����������/�ٴ�����
			}
			GPIO_WriteBit(GPIOC,GPIO_Pin_0,(BitAction)(!GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_0)));
			delay_mms(1000);
			continue;
		}
		GPIO_ResetBits(GPIOC,GPIO_Pin_0);
		if(ESP8266_INITED==0&&ESP8266_CONNECTED==1)//WIFI���ӳɹ��󣬿�ʼ��������TCP������
		{
			delay_mms(1000);//1s����1��
			if(ESP8266_Init()==0)
			{
				GPIO_WriteBit(GPIOC,GPIO_Pin_1,(BitAction)(!GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_1)));
				continue;
			}
		}
		GPIO_ResetBits(GPIOC,GPIO_Pin_1);
		//////////ESP8266������Ӳ���  END/////////////
		
		////////////[DH11����]//////////////
		if(DH11INITED==0)//DH11��ʼ��ʧ��
		{
			DH11INITED=DHT11_Init();//�������Գ�ʼ��
			//sprintf(DH11Data, "-1|-1");//���ʹ�����Ϣ
			strcpy(DH11Data,"-1|-1");
		}
		else
		{
			GPIO_ResetBits(GPIOC,GPIO_Pin_4);
			if(!DHT11_Read_Data(&DH11_T,&DH11_H))
			{
				DH11_T=(0);
				DH11_H=(0);
			}
			sprintf(DH11Data, "%d|%d",DH11_T,DH11_H);
			DH11_T=0;
			DH11_H=0;
		}
		////////[DH11����  END]///////////
		
		//////////[GY30����]///////////////
		if(GY30INITED==0)//GY30��ʼ��ʧ��
		{
			GY30INITED=Init_GY30();//�������Գ�ʼ��
			//sprintf(GY30Data, "-1");//���ʹ�����Ϣ
			strcpy(GY30Data,"-1");
		}
		else
		{
			GPIO_ResetBits(GPIOC,GPIO_Pin_5);
			Single_Write_GY30(0x10);//����Hģʽ���
			mread();//������ȡ��BUF��
			datas=BUF[0];//16λ��datas���ȶ�ȡ8λ
			datas=(datas<<8)+BUF[1];//�ϳ�����
			LightData=(float)datas/1.2;//�������ݹ�ʽ������GY30˵����
			sprintf(GY30Data, "%.4f",LightData);//floatת*char
		}
		//////////[GY30����  END]///////////	
		
		////////////[SR505����]/////////////
		sprintf(SR505Data, "%d",GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0));
		////////////////////////////////////
		
		/////////////////////��Ϣ�ϳɲ���/////////////////////
		sprintf(TotalSendDatas, "#%s|%s|%s!",GY30Data,SR505Data,DH11Data);
		//////////////////////////////////////////////////////
		
		ESP8266_SendMsg(TotalSendDatas);
		GPIO_WriteBit(GPIOC,GPIO_Pin_2,(BitAction)(!GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_2)));
		delay_s(1);//�������ʱ�������ܺ��жϵ���ʱ����һ�£�����ᵼ�º������ƻ����޷��ص�������
	}
}




