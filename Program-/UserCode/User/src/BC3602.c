#include "BC3602.h"
#include "BC3602_CMD_REG.h"
#include "BC3602_SPI.h"
#include "ML51.h"
//#include "BC3602_CONFIG_TABLE.h"
#include "BC3602_Gateway.h"//

unsigned char RF_TXFIFO[RF_Payload_Length];		//FIFO length defined in Configuration.h
unsigned char RF_RXFIFO[RF_Payload_Length];		//FIFO length defined in Configuration.h
unsigned char RF_RXFIFOSAVE[RF_Payload_Length];		//FIFO length defined in Configuration.h
unsigned char RF_IRQ;



unsigned char mRFSync[RF_SYNC_Length];			//SYNC length defined in Configuration.h
struct define_flag
{
	unsigned char Key_Stas;																	// KEY states
	unsigned char LED_Stas;																	// LED states
	unsigned char RFmode;																	// RF_MODE
	unsigned char RFstep;																	// RF process step
	unsigned char fRFINT:1;																	// RF interrupt flag
	unsigned char f8ms:1;																	// MCU timebase time-out 8ms flag
};
struct define_flag Mflag;


/*------------------------------------------------------------------*/
/*							 Function								*/
/*------------------------------------------------------------------*/
//*******************************//
// name		:	SpiWriteRegCMD
// input	:	CMD,DATA
// output	:	none
//*******************************//
void SpiWriteRegCMD(unsigned char CMD,unsigned char DATA)
{
	//SpiCSN_low();
	BC3602CSN=0 ;
	SpiWrite(CMD);
	SpiWrite(DATA);
	//SpiCSN_high();
	BC3602CSN=1 ;
}

//*******************************//
// name		:	SpiWriteByte
// input	:	CMD
// output	:	ReadDATA
//*******************************//
unsigned char SpiReadRegCMD(unsigned char CMD)
{
	unsigned char ReadDATA;
	//SpiCSN_low();
	BC3602CSN=0 ;
	SpiWrite(CMD);
	ReadDATA = SpiRead();
	//SpiCSN_high();
	BC3602CSN=1 ;
	return	ReadDATA;
}

//*******************************//
// name		:	SpiWriteStrobeCMD
// input	:	CMD
// output	:	none
//*******************************//
void SpiWriteStrobeCMD(unsigned char CMD)
{
	//SpiCSN_low();
	BC3602CSN=0 ;
	SpiWrite(CMD);
	//SpiCSN_high();
	BC3602CSN=1 ;
}



//*******************************//
// name		:	RFWriteFreq
// input	:	none
// output	:	none
//*******************************//
/*void RFWriteFreq(void)
{
	unsigned char a=0;
	a = SpiReadRegCMD(READ_REGS_CMD|OM_REGS);           //读取OM_REGS工作模式寄存器状�?
	a = (a&0x9f)|BAND_SEL;                              //取BAND_SEL�?0X20，即选择433M频�??
	SpiWriteRegCMD(WRITE_REGS_CMD|OM_REGS,a);           //把配�?写入OM_REGS工作模式寄存�?  设置频�?�成�?
	
	for(a=0;a<(sizeof Frequency_REGS_TALBE/2);a++)	SpiWriteRegCMD((WRITE_REGS_CMD|(Frequency_REGS_TALBE[a]>>8)), Frequency_REGS_TALBE[a]);
}*/

//*******************************//
// name		:	RFSetDRPram
// input	:	none
// output	:	none
//*******************************//
/*void RFSetDRPram(void)
{
	unsigned char a=0;
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3602_BANK0);   //选择BANK0
	for(a=0;a<(sizeof DM_REGS_TALBE/2);a++)	SpiWriteRegCMD((WRITE_REGS_CMD|(DM_REGS_TALBE[a]>>8)), DM_REGS_TALBE[a]);
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3602_BANK1);
	for(a=0;a<(sizeof FCF_REGS_TALBE/2);a++)	SpiWriteRegCMD((WRITE_REGS_CMD|(FCF_REGS_TALBE[a]>>8)), FCF_REGS_TALBE[a]);
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3602_BANK2);
	for(a=0;a<(sizeof CBPF_REGS_TALBE/2);a++)	SpiWriteRegCMD((WRITE_REGS_CMD|(CBPF_REGS_TALBE[a]>>8)), CBPF_REGS_TALBE[a]);
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3602_BANK0);
}*/

/*void RFSetPower(unsigned char band_sel,unsigned char power)
{
	SpiWriteRegCMD(WRITE_REGS_CMD|TX2_REGS,TxPowerValue[band_sel][power]);
}*/

