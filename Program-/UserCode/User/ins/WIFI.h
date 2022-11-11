#ifndef _WIFI_H_
#define _WIFI_H_

#define WIFI_EN_PORT Port1
#define WIFI_EN_PIN  BIT3
#define WIFI_Enable()  P13=1
#define WIFI_Disable() P13=0
typedef enum SendDataType_s	   //������������
{
	LogIn_Request,               //������������
  LogOut_Request,              //������������     
	Single_Instruction,          //���͵�ָ��     
	multiple_Instruction,        //���Ͷ�ָ��
}SendDataType_s;		


void Init_Wifi(void);

void Send_NetworkEntry(SendDataType_s SendType);



#endif
