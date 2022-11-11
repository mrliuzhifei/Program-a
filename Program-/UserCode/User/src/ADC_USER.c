#include "ML51.h"
/*NTC锟斤拷值锟斤拷*/
Sys_MODE_e Sys_MODE;
const unsigned int NTC_ADCValue[81] = {
	3950, 3890, 3829, 3766, 3703, 3638, 3572, 3506, 3439, 3371,
	3302, 3233, 3164, 3094, 3024, 2954, 2884, 2814, 2745, 2676,
	2607, 2539, 2471, 2403, 2337, 2272, 2207, 2144, 2081, 2019,
	1959, 1899, 1841, 1784, 1728, 1674, 1620, 1569, 1518, 1469,
	1421, 1374, 1328, 1284, 1241, 1200, 1159, 1120, 1082, 1046,
	1010, 976, 941, 910, 879, 848, 820, 791, 764, 737,
	712, 687, 664, 641, 619, 599, 578, 558, 539, 521,
	503, 486, 470, 454, 438, 424, 410, 396, 383, 370,
	357};

typedef struct NTC_Data_s
{
	uint16_t Min_NTC_Val;	// NTC????
	uint16_t Max_NTC_Val;	// NTC??锟斤拷?
	uint16_t NTC_Val_Sum;	// NTC16?锟斤拷??????
	uint16_t NTC_Val;		// NTC???????
	uint16_t NTC_Val_Last;	// NTC????锟斤拷?????
	uint16_t Adval_Ntc[18]; // NTC??????锟斤拷????
	uint8_t Adval_Ntc_Idx;	// NTC?????锟斤拷?
	uint16_t NTC_Temp;		// NTC????
	uint16_t NTC_Init;		// NTC????
	uint8_t NTC_Temp_i;
	uint8_t IsFistGetTemp;
} NTC_Data_s;

typedef struct Bat_Data_s
{
	uint16_t Min_Bat_Val;		// Bat????
	uint16_t Max_Bat_Val;		// Bat??锟斤拷?
	uint32_t Bat_Val_Sum;		// Bat16?锟斤拷??????
	uint16_t Bat_Val;			// Bat???????
	uint16_t Bat_Val_Last;		// Bat????锟斤拷?????
	uint16_t Adval_Bat[18];		// Bat??????锟斤拷????
	uint8_t Adval_Bat_Idx;		// Bat?????锟斤拷?
	uint32_t Bat_Voltage;		// Bat????
	uint8_t IsNeedCheckBatvolt; //?????????????
	BatMod_s BatMod;			//?????
	uint8_t BatPowerPecent;		//????????
								             // uint16_t NTC_Init;             Bat????
								             // uint8_t NTC_Temp_i;
								             // uint8_t IsFistGetTemp;
} Bat_Data_s;

typedef struct ADC_Data_s
{
	NTC_Data_s NTC_Data;
	Bat_Data_s Bat_Data;
	uint8_t Is_Bat_ChargFull;
} ADC_Data_s;

ADC_Data_s ADC_Data;
#define JBE ADC_Data
void Voltage_Display(uint8_t percent);

void ADC_Init(void)
{
	ENABLE_ADC_CH0;
	ADC_Open(ADC_SINGLE, ADC_CHANNEL_4);		//????????????4?????
	ADC_SamplingTime(3, 7);						//???锟斤拷??????
	ADC_Vref_Set(Enable, Enable, Level_2_560V); //?????????????????
	clr_ADCCON0_ADCF;							//????????锟斤拷????
	GPIO_SetMode(BatVoltCheck_PORT, BatVoltCheck_PIN, GPIO_MODE_PUSHPULL);
	P24_QUASI_MODE; // P2.4????????????????????锟斤拷??
	// GPIO_SetMode(Port5 ,BIT1,GPIO_MODE_PUSHPULL);
	// BAT_TEST_ON();
	GPIO_SetMode(Port4, BIT2, GPIO_MODE_PUSHPULL);
	// ENABLE_P63_PULLUP;
	BAT_POWER_ON();
	BatVoltCheck_ON;
	JBE.NTC_Data.IsFistGetTemp = True;
	JBE.Bat_Data.IsNeedCheckBatvolt = False;
}

