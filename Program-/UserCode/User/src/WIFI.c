#include "ML51.h"
typedef struct DeviceData_t	           //设备数据
{
	uint8_t  Data_Length;                //数据长度
	uint8_t  Command_Word;               //命令字
	uint8_t  GateWay_ID;                 //网关ID
	uint16_t Device_ID;                  //设备ID
	uint32_t Device_SN;                  //设备SN码
	uint8_t  Protocol_Versions;          //协议版本号
	uint8_t  Device_Type;                //设备类型
	uint8_t  Device_Num;                 //设备型号
	uint8_t Software_Versions ;          //软件版本
	uint8_t Hardware_Versions ;          //硬件版本号
	uint16_t Customer_ID;                //客户ID
	uint16_t Produc_Date;                //生产日期
}DeviceData_t;



typedef struct Sensor_Data_t	          //传感器数据
{
	uint16_t Temp;                        //温度
	uint16_t Humi;                        //湿度
	uint16_t CO2_PecentMax;               //二氧化碳浓度最大值
	uint16_t CO2_PecentAvr;               //二氧化碳浓度平均值
	uint16_t CO2_PecentCurrent;           //二氧化碳浓度当前值
}Sensor_Data_t;





typedef struct WIFICmdData_t            //WIFI命令字
{
	uint8_t Buzzer_OnOff;                 //蜂鸣器开关
	uint8_t Blu_OnOff;                    //背光开关	
	
}WIFICmdData_t;	

typedef struct WIFIData_t               //
{
	//SendDataType_s SendDataType;        //发送的数据类型
	DeviceData_t  DeviceData ;            //设备信息
	WIFICmdData_t WIFICmdData;            //命令字
	Sensor_Data_t Sensor_Data;            //传感器数据
	uint8_t SendToWifiBuff[50];           //发送数组
	uint8_t RecieveFromWifiBuff[50];      //接收数组
	uint8_t Serial_Num;                   //序列号
	uint16_t FH ;                         //帧头
}WIFIData_t;	

WIFIData_t WIFIData;
#define JBE  WIFIData

void Init_Wifi(void)
{
	GPIO_SetMode(WIFI_EN_PORT,WIFI_EN_PIN,GPIO_MODE_PUSHPULL); //设置为推挽输出
	WIFI_Disable(); 
}

/*******************************
函数名称：Send_NetworkEntry()
参数：    无
返回值：  无
函数功能：发送入网请求。
********************************/
void Send_NetworkEntry(SendDataType_s SendType)                //发送入网请求命令
{ 
	uint8_t i=0;
	switch (SendType)
	{
	  case LogIn_Request:		   
			 JBE.FH=0xAA55;                                          //帧头
			 JBE.DeviceData.Data_Length=0x12;                        //数据长度
			 JBE.DeviceData.Command_Word=0x01;                       //入网命令
			 JBE.DeviceData.GateWay_ID=0x00;                         //网关ID
			 JBE.DeviceData.Device_ID =0X0000;                       //设备ID
			 JBE.DeviceData.Device_SN =0X00000000;                   //设备SN码
			 JBE.DeviceData.Protocol_Versions=0x02;                  //协议版本号
			 JBE.DeviceData.Device_Type=0x06;                        //设备类型
			 JBE.DeviceData.Device_Num=0x01;                         //设备型号
			 JBE.DeviceData.Protocol_Versions=0x10;                  //软件版本号
			 JBE.DeviceData.Hardware_Versions=0x10;                  //硬件版本号
			 JBE.DeviceData.Customer_ID=0x0001;                      //客户ID
			 JBE.DeviceData.Produc_Date=0x22A1;  	                   //生产日期
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
			 JBE.SendToWifiBuff[21]=JBE.SendToWifiBuff[21]+JBE.SendToWifiBuff[i];//计算校验和
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
