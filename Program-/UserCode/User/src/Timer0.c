#include "ML51.h"

typedef struct Sys_TimeCount_t
{
	UINT8 count1ms;
	UINT8 count10ms;
	UINT16 count20ms;
	UINT16 count30ms;
	UINT16 count40ms;
	UINT16 count50ms;
	UINT16 count100ms;
	UINT16 count200ms;
	UINT16 count500ms;
	UINT16 count1s;
	UINT16 count5s;
} Sys_TimeCount_t;

typedef struct TimeEvent_t
{
	UINT8 Time_1ms_Event;
	UINT8 Time_10ms_Event;
	UINT8 Time_20ms_Event;
	UINT8 Time_30ms_Event;
	UINT8 Time_40ms_Event;
	UINT8 Time_50ms_Event;
	UINT8 Time_100ms_Event;
	UINT8 Time_200ms_Event;
	UINT8 Time_500ms_Event;
	UINT8 Time_1s_Event;
	UINT8 Time_5s_Event;
} TimeEvent_t;

typedef struct Sys_TimePar_t
{
	Sys_TimeCount_t Sys_TimeCount;
	TimeEvent_t TimeEvent_t;
	UINT8 IsTempHut_MeasureStart;
	UINT8 Sys_Second;
	UINT8 Sys_Minute;
	UINT8 IsNeedCO2Save;  //�Ƿ���Ҫ��CO2��ֵ
	UINT8 ISNeedPageSave; //�Ƿ��?��ǰҳ��ַ
	UINT8 SaveAddr_P;
	UINT8 ISOneCircleSave; //�Ƿ����һ�?
	UINT16 NTC_Value;
	UINT16 Temp;
	TempHub_Sta_s TempHub_Sta;
} Sys_TimePar_t;
Sys_TimePar_t Sys_TimePar;
#define JBE Sys_TimePar

/* if define TIMER0_FSYS_DIV12, timer = (0x1FFF-0x1000)*12/24MHz = 4.08ms */
/* if define TIMER0_FSYS, timer = (0x1FFF-0x0010)/24MHz = 340us */
#define TH0_INIT 0Xc1 // 0xF9
#define TL0_INIT 0X0f // 0x17

void Timer0_Init(void)
{
	ENABLE_TIMER0_MODE0; // Timer 0 mode configuration
	TIMER0_FSYS_DIV12;
	SFRS = 0;
	TH0 = TH0_INIT; //��ʱ0.ms
	TL0 = TL0_INIT;
	ENABLE_TIMER0_INTERRUPT; // enable Timer0 interrupt
	ENABLE_GLOBAL_INTERRUPT; // enable interrupts
	set_TCON_TR0;
}

void Timer0_ISR(void) interrupt 1 // interrupt address is 0x000B
{
	_push_(SFRS);
	SFRS = 0;
	TH0 = TH0_INIT;
	TL0 = TL0_INIT;
	TF0 = 0;
	JBE.Sys_TimeCount.count1ms++;
	JBE.Sys_TimeCount.count10ms++;
	JBE.Sys_TimeCount.count20ms++;
	JBE.Sys_TimeCount.count30ms++;
	JBE.Sys_TimeCount.count40ms++;
	JBE.Sys_TimeCount.count50ms++;
	JBE.Sys_TimeCount.count100ms++;
	JBE.Sys_TimeCount.count200ms++;
	JBE.Sys_TimeCount.count500ms++;
	JBE.Sys_TimeCount.count1s++;
	JBE.Sys_TimeCount.count5s++;
	LED_Low_Light(4);
	if (JBE.Sys_TimeCount.count1ms >= 1)
	{
		Is_IIC_OverTime();
		JBE.TimeEvent_t.Time_1ms_Event = True;
		JBE.Sys_TimeCount.count1ms = 0;
	}
	if (JBE.Sys_TimeCount.count10ms >= 10)
	{
		JBE.TimeEvent_t.Time_10ms_Event = True;
		JBE.Sys_TimeCount.count10ms = 0;
	}
	if (JBE.Sys_TimeCount.count20ms >= 20)
	{
		JBE.TimeEvent_t.Time_20ms_Event = True;
		JBE.Sys_TimeCount.count20ms = 0;
	}
	if (JBE.Sys_TimeCount.count30ms >= 30)
	{
		JBE.TimeEvent_t.Time_30ms_Event = True;
		JBE.Sys_TimeCount.count30ms = 0;
	}
	if (JBE.Sys_TimeCount.count40ms >= 40)
	{
		JBE.TimeEvent_t.Time_40ms_Event = True;
		JBE.Sys_TimeCount.count40ms = 0;
	}
	if (JBE.Sys_TimeCount.count50ms >= 50)
	{
		JBE.TimeEvent_t.Time_50ms_Event = True;
		JBE.Sys_TimeCount.count50ms = 0;
	}
	if (JBE.Sys_TimeCount.count100ms >= 100)
	{
		JBE.TimeEvent_t.Time_100ms_Event = True;
		JBE.Sys_TimeCount.count100ms = 0;
	}
	if (JBE.Sys_TimeCount.count200ms >= 300)
	{
		JBE.TimeEvent_t.Time_200ms_Event = True;
		JBE.Sys_TimeCount.count200ms = 0;
	}
	if (JBE.Sys_TimeCount.count500ms >= 500)
	{
		JBE.TimeEvent_t.Time_500ms_Event = True;
		JBE.Sys_TimeCount.count500ms = 0;
	}
	if (JBE.Sys_TimeCount.count1s >= 1000)
	{
		JBE.TimeEvent_t.Time_1s_Event = True;
		JBE.Sys_TimeCount.count1s = 0;
	}
	if (JBE.Sys_TimeCount.count5s >= 5000)
	{
		JBE.TimeEvent_t.Time_5s_Event = True;
		JBE.Sys_TimeCount.count5s = 0;
	}
	// P5 = ~P5;                              // GPIO toggle when interrupt
	_pop_(SFRS);
}

