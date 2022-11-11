
#include "ML51.h"
#include "BC3602.h"
void main (void) 
{   
	  FsysSelect(FSYS_HIRC); 
      Init_UART1();
      Init_UART0();
	  //Init_UART3();
	  Timer0_Init();
	  LED_Init();
	  Init_Lcd();
      WDT_Init();
	  Init_I2C();
	  CO2_Init();
	  ADC_Init();
	  Init_KeyPort();
	  EEPROM_ReadDataInit();	//��ȡeeprom��ֵ
	  //Init_Wifi();
    CO2_Init();
	  RF_Init();
	  
  while(1)
  {
		FEED_WDT();      //ι��
	  Time_Process();	 //��ѯ
	  BC3602_RF_Process();
  }
}
