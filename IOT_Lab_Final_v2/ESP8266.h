#ifndef __ESP8266_H 
#define __ESP8266_H     
#include "stm32f10x.h"
#include "delay.h"

int ESP8266_Init(void);
void ESP8266_SendMsg(char *msg);
int ESP8266_Check_Ack(void);
int ESP8266_Check_CONNECT_Station(void);
int ESP8266_Check_CONNECT_Server(void);
int ESP8266_Check_CONNECT_AP(void);
void ESP8266_ReadCommand(char * getCommand);
void ESP8266_HandleCommand(char * getCommand);
extern int ESP8266_CONNECTED;
extern int ESP8266_INITED;
extern int ESP8266_READY;//Œ¥ π”√
extern int ESP8266_MsgNumbers;
extern char getCommand[100];
extern int ESP8266_step;
#endif
