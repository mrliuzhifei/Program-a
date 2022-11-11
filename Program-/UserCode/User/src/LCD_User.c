#include "ML51.h"
uint16_t disvlue;
//#define TEST_LCD
void Init_Lcd(void)
{
	/*TSLiu */
	/* Enable COM pin MFP */

	MFP_P41_LCD_COM2; //Ӳ���ӵ���COM2
	MFP_P40_LCD_COM3; //Ӳ���ӵ���COM3
	MFP_P47_LCD_COM0;
	MFP_P33_LCD_COM1;
	// MFP_P45_LCD_COM4;  //Ӳ���ӵ���COM0
	// MFP_P44_LCD_COM5;  //Ӳ���ӵ���COM1
	/* Enable SEG pin MFP */
	MFP_P06_LCD_SEG0;
	MFP_P07_LCD_SEG1;
	MFP_P34_LCD_SEG2;
	MFP_P35_LCD_SEG3;
	MFP_P20_LCD_SEG4;
	// MFP_P21_LCD_SEG5;
	MFP_P26_LCD_SEG6;
	MFP_P27_LCD_SEG7;
	MFP_P64_LCD_SEG8;
	MFP_P65_LCD_SEG9;
	MFP_P66_LCD_SEG10;
	MFP_P67_LCD_SEG11;
	MFP_P30_LCD_SEG12;
	MFP_P31_LCD_SEG13;
	MFP_P32_LCD_SEG14;
	// MFP_P33_LCD_SEG15;
	// MFP_P47_LCD_SEG16;
	MFP_P44_LCD_SEG30;
	MFP_P45_LCD_SEG31;
	MFP_P46_LCD_SEG17;
	MFP_P17_LCD_SEG18;
	MFP_P16_LCD_SEG19;
	MFP_P15_LCD_SEG20;
	MFP_P14_LCD_SEG21;
	MFP_P60_LCD_SEG22;
	MFP_P61_LCD_SEG23;
	MFP_P62_LCD_SEG24; //Ӳ���ӵ���SEG5

	LCD_Clock_Setting(LIRC, LCD_FREQ_DIV4);
	LCD_Open(TYPE_B, AVDD_PIN, LCD_CPVOL_3_4V, BIAS_1_3, LCD_4COM);
	LCD_Current_Mode(Buffer_Mode, TurnOn_All);
	// LCD_Blink(Enable,3);
	LCD_Enable();
	LCD_SetAllPixels(Enable);
	// printf("\n LCDINIT OK!");
}

void Lcd_Display(void)
{
	LCD_SetAllPixels(Disable);
	// LCD_SetOnePixel(LCD_COM0, SEG_CO2, Enable);         //����CO2ͼ��
}

