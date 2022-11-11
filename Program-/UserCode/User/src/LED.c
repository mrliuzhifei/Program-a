#include "ML51.h"
/**********************************************************************
  PWM frequency = Fpwm/((PWMPH,PWMPL) + 1) <Fpwm = Fsys/PWM_CLOCK_DIV>
				= (24MHz/8)/(0x456 + 1)
				= 2.7KHz
***********************************************************************/
unsigned char Is_InToSleep=0;
unsigned int BLU_10_SecondCount = 0;
unsigned char Low_Light=0;
void LED_Init(void)
{
	/*ï¿½ï¿½ï¿½ï¿½ioï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?*/
	// GPIO_SetMode(BLU_PORT_R ,BLU_R_PIN,GPIO_MODE_PUSHPULL);
	// GPIO_SetMode(BLU_PORT_GB ,BLU_G_PIN|BLU_B_PIN,GPIO_MODE_PUSHPULL);
	GPIO_SetMode(LAMP_PORT, LAMP_R_PIN | LAMP_G_PIN | LAMP_B_PIN, GPIO_MODE_PUSHPULL);
	// BLU_ON();
	// LAMP_OFF();
	PWM123_ClockSource(PWM2, 8);
	MFP_P43_PWM2_CH0; // multi function pin define P0.3 as PWM0 channel 2 output
	P43_PUSHPULL_MODE;
	PWM123_ConfigOutputChannel(PWM2, 0, 0x456, 0); // 2.7Kï¿½ï¿½ï¿½PWMÕ¼ï¿½Õ±ï¿½50%
	set_PWM2CON0_LOAD;							   //ï¿½Âµï¿½ï¿½ï¿½ï¿½ï¿½Õ¼ï¿½Õ±ï¿½ï¿½ï¿½ï¿½ï¿½  //ï¿½ï¿½ï¿½ï¿½Ç·ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?
	PWM0_ClockSource(PWM_FSYS, 8);

	MFP_P05_PWM0_CH0; // G                                                // multi function pin define P0.5 as PWM0 channel 0 output
	P05_PUSHPULL_MODE;
	PWM0_ConfigOutputChannel(0, Independent, EdgeAligned, 0x456, 100); // setting PWM channel 0 as 10% duty high of 0x6FF PWM period = 0x00B3

	MFP_P04_PWM0_CH1; // B                                               // multi function pin define P0.5 as PWM0 channel 0 output
	P04_PUSHPULL_MODE;
	PWM0_ConfigOutputChannel(1, Independent, EdgeAligned, 0x456, 100); // setting PWM channel 0 as 10% duty high of 0x6FF PWM period = 0x00B3

	MFP_P36_PWM0_CH5; // R                                                // multi function pin define P0.5 as PWM0 channel 0 output
	P36_PUSHPULL_MODE;
	PWM0_ConfigOutputChannel(5, Independent, EdgeAligned, 0x456, 100);

	// set_PWM0CON0_PWMRUN;//pwm0ï¿½ï¿½ï¿½Ê¹ï¿½ï¿?

	// set_PWM2CON0_PWMRUN;//pwm2ï¿½ï¿½ï¿½Ê¹ï¿½ï¿?
	clr_PWM2CON0_PWM2RUN;
	PWM0_RUN();
}

void Buz_Off(void)
{
	_push_(SFRS);
	clr_PWM2CON0_PWM2RUN;
	PWM123_ConfigOutputChannel(PWM2, 0, 0x456, 0); // 2.Õ¼¿Õ±È0%//clr_PWM2CON0_PWM2RUN;
	set_PWM2CON0_PWMRUN;
	_pop_(SFRS);
	////set_PWM2CON0_LOAD;
}

void Buz_On(void)
{
	static unsigned int BuzTime = 0, i;
	BuzTime++;
	if (BuzTime < 50)
	{
		i++;
		_push_(SFRS);
		PWM123_ConfigOutputChannel(PWM2, 0, 1200 - i, 50); // 2.7KÕ¼¿Õ±È50% set_PWM2CON0_PWMRUN;
		set_PWM2CON0_LOAD;
		set_PWM2CON0_PWMRUN;
		_pop_(SFRS);
	}
	else if (BuzTime < 100)
	{
		i = 0;
		Buz_Off();
	}
	else
	{
		i = 0;
		BuzTime = 0;
	}
}

