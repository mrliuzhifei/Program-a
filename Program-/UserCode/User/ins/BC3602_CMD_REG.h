#ifndef BC3602_CMD_REG_H
#define BC3602_CMD_REG_H
/*------------------------------------------------------------------*/
/*						BC3602 strobe command 						*/
/*------------------------------------------------------------------*/
#define WRITE_REGS_CMD			0x40 //Register write command
#define READ_REGS_CMD			0xc0 //Register read command
#define REGS_BANK_CMD      		0x20 //Set Register Bank command
#define WRITE_SYNCWORD_CMD  	0x10 //Write Sync-Word command
#define READ_SYNCWORD_CMD   	0x90 //Read Sync-Word command
#define WRITE_FIFO_CMD      	0x11 //Write TX FIFO command
#define READ_FIFO_CMD       	0x91 //Read RX FIFO command
#define SOFT_RESET_CMD      	0x08 //Software reset command
#define REST_TX_POS_CMD     	0x09 //TX FIFO address pointer reset command
#define REST_RX_POS_CMD     	0x89 //RX FIFO address pointer reset command
#define DEEP_SLEEP_CMD      	0x0A //Enter Deep Sleep mode
#define IDLE_MODE_CMD       	0x0B //Enter Idle mode
#define LIGHT_SLEEP_CMD     	0x0C //Enter Light Sleep mode
#define STANDBY_MODE_CMD    	0x0D //Enter Standby mode
#define TX_MODE_CMD         	0x0E //Enter TX mode
#define RX_MODE_CMD         	0x8E //Enter RX mode
#define REGSADDR_MASK       	0x3F

/*------------------------------------------------------------------*/
/*						BC3602 Register Bank 						*/
/*------------------------------------------------------------------*/
#define REGS_BANK0       		0x00
#define REGS_BANK1      		0x01
#define REGS_BANK2     		    0x02

/*------------------------------------------------------------------*/
/*						BC3602 RF Band_SEL	 						*/
/*------------------------------------------------------------------*/
#define BAND_315MHz      		0x00
#define BAND_433MHz      		0x01
#define BAND_470MHz      		0x02
#define BAND_868MHz      		0x03
#define BAND_915MHz      		0x03

#define _315MHz_     			0x00
#define _433MHz_      			0x01
#define _470MHz_      			0x02
#define _868MHz_      			0x03
#define _915MHz_      			0x04

/*------------------------------------------------------------------*/
/*					  	  BC3602 RF MODE	 						*/
/*------------------------------------------------------------------*/
#define	RF_DeepSleep			0x00
#define	RF_Idle					0x01
#define	RF_LightSleep			0x02
#define	RF_Standby				0x03
#define	RF_TX					0x04
#define	RF_RX					0x05
#define	RF_VCO					0x06

/*------------------------------------------------------------------*/
/*					 BC3602 RF Power & DR SEL	 					*/
/*------------------------------------------------------------------*/
#define _0dBm_      			0x00
#define _5dBm_      			0x01
#define _10dBm_      			0x02
#define _13dBm_      			0x03

#define _2kbps_ 				0x00
#define _10kbps_ 				0x01
#define _50kbps_      			0x02
#define _125kbps_      			0x03
#define _250kbps_      			0x04