//*******************************//
// name		:	RFXtalReady
// input	:	none
// output	:	none
//*******************************//
/*void RFXtalReady(void)
{
	unsigned char a=0;
	while(a == 0)
	{
		a=SpiReadRegCMD(READ_REGS_CMD|RC1_REGS);
		a &= 0x20;
	}
}*/

//*******************************//
// name		:	RFCalibration
// input	:	none
// output	:	none
//*******************************//
/*void RFCalibration(void)
{
	unsigned char a=0;
	a=SpiReadRegCMD(READ_REGS_CMD|OM_REGS);
	a |= 0x08;
	SpiWriteRegCMD(WRITE_REGS_CMD|OM_REGS,a);
	while(a)
	{
		a=SpiReadRegCMD(READ_REGS_CMD|OM_REGS);
		a &= 0x08;
	}
}*/


//*******************************//
// name		:	LircCalibration
// input	:	none
// output	:	none
//*******************************//
/*void LircCalibration(void)
{
	unsigned char a=0;
	a=SpiReadRegCMD(READ_REGS_CMD|XO3_REGS);
	a |= 0x81;
	SpiWriteRegCMD(WRITE_REGS_CMD|XO3_REGS,a);
	while(a)
	{
		a=SpiReadRegCMD(READ_REGS_CMD|XO3_REGS);
		a &= 0x80;
	}
}*/

//*******************************//
// name		:	RFGetClrRFIrq
// input	:	none
// output	:	irq_status
//*******************************//
/*unsigned char RFGetClrRFIrq(void)
{
	unsigned char irq;
	
	irq=SpiReadRegCMD(READ_REGS_CMD|IRQ3_REGS);
	SpiWriteRegCMD(WRITE_REGS_CMD|IRQ3_REGS,irq);
	
	return irq;
}*/

void BC3602_InterfaceConfigure(void)//�ӿ�����
{
	
	/*Syncword[0]=0x54;//��ʱ���ڴ�
	Syncword[1]=0x52;//��ʱ���ڴ�
	Syncword[2]=0x4e;//��ʱ���ڴ�
	Syncword[3]=0x49;//��ʱ���ڴ�*/
	
	
	BC3602_SPI_IOConfig();
	
	//GCC_DELAY(200);	//100us 
	  
	DISABLE_GLOBAL_INTERRUPT;                 // �ر�ȫ���ж�
	 	/**********�����½����ж�**************************/
	MFP_P13_GPIO;
    GPIO_SetMode(Port1,BIT3,GPIO_MODE_INPUT);                              /* set P2.5 as input output mode */
    GPIO_Pull_Enable(Port1,BIT3,PULLUP );
    GPIO_EnableInt(PIT0,FALLING,EDGE,1,3);         /* Setting pin intterrupt channel 0 as P2.5 falling edge trigger. */
	  

    ENABLE_GLOBAL_INTERRUPT;      
	 
	BC3602_WriteRegister(CONFIG_REGS,0x00);
	
	//reset 1.2V	
	BC3602_WriteRegister(CLOCK_CTL_REGS,0x10);
	BC3602_WriteRegister(CLOCK_CTL_REGS,0x11);
	BC3602_WriteRegister(CLOCK_CTL_REGS,0x10);	
	
	// enter light sleep mode 
	BC3602_EnterLightSleep();
	
	//software reset
	BC3602_SoftwareReset();

	// register bank change to 0 
	BC3602_RegisterBank(REGS_BANK0);

	BC3602_CrystalReady();
}

