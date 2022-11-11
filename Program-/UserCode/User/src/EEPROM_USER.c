#include "ML51.h"
typedef struct SensorData_t
{
 SavePage_s SavePage;                    //������һҳ
 UINT8 Write_SavePageBuf[6];             //�洢��ǰ���ݴ洢��λ��
 UINT8 Read_SavePageBuf[6];	
 UINT8 Write_CO2SaveBuf[128];            //������̼���ݴ洢����
 UINT8 Read_CO2SaveBuf[128];             //������̼���ݴ洢����
 UINT8 IsCO2Save1Cycle;                  //�Ƿ������̼���ݴ洢��24Сʱ
 UINT8 ArrIndex	;	//�����±�
 UINT32 Max_Valu_CO2_24h;  //24Сʱ���ֵ
 UINT32 Avg_Valu_CO2_24h;  //24Сʱƽ��ֵ
 unsigned int Crc16;
}SensorData_t;  
SensorData_t SensorData;
#define JBE SensorData
volatile unsigned char xdata page_buffer[128];
void Write_DATAFLASH (unsigned int u16EPAddr, unsigned char *u8EPData,unsigned char len)
{
    unsigned char looptmp = 0;
    unsigned int u16_addrl_r;
    //unsigned int RAMtmp;
    //Check page start address
    u16_addrl_r = (u16EPAddr / 128) * 128;

    //Save APROM data to XRAM0
    for (looptmp = 0; looptmp < len; looptmp++)
    {
        //RAMtmp = Read_APROM_BYTE((unsigned int code *)(u16_addrl_r + looptmp));
			  if(looptmp<len)
				{
				page_buffer[looptmp] =*u8EPData;
				u8EPData++;
				}
				else page_buffer[looptmp] =0;
        
    }
    //Modify customer data in XRAM
    //page_buffer[u16EPAddr & 0x7f] = u8EPData;
    //Erase APROM DATAFLASH page
    IAPAL = u16_addrl_r & 0xff;
    IAPAH = (u16_addrl_r >> 8) & 0xff;
    IAPFD = 0xFF;
    set_CHPCON_IAPEN;
    set_IAPUEN_APUEN;
    IAPCN = 0x22;
    set_IAPTRG_IAPGO;

    //Save changed RAM data to APROM DATAFLASH
    set_CHPCON_IAPEN;
    set_IAPUEN_APUEN;
    IAPCN = 0x21;

    for (looptmp = 0; looptmp < len
		; looptmp++)
    {
        IAPAL = (u16_addrl_r & 0xff) + looptmp;
        IAPAH = (u16_addrl_r >> 8) & 0xff;
        IAPFD = page_buffer[looptmp];
        set_IAPTRG_IAPGO;
    }

    clr_IAPUEN_APUEN;
    clr_CHPCON_IAPEN;
}

void EEPROM_Write_SensorData(EEPROM_SaveType_s SaveType, DataType_s DataType)
{
switch (SaveType)
{
	unsigned int Crc16;
	case CO2:
	{
		if(DataType==PAGE)
		{
			if(JBE.SavePage==CO2_Page1)  
			{
//		 JBE.ISOneCircleSave=True;
			 JBE.SavePage=CO2_Page2;                                                                     //����һҳ�����Ժ󣬴ӵڶ�ҳ��ʼ��
			 JBE.Write_SavePageBuf[0]=CO2_Page2; 	  
			}	 
			else if(JBE.SavePage==CO2_Page2)
			{
			JBE.SavePage=CO2_Page1; 
			JBE.Write_SavePageBuf[0]=CO2_Page1;                                                          //���ڶ�ҳ�����Ժ󣬴ӵ�һҳ��ʼ�� ѭ������
			}	
			JBE.Write_SavePageBuf[5]=crc8_nrsc5(&JBE.Write_SavePageBuf, 5);
			EA=0;//���ж�
			Write_DATAFLASH (BaseAddress_Page,&JBE.Write_SavePageBuf,sizeof(JBE.Write_SavePageBuf));     //��PAGE
			Read_DATAFLASH_ARRAY(BaseAddress_Page,&JBE.Read_SavePageBuf,sizeof(JBE.Read_SavePageBuf));   //��ȡд������ݺ�ԭʼ���ݶԱȣ��Ƿ��д�����д���дһ��
			if(JBE.Read_SavePageBuf[5]!=crc8_nrsc5(&JBE.Read_SavePageBuf,5))
			{
			Write_DATAFLASH (BaseAddress_Page,&JBE.Write_SavePageBuf,sizeof(JBE.Write_SavePageBuf));     //��PAGE
			}	 
			EA=1;//���ж�	
		}
     if(DataType==Sensor)	
    {
  	 JBE.Write_CO2SaveBuf[Get_SaveAddr()]  =Get_Co2Value()>>8;                                         //����CRC У����
		 JBE.Write_CO2SaveBuf[Get_SaveAddr()+1]=Get_Co2Value();
		 JBE.Write_CO2SaveBuf[124]=Get_OneCircleSaveFlag();
		 JBE.Write_CO2SaveBuf[125]=Get_SaveAddr();
		 Crc16=CRC16MODBUS(&JBE.Write_CO2SaveBuf[0], (sizeof(JBE.Write_CO2SaveBuf)-2));	
		 JBE.Write_CO2SaveBuf[126]=Crc16>>8;
		 JBE.Write_CO2SaveBuf[127]=Crc16;
		 EA=0;//���ж�
		 Write_DATAFLASH (BaseAddress+JBE.SavePage*128,&JBE.Write_CO2SaveBuf,sizeof(JBE.Write_CO2SaveBuf));  //��CO2����
		 
		 Read_DATAFLASH_ARRAY(BaseAddress+JBE.SavePage*128,&JBE.Read_CO2SaveBuf,sizeof(JBE.Read_CO2SaveBuf));//��ȡд������ݺ�ԭʼ���ݶԱȣ��Ƿ��д�����д���дһ��	
		 JBE.Crc16=CRC16MODBUS(&JBE.Read_CO2SaveBuf[0],(sizeof(JBE.Read_CO2SaveBuf)-2));
     if((JBE.Read_CO2SaveBuf[127])!=JBE.Crc16)
			{
			Write_DATAFLASH (BaseAddress+JBE.SavePage*128,&JBE.Write_CO2SaveBuf,sizeof(JBE.Write_CO2SaveBuf)); //��CO2����
			}	 			
		 EA=1;//���ж� 	
		}			
	}
	break;
	case SHTC3:
		
	break;
	case HCHO:
		
	break;
	case PM25:
		
	break;
	case TVCO:
		
	break;
	default: break;	
}
}