uint32_t Get_NtcValu(void)
{
	uint32_t ADCdataAIN;
	uint32_t Ntc_valu;
	clr_ADCCON0_ADCF;
	set_ADCCON0_ADCS; // ADC start trig signal
	while (!(ADCCON0 & SET_BIT7))
		;
	ADCdataAIN = (ADCRH << 4) | ADCRL;
	JBE.NTC_Data.Adval_Ntc[JBE.NTC_Data.Adval_Ntc_Idx] = ADCdataAIN;
	JBE.NTC_Data.Adval_Ntc_Idx++;
	Ntc_valu = ADCdataAIN;
	clr_ADCCON0_ADCF;
	clr_ADCCON0_ADCS;
	return Ntc_valu;
}

void ADC_RUN(uint8_t Channel)
{
	uint32_t ADCdataAIN;
	/* if(Channel==Channel_Temp)
	   {
	   ENABLE_ADC_CH4;
	   clr_ADCCON0_ADCF;
       set_ADCCON0_ADCS;                  // ADC start trig signal
       while(!(ADCCON0&SET_BIT7));
       ADCdataAIN = (ADCRH<<4)|ADCRL;
	   JBE.NTC_Data.Adval_Ntc[JBE.NTC_Data.Adval_Ntc_Idx]= ADCdataAIN;
	   JBE.NTC_Data.Adval_Ntc_Idx++;
       clr_ADCCON0_ADCF;
       clr_ADCCON0_ADCS;
	   }*/
	if (Channel == Channel_BatVolt)
	{
		ENABLE_ADC_CH0;
		clr_ADCCON0_ADCF;
		set_ADCCON0_ADCS; // ADC start trig signal
		while (!(ADCCON0 & SET_BIT7))
			;
		ADCdataAIN = (ADCRH << 4) | ADCRL;
		JBE.Bat_Data.Adval_Bat[JBE.Bat_Data.Adval_Bat_Idx] = ADCdataAIN;
		JBE.Bat_Data.Adval_Bat_Idx++;
		clr_ADCCON0_ADCF;
		clr_ADCCON0_ADCS;
	}
}