void RF_Init(void)
{
	//SpiIOInti();												     // RF SPI I/O Initialization
	 BC3602_SPI_IOConfig();                    // SPI��ʼ��
	BC3602_EnterLightSleep();
	BC3602_RegisterBank(REGS_BANK0);
	BC3602_WriteRegister(CONFIG_REGS,0x40);//0x00 AGCʹ�� RX����ģʽ����
	BC3602_WriteRegister(CLOCK_CTL_REGS,0xb0);//
	//BC3602_WriteRegister(IRQ_CTL_REGS,0x02);//����ز�(Carry)
	//BC3602_WriteRegister(IRQ_CTL_REGS,0x06);//���ǰ����(Preamble)
	BC3602_WriteRegister(IRQ_CTL_REGS,0x0a);//���ͬ����(SYNCWORD)
	BC3602_WriteRegister(IRQ_ENABLE_REGS,0x01);//TX���IRQ
	BC3602_WriteRegister(IRQ_STATUS_REGS,0x00);//0x04
//	BC3602_WriteRegister(GPIO12_CTL_REGS,0xe9);//4 line spi,GIO1 is SDO and GIO2 is IRQ
	BC3602_WriteRegister(GPIO12_CTL_REGS,0x45);//0x06 3 line spi,GIO1 is no function and GIO2 is IRQ
	BC3602_WriteRegister(GPIO34_CTL_REGS,0x00);//GIO4 is no function and GIO3 is no function
	BC3602_WriteRegister(GPIO_PULL_UP_REGS,0xb9);//0b10111001
	BC3602_WriteRegister(TX_FIFO_SA_REGS,0x00);//
	BC3602_WriteRegister(FIFO2_CTL_REGS,0x01);//FIFO���ȱ߽�ѡ�� TX 8 byte and RX 8 byte
	BC3602_WriteRegister(PREAMBLE_LENG_REGS,0x03);//0x0b ǰ���볤��=3+1=4 byte
	BC3602_WriteRegister(PACKET_CTL2_REGS,0x07);//ͬ����=4 byte,RXǰ�����ⳤ��=4 byte
	BC3602_WriteRegister(PACKET_CTL3_REGS,0x00);//
	BC3602_WriteRegister(PACKET_CTL4_REGS,0x36);//
	BC3602_WriteRegister(TX_DATA_LENG_REGS,0x05);//
	BC3602_WriteRegister(RX_DATA_LENG_REGS,0x00);//
	BC3602_WriteRegister(TRX_MODE_DELAY_REGS,0x20);//
	BC3602_WriteRegister(HEADER_ADDR0_REGS,0x00);//
	BC3602_WriteRegister(HEADER_ADDR1_REGS,0x00);//
	BC3602_WriteRegister(MODULATOR_CTL1_REGS,0x05);//MOD1 DataRate=fXTAL/((XODIV2+1)*32*(DTR[8:0]+1))=19.2M/((0+1)*32*(5+1))=100K  for 19.2M
	BC3602_WriteRegister(MODULATOR_CTL2_REGS,0x60);//
	BC3602_WriteRegister(MODULATOR_CTL3_REGS,0x66);//
	BC3602_WriteRegister(DEMOULATOR_CTL1_REGS,0x0b);//for 19.2M
	BC3602_WriteRegister(DEMOULATOR_CTL2_REGS,0x40);//
	BC3602_WriteRegister(DEMOULATOR_CTL3_REGS,0xe0);//
	BC3602_WriteRegister(DEMOULATOR_CTL4_REGS,0x18);//
	BC3602_WriteRegister(DEMOULATOR_CTL5_REGS,0x30);//
	BC3602_WriteRegister(DEMOULATOR_CTL8_REGS,0x15);//
	BC3602_WriteRegister(OPERATION_CTL_REGS,0x20);//
	BC3602_WriteRegister(FRACTIONAL_N_DN_REGS,0x16 );//433.301M/19.2M=22.5678 �������� for 19.2M///////***//////////
	BC3602_WriteRegister(FRACTIONAL_N_DKL_REGS,0x8c);//
	BC3602_WriteRegister(FRACTIONAL_N_DKM_REGS,0x15);//
	BC3602_WriteRegister(FRACTIONAL_N_DKH_REGS,0x09);//
	BC3602_WriteRegister(MODE_STATUS_REGS,2);        // for 19.2M 2020.12.26
	BC3602_WriteRegister(RSSI_CTL_REGS,10);          //
	BC3602_WriteRegister(RSSI_VALUE_REGS,0);         //
	BC3602_WriteRegister(RSSI_VALUE_ID_REGS,0);      //
	BC3602_WriteRegister(ATR_CONTROL_REGS,0x4a);     //ATR1 �Զ�TX/RX���ƼĴ���
	
//	BC3602_WriteRegister(ATR_CYCLE_L_REGS,0x0a);//ATR2 ATRCT��ʱ������ֵ���ֽ�
//	BC3602_WriteRegister(ATR_CYCLE_H_REGS,0x77);//ATR3 ATRCT��ʱ������ֵ���ֽ� ��������=ATRCLK*(ATRCYC[15:0]+1)=3.1�� ���� 20220506
	BC3602_WriteRegister(ATR_CYCLE_L_REGS,0x3a);//ATR2 ATRCT��ʱ������ֵ���ֽ�
	BC3602_WriteRegister(ATR_CYCLE_H_REGS,0x63);//ATR3 ATRCT��ʱ������ֵ���ֽ� ��������=ATRCLK*(ATRCYC[15:0]+1)=(1/8192Hz)*(25402+1)=3.101��//3.101*8192-1=25402
	
	BC3602_WriteRegister(ATR_EACTIVE_L_REGS,0x05);//ATR5 RX��չ���ڵ��ֽ�
	BC3602_WriteRegister(ATR_EACTIVE_H_REGS,0x00);//ATR6 RX��չ���ڸ��ֽ� ��չ����=��λʱ��*ATRRXEP[15:0]+1)=6*250us=1.5ms
	
	BC3602_WriteRegister(ARK_CONTROL_REGS,0x72);//ATR7 ARK�ظ����ڴ���=8 �Զ������ӳ�ʱ��=488us �Զ��ط�/ACK=����
//	BC3602_WriteRegister(ARK_CONTROL_REGS,0x00);//ATR7 ARK�ظ����ڴ���=1 �Զ������ӳ�ʱ��=244us �Զ��ط�/ACK=����	
	
	BC3602_WriteRegister(ARK_ACTIVE_REGS,0x27);//ATR8 ARK RX��Ч���� ��Ч����=��λʱ��ARKRXAP[7:0]+1)=40*250us=10ms
	BC3602_WriteRegister(ATR_CNTR_L_REGS,0);//ATR9 ATR���ڶ�ʱ�����ֽ�
	BC3602_WriteRegister(ATR_CNTR_H_REGS,0);//ATR10 ATR���ڶ�ʱ�����ֽ�
//	BC3602_WriteRegister(ATR_ACTIVE_L_REGS,0x09);//ATR4 RX��Ч���ڵ��ֽ�
//	BC3602_WriteRegister(ATR_ACTIVE_H_REGS,0);//ATR11 RX��Ч���ڸ��ֽ� ��Ч����=��λʱ��*ATRRXAP[10:0]+1=10*250us=2.5ms //20201225 7%�����ղ��� @��������3.7��

//	BC3602_WriteRegister(ATR_ACTIVE_L_REGS,0x13);//ATR4 RX��Ч���ڵ��ֽ�
//	BC3602_WriteRegister(ATR_ACTIVE_H_REGS,0);//ATR11 RX��Ч���ڸ��ֽ� ��Ч����=��λʱ��*ATRRXAP[10:0]+1=20*250us=5.0ms //20220505 4��1�����20��,��2��,ȫOK @��������3.7��

//	BC3602_WriteRegister(ATR_ACTIVE_L_REGS,0x0b);//ATR4 RX��Ч���ڵ��ֽ�
//	BC3602_WriteRegister(ATR_ACTIVE_H_REGS,0);//ATR11 RX��Ч���ڸ��ֽ� ��Ч����=��λʱ��*ATRRXAP[10:0]+1=12*250us=3.0ms //20220506 4��1�����20��,��2��,ÿ�����1���ղ��� @��������3.7��
	
//	BC3602_WriteRegister(ATR_ACTIVE_L_REGS,0x0d);//ATR4 RX��Ч���ڵ��ֽ�
//	BC3602_WriteRegister(ATR_ACTIVE_H_REGS,0);//ATR11 RX��Ч���ڸ��ֽ� ��Ч����=��λʱ��*ATRRXAP[10:0]+1=14*250us=3.5ms //20220506 4��1�����20��,��2��,ȫOK @��������3.7��

//	BC3602_WriteRegister(ATR_ACTIVE_L_REGS,0x09);//ATR4 RX��Ч���ڵ��ֽ�
//	BC3602_WriteRegister(ATR_ACTIVE_H_REGS,0);//ATR11 RX��Ч���ڸ��ֽ� ��Ч����=��λʱ��*ATRRXAP[10:0]+1=10*250us=2.5ms //20201225 5%�����ղ��� @��������3.1��

	BC3602_WriteRegister(ATR_ACTIVE_L_REGS,0x0b);//ATR4 RX��Ч���ڵ��ֽ�
	BC3602_WriteRegister(ATR_ACTIVE_H_REGS,0);//ATR11 RX��Ч���ڸ��ֽ� ��Ч����=��λʱ��*ATRRXAP[10:0]+1=12*250us=3.0ms //20220506 4��1�����20��,��2��,ȫOK @��������3.1��

//	BC3602_WriteRegister(XO_CAP_CTL_REGS,65);//
//	BC3602_WriteRegister(XO_SEL_CTL_REGS,3);//16.M
	BC3602_WriteRegister(XO_CAP_CTL_REGS,0x1f);//
	BC3602_WriteRegister(XO_SEL_CTL_REGS,0x04);//19.2M
	BC3602_WriteRegister(LIRC_CTL_REGS,26);//
	BC3602_WriteRegister(TX_POWER_CTL_REGS,0x09);//0x0d

	BC3602_RegisterBank(REGS_BANK1);
	BC3602_WriteRegister(AGC_CTL2_REGS,0x40);			//AGC2 //...???
	BC3602_WriteRegister(AGC_CTL3_REGS,0x24);			//AGC3
	BC3602_WriteRegister(AGC_CTL4_REGS,0x11);			//AGC4 ѡ���������
	BC3602_WriteRegister(AGC_CTL5_REGS,0x00);			//AGC5
	BC3602_WriteRegister(AGC_CTL7_REGS,0x30);			//AGC7 //...???
//	BC3602_WriteRegister(FILTER_CTL1_REGS,0x06);		//FCF1 ƽ���˲�������1:1
//	BC3602_WriteRegister(FILTER_CTL2_REGS,0x80);		//FCF2 Ƶ��ƫ�Ʊ����������ֽ� 37.5K@100Kbps for19.2M
	BC3602_WriteRegister(FILTER_CTL3_REGS,0x00);					//FCF3 Ƶ��ƫ�Ʊ����������ֽ�
	BC3602_WriteRegister(FILTER_CTL4_REGS,0x00);					//FCF4
	BC3602_WriteRegister(FILTER_CTL5_REGS,0x00);					//FCF5
	BC3602_WriteRegister(FILTER_CTL6_REGS,0x00);					//FCF6
	BC3602_WriteRegister(FILTER_CTL7_REGS,0x00);					//FCF7
	BC3602_WriteRegister(FILTER_CTL8_REGS,0x00);					//FCF8
	BC3602_WriteRegister(FILTER_CTL9_REGS,0x00);					//FCF9
	BC3602_WriteRegister(FILTER_CTL10_REGS,0x00);					//FCF10
	BC3602_WriteRegister(FILTER_CTL11_REGS,0x00);					//FCF11
	BC3602_WriteRegister(FILTER_CTL12_REGS,0x00);					//FCF12
	BC3602_WriteRegister(FILTER_CTL13_REGS,0x00);//FCF13
	BC3602_WriteRegister(FILTER_CTL14_REGS,0x00);//FCF14
	BC3602_WriteRegister(FILTER_CTL15_REGS,0x00);//FCF15
	BC3602_WriteRegister(FILTER_CTL16_REGS,0x00);//FCF16
	BC3602_WriteRegister(FILTER_CTL17_REGS,0x00);//FCF17
	BC3602_WriteRegister(FILTER_CTL18_REGS,0x00);//FCF18
	BC3602_WriteRegister(FILTER_CTL19_REGS,0x00);//FCF19

	BC3602_WriteRegister(FILTER_CTL1_REGS,0x00);		//FCF1 ƽ���˲�������1:1
	BC3602_WriteRegister(FILTER_CTL2_REGS,0x96);		//FCF2 Ƶ��ƫ�Ʊ����������ֽ� 37.5K@100Kbps for19.2M
/******************����******************************/	
	BC3602_RegisterBank(REGS_BANK2);
	BC3602_WriteRegister(BANK2_RSV1,0x03);
	BC3602_WriteRegister(BANK2_RSV2,0x88);
	BC3602_WriteRegister(BANK2_RSV3,0xA3);
	BC3602_WriteRegister(BANK2_RSV4,0x80);
	BC3602_WriteRegister(BANK2_RSV5,0x16);
	BC3602_WriteRegister(BANK2_RSV6,0x64);//433:0x64 868:0x74
	BC3602_WriteRegister(BANK2_RSV7,0x54);//44h/54h (��100Kbps:54h)
	BC3602_WriteRegister(BANK2_RSV8,0x00);
	BC3602_WriteRegister(BANK2_RSV9,0x64);
	BC3602_WriteRegister(BANK2_RSV10,0xBC);//433:0xBC 868:0x9C
	BC3602_WriteRegister(BANK2_RSV11,0x94);


				 // Entry Deepsleep mode*/
	 Mflag.RFmode = RF_RX;
}