void ShowDigit(UINT8 digitPlace, UINT8 num, UINT8 tempUnit)
{

	UINT8 segABCD = 0;

	UINT8 segEFG = 0;

	// �������?

	// if ((digitPlace < 1 || digitPlace > 3) ||

	//   (num > 9)) {

	// return;

	// ����ʵ�ʵ��������ӣ��Լ�LCDģ��Ķ������ȷ���ı���ЩSEG������

	// ÿ��SEG��һ��8λ���ݣ�1/4Dutyʱ����4λ��ʹ�ã���4λ�����ݾ��Ƕ������COM1-4������

	switch (digitPlace)
	{
	case 1:
		segABCD = 2;
		segEFG = 1;
		break;

	case 2:
		segABCD = 4;
		segEFG = 3;
		break;

	case 3:
		segABCD = 6;
		segEFG = 24;
		break;

	case 4:
		segABCD = 8;
		segEFG = 7;
		break;
	case 5:
		segABCD = 11;
		segEFG = 10;
		break;
	case 6:
		segABCD = 13;
		segEFG = 12;
		break;
	case 7:
		// segABCD = 15;
		segABCD = 30;
		segEFG = 14;
		break;
	case 8:
		segABCD = 18;
		segEFG = 17;
		break;

	case 9:
		segABCD = 20;
		segEFG = 19;
		break;
	case 10:
		segEFG = 10;
		break;
	case 11:
		segEFG = 17;
		break;
	default:

		break;
	}

	// ȷ����SEG�Ժ󣬿�ʼ�������ָ�������(��ʵ����ȫ����������������һ��code�����Ｔ��)

	// ����ABCD������(COM0-A,COM1-B,COM2-C,COM3-D)
	SFRS = 3;
	LCDPTR = segABCD;

	switch (num)
	{
	case 0:
		LCDDAT = LCD_COM0 | LCD_COM1 | LCD_COM2 | LCD_COM3;
		break;
	case 1:

		LCDDAT = LCD_COM1 | LCD_COM2;
		break;
	case 2:
		LCDDAT = LCD_COM0 | LCD_COM1 | LCD_COM3;
		break;
	case 3:
		LCDDAT = LCD_COM0 | LCD_COM1 | LCD_COM2 | LCD_COM3;
		break;
	case 4:
		LCDDAT = LCD_COM1 | LCD_COM2;
		break;
	case 5:
		LCDDAT = LCD_COM0 | LCD_COM2 | LCD_COM3;
		break;
	case 6:
		LCDDAT = LCD_COM0 | LCD_COM2 | LCD_COM3;
		break;
	case 7:
		LCDDAT = LCD_COM0 | LCD_COM1 | LCD_COM2;
		break;
	case 8:
		LCDDAT = LCD_COM0 | LCD_COM1 | LCD_COM2 | LCD_COM3;
		break;
	case 9:
		LCDDAT = LCD_COM0 | LCD_COM1 | LCD_COM2 | LCD_COM3;
		break;

	default:

		break;
	}

	// ����EFG������(COM1-F,COM2-G,COM3-E)

	SFRS = 3;
	LCDPTR = segEFG;

	switch (num)

	{
	case 0:
		LCDDAT = LCD_COM1 | LCD_COM3;
		break;
	case 1:
		LCDDAT &= 0x00;
		break;
	case 2:
		LCDDAT = LCD_COM2 | LCD_COM3;
		break;
	case 3:
		LCDDAT = LCD_COM2;
		break;
	case 4:
		LCDDAT = LCD_COM1 | LCD_COM2;
		break;
	case 5:
		LCDDAT = LCD_COM1 | LCD_COM2;
		break;
	case 6:
		LCDDAT = LCD_COM1 | LCD_COM2 | LCD_COM3;
		break;
	case 7:
		LCDDAT = 0x00;
		break;
	case 8:
		LCDDAT = LCD_COM1 | LCD_COM2 | LCD_COM3;
		break;
	case 9:
		LCDDAT = LCD_COM1 | LCD_COM2;
		break;
	default:

		break;
	}

	// �������϶ȷ��Ŷ�����(COM0-SEG0,2,4)

	// ע������Ҫ�û����?��Ȼ�Ḳ��֮ǰ���úõ����ֲ��ֵĶ�������

	switch (tempUnit)

	{

		/* case 0xC:
			 SFRS = 3;
			 LCDPTR  = 31; // 4A�� set

			 LCDDAT |= LCD_COM2|LCD_COM3;

			 LCDPTR  = 14; // 4D�� set

			 LCDDAT |= LCD_COM0;

			 //LCDPTR  = 0; // 4G�� clear

			 //LCDDAT &= ~COM0;

			 break;

		 case 0xF:

			 LCDPTR  = 2; // 4A�� set

			 LCDDAT |= LCD_COM0;

			 LCDPTR  = 4; // 4D�� clear

			 LCDDAT &= ~LCD_COM0;

			 LCDPTR  = 0; // 4G�� set

			 LCDDAT |= LCD_COM0;

			 break;

		 default:



			 break;*/
	}
}