void Get_Temp_Bat(uint8_t Channel)
{
	uint8_t i;
	switch (Channel)
	{
	case Channel_Temp:
	{
		ADC_RUN(Channel_Temp); //目前没锟斤拷ntc
		if (JBE.NTC_Data.Adval_Ntc_Idx >= 18)
		{
			JBE.NTC_Data.Adval_Ntc_Idx = 0;
			JBE.NTC_Data.Min_NTC_Val = JBE.NTC_Data.Adval_Ntc[0]; //???????锟斤拷??????
			JBE.NTC_Data.Max_NTC_Val = JBE.NTC_Data.Adval_Ntc[0];
			JBE.NTC_Data.NTC_Val_Sum = 0; //????????????
			for (i = 1; i < 18; i++)	  //????????锟斤拷?
			{
				if (JBE.NTC_Data.Max_NTC_Val < JBE.NTC_Data.Adval_Ntc[i])
					JBE.NTC_Data.Max_NTC_Val = JBE.NTC_Data.Adval_Ntc[i];
				if (JBE.NTC_Data.Min_NTC_Val > JBE.NTC_Data.Adval_Ntc[i])
					JBE.NTC_Data.Min_NTC_Val = JBE.NTC_Data.Adval_Ntc[i];
			}
			for (i = 0; i < 18; i++) //???
			{
				JBE.NTC_Data.NTC_Val_Sum += JBE.NTC_Data.Adval_Ntc[i];
			}
			JBE.NTC_Data.NTC_Val_Sum = JBE.NTC_Data.NTC_Val_Sum - JBE.NTC_Data.Min_NTC_Val - JBE.NTC_Data.Max_NTC_Val; //????????锟斤拷?
			JBE.NTC_Data.NTC_Val_Sum >>= 4;																			   //???????????????锟斤拷?????????4
			JBE.NTC_Data.NTC_Val = JBE.NTC_Data.NTC_Val_Sum;
			JBE.NTC_Data.NTC_Val = 0.9 * JBE.NTC_Data.NTC_Val_Sum + 0.1 * JBE.NTC_Data.NTC_Val_Last;
			JBE.NTC_Data.NTC_Val_Last = JBE.NTC_Data.NTC_Val;
			for (i = 0; i < 80; i++) //?????????
			{
				if ((JBE.NTC_Data.NTC_Val <= NTC_ADCValue[i]) && (JBE.NTC_Data.NTC_Val > NTC_ADCValue[i + 1])) //???????锟斤拷
				{
					if (i < 20) //??????
					{
						JBE.NTC_Data.NTC_Temp = (19 - i) * 10 + 10 * (NTC_ADCValue[i] - JBE.NTC_Data.NTC_Val) / (NTC_ADCValue[i] - NTC_ADCValue[i + 1]);
					}
					else //???????
					{
						JBE.NTC_Data.NTC_Temp = (i - 20) * 10 + (10 * (NTC_ADCValue[i] - JBE.NTC_Data.NTC_Val)) / (NTC_ADCValue[i] - NTC_ADCValue[i + 1]);
						JBE.NTC_Data.NTC_Temp_i = i;
						JBE.NTC_Data.NTC_Temp_i = i;
						return;
					}
				}
			}
		}
		break;
	}
	case Channel_BatVolt:
	{
		uint8_t i;
		ADC_RUN(Channel_BatVolt);                                 //通锟斤拷1 锟斤拷氐锟窖癸拷锟斤拷
		if (JBE.Bat_Data.Adval_Bat_Idx >= 18)
		{
			JBE.Bat_Data.Adval_Bat_Idx = 0;
			JBE.Bat_Data.Min_Bat_Val = JBE.Bat_Data.Adval_Bat[0]; //锟斤拷锟街碉拷锟斤拷锟叫≈碉拷锟斤拷锟?
			JBE.Bat_Data.Max_Bat_Val = JBE.Bat_Data.Adval_Bat[0];
			JBE.Bat_Data.Bat_Val_Sum = 0;                         //锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
			for (i = 1; i < 18; i++)	                          //锟揭筹拷锟斤拷锟斤拷锟叫≈?
			{
				if (JBE.Bat_Data.Max_Bat_Val < JBE.Bat_Data.Adval_Bat[i])
					JBE.Bat_Data.Max_Bat_Val = JBE.Bat_Data.Adval_Bat[i];
				if (JBE.Bat_Data.Min_Bat_Val > JBE.Bat_Data.Adval_Bat[i])
					JBE.Bat_Data.Min_Bat_Val = JBE.Bat_Data.Adval_Bat[i];
			}
			for (i = 0; i < 18; i++)                              //锟斤拷锟?
			{
				JBE.Bat_Data.Bat_Val_Sum += JBE.Bat_Data.Adval_Bat[i];
			}
			JBE.Bat_Data.Bat_Val_Sum = JBE.Bat_Data.Bat_Val_Sum - JBE.Bat_Data.Min_Bat_Val - JBE.Bat_Data.Max_Bat_Val; //锟斤拷去锟斤拷锟斤拷锟叫≈?
			JBE.Bat_Data.Bat_Val_Sum >>= 4;																			   //锟斤拷锟斤拷4位锟斤拷/16
			// JBE.Bat_Data.Bat_Val=JBE.Bat_Data.Bat_Val_Sum;
			JBE.Bat_Data.Bat_Val_Sum = 0.9 * JBE.Bat_Data.Bat_Val_Sum + 0.1 * JBE.Bat_Data.Bat_Val_Last; // 1锟斤拷锟剿诧拷
			JBE.Bat_Data.Bat_Val_Last = JBE.Bat_Data.Bat_Val_Sum;
			JBE.Bat_Data.Bat_Val = JBE.Bat_Data.Bat_Val_Sum * 256 * 20 / 4095; //实锟绞碉拷压值
		}

        if (IsChargOn == True) //充电中
	    {  
		Sys_MODE=Sys_MODE_ACPower;
		BLU_10_SecondCount=0;
		Is_InToSleep=False;
		}
        else if (IsChargOn == False) //放电
		{
      if (Sys_MODE!=Sys_MODE_BATPowerLow)
			{
			Sys_MODE=Sys_MODE_BATPower;//电池供电状态	
			}
      if ((JBE.Bat_Data.Bat_Val < 3100)) //在电池供电状态下如果电压低于3.1V,进入供电电压低的状态
			{
			Sys_MODE=Sys_MODE_BATPowerLow;//进入低电压的状态
			}
			else if((JBE.Bat_Data.Bat_Val >= 3500)&&Sys_MODE==Sys_MODE_BATPowerLow)//如果进入了低电压的状态要等电压恢复到3.5V以后才能回到电池供电模式
			{
      Sys_MODE=Sys_MODE_BATPower;
			}
		}
		break;
	}
	}
}

