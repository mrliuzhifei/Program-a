#ifndef _WIFI_H_
#define _WIFI_H_

#define WIFI_EN_PORT Port1
#define WIFI_EN_PIN  BIT3
#define WIFI_Enable()  P13=1
#define WIFI_Disable() P13=0
typedef enum SendDataType_s	   //发送数据类型
{
	LogIn_Request,               //发送入网请求
  LogOut_Request,              //发送退网请求     
	Single_Instruction,          //发送单指令     
	multiple_Instruction,        //发送多指令
}SendDataType_s;		


void Init_Wifi(void);

void Send_NetworkEntry(SendDataType_s SendType);



#endif