/*void ResetLvRF(void)
{
	unsigned char a=0;
	while(a == 0)
	{
		a=SpiReadRegCMD(READ_REGS_CMD|RC1_REGS);
		a |= 0x01;
		SpiWriteRegCMD(WRITE_REGS_CMD|RC1_REGS,a);
		a=SpiReadRegCMD(READ_REGS_CMD|RC1_REGS);
		a &= 0xFE;
		SpiWriteRegCMD(WRITE_REGS_CMD|RC1_REGS,a);
	}
}*/

//*******************************//
// name		:	BC3602_Config
// input	:	none
// output	:	none
//*******************************//
//��ӦΪ������16λ������sizeof �Ժ��õ�����ʵ��������������������������?���Զ������ܵĵ�����ĸ��?
/*void BC3602_Config(void)
{
	unsigned char a=0;
	//	BC3601 IRQ/IO Configure	//
	for(a=0;a<(sizeof IRQIO_REGS_TALBE/2);a++)	SpiWriteRegCMD((WRITE_REGS_CMD|(IRQIO_REGS_TALBE[a]>>8)), IRQIO_REGS_TALBE[a]);
	//	BC3602 packet format Configure	//
	for(a=0;a<(sizeof PACKET_REGS_TALBE/2);a++)	SpiWriteRegCMD((WRITE_REGS_CMD|(PACKET_REGS_TALBE[a]>>8)), PACKET_REGS_TALBE[a]);
	//	BC3602 common Configure	//
	for(a=0;a<(sizeof COMMON_REGS_TALBE/2);a++)	SpiWriteRegCMD((WRITE_REGS_CMD|(COMMON_REGS_TALBE[a]>>8)), COMMON_REGS_TALBE[a]);
	
	//	BC3602 Bank0 Configure	//
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3602_BANK0);
	for(a=0;a<(sizeof BANK0_REGS_TALBE/2);a++)	SpiWriteRegCMD((WRITE_REGS_CMD|(BANK0_REGS_TALBE[a]>>8)), BANK0_REGS_TALBE[a]);
	//	BC3602 Bank1 Configure	//
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3602_BANK1);
	for(a=0;a<(sizeof BANK1_REGS_TALBE/2);a++)	SpiWriteRegCMD((WRITE_REGS_CMD|(BANK1_REGS_TALBE[a]>>8)), BANK1_REGS_TALBE[a]);
	//	BC3602 Bank2 Configure	//
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3602_BANK2);
	for(a=0;a<(sizeof BANK2_REGS_TALBE/2);a++)	SpiWriteRegCMD((WRITE_REGS_CMD|(BANK2_REGS_TALBE[a]>>8)), BANK2_REGS_TALBE[a]);
	SpiWriteStrobeCMD(REGS_BANK_CMD|BC3602_BANK0);
}*/