void Bat_PowerDisplay(void)
{

	if (IsChargOn == True) //充电中
	{  
		//Sys_MODE=Sys_MODE_ACPower;
		//BLU_10_SecondCount=0;
		LCD_SetOnePixel(1, 22, Enable); //充电标志显示//
		// LCD_SetOnePixel(3, 23, Enable);	 
		if (JBE.Bat_Data.Bat_Val > 4200) //大于4.2V 显示充满
		{
			JBE.Is_Bat_ChargFull = True;
			JBE.Bat_Data.BatPowerPecent = 100;
		}
		/*没有充满过小于4.2v显示充电,已经充满过一次了小于3.9v再次显示充电*/
		else if ((JBE.Bat_Data.Bat_Val < 4200 && JBE.Is_Bat_ChargFull == False) || (JBE.Bat_Data.Bat_Val < 4100 && JBE.Is_Bat_ChargFull == True))
		{
			if((JBE.Bat_Data.Bat_Val < 4100 && JBE.Is_Bat_ChargFull == True))JBE.Is_Bat_ChargFull = FALSE;//充满标志位清零
			if (JBE.Bat_Data.Bat_Val < 3691)
			{
				JBE.Bat_Data.BatPowerPecent = 0;
			}
			else if (JBE.Bat_Data.Bat_Val < 3757)
			{
				JBE.Bat_Data.BatPowerPecent = 25;
			}
			else if (JBE.Bat_Data.Bat_Val < 3872)
			{
				JBE.Bat_Data.BatPowerPecent = 50;
			}
			else if (JBE.Bat_Data.Bat_Val < 4074)
			{
				JBE.Bat_Data.BatPowerPecent = 75;
			}
			else if (JBE.Bat_Data.Bat_Val < 4200)
			{
				JBE.Bat_Data.BatPowerPecent = 100;
			}
		}

		Voltage_Display(JBE.Bat_Data.BatPowerPecent); //电量显示
	}
	else if (IsChargOn == False) //放电
	{
		
		static uint8_t Pecent80Flag = 0; //锟斤拷志位锟斤拷锟斤拷
		static uint8_t Pecent60Flag = 0;
		static uint8_t Pecent40Flag = 0;
		static uint8_t Pecent20Flag = 0;
		static uint8_t Pecent0Flag = 0;
		JBE.Is_Bat_ChargFull = False;	 //充满标志清零
		LCD_SetOnePixel(1, 22, Disable); //
       // Sys_MODE=Sys_MODE_BATPower;//电池供电状态
		if (JBE.Bat_Data.Bat_Val > 3900) //大于3.9V显示满电
		{
			if (Pecent80Flag == 0)
			{
				JBE.Bat_Data.BatPowerPecent = 100;
			}
			else if (Pecent80Flag == 1 && JBE.Bat_Data.Bat_Val > 3920) //0.02的电压差作为台阶，电压下降到80%后，如果电压回复必须回复到3.92才显示满电以防止显示抖动
			{
				JBE.Bat_Data.BatPowerPecent = 100;
				Pecent80Flag = 0;
				Pecent60Flag = 0;
				Pecent40Flag = 0;
				Pecent20Flag = 0;
				Pecent0Flag = 0;
			}
			else
			{
				JBE.Bat_Data.BatPowerPecent = 75;
			}
		}
		else if (JBE.Bat_Data.Bat_Val > 3705)
		{
			Pecent80Flag = 1;
			if (Pecent60Flag == 0)
			{
				JBE.Bat_Data.BatPowerPecent = 75;
			}
			else if (Pecent60Flag == 1 && JBE.Bat_Data.Bat_Val > 3725)
			{
				JBE.Bat_Data.BatPowerPecent = 75;

				Pecent60Flag = 0;
				Pecent40Flag = 0;
				Pecent20Flag = 0;
				Pecent0Flag = 0;
			}
			else
			{
				JBE.Bat_Data.BatPowerPecent = 50;
			}
		}
		else if (JBE.Bat_Data.Bat_Val > 3600)
		{
			Pecent60Flag = 1;
			if (Pecent40Flag == 0)
			{
				JBE.Bat_Data.BatPowerPecent = 50;
			}
			else if (Pecent40Flag == 1 && JBE.Bat_Data.Bat_Val > 3610)
			{
				JBE.Bat_Data.BatPowerPecent = 50;

				Pecent40Flag = 0;
				Pecent20Flag = 0;
				Pecent0Flag = 0;
			}
			else
			{
				JBE.Bat_Data.BatPowerPecent = 25;
			}
		}
		else if (JBE.Bat_Data.Bat_Val > 3528)
		{
			Pecent40Flag = 1;
			if (Pecent20Flag == 0)
			{
				JBE.Bat_Data.BatPowerPecent = 25;
			}
			else if (Pecent20Flag == 1 && JBE.Bat_Data.Bat_Val > 3550)
			{
				JBE.Bat_Data.BatPowerPecent = 25;

				Pecent20Flag = 0;
				Pecent0Flag = 0;
			}
			else
			{
				JBE.Bat_Data.BatPowerPecent = 0;
			}
		}
		else if (JBE.Bat_Data.Bat_Val > 3320)
		{
			Pecent20Flag = 1;
			if (Pecent0Flag == 0)
			{
				JBE.Bat_Data.BatPowerPecent = 0;
			}
			else if (Pecent0Flag == 1 && JBE.Bat_Data.Bat_Val > 3340)
			{
				JBE.Bat_Data.BatPowerPecent = 0;

				Pecent0Flag = 0;
			}
			else
			{

				JBE.Bat_Data.BatPowerPecent = 0;
			}
		}
		else
		{
			Pecent0Flag = 1;

			JBE.Bat_Data.BatPowerPecent = 0;
            /*
			if ((JBE.Bat_Data.Bat_Val < 3100)&&Sys_MODE==Sys_MODE_BATPower) //在电池供电状态下如果电压低于3.1V,进入供电电压低的状态
			{
				Sys_MODE=Sys_MODE_BATPowerLow;//进入低电压的状态
			}
			else if((JBE.Bat_Data.Bat_Val > 3200)&&Sys_MODE==Sys_MODE_BATPowerLow)//如果进入了低电压的状态要等电压恢复到3.2V以后才能回到电池供电模式
			{
             Sys_MODE=Sys_MODE_BATPower;
			}
			*/
		}
		Voltage_Display(JBE.Bat_Data.BatPowerPecent); //电量显示
	}
}

