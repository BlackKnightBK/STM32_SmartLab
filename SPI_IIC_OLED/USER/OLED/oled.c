#include "oled.h"//��ǰģ�������ͷ�ļ�
#include "oledfont.h" //�����ͷ�ļ� 	 

//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   





#if  OLED_MODE == 0  
/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
	MyIIC_Start();
	MyIIC_WriteByte(0x78);            //Slave address,SA0=0
	MyIIC_ReadACK();

	MyIIC_WriteByte(0x00);			//�����ֽ�
	MyIIC_ReadACK();

	MyIIC_WriteByte(IIC_Command); //�����ֽ� �����ֽ�
	MyIIC_ReadACK();	
	MyIIC_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
	MyIIC_Start();
	MyIIC_WriteByte(0x78);            //Slave address,SA0=0
	MyIIC_ReadACK();

	MyIIC_WriteByte(0x40);			//�����ֽ�--д����
	MyIIC_ReadACK();

	MyIIC_WriteByte(IIC_Data); //�����ֽ� �����ֽ�
	MyIIC_ReadACK();	
	MyIIC_Stop();
}

//������Ĳ���2cmdΪ1----д����   cmd==0  дָ�� ----- IIC 4��ͨ��
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
	{

		Write_IIC_Data(dat);

	}
	else 
	{
		Write_IIC_Command(dat);	
	}

}

#else 

//������Ĳ���2cmdΪ1----д����   cmd==0  дָ�� ---- SPIͨ��
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
		DC = 1;//����ģʽ
	else
		DC = 0;//ָ��ģʽ
	
	
	
	MySPI_WriteByte(dat);

	
	//DC = 1;
}
#endif






///////////////////////OLED����һЩ���ú���ʾ����///////////////////////////////////////
/********************************************
// fill_Picture
********************************************/
void fill_picture(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		OLED_WR_Byte(0xb0+m,OLED_CMD);		//page0-page1
		OLED_WR_Byte(0x00,  OLED_CMD);		//low column start address
		OLED_WR_Byte(0x10,  OLED_CMD);		//high column start address
		for(n=0;n<128;n++)
		{
			OLED_WR_Byte(fill_Data,OLED_DATA);
		}
	}
}


/***********************����ʱ****************************************/
void Delay_50ms(unsigned int Del_50ms)
{
	unsigned int m;
	for(;Del_50ms>0;Del_50ms--)
		for(m=6245;m>0;m--);
}

void Delay_1ms(unsigned int Del_1ms)
{
	unsigned char j;
	while(Del_1ms--)
	{	
		for(j=0;j<123;j++);
	}
}





//OLED��������
//800*480
//128*64   x:0~127   һ��������8�����ؿ�� y:0~7
//OLED_Set_Pos(23,0) 0001 0111 & 0000 1111 == 0000 0111 == 0x07
//     				 0001 0111 & 1111 0000 == 0001 0000 >>4 == 0000 0001 | 0001 0000 = 0001 0001 = 0x11
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	
	OLED_WR_Byte(0xb0+y,OLED_CMD);//0xB0---��0�Ŵ���  0xB7----��7�Ŵ���
	
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);//����������еĸߵ�ַ
	OLED_WR_Byte((x&0x0f),OLED_CMD); //�����еĵ͵�ַ
}   

//����OLED��ʾ    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //������ر�OLED��Դ��
	OLED_WR_Byte(0X14,OLED_CMD);  //0x14 ����   0x10 �ر�
	
	OLED_WR_Byte(0XAF,OLED_CMD);  //��������ģʽ��AE--���� ����ʾ��
}

//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	
	
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}	



//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)
			OLED_WR_Byte(0,OLED_DATA); 
	} //������ʾ
}

//ȫ����
void OLED_On(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)
			OLED_WR_Byte(1,OLED_DATA); 
	} //������ʾ
}



//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~7
//chr:����ʾ���ַ�
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
	unsigned char c=0,i=0;	
	c=chr-' ';//�õ�ƫ�ƺ��ֵ  �ո�ASCII---32			
	if(x>Max_Column-1)
	{
		x=0;
		y=y+2;
	}
	
	//�ж�Ҫ��ʾ�������С
	if(Char_Size ==16)
	{
		OLED_Set_Pos(x,y);	
		for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
		
		
		OLED_Set_Pos(x,y+1);
		
		for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
	}
	else  
	{	
		OLED_Set_Pos(x,y);
		for(i=0;i<6;i++)
		OLED_WR_Byte(F6x8[c][i],OLED_DATA);
			
	}
		
}
//m^n����
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 
//��ʾһ���ַ��Ŵ�
void OLED_ShowString(u8 x,u8 y,char *chr,u8 Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
//��ʾ����
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}

