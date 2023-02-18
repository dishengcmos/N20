
#include "spi_mid.h"
#include "spi.h"
#include "gpio_mid.h"
#include "lcd_api.h"
#include "table.h"
#include "gpio.h"

#include "flash_mid.h"
#include "pwm_mid.h"
#include "key.h"
//LCD Init For 1.44Inch LCD Panel with ST7735R.
void Lcd_Init(void)
{
	/***/
	//delay1ms(120);
	WriteComm(0x10); 
	delay1ms(10); 
	WriteComm(0x01); 
	
	WriteComm(0xB1); 
	WriteData(0x05);
	WriteData(0x3A);
	WriteData(0x3A);

	WriteComm(0xB2);
	WriteData(0x05);
	WriteData(0x3A);
	WriteData(0x3A);

	WriteComm(0xB3); 
	WriteData(0x05);  
	WriteData(0x3A);
	WriteData(0x3A);
	WriteData(0x05);
	WriteData(0x3A);
	WriteData(0x3A);

	WriteComm(0xB4); 	//Display Inversion Contro      //Dot inversion
	WriteData(0x20);	//全彩色普通模式下的反转设置Inversion setting in full Colors normal mode   

	WriteComm(0x35);	//
	WriteData(0x00);	//

	WriteComm(0xC0);   
	WriteData(0x0E);   
	WriteData(0x0E);   
	WriteData(0x04);   

	WriteComm(0xC1);     
	WriteData(0xC5);   

	WriteComm(0xC2);     
	WriteData(0x0d);   
	WriteData(0x00);   

	WriteComm(0xC3);     
	WriteData(0x8D);   
	WriteData(0x2A); 

	// ***useful*
	WriteComm(0xC4);     
	WriteData(0x8D);   
	WriteData(0xEE);   

	WriteComm(0xC5);     //VCOM
	WriteData(0x06); 	//1D  .06
	//
	
	if(flash_Cali.flash_Cali_s.flip==NormalDirection)
	{
		WriteComm(0x36);	//MX, MY, RGB mode
		WriteData(0x40);	//c8当高位是8的时候屏幕反转 40
	}
	else if(flash_Cali.flash_Cali_s.flip==TurnLeft)
	{
		WriteComm(0x36);	//MX, MY, RGB mode
		WriteData(0x20);	//WriteData(0x80)
	} 	
	else if(flash_Cali.flash_Cali_s.flip==TurnRight)
	{
		WriteComm(0x36);	//MX, MY, RGB mode
		WriteData(0xe0);	//c8当高位是8的时候屏幕反转 
	}
	else if(flash_Cali.flash_Cali_s.flip==UpsideDown)
	{
		WriteComm(0x36);	//MX, MY, RGB mode
		WriteData(0x80);	//WriteData(0x80)
	} 

	WriteComm(0x3A);    //MX, MY, RGB mode
	WriteData(0x05);   	//设置两个字节表示颜色

	WriteComm(0xF0);
	WriteData(0X4);
	WriteData(0X32);
	WriteData(0X25);
	WriteData(0X41);
	WriteData(0XBA);
	WriteData(0XE);
	WriteData(0XF);
	WriteData(0X0);
	WriteData(0X1);
	WriteData(0XB);
	WriteData(0XE);
	WriteData(0X19);
	WriteData(0X18);
	WriteData(0XF);

	WriteComm(0xF1); 
	WriteData(0X7);
	WriteData(0X3C);
	WriteData(0X2D);
	WriteData(0X50);
	WriteData(0XAC);
	WriteData(0X25);
	WriteData(0X25);
	WriteData(0X0);
	WriteData(0X1);
	WriteData(0XC);
	WriteData(0XE);
	WriteData(0X15);
	WriteData(0X14);
	WriteData(0XF);

	WriteComm(0xfe);
	WriteComm(0xff);
	WriteComm(0x21);//Display Inversion ON 0x20//颜色翻转
	
	WriteComm(0x11);// sleep out
	//delay1ms(120);
	//delay1ms(5);
	WriteComm(0x29);//display on
 
}

