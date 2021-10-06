#include "oled.h"//当前模块代码主头文件
#include "oledfont.h" //字体的头文件 	 

//OLED的显存
//存放格式如下.
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

	MyIIC_WriteByte(0x00);			//控制字节
	MyIIC_ReadACK();

	MyIIC_WriteByte(IIC_Command); //数据字节 命令字节
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

	MyIIC_WriteByte(0x40);			//控制字节--写数据
	MyIIC_ReadACK();

	MyIIC_WriteByte(IIC_Data); //数据字节 命令字节
	MyIIC_ReadACK();	
	MyIIC_Stop();
}

//当传入的参数2cmd为1----写数据   cmd==0  写指令 ----- IIC 4脚通信
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

//当传入的参数2cmd为1----写数据   cmd==0  写指令 ---- SPI通信
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
		DC = 1;//数据模式
	else
		DC = 0;//指令模式
	
	
	
	MySPI_WriteByte(dat);

	
	//DC = 1;
}
#endif






///////////////////////OLED屏的一些配置和显示操作///////////////////////////////////////
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


/***********************粗延时****************************************/
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





//OLED坐标设置
//800*480
//128*64   x:0~127   一个大行有8个像素宽度 y:0~7
//OLED_Set_Pos(23,0) 0001 0111 & 0000 1111 == 0000 0111 == 0x07
//     				 0001 0111 & 1111 0000 == 0001 0000 >>4 == 0000 0001 | 0001 0000 = 0001 0001 = 0x11
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	
	OLED_WR_Byte(0xb0+y,OLED_CMD);//0xB0---第0号大行  0xB7----第7号大行
	
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);//这个是设置列的高地址
	OLED_WR_Byte((x&0x0f),OLED_CMD); //设置列的低地址
}   

//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //开启或关闭OLED电源泵
	OLED_WR_Byte(0X14,OLED_CMD);  //0x14 开启   0x10 关闭
	
	OLED_WR_Byte(0XAF,OLED_CMD);  //进入正常模式（AE--休眠 关显示）
}

//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	
	
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}	



//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)
			OLED_WR_Byte(0,OLED_DATA); 
	} //更新显示
}

//全屏亮
void OLED_On(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)
			OLED_WR_Byte(1,OLED_DATA); 
	} //更新显示
}



//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~7
//chr:待显示的字符
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
	unsigned char c=0,i=0;	
	c=chr-' ';//得到偏移后的值  空格ASCII---32			
	if(x>Max_Column-1)
	{
		x=0;
		y=y+2;
	}
	
	//判断要显示的字体大小
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
//m^n函数
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
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
//显示一个字符号串
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
//显示汉字
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

/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
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
	////////////年////////////
		OLED_ShowNum(0,0,readYear,4,16);//2021

	OLED_ShowMyCN(32,0,10);//年
	////////////月////////////
		OLED_ShowNum(48,0,readMon,2,16);//6

	OLED_ShowMyCN(64,0,11);//月
	////////////日////////////
		OLED_ShowNum(80,0,readDay,2,16);//8

	OLED_ShowMyCN(96,0,0);//日
		
	
	OLED_ShowMyCN(0,2,7);//星
	OLED_ShowMyCN(16,2,8);//期
	OLED_ShowMyCN(32,2,weeks);
	
	////////////时////////////
		sprintf(tempStrH,"%02d",readHour);
		OLED_ShowString(0,4,tempStrH,16);

	OLED_ShowChar(16,4,':',16);//:
	////////////分////////////
		sprintf(tempStrM,"%02d",readMin);
		OLED_ShowString(24,4,tempStrM,16);

	OLED_ShowChar(40,4,':',16);//:
	////////////秒////////////
		sprintf(tempStrS,"%02d",readSec);
		OLED_ShowString(48,4,tempStrS,16);

}

void OLED_DisplayDH11(u8 temp,u8 humi)
{
	OLED_ShowMyCN(0,0,12);//温
	OLED_ShowMyCN(16,0,14);//度
	OLED_ShowChar(32,0,':',16);
	OLED_ShowNum(40,0,temp,3,16);
	OLED_ShowMyCN(0,2,13);//湿
	OLED_ShowMyCN(16,2,14);//度
	OLED_ShowChar(32,2,':',16);
	OLED_ShowNum(40,2,humi,3,16);
	
	OLED_ShowMyCN(0,6,17);//警报
	OLED_ShowMyCN(16,6,18);//警报
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
	OLED_ShowMyCN(0,0,15);//光
	OLED_ShowMyCN(16,0,16);//强
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
	OLED_ShowMyCN(32,0,10);//年
	////////////月////////////
	if(controls!=2)
	{
		OLED_ShowNum(48,0,readMon,2,16);//6
	}
	else
	{
		OLED_ShowString(48,0,"  ",16);
	}
	OLED_ShowMyCN(64,0,11);//月
	////////////日////////////
	if(controls!=3)
	{
		OLED_ShowNum(80,0,readDay,2,16);//8
	}
	else
	{
		OLED_ShowString(80,0,"  ",16);
	}
	OLED_ShowMyCN(96,0,0);//日
		
	
	OLED_ShowMyCN(0,2,7);//星
	OLED_ShowMyCN(16,2,8);//期
	OLED_ShowMyCN(32,2,weeks);
	
	////////////时////////////
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
	////////////分////////////
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
	////////////秒////////////
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
		OLED_ShowMyCN(0,0,12);//温
		OLED_ShowMyCN(16,0,14);//度
		OLED_ShowMyCN(32,0,17);//警
		OLED_ShowMyCN(48,0,18);//报
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
		
	OLED_ShowMyCN(0,4,13);//湿
	OLED_ShowMyCN(16,4,14);//度
	OLED_ShowMyCN(32,4,17);//警
	OLED_ShowMyCN(48,4,18);//报
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

//初始化SSD1306					    
void OLED_Init(void)
{ 	
	GPIO_InitTypeDef GPIO_InitStruct; 

#if OLED_MODE == 0
	MyIIC_Init();//OLED用到的IIC引脚的初始化
#else
	MySPI_Init();//OLED用到的SPI相关引脚的初始化
	
	//初始化7引脚OLED中的RES复位脚和  DC控制脚
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_8;//RES DC  
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;//输出模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//高速
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;//上拉输出
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	
	
	//复位OLED屏的驱动芯片----只有7引脚的SPI驱动才要这样复位
	RES = 1;
	delay_ms(100);
	RES = 0;
	delay_ms(200);
	RES = 1;

#endif	
	
	delay_ms(500);

	OLED_WR_Byte(0xAE,OLED_CMD);//先关掉屏幕 让它进入休眠状态 再进行以下设置
	
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
	
	OLED_WR_Byte(0x8D,OLED_CMD);//开启电源泵
	OLED_WR_Byte(0x14,OLED_CMD);//
	
	
	
	OLED_WR_Byte(0xAF,OLED_CMD);//--开启正常模式 显示
}  





