void OLED_ShowMyCN(u8 x,u8 y,u8 no)
{
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(myFont[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(myFont[2*no+1][t],OLED_DATA);
				adder+=1;
      }	
}

/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 

void OLED_DisplayDates()
{
	u8 readHour=0;
	u8 readMin=0;
	u8 readSec=0;
	u8 readSyear=0;
	u16 readYear=0;
	u8 readMon=0;
	u8 readDay=0;
	u8 weeks=0;
	char tempStrH[10];
	char tempStrM[10];
	char tempStrS[10];
	readTime(&readHour,&readMin,&readSec);
	readDate(&readSyear,&readMon,&readDay,&weeks);
	readYear=2000+readSyear;
	weeks=weeks%7;
	////////////��////////////
		OLED_ShowNum(0,0,readYear,4,16);//2021

	OLED_ShowMyCN(32,0,10);//��
	////////////��////////////
		OLED_ShowNum(48,0,readMon,2,16);//6

	OLED_ShowMyCN(64,0,11);//��
	////////////��////////////
		OLED_ShowNum(80,0,readDay,2,16);//8

	OLED_ShowMyCN(96,0,0);//��
		
	
	OLED_ShowMyCN(0,2,7);//��
	OLED_ShowMyCN(16,2,8);//��
	OLED_ShowMyCN(32,2,weeks);
	
	////////////ʱ////////////
		sprintf(tempStrH,"%02d",readHour);
		OLED_ShowString(0,4,tempStrH,16);

	OLED_ShowChar(16,4,':',16);//:
	////////////��////////////
		sprintf(tempStrM,"%02d",readMin);
		OLED_ShowString(24,4,tempStrM,16);

	OLED_ShowChar(40,4,':',16);//:
	////////////��////////////
		sprintf(tempStrS,"%02d",readSec);
		OLED_ShowString(48,4,tempStrS,16);

}

void OLED_DisplayDH11(u8 temp,u8 humi)
{
	OLED_ShowMyCN(0,0,12);//��
	OLED_ShowMyCN(16,0,14);//��
	OLED_ShowChar(32,0,':',16);
	OLED_ShowNum(40,0,temp,3,16);
	OLED_ShowMyCN(0,2,13);//ʪ
	OLED_ShowMyCN(16,2,14);//��
	OLED_ShowChar(32,2,':',16);
	OLED_ShowNum(40,2,humi,3,16);
	
	OLED_ShowMyCN(0,6,17);//����
	OLED_ShowMyCN(16,6,18);//����
	OLED_ShowString(32,6," ",16);
	if(AlarmMode==0)
	{
		OLED_ShowString(40,6,"   ",16);
		OLED_ShowString(40,6,"OFF",16);
	}
	else
	{
		OLED_ShowString(40,6,"   ",16);
		OLED_ShowString(40,6,"ON",16);
	}
}

void OLED_DisplayLight(u16 lightData)
{
	u8 displayTemp;
	u8 posTemp=0;
	OLED_ShowMyCN(0,0,15);//��
	OLED_ShowMyCN(16,0,16);//ǿ
	OLED_ShowChar(32,0,':',16);
	OLED_ShowNum(40,0,4095-lightData,5,16);
	displayTemp=8-(lightData/500);
	if(displayTemp<1)
	{
		displayTemp=1;
	}
	OLED_ShowString(0,2,"                ",16);
	//displayTemp+=1;
	while(displayTemp>0)
	{
		OLED_ShowMyCN(posTemp,2,20);
		displayTemp--;
		posTemp+=16;
	}
}

void OLED_DisPlayChangeDates(u16 readYear,u8 readMon,u8 readDay,u8 weeks,u8 readHour,u8 readMin,u8 readSec,u8 controls)
{
	char tempStrH[10];
	char tempStrM[10];
	char tempStrS[10];
	if(controls!=1)
	{
		OLED_ShowNum(0,0,readYear,4,16);//2021
	}
	else
	{
		OLED_ShowString(0,0,"    ",16);
	}
	OLED_ShowMyCN(32,0,10);//��
	////////////��////////////
	if(controls!=2)
	{
		OLED_ShowNum(48,0,readMon,2,16);//6
	}
	else
	{
		OLED_ShowString(48,0,"  ",16);
	}
	OLED_ShowMyCN(64,0,11);//��
	////////////��////////////
	if(controls!=3)
	{
		OLED_ShowNum(80,0,readDay,2,16);//8
	}
	else
	{
		OLED_ShowString(80,0,"  ",16);
	}
	OLED_ShowMyCN(96,0,0);//��
		
	
	OLED_ShowMyCN(0,2,7);//��
	OLED_ShowMyCN(16,2,8);//��
	OLED_ShowMyCN(32,2,weeks);
	
	////////////ʱ////////////
	if(controls!=4)
	{
		sprintf(tempStrH,"%02d",readHour);
		OLED_ShowString(0,4,tempStrH,16);
	}
	else
	{
		OLED_ShowString(0,4,"  ",16);
	}
	OLED_ShowChar(16,4,':',16);//:
	////////////��////////////
	if(controls!=5)
	{
		sprintf(tempStrM,"%02d",readMin);
		OLED_ShowString(24,4,tempStrM,16);
	}
	else
	{
		OLED_ShowString(24,4,"  ",16);
	}
	OLED_ShowChar(40,4,':',16);//:
	////////////��////////////
	if(controls!=6)
	{
		sprintf(tempStrS,"%02d",readSec);
		OLED_ShowString(48,4,tempStrS,16);
	}
	else
	{
		OLED_ShowString(48,4,"  ",16);
	}
}

void OLED_DisPlayChangeDHT(u8 tempTUL,u8 tempTLL,u8 tempHUL,u8 tempHLL,u8 controls)
{
	if(controls!=0)
	{
		OLED_ShowMyCN(0,0,12);//��
		OLED_ShowMyCN(16,0,14);//��
		OLED_ShowMyCN(32,0,17);//��
		OLED_ShowMyCN(48,0,18);//��
		OLED_ShowChar(64,0,':',16);
	}
	OLED_ShowString(0,2,"UL:",16);
	if(controls!=1)
		{
			OLED_ShowNum(24,2,tempTUL,3,16);
		}
		else
		{
			OLED_ShowString(24,2,"   ",16);
		}
		
		OLED_ShowString(48,2,"LL:",16);
		if(controls!=2)
		{
			OLED_ShowNum(72,2,tempTLL,3,16);
		}
		else
		{
			OLED_ShowString(72,2,"   ",16);
		}
		
	OLED_ShowMyCN(0,4,13);//ʪ
	OLED_ShowMyCN(16,4,14);//��
	OLED_ShowMyCN(32,4,17);//��
	OLED_ShowMyCN(48,4,18);//��
	OLED_ShowChar(64,4,':',16);
		
		OLED_ShowString(0,6,"UL:",16);
		if(controls!=3)
		{
			OLED_ShowNum(24,6,tempHUL,3,16);
		}
		else
		{
			OLED_ShowString(24,6,"   ",16);
		}
	
		OLED_ShowString(48,6,"LL:",16);
		if(controls!=4)
		{
			OLED_ShowNum(72,6,tempHLL,3,16);
		}
		else
		{
			OLED_ShowString(72,6,"   ",16);
		}
		
		if(controls==5)
		{
			OLED_ShowString(0,0,"error",16);
		}
}

//��ʼ��SSD1306					    
void OLED_Init(void)
{ 	
	GPIO_InitTypeDef GPIO_InitStruct; 

#if OLED_MODE == 0
	MyIIC_Init();//OLED�õ���IIC���ŵĳ�ʼ��
#else
	MySPI_Init();//OLED�õ���SPI������ŵĳ�ʼ��
	
	//��ʼ��7����OLED�е�RES��λ�ź�  DC���ƽ�
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_8;//RES DC  
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;//���ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//����
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;//�������
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	
	
	//��λOLED��������оƬ----ֻ��7���ŵ�SPI������Ҫ������λ
	RES = 1;
	delay_ms(100);
	RES = 0;
	delay_ms(200);
	RES = 1;

#endif	
	
	delay_ms(500);

	OLED_WR_Byte(0xAE,OLED_CMD);//�ȹص���Ļ ������������״̬ �ٽ�����������
	
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
	
	
	OLED_WR_Byte(0x81,OLED_CMD); // contract control
	OLED_WR_Byte(0xFF,OLED_CMD);//--255



	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
	OLED_WR_Byte(0x00,OLED_CMD);//
	
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
	OLED_WR_Byte(0x80,OLED_CMD);//
	
	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	OLED_WR_Byte(0x05,OLED_CMD);//
	
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);//
	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);//
	
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x30,OLED_CMD);//
	
	OLED_WR_Byte(0x8D,OLED_CMD);//������Դ��
	OLED_WR_Byte(0x14,OLED_CMD);//
	
	
	
	OLED_WR_Byte(0xAF,OLED_CMD);//--��������ģʽ ��ʾ
}  





























