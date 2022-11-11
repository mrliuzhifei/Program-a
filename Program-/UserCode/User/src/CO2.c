#include "ML51.h"
#include "BC3602.h"
#define BC3602_RX
uint8_t Start_Display_RXData = 0;
typedef struct CO2_Send_Data_t
{
	uint8_t SendBuf[10]; //��������
	uint8_t SendHead;	 //��������֡ͷ
	uint8_t SendLen;	 //�������ݳ���
	CO2_Cmd_s SendCmd;	 //�������ݵ�����
	uint8_t SendCs;		 //�������ݵ�У���
} CO2_Send_Data_t;

typedef struct CO2_Read_Data_t
{
	uint8_t ReadBuf[15];	//��������
	uint8_t CO2DataBuf[15]; //��������
	uint8_t ReadHead;		//��������֡ͷ
	uint8_t ReadLen;		//�������ݳ���
	CO2_Cmd_s ReadCmd;		//�������ݵ�����
	uint8_t ReadCs;			//�������ݵ�У���

} CO2_Read_Data_t;

typedef struct CO2_Data_t
{
	CO2_Send_Data_t CO2_Send_Data;
	CO2_Read_Data_t CO2_Read_Data;
	uint16_t CO2Value;
	Uart1_Sta_s Uart1_Sta;
	uint8_t Recieve_count;
	uint8_t uart_recievedate_temp; //���ڽ�������
} CO2_Data_t;

CO2_Data_t CO2_Data;
#define JBE CO2_Data

void CO2_Init(void)
{
	GPIO_SetMode(Port2, BIT1, GPIO_MODE_PUSHPULL);
	CO2_POWER_ON;          //�򿪶�����̼
}

void Get_CO2Valu(void)
{

	JBE.CO2_Send_Data.SendHead = 0x11;
	JBE.CO2_Send_Data.SendLen = 0x01;
	JBE.CO2_Send_Data.SendCmd = R_CO2Valu;
	JBE.CO2_Send_Data.SendCs = 256 - (JBE.CO2_Send_Data.SendHead + JBE.CO2_Send_Data.SendLen + JBE.CO2_Send_Data.SendCmd);
	JBE.CO2_Send_Data.SendBuf[0] = JBE.CO2_Send_Data.SendHead;
	JBE.CO2_Send_Data.SendBuf[1] = JBE.CO2_Send_Data.SendLen;
	JBE.CO2_Send_Data.SendBuf[2] = JBE.CO2_Send_Data.SendCmd;
	JBE.CO2_Send_Data.SendBuf[3] = JBE.CO2_Send_Data.SendCs;
	if (JBE.Uart1_Sta == uart_state_wait)
		Uart1senddata(&JBE.CO2_Send_Data.SendBuf[0], JBE.CO2_Send_Data.SendLen + 3);
}

void Get_CO2_VersionNum(void)
{
	JBE.CO2_Send_Data.SendHead = 0x11;
	JBE.CO2_Send_Data.SendLen = 0x01;
	JBE.CO2_Send_Data.SendCmd = R_Version_Num;
	JBE.CO2_Send_Data.SendCs = 256 - (JBE.CO2_Send_Data.SendHead + JBE.CO2_Send_Data.SendLen + JBE.CO2_Send_Data.SendCmd);
	JBE.CO2_Send_Data.SendBuf[0] = JBE.CO2_Send_Data.SendHead;
	JBE.CO2_Send_Data.SendBuf[1] = JBE.CO2_Send_Data.SendLen;
	JBE.CO2_Send_Data.SendBuf[2] = JBE.CO2_Send_Data.SendCmd;
	JBE.CO2_Send_Data.SendBuf[3] = JBE.CO2_Send_Data.SendCs;
	if (JBE.Uart1_Sta == uart_state_wait)
		Uart1senddata(&JBE.CO2_Send_Data.SendBuf[0], JBE.CO2_Send_Data.SendLen + 3);
}