void EEPROM_ReadDataInit(void)
{
 unsigned char OneCircle_Flag;
 unsigned char Save_AddrP;
 unsigned char CO2Value_H;
 unsigned char CO2Value_L;
 unsigned char i;	 
 EA=0;
 Read_DATAFLASH_ARRAY(BaseAddress_Page,&JBE.Read_SavePageBuf,sizeof(JBE.Read_SavePageBuf)); 
 if(JBE.Read_SavePageBuf[5]!=crc8_nrsc5(&JBE.Read_SavePageBuf,5))                                   //��pageҳ������ɹ��ٶ�һ��
	{
	Read_DATAFLASH_ARRAY(BaseAddress_Page,&JBE.Read_SavePageBuf,sizeof(JBE.Read_SavePageBuf));        //���ǲ��ɹ��Ͱ�ҳ�����ʼ��Ϊ0
  if(JBE.Read_SavePageBuf[5]!=crc8_nrsc5(&JBE.Read_SavePageBuf,5))
	{
	JBE.Write_SavePageBuf[0]=0;
	JBE.Write_SavePageBuf[1]=0;	
	JBE.Write_SavePageBuf[2]=0;	
	JBE.Write_SavePageBuf[3]=0;	
	JBE.Write_SavePageBuf[4]=0;	
	JBE.SavePage=JBE.Write_SavePageBuf[0];		
	}
	else
	{
	JBE.Write_SavePageBuf[0]=JBE.Read_SavePageBuf[0];                                                  //�ɹ�����pageҳ
	JBE.Write_SavePageBuf[1]=JBE.Read_SavePageBuf[1];	
	JBE.Write_SavePageBuf[2]=JBE.Read_SavePageBuf[2];	
	JBE.Write_SavePageBuf[3]=JBE.Read_SavePageBuf[3];	
	JBE.Write_SavePageBuf[4]=JBE.Read_SavePageBuf[4];	
	JBE.Write_SavePageBuf[5]=JBE.Read_SavePageBuf[5];	
  JBE.SavePage=JBE.Write_SavePageBuf[0];		
	}
		
	}
	else
	{
	JBE.Write_SavePageBuf[0]=JBE.Read_SavePageBuf[0];                                                   //�ɹ�����pageҳ
	JBE.Write_SavePageBuf[1]=JBE.Read_SavePageBuf[1];	
	JBE.Write_SavePageBuf[2]=JBE.Read_SavePageBuf[2];	
	JBE.Write_SavePageBuf[3]=JBE.Read_SavePageBuf[3];	
	JBE.Write_SavePageBuf[4]=JBE.Read_SavePageBuf[4];	
	JBE.Write_SavePageBuf[5]=JBE.Read_SavePageBuf[5];	
	JBE.SavePage=JBE.Write_SavePageBuf[0];	
	}
 
	Read_DATAFLASH_ARRAY(BaseAddress+JBE.SavePage*128,&JBE.Read_CO2SaveBuf,sizeof(JBE.Read_CO2SaveBuf));
	JBE.Crc16=CRC16MODBUS(&JBE.Read_CO2SaveBuf,(sizeof(JBE.Read_CO2SaveBuf)-2));	
	JBE.Crc16=JBE.Read_CO2SaveBuf[126];
	JBE.Crc16=JBE.Crc16<<8;
	JBE.Crc16=JBE.Crc16+JBE.Read_CO2SaveBuf[127];
		
	if(JBE.Crc16!=CRC16MODBUS(&JBE.Read_CO2SaveBuf,(sizeof(JBE.Read_CO2SaveBuf)-2)))                    //��������ɹ�
	{
	Read_DATAFLASH_ARRAY(BaseAddress+JBE.SavePage*128,&JBE.Read_CO2SaveBuf,sizeof(JBE.Read_CO2SaveBuf));//�ٶ�һ��
	
	JBE.Crc16=JBE.Read_CO2SaveBuf[126];
	JBE.Crc16=JBE.Crc16<<8;
	JBE.Crc16=JBE.Crc16+JBE.Read_CO2SaveBuf[127];	
	if(JBE.Crc16!=CRC16MODBUS(&JBE.Read_CO2SaveBuf,(sizeof(JBE.Read_CO2SaveBuf)-2)))                    //��������ɹ�
	{
	for(i=0;i<128;i++)
		{
		JBE.Write_CO2SaveBuf[i]=0;                                                                        //������ֵ����
		}
	}
	else                                                                                                //����ɹ��� �Ѷ�����ֵ��ֵ��Page
	{
	for(i=0;i<128;i++)
		{
		JBE.Write_CO2SaveBuf[i]=JBE.Read_CO2SaveBuf[i]; 
		}
		OneCircle_Flag=JBE.Write_CO2SaveBuf[124];
		Save_AddrP=JBE.Write_CO2SaveBuf[125];
		CO2Value_H=JBE.Write_CO2SaveBuf[Save_AddrP] ;
		CO2Value_L=JBE.Write_CO2SaveBuf[Save_AddrP+1];
		Set_OneCircle_Flag(OneCircle_Flag);
		Set_Save_AddrP(Save_AddrP);
	}
	}
	else
	{
	for(i=0;i<128;i++)
		{
		JBE.Write_CO2SaveBuf[i]=JBE.Read_CO2SaveBuf[i]; 
		}
	  OneCircle_Flag=JBE.Write_CO2SaveBuf[124];
		Save_AddrP=JBE.Write_CO2SaveBuf[125];
		CO2Value_H=JBE.Write_CO2SaveBuf[Save_AddrP] ;
		CO2Value_L=JBE.Write_CO2SaveBuf[Save_AddrP+1];
		Set_OneCircle_Flag(OneCircle_Flag);
		Set_Save_AddrP(Save_AddrP);
	}
	EA=1;
}


