
#ifndef __DISPLAYDRIVER_H__
#define __DISPLAYDRIVER_H__
 
#include "ddl.h"
#include "cachevalue.h"

//#define RED   	0xffe0//ÐÂ
//#define GREEN   0XF81F
//#define REALBLUE 0XF800  
//#define BLUE  	0X07FF
//#define CYAN  	0x001F
//#define PURPLE  0x07E0
//#define YELLOW  0xF800
//#define LIGHTBLUE  	0x0269
//#define ORANGE  0xFC00
//#define GRAY	0x18E3
//#define MAGENTA	0x77E0
//#define OLIVE	0xF8E3
//#define WHITE	0x0000


//#define RED   	0x001F//¾É
//#define GREEN   0X07E0
//#define REALBLUE 0XF800  
//#define SHAMBLUE  	0XF8E7
//#define CYAN  	0xffE0
//#define PURPLE  0XF81F
//#define YELLOW  0X07FF
//#define LIGHTBLUE  	0XFD96
//#define ORANGE  0x03FF
//#define GRAY	0xE71C
//#define MAGENTA	0x881F
//#define OLIVE	0x071C
//#define WHITE	0xffff  

#define RED   	0xF800// 
#define GREEN   0X07E0
#define REALBLUE 0x001F   
#define CYAN  	0x07FF
#define PURPLE  0xF81F
#define YELLOW  0Xffe0
#define LIGHTBLUE 0x69Bf
#define ORANGE  0xffc0
#define GRAY	0xE71C//  
#define WHITE	0xffff   
#define DEEP_GRAY	0xC618  

#define DEEPER_GRAY	0x8410  

void lcdDisplay(void);

void initInterfaceNum(void);
void displayFirstInterface(void);
void displaySecondInterface(void);
void displayThirdInterface(void);
void displayFourthInterface(void);
void displaySeventhInterface(void); 
			
void displayCaliForCustomInterface(void);
void displayCaliForFactoryInterface(void);
			
void temperatureAlarm(void);
void displayTemperature(void);
void operationForDisplayArray(uint8_t *arraynums,uint8_t *bitColumn);

void operationForDisplayArray_tip_highpixel(uint8_t *arraynums,uint8_t *bitColumn);
void operationForDisplayArray_highPixel(uint8_t *arraynums,uint8_t *bitColumn);
void operationForDisplayArraybig(uint8_t *arraynums,uint8_t *bitColumn);

void operationForDisplayArraybig_highPixel(uint8_t *arraynums,uint8_t *bitColumn);
void operationForDisplayArraybig_PowerOn(uint8_t *arraynums,uint8_t *bitColumn);

void operationForDisplayArraybig_highPixel_OffsetRight(uint8_t *arraynums,uint8_t *bitColumn);
void operationForDisplayNineScale(uint8_t *arraynums,uint8_t *bitColumn);
void assignSelectChannel(char page);
void getRECcolor(uint16_t color);
 

uint16_t returnRECcolor(void);

int returngrow(void);
uint8_t returnLens(void);

void operationSeventh_characterArray(uint8_t *arraynums,uint8_t *bitColumn);

void operationHelp(uint8_t *arraynums,uint8_t *bitColumn);
void sendAnyBlankBit(uint8_t num);
void displayVoltage(void);
void displayCurrent(void);

long double getTempC_ld(void);
long double getTempF_ld(void);

void displayPowerOn(const uint8_t picture1[],const uint8_t picture2[]);

void progressBar(void);
#endif //__DISPLAYDRIVER_H__