//*******************************//
// name		:	RFWriteBuf
// input	:	CMD,length,data[]
// output	:	none
//*******************************//
void RFWriteBuf(unsigned char CMD,unsigned char length,unsigned char Data[])
{
	unsigned char a;
	//SpiCSN_low();
	BC3602CSN=0 ;	
	SpiWrite(CMD);
	for(a=0;a<length;a++) SpiWrite(Data[a]);
	//SpiCSN_high();
	BC3602CSN=1 ;	
}


//*******************************//
// name		:	RFWriteSyncword
// input	:	none
// output	:	none
//*******************************//
/*void RFWriteSyncword(void)
{
	unsigned char a;
	
#if RF_SYNC_BCHcoding
	
	#if (RF_SYNC_Length != 4)
	unsigned char temp1[RF_SYNC_Length];
	#endif
	
	#if (RF_SYNC_Length == 4)
	for(a=0;a<4;a++) mLap[a]=BC3602_SYNCWORD[a];
	BCH32_Syncword();
	RFWriteBuf2(WRITE_SYNCWORD_CMD,RF_SYNC_Length,mRFSync);	
	#endif
	
	#if (RF_SYNC_Length == 6)
	for(a=0;a<4;a++) mLap[a]=BC3602_SYNCWORD[a];
	BCH32_Syncword();
	for(a=0;a<4;a++) temp1[a]=mRFSync[a];
	for(a=0;a<2;a++) mLap[a]=BC3602_SYNCWORD[a+4];
	mLap[2]=0;
	mLap[3]=0;
	BCH32_Syncword();
	for(a=0;a<2;a++) temp1[a+4]=mRFSync[a];
	for(a=0;a<6;a++) mRFSync[a]=temp1[a];
	RFWriteBuf2(WRITE_SYNCWORD_CMD,RF_SYNC_Length,mRFSync);	
	#endif
	
	#if (RF_SYNC_Length == 8)
	for(a=0;a<4;a++) mLap[a]=BC3602_SYNCWORD[a];
	BCH32_Syncword();
	for(a=0;a<4;a++) temp1[a]=mRFSync[a];
	for(a=0;a<4;a++) mLap[a]=BC3602_SYNCWORD[a+4];
	BCH32_Syncword();
	for(a=0;a<4;a++) temp1[a+4]=mRFSync[a];
	for(a=0;a<8;a++) mRFSync[a]=temp1[a];
	RFWriteBuf2(WRITE_SYNCWORD_CMD,RF_SYNC_Length,mRFSync);	
	#endif
	
#else
	
	for(a=0;a<RF_SYNC_Length;a++)
		mRFSync[a] = BC3602_SYNCWORD[a];
	RFWriteBuf(WRITE_SYNCWORD_CMD,RF_SYNC_Length,mRFSync);
	
#endif
	
}*/

