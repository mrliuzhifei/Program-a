#ifndef _LED_H_
#define _LED_H_

#define BLU_10_Second 1000
#define BLU_PORT_R   Port3
#define BLU_PORT_GB  Port0
#define LAMP_PORT    Port5
#define BLU_R_PIN    BIT6
#define BLU_G_PIN    BIT5
#define BLU_B_PIN    BIT4
#define LAMP_R_PIN   BIT4
#define LAMP_G_PIN   BIT5
#define LAMP_B_PIN   BIT7

#define BLU_ON()      P04=1;P05=1;P36=1  //����ΪѰַ
#define BLU_OFF()     P04=0;P05=0;P36=0  //����ΪѰַ
#define LAMP_GREEN_ON()  SFRS=0; P5&=0x4F; P5|=0x20   //����λѰַ
#define LAMP_YELLOW_ON() SFRS=0; P5&=0x4F; P5|=0x30
#define LAMP_RED_ON()    SFRS=0; P5&=0x4F; P5|=0x10
#define LAMP_OFF()       SFRS=0; P5&=0x4f   //����λѰַ
void LED_Init(void);
typedef enum Color_s	
{
	 White ,                 //��ɫ
	 Red,                    //��ɫ
	 Green,                  //��ɫ
	 Blue,                   //��ɫ
	 Yellow,                 //��ɫ
	 Purple,                 //��ɫ
   Orange,                 //��ɫ
	 Cyan,	                 //��ɫ
}Color_s	;
void BLU_Color(void);
//void RGB_Color(Color_s	RGBColor);
void LED_Buz(void);
void LED_Low_Light(unsigned char Level_light);
Color_s Get_Color_Sta(void);
extern unsigned int BLU_10_SecondCount;
extern unsigned char Is_InToSleep;
#endif