void BlockWrite(void)
{
//上下显示
	if(flash_Cali.flash_Cali_s.flip==NormalDirection)
	{	
		WriteComm(0x2a);
		WriteData(0x00);
		WriteData(0x00);
		WriteData(0x00);
		WriteData(0x00+0xef);
		WriteComm(0x2b);
		WriteData(0x00);
		WriteData(0x00);
		WriteData(0x00);
		WriteData(0x00+0xef);
	}
	else if(flash_Cali.flash_Cali_s.flip==UpsideDown)
	{	
		WriteComm(0x2a);
		WriteData(0x00);
		WriteData(0x00);
		WriteData(0x00);
		WriteData(0x00+0xef);
		
		WriteComm(0x2b);
		WriteData(0x00);
		WriteData(0x00);
		WriteData(0x00);
		WriteData(0x00+0xef);
	}
	else if(flash_Cali.flash_Cali_s.flip==TurnLeft)//左右显示
	{
		WriteComm(0x2a);
		WriteData(0x00);
		WriteData(0x00);
		WriteData(0x00);
		WriteData(0x00+0xef);
		WriteComm(0x2b);
		WriteData(0x00);
		WriteData(0x00);
		WriteData(0x00);
		WriteData(0x00+0xef);
	}
	else if(flash_Cali.flash_Cali_s.flip==TurnRight)//左右显示
	{
		WriteComm(0x2a);
		WriteData(0x00);
		WriteData(0x00);
		WriteData(0x00);
		WriteData(0x00+0xef);
		
		WriteComm(0x2b);
		WriteData(0x00);
		WriteData(0x00);
		WriteData(0x00);
		WriteData(0x00+0xef);
	}
	WriteComm(0x2c);
}
void Lcd_SpiGpioInit(void)
{
    LcdgpiospiInit();
	LcdgpioInit();
	
	LcdspiInit(); 
}

void displayDplusDminSmallNum_highPixel(uint8_t num,uint8_t arraynums,uint8_t bitColumn,\
	uint8_t interval1,uint8_t interval2,uint8_t interval3,\
	uint16_t colour1,uint16_t colour2,uint16_t colour3,uint16_t colour4,uint8_t PlusMinNum)
{		
	unsigned char j;	
	unsigned char colourH,colourL;
	if(arraynums<=interval1)
	{
		colourH=colour1>>8;
		colourL=colour1;
		for ( j = 0; j < 4; j++)
		{	
			sendOneChar( NumberBig[num][bitColumn][j],  colourH, colourL);	
		}	
	}
	else if((arraynums>interval1)&&(arraynums<=interval2))
	{	
		colourH=colour2>>8; 
		colourL=colour2;
		for ( j = 0; j < 4; j++)
		{
			sendOneCircleDChar( NumberBig[num][bitColumn][j],  colourH, colourL,NumberBig[PlusMinNum][bitColumn][j]);	
		}	
	}	
	else if((arraynums>interval2)&&(arraynums<=interval3))
	{
		colourH=colour3>>8;
		colourL=colour3;
		for ( j = 0; j < 4; j++)
		{
			sendOneCircleDChar( NumberBig[num][bitColumn][j],  colourH, colourL,NumberBig[PlusMinNum][bitColumn][j]);	
		}	
	}
	else if((arraynums>interval3))
	{
		colourH=colour4>>8;
		colourL=colour4;
		for ( j = 0; j < 4; j++)
		{
			sendOneChar( NumberBig[num][bitColumn][j],  colourH, colourL);	
		}	
	}			
}
void displayThirdCurrRECSmallNum(uint8_t num,uint8_t arraynums,uint8_t bitColumn,uint8_t fixedvalue,uint16_t colourleft,uint16_t colourright)
{		
	unsigned char j;
	unsigned char colourH,colourL;
	if(arraynums<=fixedvalue)
	{	
		colourH=colourleft>>8;
		colourL=colourleft;
	}	
	else
	{	
		colourH=colourright>>8;
		colourL=colourright;
	}	
			
	for ( j = 0; j < 4; j++) 
	{	
		if(SettingState.page3==1)
		{
			if(6==arraynums)
				sendOneCharThirdCurrREC( NumberBig[num][bitColumn][j], 0xf8,0x00);	
			else								
				sendOneChar( NumberBig[num][bitColumn][j], colourH, colourL);
		}
		else
			sendOneChar( NumberBig[num][bitColumn][j], colourH, colourL);
	}		
}	