//*********************************************//
// name		:	DATA_Process
// input	:	RF_TXFIFO
// output	:	none
//*********************************************//
void DATA_Process(unsigned char *FIFO)
{
	FIFO++;
	*FIFO = 0x22;
	//�K�K
	//�K�K
	//�K�K
	FIFO++;
	*FIFO = 0x33;
}

//*********************************************//
// name		:	KEY_Process
// input	:	key_status
// output	:	none
//*********************************************//
void KEY_BC3602(unsigned char key_status)
{
	switch(key_status)
	{
		case 1:
			RF_TXFIFO[0] = 0x01;															// put data into RF_TXFIFO
			DATA_Process(RF_TXFIFO);														// put data into RF_TXFIFO
			Mflag.RFstep = 0;																// reset RF step
			Mflag.RFmode = RF_TX;															// set RF mode flag as TX															
			break;
			
		case 2:
			RF_TXFIFO[0] = 0x02;
			DATA_Process(RF_TXFIFO);
			Mflag.RFstep = 0;
			Mflag.RFmode = RF_TX;
			break;
			
		case 3:
			RF_TXFIFO[0] = 0x03;
			DATA_Process(RF_TXFIFO);
			Mflag.RFstep = 0;
			Mflag.RFmode = RF_TX;
			break;
			
		case 4:
			RF_TXFIFO[0] = 0x04;
			DATA_Process(RF_TXFIFO);
			Mflag.RFstep = 0;
			Mflag.RFmode = RF_TX;
			break;	
			
		default:
			break;
	}
}