void LED_Low_Light(unsigned char Level_light)
{
	static unsigned char LED_Times = 0;
	if (Low_Light == True)
	{

		if (Get_Co2Value() <= 1000)
		{
			LED_Times++;
			if (LED_Times == 1)
			{
				LAMP_GREEN_ON();
			}
			else if (LED_Times <= Level_light)
			{

				LAMP_OFF();
			}
			else
			{
				LED_Times = 0;
			}
		}
		else if (Get_Co2Value() <= 1400)
		{
			LED_Times++;
			if (LED_Times == 1)
			{

				LAMP_YELLOW_ON();
			}
			else if (LED_Times <= Level_light)
			{

				LAMP_OFF();
			}
			else
			{
				LED_Times = 0;
			}
		}
		else if (Get_Co2Value() <= 1800)
		{
			LED_Times++;
			if (LED_Times == 1)
			{

				
					LAMP_RED_ON();
			}
			else if (LED_Times <= Level_light)
			{

				LAMP_OFF();
			}
			else
			{
				LED_Times = 0;
			}
		}
		else
		{
			LED_Times++;
			if (LED_Times == 1)
			{
				
					LAMP_RED_ON();
			}
			else if (LED_Times <= Level_light)
			{

				LAMP_OFF();
			}
			else
			{
				LED_Times = 0;
			}
		}
	}
}