void displaycolorSmallNum_highPixel(uint8_t num,uint8_t arraynums,uint8_t bitColumn,uint8_t fixedvalue,uint16_t colourleft,uint16_t colourright)
{		
	unsigned char j;
	unsigned char colourH,colourL;
	if(arraynums<=fixedvalue)
	{	
		colourH=colourleft>>8;
		colourL=colourleft;
	}	
	else
	{	
		colourH=colourright>>8;
		colourL=colourright;
	}	 
	for ( j = 0; j < 4; j++) 
	{																	
		sendOneChar( NumberBig[num][bitColumn][j], colourH, colourL);	
	}																	
}
void displayTemperatureNum(uint8_t num,uint8_t arraynums,uint8_t bitColumn,uint8_t fixedvalue,uint16_t colourleft,uint16_t colourright)
{		
	unsigned char j;
	unsigned char colourH,colourL;
	if(arraynums<=fixedvalue)
	{	
		colourH=colourleft>>8;
		colourL=colourleft;
	}	
	else
	{	
		colourH=colourright>>8;
		colourL=colourright;
	}	 
	for ( j = 0; j < 4; j++) 
	{			
		if(SventhState_whole==Temperature_e)
		{		
			if(1==arraynums)
				sendOneCharThirdCurrREC( NumberBig[num][bitColumn][j], 0xf8,0x00);	
			else
				sendOneChar( NumberBig[num][bitColumn][j], colourH, colourL);	
		}
		else
			sendOneChar( NumberBig[num][bitColumn][j], colourH, colourL);	
	}					
}
void displayBrightness(uint8_t num,uint8_t arraynums,uint8_t bitColumn,uint8_t fixedvalue,uint16_t colourleft,uint16_t colourright)
{		
	unsigned char j;
	unsigned char colourH,colourL;
	if(arraynums<=fixedvalue)
	{	
		colourH=colourleft>>8;
		colourL=colourleft;
	}	
	else
	{	
		colourH=colourright>>8;
		colourL=colourright;
	}	 
	for ( j = 0; j < 4; j++) 
	{			
		if(SventhState_whole==Brightness_e)
		{		
			if(1==arraynums)
				sendOneCharThirdCurrREC( NumberBig[num][bitColumn][j], 0xf8,0x00);	
			else
				sendOneChar( NumberBig[num][bitColumn][j], colourH, colourL);	
		}
		else
			sendOneChar( NumberBig[num][bitColumn][j], colourH, colourL);	
			
	}					
}
void displayMinutes(uint8_t num,uint8_t arraynums,uint8_t bitColumn,uint8_t fixedvalue,uint16_t colourleft,uint16_t colourright)
{		
	unsigned char j;
	unsigned char colourH,colourL;
	if(arraynums<=fixedvalue)
	{	
		colourH=colourleft>>8;
		colourL=colourleft;
	}	
	else
	{	
		colourH=colourright>>8;
		colourL=colourright;
	}	 
	for ( j = 0; j < 4; j++) 
	{			
		if(SventhState_whole==Minutes_e)
		{		
			if(1==arraynums)
				sendOneCharThirdCurrREC( NumberBig[num][bitColumn][j], 0xf8,0x00);	
			else
				sendOneChar( NumberBig[num][bitColumn][j], colourH, colourL);	
		}
		else
			sendOneChar( NumberBig[num][bitColumn][j], colourH, colourL);	
	}					
}