void PinInterrupt_ISR (void) interrupt 7
{
    _push_(SFRS);
  
    SFRS = 0;
    switch(PIF)
    {
      case 0x01:
			Mflag.fRFINT = 1; 
			break;
      case 0x02:// printf("\n PIT1 interrupt!"); break;
      default: break;
    }                 
    PIF = 0; 
  
    _pop_(SFRS);
}

unsigned char Get_RF_Mode(void) 
{
return Mflag.RFmode;
}



//*******************************//
// name		:	WriteFIFO
// input	:	none
// output	:	none
//*******************************//
void WriteFIFO(unsigned char *rf_fifo,unsigned char payload_length)
{
	unsigned char a=0;	
	BC3602CSN=0;
	SpiWrite(WRITE_FIFO_CMD);
	for(a=0;a<payload_length;a++)
	{
		SpiWrite(*rf_fifo);
		rf_fifo++;
	}
	BC3602CSN=1;
}

//*******************************//
// name		:	ReadFIFO
// input	:	none
// output	:	none
//*******************************//
void ReadFIFO(unsigned char *rf_fifo,unsigned char length)
{
	unsigned char a=0;	
	BC3602CSN=0;
	SpiWrite(READ_FIFO_CMD);
	for(a=0;a<length;a++)
	{
		*rf_fifo = SpiRead();
		rf_fifo++;
	}
	BC3602CSN=1;
}


/*------------------------------------------------------------------*/
/*							 Function								*/
/*------------------------------------------------------------------*/
//*******************************//
// name		:	SimpleFIFO_TX_Process
// input	:	step
// output	:	RF_Mode
//*******************************//
void SimpleFIFO_TX_Process(unsigned char *step,unsigned char *IRQ,unsigned char *Mode)
{
	switch(*step)
	{
		case 0:
			SpiWriteStrobeCMD(LIGHT_SLEEP_CMD);													// RF entry LightSleep mode
			RFXtalReady();																		// waitting XTAL ready
			
			SpiWriteRegCMD(WRITE_REGS_CMD|IRQ_ENABLE_REGS,0x01);										// enable TXCMPIF
			
			SpiWriteStrobeCMD(REST_TX_POS_CMD);													// reset RF TX FIFO
			SpiWriteRegCMD(WRITE_REGS_CMD|TX_DATA_LENG_REGS,RF_Payload_Length);							// set RF TX payload length
			WriteFIFO(RF_TXFIFO,RF_Payload_Length);												// write data to RF FIFO
			
			SpiWriteStrobeCMD(TX_MODE_CMD);														// RF entry TX mode
			(*Mode) = RF_TX;																	// set RF mode flag as TX
			(*step)++;																			// increase RF step
			break;
			
		case 1:
			if(RF_INT==0)																		// while RF IQR line being low
				(*IRQ) = RFGetClrRFIrq();														// read RF IRQ status
			
			if(RF_IRQ&0x01)																		// while RF IQR equal TXCMPIF
			{
				SpiWriteStrobeCMD(LIGHT_SLEEP_CMD);												// RF entry LightSleep mode
				(*Mode) = RF_LightSleep;														// set RF mode flag as Lightsleep
				(*step) = 0;																	// reset RF step
			}
			break;
			
		default:
			(*step) = 0;
			break;
	}
}


