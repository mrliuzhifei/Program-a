#ifndef _KEY_H_
#define _KEY_H_
//#include "ML51.h"
#include "TYPE.h"
//#include "LED.h"
typedef enum Key_Value_s	//��ֵ
{
	 Key_Null,              //�ް���
	 Key1_ClickSingle,       //����
	 Key1_ClickSingle_1,     //����1��
	 Key1_ClickSingle_2,     //��������3��
	 Key1_ClickSingle_3,     //��������3��
	 Key1_LongPress,         //������
	
	 Key2_ClickSingle,       //����
	 Key2_ClickSingle_1,     //����1��
	 Key2_ClickSingle_2,     //��������3��
	 Key2_ClickSingle_3,     //��������3��
	 Key2_LongPress,         //������
}Key_Value_s;


typedef enum Light_Sw_s	   //�ƿ���
{
	 Light_off,                    
	 Light_on,
}Light_Sw_s	;

typedef enum Bell_Sw_s	   //��������
{
	 Bell_off,                    
	 Bell_on,
}Bell_Sw_s		;

typedef enum Unit_Sw_s	   //��λ�л�
{
	 Unit_C,                    
	 Unit_F, 
}Unit_Sw_s	;


typedef enum Co2DisMod_s	   //������̼��ʾģʽ
{
	 Current_Valu,             //��ǰֵ                
	 Max_Valu_24h,             //24Сʱ���ֵ
	 Avg_Valu_24h,             //24Сʱƽ��ֵ
}Co2DisMod_s	;


typedef enum Key_Task_s	//��ֵ
{
	 Task_Null,              //������
	 Task_OpenBell, 
	 Task_CloseBell,
	 Task_OnLight,
	 Task_OffLight,
	 Task_SwitchUnit,
}Key_Task_s;



typedef enum NetWorkMode_s
{	
 NetWork_Null,
 NetWork_Entry, //����
 NetWork_Exite, //����

}NetWorkMode_s;


typedef enum DIS_MODE_e   //��ʾģʽ
{                   
  DisMode_Null,
} 
DIS_MODE_e;
void Get_KeyValue(void);
void KeyProcess(void);
void Init_KeyPort(void);
uint8_t Get_Light_Sta(void);
uint8_t Get_Bell_Sta(void);
uint8_t Get_Unit_Sta(void);

Co2DisMod_s Get_Co2DisMod(void);

uint8_t Get_LCD_Sta(void);
void Crl_LCD_Sta(void);
uint8_t Get_AccessNet_Request(void);
void Clr_AccessNet_Request(void);
#endif