void displayHelpchar(uint8_t num,uint8_t bitColumn,uint16_t colour)
{		
	unsigned char j;
	unsigned char colourH,colourL;

	colourH=colour>>8;
	colourL=colour;
	for ( j = 0; j < 2; j++) 
	{			
//		if(j == 0)	
			sendOneChar( NumberHelpChar[num][bitColumn][j], colourH, colourL);
//		
//		else
//			sendThreeBit( NumberHelpChar[num][bitColumn][j], colourH, colourL);
	}					
}
void sendTweBit(uint8_t col8num, uint8_t colourH,uint8_t colourL)
{	
	for (unsigned char k = 0; k < 2; k++)
	{	
		if (0x01 & col8num)
		{	
			sendOneBit(colourH,colourL);
		}
		else
		{ 
			sendOneBit(0x00,0x00);
		}
		col8num >>= 1;
	}	
}

void displayNineScale(uint8_t num,uint8_t arraynums,uint8_t bitColumn_smallData)
{		
	unsigned char j;
		
	for ( j = 0; j < 2; j++) 
	{				
		//if(j <= 1) 
			sendOneChar( NumberScale[num][bitColumn_smallData][j], 0xff, 0xff);
		//else
		//	sendFourBit( NumberScale[num][bitColumn_smallData][j], 0xff, 0xff);
	}	
}			
				
void display_nineScale_front(uint8_t num,uint8_t arraynums,uint8_t bitColumn_smallData,uint8_t *offsetbit,uint8_t stopbit)	
{					
	uint8_t unstopbit;	
	unsigned char j;		
	uint8_t j_const=2;			
									
	unstopbit=16-stopbit;				
														
	for ( j = 0; j < j_const; j++) 							
	{															
		uint8_t col8num=NumberScale[num][bitColumn_smallData][j];
		if(j <= 1) 			
		{							
			for (unsigned char k = 0; k < 8; k++)	
			{											
				if(k+1+8*j<=unstopbit)						
				{												
					col8num >>= 1;	
					continue;		
				}					
													
				if (0x01 & col8num)	
				{	
					sendOneBit(0xff,0xff);
				}
				else
				{ 
					sendOneBit(0x00,0x00);
				}
				col8num >>= 1;
				
				(*offsetbit)++;
				if(*offsetbit == stopbit)
					break;
			}
		}

 	}						
}

void sendOneChar_nineScale_front(uint8_t col8num, uint8_t colourH, uint8_t colourL,uint8_t *offsetbit,uint8_t stopbit)
{		
	for (unsigned char k = 0; k < 8; k++)
	{	
		if (0x01 & col8num)
		{	
			sendOneBit(colourH,colourL);
		}
		else
		{ 
			sendOneBit(0x00,0x00);
		}
		col8num >>= 1;
		
		(*offsetbit)++;
		if(*offsetbit == stopbit)
			break;
	}	
}