void TempHub_Display_Off(void)
{

	LCD_SetOnePixel(0, 9, Disable);
	LCD_SetOnePixel(0, 21, Disable);
	LCD_SetOnePixel(1, 21, Disable);
	LCD_SetOnePixel(0, 31, Disable);
	LCD_SetOnePixel(1, 31, Disable);

	LCD_SetOnePixel(2, 31, Disable);
	LCD_SetOnePixel(3, 31, Disable);
	//LCD_SetOnePixel(0, 23, Disable);
	LCD_SetOnePixel(0, 25, Disable);

	LCD_SetOnePixel(0, 10, Disable);
	LCD_SetOnePixel(1, 10, Disable);
	LCD_SetOnePixel(2, 10, Disable);
	LCD_SetOnePixel(3, 10, Disable);

	LCD_SetOnePixel(0, 11, Disable);
	LCD_SetOnePixel(1, 11, Disable);
	LCD_SetOnePixel(2, 11, Disable);
	LCD_SetOnePixel(3, 11, Disable);

	LCD_SetOnePixel(0, 12, Disable);
	LCD_SetOnePixel(1, 12, Disable);
	LCD_SetOnePixel(2, 12, Disable);
	LCD_SetOnePixel(3, 12, Disable);

	LCD_SetOnePixel(0, 13, Disable);
	LCD_SetOnePixel(1, 13, Disable);
	LCD_SetOnePixel(2, 13, Disable);
	LCD_SetOnePixel(3, 13, Disable);

	LCD_SetOnePixel(0, 14, Disable);
	LCD_SetOnePixel(1, 14, Disable);
	LCD_SetOnePixel(2, 14, Disable);
	LCD_SetOnePixel(3, 14, Disable);

	LCD_SetOnePixel(0, 30, Disable);
	LCD_SetOnePixel(1, 30, Disable);
	LCD_SetOnePixel(2, 30, Disable);
	LCD_SetOnePixel(3, 30, Disable);

	LCD_SetOnePixel(0, 17, Disable);
	LCD_SetOnePixel(1, 17, Disable);
	LCD_SetOnePixel(2, 17, Disable);
	LCD_SetOnePixel(3, 17, Disable);

	LCD_SetOnePixel(0, 18, Disable);
	LCD_SetOnePixel(1, 18, Disable);
	LCD_SetOnePixel(2, 18, Disable);
	LCD_SetOnePixel(3, 18, Disable);

	LCD_SetOnePixel(0, 19, Disable);
	LCD_SetOnePixel(1, 19, Disable);
	LCD_SetOnePixel(2, 19, Disable);
	LCD_SetOnePixel(3, 19, Disable);

	LCD_SetOnePixel(0, 20, Disable);
	LCD_SetOnePixel(1, 20, Disable);
	LCD_SetOnePixel(2, 20, Disable);
	LCD_SetOnePixel(3, 20, Disable);
}

void TempHub_Display_On(uint16_t TValue, uint16_t HValue)
{
	unsigned char tgw, tsw, tbw, tqw, hgw, hsw, hbw; //��,segAg,segFg,segAs,segFs,segAb,segFb,segFq;
	unsigned char F_PositFlag = 0;
	if (Get_Unit_Sta() == Unit_C)
	{
	}
	else if (Get_Unit_Sta() == Unit_F)
	{

		if (Get_TempPositive() == False)
		{
			if (TValue * 1.8 > 320)
			{
				F_PositFlag = 1;
				TValue = TValue * 1.8 - 320;
				// LCD_SetOnePixel(0,12, Enable);
			}
			else
			{
				F_PositFlag = 0;
				TValue = 320 - TValue * 1.8;
				// Set_TempPositive();//�����¶�Ϊ���¶�
			}
		}
		else
		{
			F_PositFlag = 0;
			TValue = TValue * 1.8 + 320;
		}
	}
	tqw = TValue / 1000;
	tbw = TValue % 1000 / 100;
	tsw = TValue % 100 / 10;
	tgw = TValue % 10;

	hbw = HValue / 100;
	hsw = HValue % 100 / 10;
	hgw = HValue % 10;
	ShowDigit(7, tgw, 0XC);
	ShowDigit(6, tsw, 0XC);
	if (Get_Unit_Sta() == Unit_C)
	{
		if (Get_TempPositive() == False)
		{
			if (TValue < 100)
			{
				LCD_SetOnePixel(2, 10, Enable);
				LCD_SetOnePixel(0, 12, Disable);
			}
			else
			{
				LCD_SetOnePixel(0, 12, Enable);
				LCD_SetOnePixel(2, 10, Disable);
			}
		}
		LCD_SetOnePixel(0, 14, Enable); //С����
		LCD_SetOnePixel(2, 31, Enable);
		LCD_SetOnePixel(3, 31, Enable);
		LCD_SetOnePixel(1, 31, Disable);
	}
	else
	{
		if (F_PositFlag == 1)
		{
			if (TValue < 100)
			{
				LCD_SetOnePixel(2, 10, Enable);
				LCD_SetOnePixel(0, 12, Disable);
			}
			else
			{
				LCD_SetOnePixel(0, 12, Enable);
				LCD_SetOnePixel(2, 10, Disable);
			}
		}
		LCD_SetOnePixel(0, 14, Enable); //С����
		LCD_SetOnePixel(2, 31, Enable);
		LCD_SetOnePixel(1, 31, Enable);
		LCD_SetOnePixel(3, 31, Disable);
	}

	if (TValue >= 100)
		ShowDigit(5, tbw, 0XC); //��λ��ʾ
	if (TValue >= 1000)
	{
		LCD_SetOnePixel(0, 10, Enable); //ǧλ��ʾ1
	}
	LCD_SetOnePixel(0, 14, Enable); //�¶���ʾ��С����

	ShowDigit(9, hgw, 0);
	ShowDigit(8, hsw, 0);
	if (HValue >= 100)
	{
		LCD_SetOnePixel(0, 17, Enable);
	}
	LCD_SetOnePixel(0, 19, Enable);
	if (((180 < TValue) && (TValue <= 280)) && ((40 <= HValue) && (HValue <= 70)))
	{
		LCD_SetOnePixel(0, 9, Enable);
		LCD_SetOnePixel(0, 21, Disable);
		LCD_SetOnePixel(0, 31, Disable);
		LCD_SetOnePixel(1, 21, Enable);
	}
	if ((((120 < TValue) && (TValue <= 180)) || ((280 < TValue) && (TValue <= 320))) || ((25 < HValue) && (HValue <= 39)) || ((70 < HValue) && (HValue <= 85)))
	{
		LCD_SetOnePixel(0, 9, Disable);
		LCD_SetOnePixel(0, 21, Disable);
		LCD_SetOnePixel(1, 21, Enable);
		LCD_SetOnePixel(0, 31, Enable);
	}
	if (((TValue <= 120) || (320 < TValue)) || ((HValue <= 25)) || ((85 < HValue)))
	{
		LCD_SetOnePixel(0, 9, Disable);
		LCD_SetOnePixel(0, 21, Enable);
		LCD_SetOnePixel(0, 31, Disable);
		LCD_SetOnePixel(1, 21, Enable);
	}
}

