#include "ML51.h"
typedef struct DeviceData_t	           //�豸����
{
	uint8_t  Data_Length;                //���ݳ���
	uint8_t  Command_Word;               //������
	uint8_t  GateWay_ID;                 //����ID
	uint16_t Device_ID;                  //�豸ID
	uint32_t Device_SN;                  //�豸SN��
	uint8_t  Protocol_Versions;          //Э��汾��
	uint8_t  Device_Type;                //�豸����
	uint8_t  Device_Num;                 //�豸�ͺ�
	uint8_t Software_Versions ;          //����汾
	uint8_t Hardware_Versions ;          //Ӳ���汾��
	uint16_t Customer_ID;                //�ͻ�ID
	uint16_t Produc_Date;                //��������
}DeviceData_t;



typedef struct Sensor_Data_t	          //����������
{
	uint16_t Temp;                        //�¶�
	uint16_t Humi;                        //ʪ��
	uint16_t CO2_PecentMax;               //������̼Ũ�����ֵ
	uint16_t CO2_PecentAvr;               //������̼Ũ��ƽ��ֵ
	uint16_t CO2_PecentCurrent;           //������̼Ũ�ȵ�ǰֵ
}Sensor_Data_t;





typedef struct WIFICmdData_t            //WIFI������
{
	uint8_t Buzzer_OnOff;                 //����������
	uint8_t Blu_OnOff;                    //���⿪��	
	
}WIFICmdData_t;	

typedef struct WIFIData_t               //
{
	//SendDataType_s SendDataType;        //���͵���������
	DeviceData_t  DeviceData ;            //�豸��Ϣ
	WIFICmdData_t WIFICmdData;            //������
	Sensor_Data_t Sensor_Data;            //����������
	uint8_t SendToWifiBuff[50];           //��������
	uint8_t RecieveFromWifiBuff[50];      //��������
	uint8_t Serial_Num;                   //���к�
	uint16_t FH ;                         //֡ͷ
}WIFIData_t;	

WIFIData_t WIFIData;
#define JBE  WIFIData

void Init_Wifi(void)
{
	GPIO_SetMode(WIFI_EN_PORT,WIFI_EN_PIN,GPIO_MODE_PUSHPULL); //����Ϊ�������
	WIFI_Disable(); 
}

/*******************************
�������ƣ�Send_NetworkEntry()
������    ��
����ֵ��  ��
�������ܣ�������������
********************************/
void Send_NetworkEntry(SendDataType_s SendType)                //����������������
{ 
	uint8_t i=0;
	switch (SendType)
	{
	  case LogIn_Request:		   
			 JBE.FH=0xAA55;                                          //֡ͷ
			 JBE.DeviceData.Data_Length=0x12;                        //���ݳ���
			 JBE.DeviceData.Command_Word=0x01;                       //��������
			 JBE.DeviceData.GateWay_ID=0x00;                         //����ID
			 JBE.DeviceData.Device_ID =0X0000;                       //�豸ID
			 JBE.DeviceData.Device_SN =0X00000000;                   //�豸SN��
			 JBE.DeviceData.Protocol_Versions=0x02;                  //Э��汾��
			 JBE.DeviceData.Device_Type=0x06;                        //�豸����
			 JBE.DeviceData.Device_Num=0x01;                         //�豸�ͺ�
			 JBE.DeviceData.Protocol_Versions=0x10;                  //����汾��
			 JBE.DeviceData.Hardware_Versions=0x10;                  //Ӳ���汾��
			 JBE.DeviceData.Customer_ID=0x0001;                      //�ͻ�ID
			 JBE.DeviceData.Produc_Date=0x22A1;  	                   //��������
			 JBE.SendToWifiBuff[0]=0xAA;
			 JBE.SendToWifiBuff[1]=0x55;	
			 JBE.SendToWifiBuff[2]=JBE.Serial_Num;
			 JBE.SendToWifiBuff[3]=JBE.DeviceData.Data_Length;
			 JBE.SendToWifiBuff[4]=JBE.DeviceData.Command_Word;
			 JBE.SendToWifiBuff[5]=JBE.DeviceData.GateWay_ID;
			 JBE.SendToWifiBuff[6]=JBE.DeviceData.Device_ID>>8;
			 JBE.SendToWifiBuff[7]=JBE.DeviceData.Device_ID;
			 JBE.SendToWifiBuff[8]=JBE.DeviceData.Device_SN>>24;
			 JBE.SendToWifiBuff[9]=JBE.DeviceData.Device_SN>>16;	
			 JBE.SendToWifiBuff[10]=JBE.DeviceData.Device_SN>>8;	
			 JBE.SendToWifiBuff[11]=JBE.DeviceData.Device_SN;		
			 JBE.SendToWifiBuff[12]=JBE.DeviceData.Protocol_Versions;
			 JBE.SendToWifiBuff[13]=JBE.DeviceData.Device_Type;
			 JBE.SendToWifiBuff[14]=JBE.DeviceData.Device_Num;
			 JBE.SendToWifiBuff[15]=JBE.DeviceData.Protocol_Versions;
			 JBE.SendToWifiBuff[16]=JBE.DeviceData.Hardware_Versions;
			 JBE.SendToWifiBuff[17]=JBE.DeviceData.Customer_ID>>8;
			 JBE.SendToWifiBuff[18]=JBE.DeviceData.Customer_ID;
			 JBE.SendToWifiBuff[19]=JBE.DeviceData.Produc_Date>>8;
			 JBE.SendToWifiBuff[20]=JBE.DeviceData.Produc_Date;
			 for(i=1;i<21;i++)
			 {
			 JBE.SendToWifiBuff[21]=JBE.SendToWifiBuff[21]+JBE.SendToWifiBuff[i];//����У���
			 }
			 Uart0senddata(&JBE.SendToWifiBuff[0],JBE.DeviceData.Data_Length+4);
			 JBE.Serial_Num++;
		   break;
		
		case LogOut_Request:
		break;
    case Single_Instruction:
    break;
    case multiple_Instruction:
    break;
    default:
    break;		
	}

}


/**
 * @brief       UART0 interrupt subroutine
 * @param       UART0~3, baudrate value
 * @return      none
 * @details     none
 * @note        Since printf function TI must be 1. Check with printf flag, for printf enable, not clear TI . 
 */
void UART0_ISR(void) interrupt 4
{
    _push_(SFRS);
    if (RI)
    {
        uart0_receive_flag = 1;
        uart0_receive_data = SBUF;
        clr_SCON_RI;                                         //Clear RI (Receive Interrupt).
			  
    }
    if (TI)
    {
//   if (!UART0PRINTFG)
//   TI = 0;
    }
    _pop_(SFRS);
}