void sendTweBit_nineScale_front(uint8_t col8num, uint8_t colourH,uint8_t colourL,uint8_t *offsetbit,uint8_t stopbit)
{	
	for (unsigned char k = 0; k < 2; k++)
	{	
		if (0x01 & col8num)
		{	
			sendOneBit(colourH,colourL);
		}
		else
		{ 
			sendOneBit(0x00,0x00);
		}
		col8num >>= 1;
		
		(*offsetbit)++;
		if(*offsetbit == stopbit)
			break;
	}	
}

	
void display_nineScale_behind(uint8_t num,uint8_t arraynums,uint8_t bitColumn_smallData,uint8_t *offsetbit,uint8_t stopbit)
{																																							
	unsigned char j;
	uint8_t j_const=0;
	
	if(stopbit>8)
		j_const=2;
	else 
		j_const=1;
	
	for ( j = 0; j < j_const; j++) 
	{		
		uint8_t col8num=NumberScale[num][ bitColumn_smallData][j];
		if(j <= 1) 	
		{
			for (unsigned char k = 0; k < 8; k++)
			{	
				if(*offsetbit >= stopbit)
					break;
				else
					(*offsetbit)++;
					
				if (0x01 & col8num)
				{	
					sendOneBit(0xff,0xff);
				}
				else
				{ 
					sendOneBit(0x00,0x00);
				}
				col8num >>= 1;
			}
		}
//		else
//		{
//			for (unsigned char k = 0; k < 2; k++)
//			{	
//				if(*offsetbit >= stopbit)
//					break;
//				else
//					(*offsetbit)++;
//				
//				if (0x01 & col8num)
//				{	
//					sendOneBit(0xff,0xff);
//				}
//				else
//				{ 
//					sendOneBit(0x00,0x00);
//				}
//				col8num >>= 1;
// 
//			}	
//		}
 	}	


	
}

void sendOneChar_nineScale_behind(uint8_t col8num, uint8_t colourH, uint8_t colourL,uint8_t *offsetbit,uint8_t stopbit)
{		
	for (unsigned char k = 0; k < 8; k++)
	{	
		if (0x01 & col8num)
		{	
			sendOneBit(colourH,colourL);
		}
		else
		{ 
			sendOneBit(0x00,0x00);
		}
		col8num >>= 1;
		
		(*offsetbit)++;
		if(*offsetbit == stopbit)
			break;
	}	
}

void sendTweBit_nineScale_behind(uint8_t col8num, uint8_t colourH,uint8_t colourL,uint8_t *offsetbit,uint8_t stopbit)
{	
	for (unsigned char k = 0; k < 2; k++)
	{	
		if (0x01 & col8num)
		{	
			sendOneBit(colourH,colourL);
		}
		else
		{ 
			sendOneBit(0x00,0x00);
		}
		col8num >>= 1;
		
		(*offsetbit)++;
		if(*offsetbit == stopbit)
			break;
	}	
}
void displayKepTip(uint8_t num,uint8_t arraynums,uint8_t bitColumn_smallData,uint8_t fixedvalue,uint16_t colourleft,uint16_t colourright)
{							
	unsigned char j;		
	unsigned char colourH,colourL;
	if(arraynums<=fixedvalue)
	{		
		colourH=colourleft>>8;
		colourL=colourleft;
			
		for ( j = 0; j < 3; j++)
		{	
			if((3<=arraynums)&&(arraynums<=6))			
				sendOneCharKepTip_highPixel( NumberMinddle_HighPxiel[num][bitColumn_smallData][j], colourH,colourL);
			else										
				sendOneChar_tip_highpixel( NumberMinddle_HighPxiel[num][bitColumn_smallData][j], colourH, colourL);
		}	
	}		
	else	
	{		
		colourH=colourright>>8;
		colourL=colourright;
		for ( j = 0; j < 3; j++)
		{	
			if((13<=arraynums)&&(arraynums<=16))
				sendOneCharKepTip_highPixel( NumberMinddle_HighPxiel[num][bitColumn_smallData][j], colourH,colourL);
			else
				sendOneChar_tip_highpixel( NumberMinddle_HighPxiel[num][bitColumn_smallData][j], colourH, colourL);
		}		
	}		
}			
			
void displaySelectChannel(uint8_t dataChar,uint8_t charNum,uint8_t columnBit)
{
	unsigned char j;
	for ( j = 0; j < 3; j++)
	{							
//		if((charNum==3)||(charNum==5)||(charNum==7)||(charNum==9)||(charNum==11)||(charNum==13)||(charNum==15)) 
//  			sendOneSelectChannelChar( NumberMinddle_HighPxiel[dataChar][columnBit][j],  0xff, 0xff);
		if((charNum>=6)&&(charNum<=12))
			sendOneSelectChannelChar( NumberMinddle_HighPxiel[dataChar][columnBit][j],  0xff, 0xff);
		else
			sendOneChar( NumberMinddle_HighPxiel[dataChar][columnBit][j],0x00,0x1f);
	}	
	
}


