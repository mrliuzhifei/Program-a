#ifndef _CO2_H_
#define _CO2_H_
#include "KEY.h"
#define CO2_POWER_ON  P21=1
#define CO2_POWER_OFF P21=0
typedef enum CO2_Cmd_s	
{
	 R_CO2Valu=0x01,                 //��ȡCO2��ֵ
	 Calibrate_Single=0x03,          //����У��
	 R_Version_Num=0x1e,             //���̼��汾��
	 OnOff_Self_calibrate0=0x10,     //�����͹ر������У׼���Լ������У׼��������
	 R_Serial_Num=0x1f,             //���������?
	
}CO2_Cmd_s;

typedef enum Uart1_Sta_s	
{
	uart_state_wait,                //�ȴ�״̬
	uart_state_recievebusy,         //����æ
	uart_state_recievefinish,       //�������״�?
	uart_state_send_Coldata,        //
  uart_state_send_Paradata,       //
  uart_state_send_CommandReturn,  //
}Uart1_Sta_s;
void Get_CO2Valu(void);
void Get_CO2_VersionNum(void);
void Recieve_Data_Analysis(void);
uint8_t Get_Cs(uint8_t Lens);    //��ȡУ��ֵ
uint16_t Get_Co2Value(void);
extern uint8_t Start_Display_RXData;
void Display_Co2(Co2DisMod_s Dis_Mod);//co2��ʾ
void CO2_Init(void);
void Get_CO2_Data(void);
#endif