uint16_t Get_NTC_Temp(void)
{
	if (JBE.NTC_Data.IsFistGetTemp == True)
	{
		JBE.NTC_Data.NTC_Init = JBE.NTC_Data.NTC_Temp;
		JBE.NTC_Data.IsFistGetTemp = False;
	}
	return JBE.NTC_Data.NTC_Temp;
}

uint16_t Get_NTC_Init(void)
{
	return JBE.NTC_Data.NTC_Init;
}

void Voltage_Display(uint8_t percent)
{
	static uint16_t flash_time = 0;
	if (IsChargOn == True) //充电中
	{
		switch (percent)
		{
		case 100:
			if (JBE.Bat_Data.Bat_Val > 4200)
			{
				LCD_SetOnePixel(0, 22, Enable);
				LCD_SetOnePixel(2, 23, Enable);
				LCD_SetOnePixel(3, 23, Enable);
				LCD_SetOnePixel(1, 23, Enable);
				LCD_SetOnePixel(0, 23, Enable);
				JBE.Is_Bat_ChargFull = True;
			}
			else if ((JBE.Bat_Data.Bat_Val <= 4200 && JBE.Is_Bat_ChargFull == False) || (JBE.Bat_Data.Bat_Val < 3900 && JBE.Is_Bat_ChargFull == True))
			{
				LCD_SetOnePixel(0, 22, Enable);
				LCD_SetOnePixel(1, 23, Enable);
				LCD_SetOnePixel(0, 23, Enable);
				LCD_SetOnePixel(3, 23, Enable);
				flash_time++;

				if (flash_time == 1)
				{
					LCD_SetOnePixel(2, 23, Enable);
				}
				else if (flash_time == 2)
				{
					LCD_SetOnePixel(2, 23, Disable);
					flash_time = 0;
				}
			}
			break;

		case 75:
			LCD_SetOnePixel(0, 22, Enable);
			LCD_SetOnePixel(2, 23, Disable);
			LCD_SetOnePixel(0, 23, Enable);
			LCD_SetOnePixel(3, 23, Enable);
			flash_time++;

			if (flash_time == 1)
			{
				LCD_SetOnePixel(1, 23, Enable);
			}
			else if (flash_time == 2)
			{
				LCD_SetOnePixel(1, 23, Disable);
				flash_time = 0;
			}
			break;
		case 50:
			LCD_SetOnePixel(0, 22, Enable);
			LCD_SetOnePixel(2, 23, Disable);
			LCD_SetOnePixel(1, 23, Disable);
			LCD_SetOnePixel(3, 23, Enable);
			flash_time++;

			if (flash_time == 1)
			{
				LCD_SetOnePixel(0, 23, Enable);
			}
			else if (flash_time == 2)
			{
				LCD_SetOnePixel(0, 23, Disable);
				flash_time = 0;
			}

			break;
		case 25:
			flash_time++;

			LCD_SetOnePixel(2, 23, Disable);
			LCD_SetOnePixel(1, 23, Disable);
			LCD_SetOnePixel(0, 23, Disable);
			LCD_SetOnePixel(3, 23, Enable);
			if (flash_time == 1)
			{
				LCD_SetOnePixel(0, 22, Enable);
			}
			else if (flash_time == 2)
			{
				LCD_SetOnePixel(0, 22, Disable);
				flash_time = 0;
			}
			break;
		case 0:

			flash_time++;
			LCD_SetOnePixel(2, 23, Disable);
			LCD_SetOnePixel(1, 23, Disable);
			LCD_SetOnePixel(0, 23, Disable);
			LCD_SetOnePixel(0, 22, Disable);
			if (flash_time == 1)
			{
				LCD_SetOnePixel(3, 23, Enable);
			}
			else if (flash_time == 2)
			{
				LCD_SetOnePixel(3, 23, Disable);
				flash_time = 0;
			}
			break;
		default:
			break;
		}
	}
	else if (IsChargOn == False) //电池供电
	{
		switch (percent)
		{
		case 100:
			LCD_SetOnePixel(0, 22, Enable);
			LCD_SetOnePixel(2, 23, Enable);
			LCD_SetOnePixel(3, 23, Enable);
			LCD_SetOnePixel(1, 23, Enable);
			LCD_SetOnePixel(0, 23, Enable);

			break;
		case 75:
			LCD_SetOnePixel(0, 22, Enable);
			LCD_SetOnePixel(2, 23, Disable);
			LCD_SetOnePixel(3, 23, Enable);
			LCD_SetOnePixel(1, 23, Enable);
			LCD_SetOnePixel(0, 23, Enable);
			break;
		case 50:
			LCD_SetOnePixel(0, 22, Enable);
			LCD_SetOnePixel(2, 23, Disable);
			LCD_SetOnePixel(3, 23, Enable);
			LCD_SetOnePixel(1, 23, Disable);
			LCD_SetOnePixel(0, 23, Enable);
		case 25:
			LCD_SetOnePixel(0, 22, Enable);
			LCD_SetOnePixel(2, 23, Disable);
			LCD_SetOnePixel(3, 23, Enable);
			LCD_SetOnePixel(1, 23, Disable);
			LCD_SetOnePixel(0, 23, Disable);
			break;
		case 0: //
			if (JBE.Bat_Data.Bat_Val > 3320)
			{
				LCD_SetOnePixel(0, 22, Disable);
				LCD_SetOnePixel(2, 23, Disable);
				LCD_SetOnePixel(3, 23, Enable);
				LCD_SetOnePixel(1, 23, Disable);
				LCD_SetOnePixel(0, 23, Disable);
			}
			else
			{
				flash_time++;
				LCD_SetOnePixel(0, 22, Disable);
				LCD_SetOnePixel(2, 23, Disable);
				// LCD_SetOnePixel(3, 23, Enable);
				LCD_SetOnePixel(1, 23, Disable);
				LCD_SetOnePixel(0, 23, Disable);
				if (flash_time == 1)
				{
					LCD_SetOnePixel(1, 22, Enable); //
					LCD_SetOnePixel(3, 23, Enable);
				}
				else if (flash_time == 2)
				{
					LCD_SetOnePixel(1, 22, Disable);
					LCD_SetOnePixel(3, 23, Disable);
					flash_time = 0;
				}
			}

			break;

		default:
			break;
		}
	}
}