void displaycolorSmallNumMoreColor(uint8_t dataChar,uint8_t charNum,uint8_t columnBit,\
	uint8_t interval1,uint8_t interval2,uint8_t interval3,\
	uint16_t colour1,uint16_t colour2,uint16_t colour3,uint16_t colour4)
{	
	unsigned char j;
	unsigned char colourH,colourL;
	if(charNum<=interval1)
	{
		colourH=colour1>>8;
		colourL=colour1;
	}
	else if((charNum>interval1)&&(charNum<=interval2))
	{
		colourH=colour2>>8;
		colourL=colour2;
	}
	else if((charNum>interval2)&&(charNum<=interval3))
	{
		colourH=colour3>>8;
		colourL=colour3;
	
	}
	else if((charNum>interval3))
	{
		colourH=colour4>>8;
		colourL=colour4;
	}
	for ( j = 0; j < 4; j++)
	{
		sendOneChar( NumberBig[dataChar][columnBit][j],  colourH, colourL);	
	}
}	

void displaycolorPageBigNum(uint8_t num,uint8_t arraynums,uint8_t bitColumn,uint8_t fixedvalue,uint16_t colourleft,uint16_t colourright)
{	
	unsigned char j;
	unsigned char colourH,colourL;
	if(arraynums<=fixedvalue)
	{
		colourH=colourleft>>8;
		colourL=colourleft; 
	}
	else
	{
		colourH=colourright>>8;
		colourL=colourright;
	}
    for ( j = 0; j < 4; j++)
    {
		sendOneChar(NumberBig[num][bitColumn][j], colourH, colourL);
    }			
}

void displaycolorPageBigNum_highPixel(uint8_t num,uint8_t arraynums,uint8_t bitColumn,uint8_t fixedvalue,uint16_t colourleft,uint16_t colourright)
{	
	unsigned char j;
	unsigned char colourH,colourL;
	if(arraynums<=fixedvalue)
	{
		colourH=colourleft>>8;
		colourL=colourleft; 
	}
	else
	{
		colourH=colourright>>8;
		colourL=colourright;
	}
    for ( j = 0; j < 8; j++)
    {
		sendOneChar(NumberBig_8charHighly[num][bitColumn][j], colourH, colourL);
    }			
}



void displaycolorPatternBigNum1(uint8_t num,uint8_t arraynums,uint8_t bitColumn,uint16_t colour)
{	
	unsigned char j;
	unsigned char colourH,colourL;
	colourH=colour>>8;
	colourL=colour;
    for ( j = 0; j < 8; j++)
    {
		sendOnePatternChar1(NumberBig_8charHighly[num][bitColumn][j], colourH, colourL,bitColumn,j);
    }			
}	
	