void Time_Process(void)
{
	if (JBE.TimeEvent_t.Time_1ms_Event == True)
	{

		if (JBE.IsTempHut_MeasureStart == True)
		{
			Get_TempHut();
			// JBE.IsTempHut_MeasureStart=False;
		}
		// Bat_PowerDisplay();

		Recieve_Data_Analysis();
		JBE.TimeEvent_t.Time_1ms_Event = False;
	}
	else if (JBE.TimeEvent_t.Time_10ms_Event == True)
	{

		// Lcd_Display();
		KeyProcess();
		LED_Buz();
		BLU_Color();

		JBE.TimeEvent_t.Time_10ms_Event = False;
	}
	else if (JBE.TimeEvent_t.Time_20ms_Event == True)
	{
		// if(JBE.IsTempHut_MeasureStart==True)Get_TempHut();
		Get_CO2Valu();
		Get_Temp_Bat(Channel_Temp);
		Get_Temp_Bat(Channel_BatVolt);

		if (JBE.IsNeedCO2Save == True)
		{
			if (JBE.ISNeedPageSave == True)
			{
				EEPROM_Write_SensorData(CO2, PAGE);
				JBE.ISNeedPageSave = False;
			}
			EEPROM_Write_SensorData(CO2, Sensor);
			Get_Max_Valu_24h();
			Get_Avg_Valu_24h();
			JBE.SaveAddr_P = JBE.SaveAddr_P + 2;
			if (JBE.SaveAddr_P > 95)
			{
				JBE.ISNeedPageSave = True;
				JBE.ISOneCircleSave = True;
				JBE.SaveAddr_P = 0;
			}
			JBE.IsNeedCO2Save = False;
		}

		JBE.TimeEvent_t.Time_20ms_Event = False;
	}

	else if (JBE.TimeEvent_t.Time_200ms_Event == True)
	{

		LCD_Display_TempHub(Get_TempValue(), Get_HubValue());
		JBE.TimeEvent_t.Time_200ms_Event = False;
	}

	else if (JBE.TimeEvent_t.Time_1s_Event == True)
	{
		// LCD_Display_TempHub(Get_TempValue(),TEMP);
		// EEPROM_Write_SensorData(CO2, PAGE);
		static UINT8 times, stable_count;
		JBE.Temp = Get_NTC_Temp();
		JBE.IsTempHut_MeasureStart = True;

		// if(JBE.ISOneCircleSave==0&&JBE.SaveAddr_P==0)JBE.IsNeedCO2Save=True;	//�ո��ϵ��Դ�һ�ζ�����ֵ̼
		JBE.Sys_Second++;
		if (JBE.Sys_Second >= 60)
		{
			JBE.Sys_Minute++;
			stable_count++;
			JBE.Sys_Second = 0;
			JBE.IsNeedCO2Save = True;
		}
		if (JBE.Sys_Minute > 30) // 30���Ӵ�һ��
		{
			JBE.Sys_Minute = 0;
			JBE.IsNeedCO2Save = True;
		}
		if (stable_count > 10)
		{
			JBE.TempHub_Sta = stable;
			stable_count = 11;
		}
		else
			JBE.TempHub_Sta = unstable;
		Get_CO2Valu();

		// Send_NetworkEntry();
		JBE.TimeEvent_t.Time_1s_Event = False;
	}
	else if (JBE.TimeEvent_t.Time_5s_Event == True)
	{

		JBE.TimeEvent_t.Time_5s_Event = False;
	}
}

UINT8 Get_TempHutStartFlag(void)
{
	return JBE.IsTempHut_MeasureStart;
}

void Clr__TempHutStartFlag(void)
{
	JBE.IsTempHut_MeasureStart = False;
}

UINT8 Get_SaveAddr(void)
{
	return JBE.SaveAddr_P;
}

UINT8 Get_OneCircleSaveFlag(void)
{
	return JBE.ISOneCircleSave;
}

void Set_OneCircle_Flag(UINT8 Flag)
{
	JBE.ISOneCircleSave = Flag;
}

void Set_Save_AddrP(UINT8 Addr)
{
	JBE.SaveAddr_P = Addr;
}

UINT16 GET_NtcTEMP(void)
{
	return JBE.Temp;
}

TempHub_Sta_s GET_TempHub_Sta(void)
{
	return JBE.TempHub_Sta;
}
