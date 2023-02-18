#ifndef __LCDDRIVE_H
#define __LCDDRIVE_H 	

#include "ddl.h"

#include <stdbool.h>
void Lcd_Init(void);
void BlockWrite(void);
void Lcd_SpiGpioInit(void);

void displaycolorSmallNum_highPixel(uint8_t num,uint8_t arraynums,uint8_t bitColumn,uint8_t fixedvalue,uint16_t colourleft,uint16_t colourright);
void displayThirdCurrRECSmallNum(uint8_t num,uint8_t arraynums,uint8_t bitColumn,uint8_t fixedvalue,uint16_t colourleft,uint16_t colourright);

void displayDplusDminSmallNum_highPixel(uint8_t num,uint8_t arraynums,uint8_t bitColumn,\
	uint8_t interval1,uint8_t interval2,uint8_t interval3,\
	uint16_t colour1,uint16_t colour2,uint16_t colour3,uint16_t colour4,uint8_t PlusMinNum);


void displaySelectChannel(uint8_t dataChar,uint8_t charNum,uint8_t columnBit);
void displayKepTip(uint8_t num,uint8_t arraynums,uint8_t bitColumn_smallData,uint8_t fixedvalue,uint16_t colourleft,uint16_t colourright);
 
void displayKepTipPowerOn(void);
void displaycolorSmallNumMoreColor(uint8_t dataChar,uint8_t charNum,uint8_t columnBit,\
	uint8_t interval1,uint8_t interval2,uint8_t interval3,\
	uint16_t colour1,uint16_t colour2,uint16_t colour3,uint16_t colour4);

 
void displayMinutes(uint8_t num,uint8_t arraynums,uint8_t bitColumn,uint8_t fixedvalue,uint16_t colourleft,uint16_t colourright);
void displayBrightness(uint8_t num,uint8_t arraynums,uint8_t bitColumn,uint8_t fixedvalue,uint16_t colourleft,uint16_t colourright);
void displayTemperatureNum(uint8_t num,uint8_t arraynums,uint8_t bitColumn,uint8_t fixedvalue,uint16_t colourleft,uint16_t colourright);



void displaycolorPatternBigNum1(uint8_t num,uint8_t arraynums,uint8_t bitColumn,uint16_t colour);
void displaycolorPatternBigNum2(uint8_t num,uint8_t arraynums,uint8_t bitColumn,uint16_t colour);
void displaycolorPatternBigNum3(uint8_t num,uint8_t arraynums,uint8_t bitColumn,uint16_t colour);

void displaycolorPageBigNum(uint8_t num,uint8_t arraynums,uint8_t bitColumn,uint8_t fixedvalue,uint16_t colourleft,uint16_t colourright);
void displaycolorPageBigNum_highPixel(uint8_t num,uint8_t arraynums,uint8_t bitColumn,uint8_t fixedvalue,uint16_t colourleft,uint16_t colourright);
void displaycolorPageBigNum_PowerOn(uint8_t num,uint8_t arraynums,uint8_t bitColumn,uint8_t fixedvalue,uint16_t colourleft,uint16_t colourright);


void displayNineScale(uint8_t num,uint8_t arraynums,uint8_t bitColumn_smallData);
void displayNineScaleOneChar(uint8_t num,uint8_t arraynums,uint8_t bitColumn_smallData);

void sendOneChar(uint8_t col8num, uint8_t colourH,uint8_t colourL);
void sendOneChar_tip_highpixel(uint8_t col8num, uint8_t colourH, uint8_t colourL);

void sendOnePatternChar1(uint8_t col8num, uint8_t colourH, uint8_t colourL,uint8_t bitColumn,uint8_t j);
void sendOnePatternChar2(uint8_t col8num, uint8_t colourH, uint8_t colourL,uint8_t bitColumn,uint8_t j);
void sendOnePatternChar3(uint8_t col8num, uint8_t colourH, uint8_t colourL,uint8_t bitColumn,uint8_t j);

void sendOneSelectChannelChar(uint8_t col8num, uint8_t colourH,uint8_t colourL);
void sendOneCircleDChar(uint8_t col8num, uint8_t colourH,uint8_t colourL,uint8_t col8num2);
void sendOneCharKepTip(uint8_t col8num, uint8_t colourH,uint8_t colourL);
void sendOneCharKepTip_highPixel(uint8_t col8num, uint8_t colourH,uint8_t colourL);


void sendOneCharThirdCurrREC(uint8_t col8num, uint8_t colourH,uint8_t colourL);
void sendOneBit(uint8_t colourH,uint8_t colourL);
void sendFourBit(uint8_t col8num, uint8_t colourH,uint8_t colourL);
 
 
void display_nineScale_behind(uint8_t num,uint8_t arraynums,uint8_t bitColumn_smallData,uint8_t *offsetbit,uint8_t stopbit);
void sendOneChar_nineScale_behind(uint8_t col8num, uint8_t colourH, uint8_t colourL,uint8_t *offsetbit,uint8_t stopbit);
void sendTweBit_nineScale_behind(uint8_t col8num, uint8_t colourH,uint8_t colourL,uint8_t *offsetbit,uint8_t stopbit);
 
void display_nineScale_front(uint8_t num,uint8_t arraynums,uint8_t bitColumn_smallData,uint8_t *offsetbit,uint8_t stopbit);
void sendOneChar_nineScale_front(uint8_t col8num, uint8_t colourH, uint8_t colourL,uint8_t *offsetbit,uint8_t stopbit);
void sendTweBit_nineScale_front(uint8_t col8num, uint8_t colourH,uint8_t colourL,uint8_t *offsetbit,uint8_t stopbit);



void sendThreeBit(uint8_t col8num, uint8_t colourH,uint8_t colourL);
void displayFlip(void);
void displayBlack (void);
//void displayPicture(const uint8_t picture[]);

void displayHelpchar(uint8_t num,uint8_t bitColumn,uint16_t colour);
 
void sleepOut(void);

void displayLogo(void);

void displayLogoPreview (void);
void NumberPowerOn_fill(void);


#endif