/* UART1 interrupt subroutine */
void UART1_ISR(void) interrupt 15
{
	_push_(SFRS);
	if (RI_1)
	{
		JBE.uart_recievedate_temp = SBUF1;
		if (JBE.Uart1_Sta == uart_state_wait)
		{
			if (JBE.uart_recievedate_temp == 0x16) //���յ���֡ͷ
			{
				memset(&JBE.CO2_Read_Data.ReadBuf, 0, sizeof(JBE.CO2_Read_Data.ReadBuf)); //������������
				JBE.CO2_Read_Data.ReadBuf[0] = JBE.uart_recievedate_temp;
				JBE.Uart1_Sta = uart_state_recievebusy;
			}
		}
		else if (JBE.Uart1_Sta == uart_state_recievebusy)
		{
			JBE.Recieve_count++;
			JBE.CO2_Read_Data.ReadBuf[JBE.Recieve_count] = JBE.uart_recievedate_temp;
			if (JBE.Recieve_count == (JBE.CO2_Read_Data.ReadBuf[1] + 2))
			{
				JBE.Uart1_Sta = uart_state_recievefinish; //���յ��ĸ����ﵽ���Ժ��������
			}
		}

		RI_1 = 0; /* clear reception flag for next reception */
	}
	_pop_(SFRS);
}

void Recieve_Data_Analysis(void)
{
	static uint16_t Times = 0;
	if (JBE.Uart1_Sta == uart_state_recievefinish)
	{

		if (Get_Cs(JBE.Recieve_count) == JBE.CO2_Read_Data.ReadBuf[JBE.Recieve_count])
		{
			memcpy(JBE.CO2_Read_Data.CO2DataBuf, JBE.CO2_Read_Data.ReadBuf, 15);
			JBE.Uart1_Sta = uart_state_wait;
			JBE.CO2Value = JBE.CO2_Read_Data.CO2DataBuf[3] * 256 + JBE.CO2_Read_Data.CO2DataBuf[4];
			JBE.Recieve_count = 0;
			Times = 0;
		}
		else
		{
			JBE.Uart1_Sta = uart_state_wait;
			JBE.Recieve_count = 0;
			Times = 0;
		}
	}
	else if (JBE.Uart1_Sta != uart_state_wait)
	{
		Times++;
		if (Times > 1000)
		{
			JBE.Uart1_Sta = uart_state_wait;
			JBE.Recieve_count = 0;
			Times = 0;
		}
	}
}

uint8_t Get_Cs(uint8_t Lens)
{
	uint16_t Sum = 0;
	uint8_t i = 0;
	uint8_t Cs_r = 0;
	for (i = 0; i < Lens; i++)
	{
		Sum = Sum + JBE.CO2_Read_Data.ReadBuf[i];
	}
	Cs_r = 256 - Sum;
	return Cs_r;
}

