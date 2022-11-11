#include "BC3602_Gateway.h"
#include "BC3602_CMD_REG.h"
#include "BC3602.h"
#include "KEY.h"
#include "ML51.h"

typedef struct DeviceData_t //设备数据
{
    uint8_t Data_Length;       //数据长度
    uint8_t Command_Word;      //命令字
    uint8_t GateWay_ID;        //网关ID
    uint16_t Device_ID;        //设备ID
    uint32_t Device_SN;        //设备SN码
    uint8_t Protocol_Versions; //协议版本号
    uint8_t Device_Type;       //设备类型
    uint8_t Device_Num;        //设备型号
    uint8_t Software_Versions; //软件版本
    uint8_t Hardware_Versions; //硬件版本号
    uint16_t Customer_ID;      //客户ID
    uint16_t Produc_Date;      //生产日期
} DeviceData_t;

typedef struct Sensor_Data_t //传感器数据
{
    uint16_t Temp;              //温度
    uint16_t Humi;              //湿度
    uint16_t CO2_PecentMax;     //二氧化碳浓度最大值
    uint16_t CO2_PecentAvr;     //二氧化碳浓度平均值
    uint16_t CO2_PecentCurrent; //二氧化碳浓度当前值
} Sensor_Data_t;

typedef enum BC3602_CmdData_s // 3602命令字
{
    NetWoke_Access = 0x01,
} BC3602_CmdData_s;

/*typedef  enum BC3602_RXSta_s	   //3602状态
{

}BC3602_RX_s;

typedef  enum BC3602_TXSta_s	   //3602状态
{

}BC3602_TX_s;*/

typedef enum BC3602_TXMod_s // 3602发送模式
{
    Idle,               //空闲模式
    Mod_NetWoke_Access, //入网请求模式
    Mod_Active_Report,  //主动上报
    Mod_Passive_Report, //被动上报

} BC3602_TXMod_s;

void Prepare_SendData(BC3602_TXMod_s BC3602_TX_M); //发送数据准备

typedef struct BC3602_Data_t // 3602数据
{
    uint8_t RX_buf[32]; //接收数组
    uint8_t TX_buf[32]; //发送数组
    BC3602_TXMod_s BC3602_TXMod;
    DeviceData_t DeviceData;
} BC3602_Data_t;
BC3602_Data_t BC3602_Data;


void BC3602_TX_Process(void)
{
    if (Get_AccessNet_Request() == TRUE)
    {
        if (Get_RF_Mode() != RF_TX)
        {
            if (BC3602_Data.BC3602_TXMod == Idle)
                BC3602_Data.BC3602_TXMod = Mod_NetWoke_Access; //没有工作在接收模式且发送功能在空闲状态中，把发送模式切到入网模式
        }
        Prepare_SendData(BC3602_Data.BC3602_TXMod);     //准备所要发送的数据//
				BC3602_Data.BC3602_TXMod =Idle;
        Clr_AccessNet_Request();//清除按键请求
    }
}

/******************************校验和计算***************************************/
uint8_t Get_Checksum(uint8_t *pucFrame, uint8_t usLen)
{
    uint8_t Checksum, i;
    for (i = 0; i < usLen + 1; i++)
    {
        Checksum = Checksum + pucFrame[i + 2];
    }
    return Checksum;
}

/*******************************准备发送的数据***********************************/
void Prepare_SendData(BC3602_TXMod_s BC3602_TX_M) //发送数据准备
{
    switch (BC3602_TX_M)
    {
    case Mod_NetWoke_Access:
        BC3602_Data.DeviceData.Data_Length = 0x14;
        BC3602_Data.DeviceData.Command_Word = 0x01;
        BC3602_Data.DeviceData.GateWay_ID = 0x00;
        BC3602_Data.DeviceData.Device_ID = 0x0000;
        BC3602_Data.DeviceData.Device_SN = 0x12345678;
        BC3602_Data.DeviceData.Protocol_Versions = 0x02;
        BC3602_Data.DeviceData.Device_Type = 0x06;
        BC3602_Data.DeviceData.Device_Num = 0x01;
        BC3602_Data.DeviceData.Software_Versions = 0x01;
        BC3602_Data.DeviceData.Hardware_Versions = 0x01;
        BC3602_Data.DeviceData.Customer_ID = 0x0001;
        BC3602_Data.DeviceData.Produc_Date = 0x2150;
        BC3602_Data.TX_buf[0] = 0xAA;
        BC3602_Data.TX_buf[1] = 0x55; //帧头
        BC3602_Data.TX_buf[2] = 0x01; //序列号
        BC3602_Data.TX_buf[3] = BC3602_Data.DeviceData.Data_Length;
        BC3602_Data.TX_buf[4] = BC3602_Data.DeviceData.Command_Word;
        BC3602_Data.TX_buf[5] = BC3602_Data.DeviceData.GateWay_ID;
        BC3602_Data.TX_buf[6] = BC3602_Data.DeviceData.Device_ID >> 8;
        BC3602_Data.TX_buf[7] = BC3602_Data.DeviceData.Device_ID;
        BC3602_Data.TX_buf[8] = BC3602_Data.DeviceData.Device_SN >> 24;
        BC3602_Data.TX_buf[9] = BC3602_Data.DeviceData.Device_SN >> 16;
        BC3602_Data.TX_buf[10] = BC3602_Data.DeviceData.Device_SN >> 8;
        BC3602_Data.TX_buf[11] = BC3602_Data.DeviceData.Device_SN;
        BC3602_Data.TX_buf[12] = BC3602_Data.DeviceData.Protocol_Versions;
        BC3602_Data.TX_buf[13] = BC3602_Data.DeviceData.Device_Type;
        BC3602_Data.TX_buf[14] = BC3602_Data.DeviceData.Device_Num;
        BC3602_Data.TX_buf[15] = BC3602_Data.DeviceData.Software_Versions;
        BC3602_Data.TX_buf[16] = BC3602_Data.DeviceData.Hardware_Versions;
        BC3602_Data.TX_buf[17] = BC3602_Data.DeviceData.Customer_ID >> 8;
        BC3602_Data.TX_buf[18] = BC3602_Data.DeviceData.Customer_ID;
        BC3602_Data.TX_buf[19] = BC3602_Data.DeviceData.Produc_Date >> 8;
        BC3602_Data.TX_buf[20] = BC3602_Data.DeviceData.Produc_Date;
        BC3602_Data.TX_buf[21] = 0x00;
        BC3602_Data.TX_buf[22] = 0x00;
        BC3602_Data.TX_buf[23] = Get_Checksum(BC3602_Data.TX_buf, BC3602_Data.DeviceData.Data_Length);
        memcpy(RF_TXFIFO,BC3602_Data.TX_buf,BC3602_Data.DeviceData.Data_Length+4);
        ReSet_RFstep();																// reset RF step
		    Set_RFmode(RF_TX);
        break;

    default:
        break;
    }
}