void displaycolorPatternBigNum2(uint8_t num,uint8_t arraynums,uint8_t bitColumn,uint16_t colour)
{	
	unsigned char j; 
	unsigned char colourH,colourL;
	colourH=colour>>8;
	colourL=colour;
    for ( j = 0; j < 8; j++)
    {
		sendOnePatternChar2(NumberBig_8charHighly[num][bitColumn][j], colourH, colourL,bitColumn,j);
    }			
}	
void displaycolorPatternBigNum3(uint8_t num,uint8_t arraynums,uint8_t bitColumn,uint16_t colour)
{	
	unsigned char j; 
	unsigned char colourH,colourL;
	colourH=colour>>8;
	colourL=colour;
    for ( j = 0; j < 8; j++)
    {
		sendOnePatternChar3(NumberBig_8charHighly[num][bitColumn][j], colourH, colourL,bitColumn,j);
    }			
}
void sendOnePatternChar3(uint8_t col8num, uint8_t colourH, uint8_t colourL,uint8_t bitColumn,uint8_t j)
{		
	for (unsigned char k = 0; k < 8; k++)
	{	
		if((j==0&&k==3)||(j==7&&k==3))
		{
			sendOneBit(0xff,0xff);
		}
		else if (0x01 & col8num)
		{	
			sendOneBit(colourH,colourL);
		}
		else
		{ 
			sendOneBit(0x00,0x00);
		}
		col8num >>= 1;
	}	
}
void sendOnePatternChar2(uint8_t col8num, uint8_t colourH, uint8_t colourL,uint8_t bitColumn,uint8_t j)
{		
	for (unsigned char k = 0; k < 8; k++)
	{	
		if(j==0&&k==3)
		{
			sendOneBit(0xff,0xff);
		}
		else if (0x01 & col8num)
		{	
			sendOneBit(colourH,colourL);
		}
		else
		{ 
			sendOneBit(0x00,0x00);
		}
		col8num >>= 1;
	}	
}

void sendOnePatternChar1(uint8_t col8num, uint8_t colourH, uint8_t colourL,uint8_t bitColumn,uint8_t j)
{		
	for (unsigned char k = 0; k < 8; k++)
	{	
		if(j==0&&k==4)
		{
			sendOneBit(0xff,0xff);
		}
		else if (0x01 & col8num)
		{	
			sendOneBit(colourH,colourL);
		}
		else
		{ 
			sendOneBit(0x00,0x00);
		}
		col8num >>= 1;
	}	
}

void sendOneChar(uint8_t col8num, uint8_t colourH, uint8_t colourL)
{		
	for (unsigned char k = 0; k < 8; k++)
	{	
		if (0x01 & col8num)
		{	
			sendOneBit(colourH,colourL);
		}
		else
		{ 
			sendOneBit(0x00,0x00);
		}
		col8num >>= 1;
	}	
}	
void sendOneChar_tip_highpixel(uint8_t col8num, uint8_t colourH, uint8_t colourL)
{		
	for (unsigned char k = 0; k < 8; k++)
	{	
		if (0x01 & col8num)
		{	
			sendOneBit(colourH,colourL);
		}
		else
		{ 
			sendOneBit(0x00,0x00);
		}
		col8num >>= 1;
	}	
}
void sendFourBit(uint8_t col8num, uint8_t colourH,uint8_t colourL)
{	
	for (unsigned char k = 0; k < 2; k++)
	{	
		if (0x01 & col8num)
		{	
			sendOneBit(colourH,colourL);
		}
		else
		{ 
			sendOneBit(0x00,0x00);
		}
		col8num >>= 1;
	}	
}
void sendThreeBit(uint8_t col8num, uint8_t colourH,uint8_t colourL)
{	
	for (unsigned char k = 0; k < 3; k++)
	{	
		if (0x01 & col8num)
		{	
			sendOneBit(colourH,colourL);
		}
		else
		{ 
			sendOneBit(0x00,0x00);
		}
		col8num >>= 1;
	}	
}

void sendOneSelectChannelChar(uint8_t col8num, uint8_t colourH,uint8_t colourL)
{	
	for (unsigned char k = 0; k < 8; k++)
	{	
		if (0x01 & col8num)
		{	
			sendOneBit(colourH,colourL);
		}
		else
		{ 
			sendOneBit(0x00,0x1f);
		}
		col8num >>= 1;
	}	
}

void sendOneCircleDChar(uint8_t col8num, uint8_t colourH,uint8_t colourL,uint8_t col8num2)
{	
	for (unsigned char k = 0; k < 8; k++)
	{	
		
		if ((0x01&col8num) ==1 && (0x01&col8num2)==0)
		{	
			sendOneBit(colourH,colourL);
		}
		else if((0x01&col8num) ==1 && (0x01&col8num2)==1)
		{
			sendOneBit(0xff,0xff);
		}
		else
		{ 
			sendOneBit(0x00,0x00);
		}
		col8num >>= 1;
		col8num2 >>= 1;
	}	
}