void Display_Co2(Co2DisMod_s Dis_Mod)
{
	uint8_t co2gw, co2sw, co2bw, co2qw;
	uint16_t C02Value_Dis;
#ifdef BC3602_RX
	static uint8_t Time_Reflash = 0, i;
	if (Start_Display_RXData == True)
	{
		Time_Reflash++;
		if (Time_Reflash <= 2)
		{
			co2qw = RF_RXFIFOSAVE[i] / 1000;
			co2bw = RF_RXFIFOSAVE[i] % 1000 / 100;
			co2sw = RF_RXFIFOSAVE[i] % 100 / 10;
			co2gw = RF_RXFIFOSAVE[i] % 10;
			if (RF_RXFIFOSAVE[i] < 10)
			{
				ShowDigit(4, co2gw, 0);
				LCD_SetOnePixel(0, 2, Disable);
				LCD_SetOnePixel(1, 2, Disable);
				LCD_SetOnePixel(2, 2, Disable);
				LCD_SetOnePixel(3, 2, Disable);

				LCD_SetOnePixel(1, 1, Disable);
				LCD_SetOnePixel(2, 1, Disable);
				LCD_SetOnePixel(3, 1, Disable);

				LCD_SetOnePixel(0, 4, Disable);
				LCD_SetOnePixel(1, 4, Disable);
				LCD_SetOnePixel(2, 4, Disable);
				LCD_SetOnePixel(3, 4, Disable);

				LCD_SetOnePixel(0, 3, Disable);
				LCD_SetOnePixel(1, 3, Disable);
				LCD_SetOnePixel(2, 3, Disable);
				LCD_SetOnePixel(3, 3, Disable);

				LCD_SetOnePixel(0, 6, Disable);
				LCD_SetOnePixel(1, 6, Disable);
				LCD_SetOnePixel(2, 6, Disable);
				LCD_SetOnePixel(3, 6, Disable);

				LCD_SetOnePixel(1, 24, Disable);
				LCD_SetOnePixel(2, 24, Disable);
				LCD_SetOnePixel(3, 24, Disable);
			}
			if (RF_RXFIFOSAVE[i] >= 10 && RF_RXFIFOSAVE[i] < 100)
			{
				ShowDigit(4, co2gw, 0);
				ShowDigit(3, co2sw, 0);
				LCD_SetOnePixel(0, 2, Disable);
				LCD_SetOnePixel(1, 2, Disable);
				LCD_SetOnePixel(2, 2, Disable);
				LCD_SetOnePixel(3, 2, Disable);

				LCD_SetOnePixel(1, 1, Disable);
				LCD_SetOnePixel(2, 1, Disable);
				LCD_SetOnePixel(3, 1, Disable);

				LCD_SetOnePixel(0, 4, Disable);
				LCD_SetOnePixel(1, 4, Disable);
				LCD_SetOnePixel(2, 4, Disable);
				LCD_SetOnePixel(3, 4, Disable);

				LCD_SetOnePixel(1, 3, Disable);
				LCD_SetOnePixel(2, 3, Disable);
				LCD_SetOnePixel(3, 3, Disable);
			}
			if (RF_RXFIFOSAVE[i] >= 100 && RF_RXFIFOSAVE[i] < 1000)
			{
				ShowDigit(4, co2gw, 0);
				ShowDigit(3, co2sw, 0);
				ShowDigit(2, co2bw, 0);

				LCD_SetOnePixel(0, 2, Disable);
				LCD_SetOnePixel(1, 2, Disable);
				LCD_SetOnePixel(2, 2, Disable);
				LCD_SetOnePixel(3, 2, Disable);

				LCD_SetOnePixel(1, 1, Disable);
				LCD_SetOnePixel(2, 1, Disable);
				LCD_SetOnePixel(3, 1, Disable);
			}
			if (RF_RXFIFOSAVE[i] >= 1000)
			{
				ShowDigit(1, co2qw, 0);
				ShowDigit(3, co2sw, 0);
				ShowDigit(2, co2bw, 0);
				ShowDigit(4, co2gw, 0);
			}
		}
		else
		{
			Time_Reflash = 0;
			i++;
			if (i > 24)
			{
				Start_Display_RXData = False;
				i = 0;
			}
		}
	}
	
/*#ifdef BC3602_RX
co2qw=RF_RXFIFO[0]/1000;
co2bw=RF_RXFIFO[0]%1000/100;
co2sw=RF_RXFIFO[0]%100/10;
co2gw=RF_RXFIFO[0]%10;
ShowDigit(4,co2gw,0);
if(C02Value_Dis>=10)  ShowDigit(3, co2sw,0);
if(C02Value_Dis>=100) ShowDigit(2, co2bw,0);
if(C02Value_Dis>=1000)ShowDigit(1, co2qw,0);*/
	
#else
	if (Dis_Mod == Current_Valu)
	{
		LCD_SetOnePixel(1, 9, Disable);
		LCD_SetOnePixel(2, 9, Disable);
		LCD_SetOnePixel(3, 9, Disable);
		C02Value_Dis = JBE.CO2Value;
	}
	else if (Dis_Mod == Max_Valu_24h)
	{
		LCD_SetOnePixel(1, 9, Disable);
		LCD_SetOnePixel(2, 9, Enable);
		LCD_SetOnePixel(3, 9, Enable);
		C02Value_Dis = Get_Max_Valu_CO2_24h();
	}
	else if (Dis_Mod == Avg_Valu_24h)
	{
		LCD_SetOnePixel(1, 9, Enable);
		LCD_SetOnePixel(2, 9, Disable);
		LCD_SetOnePixel(3, 9, Enable);
		C02Value_Dis = Get_Avg_Valu_CO2_24h();
	}
	co2qw = C02Value_Dis / 1000;
	co2bw = C02Value_Dis % 1000 / 100;
	co2sw = C02Value_Dis % 100 / 10;
	co2gw = C02Value_Dis % 10;
	if (C02Value_Dis < 10)
	{
		ShowDigit(4, co2gw, 0);

		LCD_SetOnePixel(0, 2, Disable);
		LCD_SetOnePixel(1, 2, Disable);
		LCD_SetOnePixel(2, 2, Disable);
		LCD_SetOnePixel(3, 2, Disable);

		LCD_SetOnePixel(1, 1, Disable);
		LCD_SetOnePixel(2, 1, Disable);
		LCD_SetOnePixel(3, 1, Disable);

		LCD_SetOnePixel(0, 4, Disable);
		LCD_SetOnePixel(1, 4, Disable);
		LCD_SetOnePixel(2, 4, Disable);
		LCD_SetOnePixel(3, 4, Disable);

		LCD_SetOnePixel(0, 3, Disable);
		LCD_SetOnePixel(1, 3, Disable);
		LCD_SetOnePixel(2, 3, Disable);
		LCD_SetOnePixel(3, 3, Disable);

		LCD_SetOnePixel(0, 6, Disable);
		LCD_SetOnePixel(1, 6, Disable);
		LCD_SetOnePixel(2, 6, Disable);
		LCD_SetOnePixel(3, 6, Disable);

		LCD_SetOnePixel(1, 24, Disable);
		LCD_SetOnePixel(2, 24, Disable);
		LCD_SetOnePixel(3, 24, Disable);
	}
	if (C02Value_Dis >= 10 && C02Value_Dis < 100)
	{
		ShowDigit(4, co2gw, 0);
		ShowDigit(3, co2sw, 0);
		LCD_SetOnePixel(0, 2, Disable);
		LCD_SetOnePixel(1, 2, Disable);
		LCD_SetOnePixel(2, 2, Disable);
		LCD_SetOnePixel(3, 2, Disable);

		LCD_SetOnePixel(1, 1, Disable);
		LCD_SetOnePixel(2, 1, Disable);
		LCD_SetOnePixel(3, 1, Disable);

		LCD_SetOnePixel(0, 4, Disable);
		LCD_SetOnePixel(1, 4, Disable);
		LCD_SetOnePixel(2, 4, Disable);
		LCD_SetOnePixel(3, 4, Disable);

		LCD_SetOnePixel(1, 3, Disable);
		LCD_SetOnePixel(2, 3, Disable);
		LCD_SetOnePixel(3, 3, Disable);
	}
	if (C02Value_Dis >= 100 && C02Value_Dis < 1000)
	{
		ShowDigit(4, co2gw, 0);
		ShowDigit(3, co2sw, 0);
		ShowDigit(2, co2bw, 0);

		LCD_SetOnePixel(0, 2, Disable);
		LCD_SetOnePixel(1, 2, Disable);
		LCD_SetOnePixel(2, 2, Disable);
		LCD_SetOnePixel(3, 2, Disable);

		LCD_SetOnePixel(1, 1, Disable);
		LCD_SetOnePixel(2, 1, Disable);
		LCD_SetOnePixel(3, 1, Disable);
	}
	if (C02Value_Dis >= 1000)
	{
		ShowDigit(1, co2qw, 0);
		ShowDigit(3, co2sw, 0);
		ShowDigit(2, co2bw, 0);
		ShowDigit(4, co2gw, 0);
	}
#endif
}

uint16_t Get_Co2Value(void)
{
	return JBE.CO2Value;
}

void Get_CO2_Data(void)
{
	switch (Sys_MODE)
	{
	case Sys_MODE_ACPower:
		CO2_POWER_ON;
		Get_CO2Valu();
		break;
	case Sys_MODE_BATPower:
		CO2_POWER_ON;
		Get_CO2Valu();
		break;
	case Sys_MODE_BATPowerLow:
		CO2_POWER_OFF;
		break;
	default:

		break;
	}
}
