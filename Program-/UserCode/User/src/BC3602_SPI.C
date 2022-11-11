#include "BC3602_SPI.h "
#include "BC3602_CMD_REG.h"
#include "ML51.h"
void BC3602_SPI_IOConfig(void)
{
/*IO¿Ú³õÊ¼»¯*/
GPIO_SetMode(RFCSN_PORT ,RFCSN_PIN,GPIO_MODE_PUSHPULL);
Set_RFSDIO_Dir(Output);
GPIO_SetMode(RFSCK_PORT,RFSCK_PIN ,GPIO_MODE_PUSHPULL);
BC3602CSN=1 ;	
BC3602MOSI=1;
BC3602SCK=1 ;				
}

void Set_RFSDIO_Dir(uint8_t Director)//ï¿½ï¿½ï¿½ï¿½RFSDIOï¿½ÚµÄ·ï¿½ï¿½ï¿½
{
	if(Director==Output)
	{
   GPIO_SetMode(RFSDIO_PORT,RFSDIO_PIN ,GPIO_MODE_PUSHPULL);
		
	}
	else if(Director==Input)
	{
    GPIO_SetMode(RFSDIO_PORT,RFSDIO_PIN ,GPIO_MODE_INPUT );
	 GPIO_Pull_Enable(RFSDIO_PORT,RFSDIO_PIN,PULLUP);
	}
}

void SpiWrite(unsigned char SIMD)
{
//#if (RF_SPI_line==3)
	unsigned char	cnt;
	//RF_SDIO_CTR = 0;
	Set_RFSDIO_Dir(Output);//ï¿½ï¿½ï¿½ï¿½Îªï¿½ï¿½ï¿?
	for	(cnt=0;cnt<8;cnt++)
	{
		//RF_SCK = 0;
		BC3602SCK=0;
		if( SIMD & 0x80)
			BC3602MOSI = 1; 
		else
			BC3602MOSI= 0;
		SIMD = SIMD<<1;
		//RF_SCK = 1;
		BC3602SCK=1;
	}
	//RF_SCK = 0;
	BC3602SCK=0;
	
}

//*******************************//
// name		:	SpiRead
// input	:	none
// output	:	SPIdata
// stack	:	1
//*******************************//
unsigned char SpiRead(void)
{
//#if (RF_SPI_line==3)
	unsigned char cnt,SIMD=0;
	//RF_SDIO_CTR = 1;
	Set_RFSDIO_Dir(Input);
	//RF_SCK	= 0;
	BC3602SCK=0;
	for	(cnt=0;cnt<8;cnt++)
	{
		SIMD = SIMD<<1;
		//RF_SCK	= 1;
		BC3602SCK=1;
		if(BC3602MOSI)
			SIMD |= 0x01; 
		//RF_SCK	= 0;
		BC3602SCK=0;
	}
	return SIMD;

}

void BC3602_StrobeCommand(unsigned char StrobeCmd)
{	
		unsigned char cnt;
		
		Set_RFSDIO_Dir(Output);          //ÉèÖÃdioÎªÊä³ö
		
		BC3602CSN = 0;
		for	(cnt=0;cnt<8;cnt++)
		{
			BC3602SCK = 0;
			if( StrobeCmd & 0x80)
				BC3602MOSI = 1; 
			else
				BC3602MOSI = 0;
			StrobeCmd = StrobeCmd<<1;
			BC3602SCK = 1;
		}
		BC3602SCK = 0;	
		BC3602CSN = 1;
}

void BC3602_EnterLightSleep(void)
{
	BC3602_StrobeCommand(LIGHT_SLEEP_CMD);
}

void BC3602_RegisterBank(unsigned char reg_Bank)
{
		reg_Bank &= 0x03;
		reg_Bank |= REGS_BANK_CMD;
		BC3602_StrobeCommand(reg_Bank);	
}

void BC3602_WriteRegister(unsigned char reg,unsigned char value)
{	
		unsigned char cnt;
		
		Set_RFSDIO_Dir(Output);          //ÉèÖÃdioÎªÊä³ö
		
		reg &= REGSADDR_MASK;
		reg |= WRITE_REGS_CMD;
		BC3602CSN = 0;
		for	(cnt=0;cnt<8;cnt++)
		{
			BC3602SCK = 0;
			if( reg & 0x80)
				BC3602MOSI = 1; 
			else
				BC3602MOSI = 0;
			reg = reg<<1;
			BC3602SCK = 1;
		}
		BC3602SCK = 0;	
		
		for	(cnt=0;cnt<8;cnt++)
		{
			BC3602SCK = 0;
			if( value & 0x80)
				BC3602MOSI = 1; 
			else
				BC3602MOSI = 0;
			value = value<<1;
			BC3602SCK = 1;
		}
		BC3602SCK = 0;	
		
		BC3602CSN = 1;
}