void sendOneCharThirdCurrREC(uint8_t col8num, uint8_t colourH,uint8_t colourL)
{	
	for (unsigned char k = 0; k < 8; k++)
	{	
		if (0x01 & col8num)
		{	
			sendOneBit(0Xff,0Xff);// 0XF800  
		}	
		else
		{ 
			sendOneBit(colourH,colourL);
		}
		col8num >>= 1;
	}	
}
void sendOneCharKepTip(uint8_t col8num, uint8_t colourH,uint8_t colourL)
{	
	for (unsigned char k = 0; k < 8; k++)
	{	
		if (0x01 & col8num)
		{	 
			sendOneBit(0Xff,0Xff);//0XFC
		}
		else
		{ 
			sendOneBit(colourH,colourL);
		}
		col8num >>= 1;
	}	
}
void sendOneCharKepTip_highPixel(uint8_t col8num, uint8_t colourH,uint8_t colourL)
{	
	for (unsigned char k = 0; k < 8; k++)
	{	
		if (0x01 & col8num)
		{	 
			sendOneBit(0Xff,0Xff);//0XFC
		}
		else
		{ 
			sendOneBit(colourH,colourL);
		}
		col8num >>= 1;
	}	
}

void sendOneBit(uint8_t colourH,uint8_t colourL)
{
	while(Spi_GetStatus(M0P_SPI0, SpiTxe) == FALSE);    //等待发送缓冲器空
	Spi_SendData(M0P_SPI0,colourH );//tx_buf[tx_cnt++] 
															
	while(Spi_GetStatus(M0P_SPI0, SpiTxe) == FALSE);    //等待发送缓冲器空
	Spi_SendData(M0P_SPI0,colourL );//tx_buf[tx_cnt++] 
}
 
void displayLogo (void)
{
	displayPicture(LOGO);
}

void displayBlack(void)
{
 	compareSet(500);
/*******这部分执行的目的是要与各个界面都要有相似的运算流程，否则在此处理其他运算速度会太快************/
	unsigned char i;
    BlockWrite();							
	Spi_SetCS(M0P_SPI0,   FALSE);			
	Gpio_ClrIO(GpioPortA, GpioPin15);	//cs 
	Gpio_SetIO(GpioPortB, GpioPin4);	//rs 
											
    for ( i = 0; i < 128; i++)				
    { 
		for (uint8_t j = 0; j < 16; j++)
		{
			for (uint8_t k = 0; k < 8; k++)
			{
				sendOneBit(0x00,0x00);
			}
		}
	}
	Spi_SetCS(M0P_SPI0, TRUE);
	Gpio_SetIO(GpioPortA, GpioPin15);//cs
	/*************end******/
}
void displayPicture(const uint8_t picture[])
{
	unsigned int i;
    BlockWrite();							
	Spi_SetCS(M0P_SPI0,   FALSE);			
	Gpio_ClrIO(GpioPortA, GpioPin15);	//cs
	Gpio_SetIO(GpioPortB, GpioPin4);	//rs
	
    for ( i = 0; i < 240; i++)				
    { 
		for (uint8_t j = 0; j < 30; j++)
		{
 			unsigned char col8num;
 			unsigned char k;
			col8num =picture[30*i+j] ; 
			for (k = 0; k < 8; k++)
			{
				if (0x01 & col8num)
				{ 
					sendOneBit(0x00,0x1f);//
				}
				else
				{
					sendOneBit(0xff,0xff);
				}
                col8num >>= 1;
			}
		}
	}
	Spi_SetCS(M0P_SPI0, TRUE);
	Gpio_SetIO(GpioPortA, GpioPin15);//cs
}  

 