void LCD_Display_TempHub(uint16_t TValue, uint16_t HValue)
{

	// unsigned int  test_value;
	static unsigned int test_time;

	if (((Sys_MODE == Sys_MODE_BATPower) && (Is_InToSleep == True)) || (Sys_MODE == Sys_MODE_BATPowerLow))
	{
		LCD_SetAllPixels(Disable);
		return;
	}
	//

	if (GET_TempHub_Sta() == unstable)
	{
		test_time++;
		if (test_time <= 2)
		{
			TempHub_Display_On(TValue, HValue);
		}
		else if (test_time <= 4)
		{
			TempHub_Display_Off();
		}
		else
			test_time = 0;
	}
	else if (GET_TempHub_Sta() == stable)
	{
		TempHub_Display_On(TValue, HValue);
	}

#ifdef TEST_LCD
	test_time++;
	if (test_time == 2)
	{
		test_value = GET_NtcTEMP();
		LCD_SetOnePixel(0, 0, Enable);
	}
	else if (test_time == 4)
	{
		test_value = GET_NtcTEMP();
		LCD_SetOnePixel(1, 0, Enable);
	}
	else if (test_time == 6)
	{
		test_value = GET_NtcTEMP();
		LCD_SetOnePixel(2, 0, Enable);
	}
	else if (test_time == 8)
	{
		test_value = GET_NtcTEMP();
		LCD_SetOnePixel(3, 0, Enable);
		test_time = 0;
	}

#else
	// test_time++;
	if (test_time == 2)
	{
		LCD_SetOnePixel(0, 0, Enable);
	}
	else if (test_time == 4)
	{

		LCD_SetOnePixel(0, 0, Enable);
	}
	else if (test_time == 6)
	{

		LCD_SetOnePixel(0, 0, Enable);
	}
	else if (test_time == 8)
	{

		LCD_SetOnePixel(0, 0, Enable);
		// test_time=0;
	}
	LCD_SetOnePixel(0, 0, Enable);
	LCD_SetOnePixel(1, 0, Disable);
	LCD_SetOnePixel(2, 0, Disable);
	LCD_SetOnePixel(3, 0, Disable);

	Display_Co2(Get_Co2DisMod());

	// LCD_SetOnePixel(0, 0, Enable);
#endif
	if (Get_Light_Sta() == Light_off)
	{
		LCD_SetOnePixel(0, 7, Disable);
	}
	else if (Get_Light_Sta() == Light_on)
	{
		LCD_SetOnePixel(0, 7, Enable);
	}

	if (Get_Bell_Sta() == Bell_off)
	{
		LCD_SetOnePixel(0, 24, Disable);
	}
	else if (Get_Bell_Sta() == Bell_on)
	{
		LCD_SetOnePixel(0, 24, Enable);
	}

	LCD_SetOnePixel(0, 1, Disable);
	LCD_SetOnePixel(2, 21, Enable);
	LCD_SetOnePixel(3, 21, Enable);
	LCD_SetOnePixel(2, 22, Enable);
	LCD_SetOnePixel(3, 22, Enable);

	Bat_PowerDisplay();
}