void LED_Buz(void)
{
	switch (Sys_MODE)
	{
	case Sys_MODE_ACPower:
		Low_Light = False;
		if (Get_Light_Sta() == Light_on)
		{
			if (Get_Co2Value() <= 1000)
			{
				LAMP_GREEN_ON();
			}
			else if (Get_Co2Value() <= 1400)
			{
				LAMP_YELLOW_ON();
			}
			else if (Get_Co2Value() <= 1800)
			{
				LAMP_RED_ON();
			}
			else
			{
				LAMP_RED_ON();
			}
		}
		else if (Get_Light_Sta() == Light_off)
		{
			LAMP_OFF();
			// clr_PWM2CON0_PWM2RUN;
		}

		if (Get_Co2Value() > 1800)
		{
			if (Get_Bell_Sta() == Bell_off)
			{
				Buz_Off();
			}
			else if (Get_Bell_Sta() == Bell_on)
			{
				Buz_On();
			}
		}
		else
		{
			Buz_Off();
		}
		break;
	case Sys_MODE_BATPower:

		if (Get_Light_Sta() == Light_on)
		{  
			if (BLU_10_SecondCount>= BLU_10_Second)
			{
				Low_Light = True;
			}
			else 
			{
             Low_Light = False;
             if (Get_Co2Value() <= 1000)
			 {
				LAMP_GREEN_ON();
			 }
			 else if (Get_Co2Value() <= 1400)
			 {
				LAMP_YELLOW_ON();
			 }
			 else if (Get_Co2Value() <= 1800)
			 {
				LAMP_RED_ON();
			 }
			 else
			 {
				LAMP_RED_ON();
			 }
			}
		}
		else if (Get_Light_Sta() == Light_off)
		{
			Low_Light = False;
			LAMP_OFF();
			// clr_PWM2CON0_PWM2RUN;
		}

		if (Get_Co2Value() > 1800)
		{
			if (Get_Bell_Sta() == Bell_off)
			{
				Buz_Off();
			}
			else if (Get_Bell_Sta() == Bell_on)
			{
				Buz_On();
			}
		}
		else
		{
			Buz_Off();
		}

		break;
	case Sys_MODE_BATPowerLow:
		Low_Light = False;
		LAMP_OFF();
		Buz_Off();
		break;
	default:
		Low_Light = False;
		LAMP_OFF();
		Buz_Off();
		break;
	}
}
void RGB_Color(Color_s RGBColor)
{
	_push_(SFRS);
	PWM0_STOP();
	switch (RGBColor)
	{
	case White:
		PWM0_ConfigOutputChannel(0, Independent, EdgeAligned, 0x456, 100); // G
		PWM0_ConfigOutputChannel(1, Independent, EdgeAligned, 0x456, 100); // B
		PWM0_ConfigOutputChannel(5, Independent, EdgeAligned, 0x456, 100); // R
		break;
	case Red:
		PWM0_ConfigOutputChannel(0, Independent, EdgeAligned, 0x456, 0);   // G
		PWM0_ConfigOutputChannel(1, Independent, EdgeAligned, 0x456, 0);   // B
		PWM0_ConfigOutputChannel(5, Independent, EdgeAligned, 0x456, 100); // R
		break;
	case Green:
		PWM0_ConfigOutputChannel(0, Independent, EdgeAligned, 0x456, 100); // G
		PWM0_ConfigOutputChannel(1, Independent, EdgeAligned, 0x456, 0);   // B
		PWM0_ConfigOutputChannel(5, Independent, EdgeAligned, 0x456, 0);   // R
		break;
	case Blue:
		PWM0_ConfigOutputChannel(0, Independent, EdgeAligned, 0x456, 0);   // G
		PWM0_ConfigOutputChannel(1, Independent, EdgeAligned, 0x456, 100); // B
		PWM0_ConfigOutputChannel(5, Independent, EdgeAligned, 0x456, 0);   // R
		break;
	case Yellow:
		PWM0_ConfigOutputChannel(0, Independent, EdgeAligned, 0x456, 100); // G
		PWM0_ConfigOutputChannel(1, Independent, EdgeAligned, 0x456, 0);   // B
		PWM0_ConfigOutputChannel(5, Independent, EdgeAligned, 0x456, 100); // R
		break;
	case Purple:
		PWM0_ConfigOutputChannel(0, Independent, EdgeAligned, 0x456, 0);   // G
		PWM0_ConfigOutputChannel(1, Independent, EdgeAligned, 0x456, 100); // B
		PWM0_ConfigOutputChannel(5, Independent, EdgeAligned, 0x456, 100); // R
		break;
	case Orange:
		PWM0_ConfigOutputChannel(0, Independent, EdgeAligned, 0x456, 70);  // G
		PWM0_ConfigOutputChannel(1, Independent, EdgeAligned, 0x456, 20);  // B
		PWM0_ConfigOutputChannel(5, Independent, EdgeAligned, 0x456, 100); // R
		break;
	case Cyan:
		PWM0_ConfigOutputChannel(0, Independent, EdgeAligned, 0x456, 100); // G
		PWM0_ConfigOutputChannel(1, Independent, EdgeAligned, 0x456, 100); // B
		PWM0_ConfigOutputChannel(5, Independent, EdgeAligned, 0x456, 0);   // R
		break;
	default:
		break;
	}
	PWM0_RUN();
	_pop_(SFRS);
}

void RGB_Color_Off(void)
{
	_push_(SFRS);
	PWM0_STOP();
	PWM0_ConfigOutputChannel(0, Independent, EdgeAligned, 0x456, 0); // G
	PWM0_ConfigOutputChannel(1, Independent, EdgeAligned, 0x456, 0); // B
	PWM0_ConfigOutputChannel(5, Independent, EdgeAligned, 0x456, 0); // R
	PWM0_RUN();
	_pop_(SFRS);
}

void BLU_Color(void)
{
	switch (Sys_MODE)
	{
	case Sys_MODE_ACPower:
		RGB_Color(Get_Color_Sta());
		break;
	case Sys_MODE_BATPower:
		BLU_10_SecondCount++;
		if (BLU_10_SecondCount > BLU_10_Second)
		{
			BLU_10_SecondCount=BLU_10_Second;
			Is_InToSleep=True;
			RGB_Color_Off();
		}
		else
		{
			RGB_Color(Get_Color_Sta());
		}
		break;
	case Sys_MODE_BATPowerLow:
		RGB_Color_Off();
		break;
	default:
		RGB_Color_Off();
		break;
	}
}