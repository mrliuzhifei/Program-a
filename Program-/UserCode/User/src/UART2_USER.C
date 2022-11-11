#include "ML51.h"
/*for WIFI*/
void Init_UART0(void)
{
    MFP_P01_UART0_TXD;                              /* 设置P0.0和P0.1为UART2端口*/
	  P01_QUASI_MODE; 
    MFP_P00_UART0_RXD;
	  P00_INPUT_MODE; 
    UART_Open(24000000,UART0_Timer3,9600);

    ENABLE_UART0_INTERRUPT;                                   /* Enable UART0 interrupt */
    ENABLE_GLOBAL_INTERRUPT; 
    
}

void Uart0senddata(UINT8* Data,UINT8 len)
 {
   UINT8 i;
   for(i=0;i<len;i++)
	 {
		 UART_Send_Data(UART0, *(Data+i));
//	 send_byte(*(date+i));	 
	 }
 }

