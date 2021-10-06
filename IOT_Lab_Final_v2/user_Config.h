
/*8个LED接口定义*/
#define RCC_GPIO_LED8                   RCC_APB2Periph_GPIOC    //LED使用的GPIO时钟
#define LEDn                            8                       //LED数量为8
#define GPIO_LED8                       GPIOC                   //LED使用的GPIO为PC

#define DS1_PIN                         GPIO_Pin_0             	//DS1使用的GPIO管脚
#define DS2_PIN                         GPIO_Pin_1							//DS2使用的GPIO管脚
#define DS3_PIN                         GPIO_Pin_2  						//DS3使用的GPIO管脚
#define DS4_PIN                         GPIO_Pin_3							//DS4使用的GPIO管脚
#define DS5_PIN                         GPIO_Pin_4             	//DS5使用的GPIO管脚
#define DS6_PIN                         GPIO_Pin_5							//DS6使用的GPIO管脚
#define DS7_PIN                         GPIO_Pin_6  						//DS7使用的GPIO管脚
#define DS8_PIN                         GPIO_Pin_7							//DS8使用的GPIO管脚

/*变量定义区*/
GPIO_InitTypeDef GPIO_InitStructure;     												//定义一个结构体变量GPIO_InitStructure，用于初始化GPIO操作
ErrorStatus HSEStartUpStatus;
u8 count=0;

/*函数声明区*/
void RCC_Configuration(void);
void NVIC_Configuration(void);
void Delay(vu32 nCount);
void LED_Runing(u8 LED_NUM);

void SDA_IN();
void SDA_OUT();
void IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
u8 IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);
void IIC_Send_Byte(u8 txd);
u8 IIC_Read_Byte(void);