void Get_Max_Valu_24h(void)
{
unsigned char i;	
unsigned int Max_Valu_24h;
unsigned int Max_Valu_24h_Temp;	
for(i=0;i<96;i=i+2)
	{	
	Max_Valu_24h_Temp=JBE.Write_CO2SaveBuf[i];
	Max_Valu_24h_Temp=Max_Valu_24h_Temp<<8;
	Max_Valu_24h_Temp=Max_Valu_24h_Temp+JBE.Write_CO2SaveBuf[i+1];	
	if(Max_Valu_24h<Max_Valu_24h_Temp)	
	{
	Max_Valu_24h=Max_Valu_24h_Temp;
	}	
	}
  JBE.Max_Valu_CO2_24h=Max_Valu_24h;
}

void Get_Avg_Valu_24h(void)
{
	unsigned char i;	
  unsigned int  Avg_Valu_24h=0;
  unsigned int  Avg_Valu_24h_Temp=0;	
	unsigned long Avg_Valu_24h_Sum=0;
  if(JBE.Write_CO2SaveBuf[124]==0)
	{
		for(i=0;i<(JBE.Write_CO2SaveBuf[125]+2);i=i+2)
		{
		 Avg_Valu_24h_Temp=JBE.Write_CO2SaveBuf[i];
		 Avg_Valu_24h_Temp=Avg_Valu_24h_Temp<<8;
		 Avg_Valu_24h_Temp=Avg_Valu_24h_Temp+JBE.Write_CO2SaveBuf[i+1];	
		 Avg_Valu_24h_Sum=Avg_Valu_24h_Sum+ Avg_Valu_24h_Temp;
		}
		Avg_Valu_24h=Avg_Valu_24h_Sum*2/(JBE.Write_CO2SaveBuf[125]+2);
			  
	}
	else 
	{
		for(i=0;i<96;i=i+2)
		{
		 Avg_Valu_24h_Temp=JBE.Write_CO2SaveBuf[i];
		 Avg_Valu_24h_Temp=Avg_Valu_24h_Temp<<8;
		 Avg_Valu_24h_Temp=Avg_Valu_24h_Temp+JBE.Write_CO2SaveBuf[i+1];	
		 Avg_Valu_24h_Sum=Avg_Valu_24h_Sum+ Avg_Valu_24h_Temp;
		}
		Avg_Valu_24h=Avg_Valu_24h_Sum/48;
	
	}
	
	
	
  JBE.Avg_Valu_CO2_24h=Avg_Valu_24h;
}

unsigned int Get_Max_Valu_CO2_24h(void)
{
 return JBE.Max_Valu_CO2_24h;
}

unsigned int Get_Avg_Valu_CO2_24h(void)
{
 return JBE.Avg_Valu_CO2_24h;
}