/*------------------------------------------------------------------*/
/*						 BC3602 Register	 						*/
/*------------------------------------------------------------------*/
//Common register
//------------- BC3601 Common & Bank0 register memory map ------------------
#define CONFIG_REGS 				0x00		// Configuration Control Register 
#define CLOCK_CTL_REGS 				0x01		// Reset/Clock Control Register 
#define IRQ_CTL_REGS				0x02		// Interrupt Control Register 
#define IRQ_ENABLE_REGS				0x03		// Interrupt enable register 
#define IRQ_STATUS_REGS				0x04		// interrupt status register 
#define GPIO12_CTL_REGS 			0x06		// GPIO 1/2 control register 
#define GPIO34_CTL_REGS				0x07		// GPIO 3/4 control register 
#define GPIO_PULL_UP_REGS			0x08		// SPI/GPIO pull high control register 
#define TX_FIFO_SA_REGS				0x09		// TX FIFO start address register 
#define FIFO2_CTL_REGS				0x0A		// Packet mode FIFO control regster 
#define PREAMBLE_LENG_REGS			0x0B		// Packet preamble length register 
#define PACKET_CTL2_REGS			0x0C
#define PACKET_CTL3_REGS			0x0D
#define PACKET_CTL4_REGS			0x0E
#define TX_DATA_LENG_REGS			0x0F
#define RX_DATA_LENG_REGS			0x10
#define TRX_MODE_DELAY_REGS			0x11
#define HEADER_ADDR0_REGS			0x12
#define HEADER_ADDR1_REGS			0x13
#define MODULATOR_CTL1_REGS 		0x14
#define MODULATOR_CTL2_REGS			0x15
#define MODULATOR_CTL3_REGS			0x16
#define DEMOULATOR_CTL1_REGS 		0x17
#define DEMOULATOR_CTL2_REGS		0x18
#define DEMOULATOR_CTL3_REGS		0x19
#define DEMOULATOR_CTL4_REGS		0x1A
#define DEMOULATOR_CTL5_REGS		0x1B
#define DEMOULATOR_CTL6_REGS		0x1C
#define DEMOULATOR_CTL7_REGS		0x1D
#define DEMOULATOR_CTL8_REGS		0x1E
#define OPERATION_CTL_REGS 			0x20
#define FRACTIONAL_N_DN_REGS 		0x22
#define FRACTIONAL_N_DKL_REGS		0x23
#define FRACTIONAL_N_DKM_REGS		0x24
#define FRACTIONAL_N_DKH_REGS		0x25
#define MODE_STATUS_REGS			0x26
#define RSSI_CTL_REGS 				0x28
#define RSSI_VALUE_REGS				0x29
#define RSSI_VALUE_ID_REGS			0x2A
#define ATR_CONTROL_REGS			0x2B
#define ATR_CYCLE_L_REGS			0x2C
#define ATR_CYCLE_H_REGS			0x2D
#define ATR_ACTIVE_L_REGS			0x2E
#define ATR_EACTIVE_L_REGS			0x2F
#define ATR_EACTIVE_H_REGS			0x30
#define ARK_CONTROL_REGS			0x31
#define ARK_ACTIVE_REGS				0x32
#define ATR_CNTR_L_REGS				0x33
#define ATR_CNTR_H_REGS				0x34
#define ATR_ACTIVE_H_REGS			0x35
#define XO_CAP_CTL_REGS 			0x3C
#define XO_SEL_CTL_REGS				0x3D
#define LIRC_CTL_REGS				0x3E
#define TX_POWER_CTL_REGS			0x3F

//------------------ BC3601 Bank1 register memory map ----------------------
#define AGC_CTL2_REGS				0x21
#define AGC_CTL3_REGS				0x22
#define AGC_CTL4_REGS				0x23
#define AGC_CTL5_REGS				0x24
#define AGC_CTL6_REGS				0x25
#define AGC_CTL7_REGS				0x26
#define FILTER_CTL1_REGS 			0x2C
#define FILTER_CTL2_REGS 			0x2D
#define FILTER_CTL3_REGS 			0x2E
#define FILTER_CTL4_REGS 			0x2F
#define FILTER_CTL5_REGS 			0x30
#define FILTER_CTL6_REGS 			0x31
#define FILTER_CTL7_REGS 			0x32
#define FILTER_CTL8_REGS 			0x33
#define FILTER_CTL9_REGS 			0x34
#define FILTER_CTL10_REGS 			0x35
#define FILTER_CTL11_REGS 			0x36
#define FILTER_CTL12_REGS 			0x37
#define FILTER_CTL13_REGS 			0x38
#define FILTER_CTL14_REGS 			0x39
#define FILTER_CTL15_REGS 			0x3A
#define FILTER_CTL16_REGS 			0x3B
#define FILTER_CTL17_REGS 			0x3C
#define FILTER_CTL18_REGS 			0x3D
#define FILTER_CTL19_REGS 			0x3E

//------------------------- BC3601 Bank2 register ------------------------
//#define GAIN_CTL_REGS 				0x21
//#define RX2_CTL_REGS 				0x2F

#define BANK2_RSV1		 			0x26
#define BANK2_RSV2		 			0x27
#define BANK2_RSV3		 			0x28
#define BANK2_RSV4		 			0x29
#define BANK2_RSV5		 			0x2D
#define BANK2_RSV6		 			0x2E
#define BANK2_RSV7		 			0x2F
#define BANK2_RSV8		 			0x30
#define BANK2_RSV9		 			0x31
#define BANK2_RSV10		 			0x34
#define BANK2_RSV11		 			0x3A
#endif