//*******************************//
// name		:	SimpleFIFO_RX_Process
// input	:	step
// output	:	RF_Mode
//*******************************//
void SimpleFIFO_RX_Process(unsigned char *step,unsigned char *IRQ,unsigned char *Mode)
{
	unsigned char a;
	
	switch(*step)
	{
		case 0:
			SpiWriteStrobeCMD(LIGHT_SLEEP_CMD);													// RF entry LightSleep mode
			RFXtalReady();																		// waitting XTAL ready
			
			SpiWriteRegCMD(WRITE_REGS_CMD|IRQ_ENABLE_REGS,0x12);										// enable RXERRIF & RXCMPIF
			
			for(a=0;a<RF_Payload_Length;a++)													// clear RF_RXFIFO
				RF_RXFIFO[a] = 0;	
			
			SpiWriteStrobeCMD(REST_RX_POS_CMD);													// reset RF RX FIFO
			SpiWriteRegCMD(WRITE_REGS_CMD|RX_DATA_LENG_REGS,RF_Payload_Length);							// set RF RX payload length
			
			SpiWriteStrobeCMD(RX_MODE_CMD);														// RF entry RX mode
			(*Mode) = RF_RX;																	// set RF mode flag as RX
			(*step)++;																			// increase RF step
			break;
			
		case 1:
			if(RF_INT==0)																		// while RF IQR line being low
				(*IRQ) = RFGetClrRFIrq();														// read RF IRQ status
			
			if(RF_IRQ&0x12)																		// while RF IQR equal RXERRIF or RXCMPIF
			{
				if(RF_IRQ & 0x02)																// while RF IQR equal RXCMPIF
					ReadFIFO(RF_RXFIFO,RF_Payload_Length);										// read RX FIFO
					memcpy(RF_RXFIFOSAVE,RF_RXFIFO,RF_Payload_Length);
				SpiWriteStrobeCMD(LIGHT_SLEEP_CMD);												// RF entry LightSleep mode
				(*Mode) = RF_LightSleep;														// set RF mode flag as Lightsleep
				(*step) = 0;																	// reset RF step
				Start_Display_RXData=True;
			}
			break;
			
		default:
			(*step) = 0;
			break;
	}
}
 



//*********************************************//
// name		:	RF_Finished_Process
// input	:	key_status
// output	:	none
//*********************************************//
void RF_Finished_Process(void)
{
	if(RF_IRQ & 0x01)																		// while TX finished
	{
		RF_IRQ = 0;																			// clear IRQ flag
		Mflag.RFmode = RF_RX;																// set RF mode flag as RX
		
		switch(RF_TXFIFO[0])																// display LED show TX success
		{
			case 0x01:
//				LEDx_Flash(LED1,1,10);
			  //LED_FLASH(1);
				break;
			case 0x02:
//				LEDx_Flash(LED2,1,10);
			  //LED_FLASH(2);
				break;
			case 0x03:
//				LEDx_Flash(LED3,1,10);
			 // LED_FLASH(3);
				break;
			case 0x04:
//				LEDx_Flash(LED4,1,10);
			  //LED_FLASH(4);
				break;
			default:
				break;
		}
		
	}
	
	if(RF_IRQ & 0x12)																		// while RX finished
	{
		RF_IRQ = 0;																			// clear IRQ flag
		Mflag.RFmode = RF_RX;																// set RF mode flag as RX
		if(RF_IRQ & 0x10)																	// while RX finished but CRC error
		{
			
			
			
		}
		else																				// while RX finished and CRC OK
		{
			switch(RF_RXFIFO[0])															// check RF DATA
			{
				case 0x01:
					//LEDx_Flash(LED1,1,10);
//				  LED_FLASH(1);
					break;
				case 0x02:
					//LEDx_Flash(LED2,1,10);
				  //LED_FLASH(2);
					break;
				case 0x03:
				//	LEDx_Flash(LED3,1,10);
				 // LED_FLASH(3);
					break;
				case 0x04:
					//LEDx_Flash(LED4,1,10);
				  //LED_FLASH(4);
					break;
				default:
					break;
			}
		}
	}
}


void BC3602_RF_Process(void)
{
	  BC3602_TX_Process();
	if(Mflag.RFmode == RF_TX)															                  // while RF mode flag = TX
		SimpleFIFO_TX_Process(&Mflag.RFstep,&RF_IRQ,&Mflag.RFmode);						// RF Simple FIFO TX porcess	
	
	if(Mflag.RFmode == RF_RX)															                  // while RF mode flag = RX
		SimpleFIFO_RX_Process(&Mflag.RFstep,&RF_IRQ,&Mflag.RFmode);						// RF Simple FIFO RX porcess			
  	RF_Finished_Process();
	 // RF_Finished_Process(RF_IRQ)
}
  
void Set_RFmode(unsigned char Mode_x)
{
switch(Mode_x)															// check RF DATA
			{
				case RF_TX:
					
				  Mflag.RFmode = RF_TX;
					break;
				case RF_RX:
					Mflag.RFmode = RF_RX;
				  
					break;
					break;
				default:
					break;
			}
}
	
void ReSet_RFstep(void)
{
Mflag.RFstep = 0;	//
}
		

