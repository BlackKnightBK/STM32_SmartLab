//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//嵌入式开发网
//店铺地址：mcudev.taobao.com
 
//  功能描述   : 0.96寸OLED 接口演示例程(STM32F103ZE系列IIC)
//              说明: 
//              ----------------------------------------------------------------
//              GND   电源地
//              VCC   接5V或3.3v电源
//              SCL   接PB8（SCL）
//              SDA   接PB9（SDA）            
//              ----------------------------------------------------------------
 
//////////////////////////////////////////////////////////////////////////////////
 
//////////////////////////////////////////////////////////////////////////////////
#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	 
#include "delay.h"
#include "iic.h"
#include "spi.h"
#include "rtc.h"
#include "dht11.h"
#include "key.h"

//宏开关  1：7引脚SPI模式驱动OLED   0:4引脚IIC模式驱动OLED
#define OLED_MODE 1



#if OLED_MODE==1
#define RES  PCout(6) //SPI模式下 这两个引脚是用来控制OLED的复位和指令/数据发送的
#define DC   PCout(8)
#endif


#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED IIC端口定义----------------  					   
 		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


//OLED控制用函数
void OLED_WR_Byte(unsigned dat,unsigned cmd);  
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);//OLED初始化函数 + IIC引脚初始化
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y, char *chr,u8 Char_Size);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void Delay_50ms(unsigned int Del_50ms);
void Delay_1ms(unsigned int Del_1ms);
void fill_picture(unsigned char fill_Data);

void OLED_ShowMyCN(u8 x,u8 y,u8 no);
void OLED_DisplayDates(void);
void OLED_DisplayDH11(u8 temp,u8 humi);
void OLED_DisplayLight(u16 lightData);

void OLED_DisPlayChangeDates(u16 readYear,u8 readMon,u8 readDay,u8 weeks,u8 readHour,u8 readMin,u8 readSec,u8 controls);
void OLED_DisPlayChangeDHT(u8 tempTUL,u8 tempTLL,u8 tempHUL,u8 tempHLL,u8 controls);


//void Picture();
//void IIC_Start();
//void IIC_Stop();
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);


//void IIC_Wait_Ack();
#endif  
	 



