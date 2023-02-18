
#include "displaydriver.h" 
#include "lcd_api.h"
#include "spi.h"
#include "gpio.h"
#include "adc_mid.h" 
#include "key.h"  
#include "spi_mid.h" 
#include "flash_mid.h" 
#include "chart.h"
#include "wire.h"
#include "main.h"
#include "dishelp.h"
#include "cachevalue.h" 
 
//#define XNAME(n)	switchcase##n() 	
static uint16_t TEMPERATURE_COLOR=YELLOW; 	
static uint16_t REC_Color;
static uint32_t Temperature_u32,Temperature_Fahr_u32;
/****/
void lcdswitchcase(unsigned char numpage,const uint8_t Help_up_r1[],const uint8_t Help_up_r2[],const uint8_t Help_up_r3[],\
	const uint8_t Help_down_r1[],const uint8_t Help_down_r2[],const uint8_t Help_down_r3[],const uint8_t Help_down_r4[],void displayInterface(void),void keyControl(void),void help(unsigned char ,const uint8_t*,const uint8_t*,const uint8_t*,\
	const uint8_t*,const uint8_t*,const uint8_t*,const uint8_t*))
{
	if(g_displayHelpPage==0)
	{
		displayInterface();
		keyControl();
	}				
	else if(g_displayHelpPage==1)
	{
		help(numpage,Help_up_r1,Help_up_r2,Help_up_r3,Help_down_r1,Help_down_r2,Help_down_r3,Help_down_r4);
	}
}
void displayFourthInterfaceBatteryWire(void)
{
	displayFourthInterface(); 
	displaybatteryWire();
}	
void blank(void)
{} 
void lcdDisplay(void)			
{	 
	BlockWrite();
	Spi_SetCS (M0P_SPI0, FALSE);		// 		
	Gpio_ClrIO(GpioPortA, GpioPin15);	//cs	
	Gpio_SetIO(GpioPortB, GpioPin4);	//rs	
	if(LockScreen.lock==0)			
	{								
		switch(Interface)										
		{						
			case FirstInterface:  																																	
				lcdswitchcase(1,Help_p1r1,Help_p1r2,Help_p1r3,Help_p1r4,Help_p1r5,Help_p1r6,Help_p1r_non,&displayFirstInterface,&keyFirSecpageControl, &displayHelp1Interface); 
			break;																																																															
			case SecondInterface:   
				lcdswitchcase(2,Help_p1r1,Help_p1r2,Help_p1r3,Help_p1r_non,Help_p1r_non,Help_p1r_non,Help_p1r_non,&displaySecondInterface,&keyFirSecpageControl,&displayHelp1Interface); 	 
			break;					
			case ThirdInterface: 	
				lcdswitchcase(3,Help_p1r1,Help_p1r2,Help_p1r3,Help_p3r5,Help_p3r6,Help_p3r7,Help_p3r8,&displayThirdInterface,&keyThirdpageControl,&displayHelp1Interface);  	 
			break;	
			case UsbFourLineInterface: 
				lcdswitchcase(4,Help_p1r1,Help_p1r2,Help_p1r3,Help_p4r5,Help_p4r6,Help_p4r7,Help_p4r8,&displayFourthInterfaceBatteryWire,&keyFourthpageControl,&displayHelp1Interface);  	  
			break;	
			case chartVoltageInterface:  
				lcdswitchcase(5,Help_p1r1,Help_p1r2,Help_p1r3,Help_p1r_non,Help_p1r_non,Help_p1r_non,Help_p1r_non,&displayVolChart,&keyCalpage,&displayHelp1Interface);   
			break;	
			case chartCurrentInterface: 
				lcdswitchcase(6,Help_p1r1,Help_p1r2,Help_p1r3,Help_p1r_non,Help_p1r_non,Help_p1r_non,Help_p1r_non,&displayCurChart,&blank,&displayHelp1Interface);   	
			break;	
			case setOptionInterface: 
				lcdswitchcase(7,Help_p1r1,Help_p1r2,Help_p1r3,Help_p3r5,Help_p3r6,Help_p3r7,Help_p3r8,&displaySeventhInterface,&keySeventhpageControl,&displayHelp1Interface); 		  
			break;																
			case BlackScreen:		
				displayBlack();		
				keyCalpage();	
			break;				
			case CaliForCustomInterface: 	 		
				displayCaliForCustomInterface(); 
				Calibrationhandling();
				keyCalipageControl(); 
			break;
			case CaliForFactoryInterface:				
				displayCaliForFactoryInterface(); 
				keyCaliForFactoryInterface();
				disCaliSign();
			break;
			default:
			break;
		}  
	}
	else
	{
		displayBlack();
	}
	
	
	Spi_SetCS(M0P_SPI0, TRUE);
	Gpio_SetIO(GpioPortA, GpioPin15);//cs
}	

void initInterfaceNum(void)					
{
	if((flash_Cali.flash_Cali_s.interfaceNum==0xff)||(flash_Cali.flash_Cali_s.OnlyOnceForcurTwoPointSevenFiveFlag!=curTwoPointSevenFive_alldoneFlag))
		Interface=CaliForFactoryInterface;
	else  if(READKEY1==1)
		Interface=CaliForCustomInterface;
	else if(Interface==CaliForFactoryInterface)
		Interface=FirstInterface;
	else
		Interface=(Interface_e)flash_Cali.flash_Cali_s.interfaceNum;
		 
	for (int i=0;i<DATA_SIZE;i++)
	{
		pVoltline[i]=0xffffffff;	//0xffffffff 00000000
		pCurrline[i]=0xffffffff;	
		voltageOrigiChart[i]=0xffffffff;	
		currentOrigiChart[i]=0xffffffff;
	}
	getRECcolor(RED);
#ifdef DEBUG_MODE
Interface=FirstInterface;	
//flash_Cali.flash_Cali_s.calibration=No_cali;
flash_Cali.flash_Cali_s.flip =NormalDirection;		
#endif
}		

void operationForDisplayNineScale(uint8_t *arraynums,uint8_t *bitColumn)
{
	(*bitColumn)++;
	if(*bitColumn == 8)
	{
		*bitColumn = 0;  
		(*arraynums)++;
	} 
}

void operationHelp(uint8_t *arraynums,uint8_t *bitColumn)
{
	(*bitColumn)++;
	
	if(*bitColumn == 8)
	{
		*bitColumn = 0;  
		(*arraynums)++;
	} 
	
//	if(*arraynums==10)
//	{
//		if(*bitColumn == 3)
//		{
//			*bitColumn = 0;  
//			(*arraynums)++;
//		} 
//	}
//	else 
//	{
//		if(*bitColumn == 6)
//		{
//			*bitColumn = 0;  
//			(*arraynums)++;
//		} 
//	}
}

void operationForDisplayArray(uint8_t *arraynums,uint8_t *bitColumn)
{	
	(*bitColumn)++;
	
	if(*bitColumn == 8)
	{
		*bitColumn = 0;  
		(*arraynums)++;
	} 
}
void operationForDisplayArray_tip_highpixel(uint8_t *arraynums,uint8_t *bitColumn)
{	
	(*bitColumn)++;
	
	if(*bitColumn == 12)
	{
		*bitColumn = 0;  
		(*arraynums)++;
	} 
}


void operationForDisplayArray_highPixel(uint8_t *arraynums,uint8_t *bitColumn)
{	
	(*bitColumn)++;
	
	if(*bitColumn == 16)
	{
		*bitColumn = 0;  
		(*arraynums)++;
	} 
}

void operationSeventh_characterArray(uint8_t *arraynums,uint8_t *bitColumn)
{	
	(*bitColumn)++;
	if((*arraynums==0)||(*arraynums==2))
	{
		if(*bitColumn == 4)
		{
			*bitColumn = 0;  
			(*arraynums)++;
		} 
	}
	else
	{
		if(*bitColumn == 16)
		{
			*bitColumn = 0;  
			(*arraynums)++;
		} 
	}
}
void operationForDisplayArraybig(uint8_t *arraynums,uint8_t *bitColumn)
{
	(*bitColumn)++;
	
	if(*bitColumn == 32)
	{
		*bitColumn = 0;  
		(*arraynums)++;
	} 
}	

void operationForDisplayArraybig_highPixel(uint8_t *arraynums,uint8_t *bitColumn)
{
	(*bitColumn)++;
	
	if(*bitColumn == 32)
	{
		*bitColumn = 0;  
		(*arraynums)++;								
	} 											
}
void operationForDisplayArraybig_PowerOn(uint8_t *arraynums,uint8_t *bitColumn)
{
	(*bitColumn)++;
	
	if(*bitColumn == 32)
	{
		*bitColumn = 0;  
		(*arraynums)++;								
	} 											
}
void operationForDisplayArraybig_highPixel_OffsetRight(uint8_t *arraynums,uint8_t *bitColumn)
{
	(*bitColumn)++;
	if((*arraynums==0)&& ((*bitColumn)>=10))
	{
		*bitColumn = 0;  
		(*arraynums)++;								
	} 	
	
	if(*bitColumn == 32)
	{
		*bitColumn = 0;  
		(*arraynums)++;								
	} 											
}	
 
void displayFirstInterface(void)					
{								
    uint8_t  arraynums1 = 0,arraynums2 = 0,arraynums3 = 0,arraynums4 = 0,arraynums5 = 0,arraynums6 = 0;
    uint8_t  bitColumn1=0,bitColumn2=0,bitColumn3=0,bitColumn4=0,bitColumn5=0,bitColumn6=0;
	uint8_t arraynums2_1=0,arraynums2_2=0;
	uint8_t bitColumn2_1=0,bitColumn2_2=0;
	uint8_t arraynums3_1=0,arraynums3_2=0;
	uint8_t bitColumn3_1=0,bitColumn3_2=0;
	
	assignSelectChannel(1);
    for(int i=0;i<240;i++)		
    {	
		//第一行 
		displaySelectChannel(SelectChannel[arraynums1],arraynums1,bitColumn1);	
		operationForDisplayArray_tip_highpixel( &arraynums1,  &bitColumn1 );					
		//第二行																
		if(arraynums2<=5)														
		{																		
			displaycolorPageBigNum_highPixel(Volt_b[arraynums2],arraynums2,bitColumn2,5,GREEN,YELLOW);
			operationForDisplayArraybig_highPixel( &arraynums2,  &bitColumn2 );
		}
		else
		{	
			displaycolorSmallNum_highPixel(Thermometer_ss[arraynums2_1],arraynums2_1,bitColumn2_1,8,YELLOW,REALBLUE); 			
			operationForDisplayArray_highPixel(&arraynums2_1, &bitColumn2_1 );												
																												
			displaycolorSmallNum_highPixel(Temperature_ss[arraynums2_2],arraynums2_2,bitColumn2_2,8,TEMPERATURE_COLOR,WHITE); 
			operationForDisplayArray_highPixel(&arraynums2_2, &bitColumn2_2);   
																		
			operationForDisplayArraybig_highPixel( &arraynums2,  &bitColumn2 );
		}
		
		//第三行	
		if(arraynums3<=5)
		{
			displaycolorPageBigNum_highPixel(Curr_b[arraynums3],arraynums3,bitColumn3,5,CYAN,RED);
			operationForDisplayArraybig_highPixel( &arraynums3,  &bitColumn3 );
		}
		else
		{	
			displaycolorSmallNum_highPixel(Group_ss_highPixel[arraynums3_1],arraynums3_1,bitColumn3_1,8,RED,REALBLUE); 
			operationForDisplayArray_highPixel( &arraynums3_1, &bitColumn3_1 );
			
			displaycolorSmallNum_highPixel(GroupNom_ss_highPixel[arraynums3_2],arraynums3_2,bitColumn3_2,8,RED,WHITE);
			operationForDisplayArray_highPixel( &arraynums3_2, &bitColumn3_2 );   
			operationForDisplayArraybig_highPixel( &arraynums3,  &bitColumn3 );
		}
		
		//第四行
		displaycolorSmallNum_highPixel(QuanResi_s[arraynums4],arraynums4,bitColumn4,8,RED,REALBLUE); 
		operationForDisplayArray_highPixel( &arraynums4, &bitColumn4 ); 
		
		//第五行
		displaycolorSmallNum_highPixel(WorkPowe_s[arraynums5],arraynums5,bitColumn5,8,YELLOW,WHITE); 
		operationForDisplayArray_highPixel( &arraynums5, &bitColumn5 );
 
 		displayKepTip(KepTip[arraynums6],arraynums6,bitColumn6,7,REALBLUE,REALBLUE); 	
 		operationForDisplayArray_tip_highpixel( &arraynums6,  &bitColumn6 );
    }
}
 
void displaySecondInterface(void)
{
	uint8_t	arraynums1 = 0,arraynums2 = 0,arraynums3 = 0,arraynums4 = 0,arraynums5 = 0,arraynums6 = 0;
    uint8_t	bitColumn1=0,bitColumn2=0,bitColumn3=0,bitColumn4=0,bitColumn5=0,bitColumn6=0;
	uint8_t arraynums2_1=0,arraynums2_2=0;
	uint8_t bitColumn2_1=0,bitColumn2_2=0;
	uint8_t arraynums3_1=0,arraynums3_2=0;
	uint8_t bitColumn3_1=0,bitColumn3_2=0;

	assignSelectChannel(2);
    for(int i=0;i<240;i++)		
    {	
		//第一行	
		displaySelectChannel(SelectChannel[arraynums1],arraynums1,bitColumn1); 
		operationForDisplayArray_tip_highpixel(  &arraynums1,  &bitColumn1 );			
		//第二行	
		if(arraynums2<=5) 
		{
			displaycolorPageBigNum_highPixel(Volt_b[arraynums2],arraynums2,bitColumn2,5,GREEN,YELLOW);
			operationForDisplayArraybig_highPixel(  &arraynums2,  &bitColumn2 );
		}
		else
		{	
			displaycolorSmallNum_highPixel(Thermometer_ss[arraynums2_1],arraynums2_1,bitColumn2_1,8,YELLOW,REALBLUE); 
			operationForDisplayArray_highPixel( &arraynums2_1, &bitColumn2_1 );
			
			displaycolorSmallNum_highPixel(Temperature_ss[arraynums2_2],arraynums2_2,bitColumn2_2,8,TEMPERATURE_COLOR,WHITE);
			operationForDisplayArray_highPixel( &arraynums2_2, &bitColumn2_2 );   
			
			operationForDisplayArraybig_highPixel( &arraynums2,  &bitColumn2 );
		}
		//第三行	
		if(arraynums3<=5)
		{
			displaycolorPageBigNum_highPixel(Curr_b[arraynums3],arraynums3,bitColumn3,5,CYAN,RED);
			operationForDisplayArraybig_highPixel(  &arraynums3,  &bitColumn3 );
		}
		else
		{	
			displaycolorSmallNum_highPixel(Group_ss_highPixel[arraynums3_1],arraynums3_1,bitColumn3_1,8,RED,REALBLUE); 
			operationForDisplayArray_highPixel( &arraynums3_1, &bitColumn3_1 );
			
			displaycolorSmallNum_highPixel(GroupNom_ss_highPixel[arraynums3_2],arraynums3_2,bitColumn3_2,8,RED,WHITE);
			operationForDisplayArray_highPixel( &arraynums3_2, &bitColumn3_2 );   
			operationForDisplayArraybig_highPixel( &arraynums3,  &bitColumn3 );
		}		 
		//第四行
		displayDplusDminSmallNum_highPixel(Dplus_s_highPixel[arraynums4],arraynums4,bitColumn4,0,1,8,RED,RED,DEEP_GRAY,YELLOW,Plus[0]); 
		operationForDisplayArray_highPixel( &arraynums4, &bitColumn4 );			
		//第五行	
		displayDplusDminSmallNum_highPixel(DminusMode_s_highPixel[arraynums5],arraynums5,bitColumn5,0,1,7,REALBLUE,REALBLUE,DEEP_GRAY,YELLOW,Minu[0]);  
		operationForDisplayArray_highPixel( &arraynums5, &bitColumn5 );  	 
		//第六行	
		displayKepTip(KepTip[arraynums6],arraynums6,bitColumn6,7,REALBLUE,REALBLUE); 	
		operationForDisplayArray_tip_highpixel(&arraynums6,&bitColumn6);
    }
}
void displayThirdInterface(void)
{    
    uint8_t  arraynums1 = 0,arraynums2 = 0,arraynums3 = 0,arraynums4 = 0,arraynums5 = 0,arraynums6 = 0;
    uint8_t  bitColumn1=0,bitColumn2=0,bitColumn3=0,bitColumn4=0,bitColumn5=0,bitColumn6=0;
	
	uint16_t rec_color;
	rec_color=returnRECcolor();
	assignSelectChannel(3);
    for(int i=0;i<240;i++)		
    {			 
		//第一行 
		displaySelectChannel(SelectChannel[arraynums1],arraynums1,bitColumn1); 
		operationForDisplayArray_tip_highpixel( &arraynums1,  &bitColumn1);
		//第二行 
		displaycolorPageBigNum_highPixel(Quan[arraynums2],arraynums2,bitColumn2,8,GREEN,GREEN);
		operationForDisplayArraybig_highPixel_OffsetRight(  &arraynums2,  &bitColumn2);
		//第三行 
		displaycolorPageBigNum_highPixel(Work[arraynums3],arraynums3,bitColumn3,8,CYAN,CYAN);
		operationForDisplayArraybig_highPixel_OffsetRight( &arraynums3,  &bitColumn3);
		//第四行 
		displaycolorSmallNum_highPixel(Timing[arraynums4],arraynums4,bitColumn4,16,WHITE,WHITE); 
		operationForDisplayArray_highPixel( &arraynums4, &bitColumn4 );
		//第五行 
		displayThirdCurrRECSmallNum(CurrREC[arraynums5],arraynums5,bitColumn5,8,WHITE,rec_color); 
		//displaycolorSmallNum_highPixel(CurrREC[arraynums5],arraynums5,bitColumn5,8,WHITE,rec_color); 
		operationForDisplayArray_highPixel( &arraynums5, &bitColumn5);  	 
		//第六行 
		displayKepTip(KepTip[arraynums6],arraynums6,bitColumn6,7,REALBLUE,REALBLUE); 	
		operationForDisplayArray_tip_highpixel(  &arraynums6,  &bitColumn6 );
    }							
}																
void displayFourthInterface(void)	
{											
	uint8_t  arraynums1 = 0,arraynums2 = 0,arraynums3 = 0,arraynums4 = 0,arraynums5 = 0,arraynums6 = 0,arraynums7 = 0,arraynums8 = 0;											
	uint8_t  bitColumn1=0,bitColumn2=0,bitColumn3=0,bitColumn4=0,bitColumn5=0,bitColumn6=0,bitColumn7=0,bitColumn8=0;	
	assignSelectChannel(4);		
    for(int i=0;i<240;i++)		
    {							
		displaySelectChannel(SelectChannel[arraynums1],arraynums1,bitColumn1); 
		displaycolorSmallNum_highPixel(VoltageNO1_sb[arraynums2],	arraynums2,bitColumn2,7,GREEN,CYAN); 
		displaycolorSmallNum_highPixel(CurrentNO1_sb[arraynums3],	arraynums3,bitColumn3,7,GREEN,CYAN); 
		displaycolorSmallNum_highPixel(VoltageNO2_sb[arraynums4],	arraynums4,bitColumn4,7,YELLOW,CYAN); 
		displaycolorSmallNum_highPixel(CurrentNO2_sb[arraynums5],	arraynums5,bitColumn5,7,YELLOW,CYAN); 
		displaycolorSmallNum_highPixel(Error_sb[arraynums6],	arraynums6,bitColumn6,16,RED,GRAY); 
		displaycolorSmallNum_highPixel(Resistan_sb[arraynums7],	arraynums7,bitColumn7,16,WHITE,WHITE); 	
		displayKepTip(KepTip[arraynums8], arraynums8,bitColumn8,7,REALBLUE,REALBLUE); 				 
		
		operationForDisplayArray_tip_highpixel(&arraynums1, &bitColumn1 ); 
		operationForDisplayArray_highPixel(&arraynums2, &bitColumn2 );
		operationForDisplayArray_highPixel(&arraynums3, &bitColumn3 );
		operationForDisplayArray_highPixel(&arraynums4, &bitColumn4 );
		operationForDisplayArray_highPixel(&arraynums5, &bitColumn5 );
		operationForDisplayArray_highPixel(&arraynums6, &bitColumn6 );
		operationForDisplayArray_highPixel(&arraynums7, &bitColumn7 );
		operationForDisplayArray_tip_highpixel(&arraynums8, &bitColumn8 );
    }
}	

static int  s_row;			
static uint8_t moveLens;	
void chartvolProcessing(int pData[],uint8_t Ob_chart) 	
{																
	uint8_t arraynumStart= 0,arraynumEnd= 0;
	uint8_t bitColumnStart=0, bitColumnEnd=0;   
	uint8_t arrnum1 = 0,arrnum2 = 0,arrnum3 = 0,arrnum4 = 0,arrnum5 = 0,arrnum6 = 0,arrnum7 = 0,arrnum8 = 0, arrnum9 = 0, arrnum10 = 0, arrnum11 = 0,arrnum12 = 0;
	uint8_t bitColu1=0,bitColu2=0,bitColu3=0,bitColu4=0,bitColu5=0,bitColu6=0, bitColu7=0, bitColu8=0,bitColu9=0,bitColu10=0,bitColu11=0,bitColu12=0;
	uint8_t i;			 
	uint8_t	flag_chart=0;
	
	assignSelectChannel(5);	
	
    for ( i = 0; i < 240; i++)				
    {	
		displaySelectChannel(SelectChannel[arraynumStart],arraynumStart,bitColumnStart); 
		operationForDisplayArray_tip_highpixel(&arraynumStart, &bitColumnStart ); 
		if(i<56)					
		{						
								
 			//display_nineScale_front(scaleVolPlate13[arrnum13],arrnum13,bitColu13,&offsetBit_front,offset_10mv);	
			displayNineScale(scaleVolPlate12[arrnum12],arrnum12,bitColu12);   
			displayNineScale(scaleVolPlate11[arrnum11],arrnum11,bitColu11);  
			displayNineScale(scaleVolPlate10[arrnum10],arrnum10,bitColu10);   
			displayNineScale(scaleVolPlate9[arrnum9],arrnum9,bitColu9);  
			displayNineScale(scaleVolPlate8[arrnum8],arrnum8,bitColu8); 
			displayNineScale(scaleVolPlate7[arrnum7],arrnum7,bitColu7); 
			displayNineScale(scaleVolPlate6[arrnum6],arrnum6,bitColu6); 
			displayNineScale(scaleVolPlate5[arrnum5],arrnum5,bitColu5); 
			displayNineScale(scaleVolPlate4[arrnum4],arrnum4,bitColu4); 
			displayNineScale(scaleVolPlate3[arrnum3],arrnum3,bitColu3); 
			displayNineScale(scaleVolPlate2[arrnum2],arrnum2,bitColu2); 
			displayNineScale(scaleVolPlate1[arrnum1],arrnum1,bitColu1); 
			//display_nineScale_behind(scaleVolPlate1[arrnum1],arrnum1,bitColu1,&offsetBit_behind,16-offset_10mv);	
												
			//operationForDisplayNineScale(&arrnum13,&bitColu13);  
			operationForDisplayNineScale(&arrnum12,&bitColu12);
			operationForDisplayNineScale(&arrnum11,&bitColu11);
			operationForDisplayNineScale(&arrnum10,&bitColu10);
			operationForDisplayNineScale(&arrnum9,&bitColu9);
			operationForDisplayNineScale(&arrnum8,&bitColu8);
			operationForDisplayNineScale(&arrnum7,&bitColu7);
			operationForDisplayNineScale(&arrnum6,&bitColu6);
			operationForDisplayNineScale(&arrnum5,&bitColu5);
			operationForDisplayNineScale(&arrnum4,&bitColu4);
			operationForDisplayNineScale(&arrnum3,&bitColu3);
			operationForDisplayNineScale(&arrnum2,&bitColu2);
			operationForDisplayNineScale(&arrnum1,&bitColu1);
		}
/***/		
		else							
		{							
			for (int j = 0; j < 192; j++)
			{						
				if (pData[i-56]==(191-j)+1)	
				{
					//if(Ob_chart==0xff)
						sendOneBit(0x07,0xE0);	//绿色
					//else
					//	sendOneBit(0x00,0x00);	// 
					
					if(i==56) 
					{
						flag_chart=1;	
						s_row=191-j;	//如果上面的判断语句改成大于或者小于会直接进入这里,则ROW实时变化,会造成线条实时生成
					}	
				}					
				else	//星星背景			
				{	 
					if((((j+1)&0x0f)==0)&&((i&0x0f)==0))
					{ 
						sendOneBit(0xff,0xff);// 
					}
					else
					{
						sendOneBit(0x00,0x00);
					}
				}
			}
			if((i==56)&&(flag_chart==0))
				moveLens=1; 
			else if((i==56)&&(flag_chart==1))
				moveLens=0; 
		}	
/***/			
		displayKepTip(KepTip[arraynumEnd], arraynumEnd,bitColumnEnd,7,REALBLUE,REALBLUE); 				 
		operationForDisplayArray_tip_highpixel( &arraynumEnd,  &bitColumnEnd);
	} 
}
void chartcurrProcessing(int pData[],uint8_t offset_10ma) 	
{		
	uint8_t arraynumStart= 0,arraynumEnd= 0;
	uint8_t bitColumnStart=0, bitColumnEnd=0;
	uint8_t arrnum1 = 0,arrnum2 = 0,arrnum3 = 0,arrnum4 = 0,arrnum5 = 0,arrnum6 = 0,arrnum7 = 0,arrnum8 = 0, arrnum9 = 0, arrnum10 = 0,arrnum11 = 0,arrnum12 = 0;
	uint8_t bitColu1=0,bitColu2=0,bitColu3=0,bitColu4=0,bitColu5=0,bitColu6=0, bitColu7=0, bitColu8=0,bitColu9=0,bitColu10=0,bitColu11=0,bitColu12=0;
	uint8_t i;		 						
	uint8_t	flag_chart=0;	
	 
	assignSelectChannel(6);				
    for ( i = 0; i < 240; i++)				
    {	
		displaySelectChannel(SelectChannel[arraynumStart],arraynumStart,bitColumnStart); 
		operationForDisplayArray_tip_highpixel( &arraynumStart, &bitColumnStart ); 
		if(i<56)					
		{			
		 					
 			//display_nineScale_front(scaleCurPlate13[arrnum13],arrnum13,bitColu13,&offsetBit_front,offset_10ma);	
			displayNineScale(scaleCurPlate12[arrnum12],arrnum12,bitColu12); 			
			displayNineScale(scaleCurPlate11[arrnum11],arrnum11,bitColu11); 			
			displayNineScale(scaleCurPlate10[arrnum10],arrnum10,bitColu10); 			
			displayNineScale(scaleCurPlate9[arrnum9],arrnum9,bitColu9);  		
			displayNineScale(scaleCurPlate8[arrnum8],arrnum8,bitColu8); 		
			displayNineScale(scaleCurPlate7[arrnum7],arrnum7,bitColu7); 		
			displayNineScale(scaleCurPlate6[arrnum6],arrnum6,bitColu6); 		
			displayNineScale(scaleCurPlate5[arrnum5],arrnum5,bitColu5); 	
			displayNineScale(scaleCurPlate4[arrnum4],arrnum4,bitColu4); 	
			displayNineScale(scaleCurPlate3[arrnum3],arrnum3,bitColu3); 	
			displayNineScale(scaleCurPlate2[arrnum2],arrnum2,bitColu2); 	
			displayNineScale(scaleCurPlate1[arrnum1],arrnum1,bitColu1); 	 
			//display_nineScale_behind(scaleCurPlate1[arrnum1],arrnum1,bitColu1,&offsetBit_behind,16-offset_10ma);	
			
			//operationForDisplayNineScale(&arrnum13,&bitColu13);
			operationForDisplayNineScale(&arrnum12,&bitColu12);
			operationForDisplayNineScale(&arrnum11,&bitColu11);
			operationForDisplayNineScale(&arrnum10,&bitColu10);
			operationForDisplayNineScale(&arrnum9,&bitColu9); 
			operationForDisplayNineScale(&arrnum8,&bitColu8);
			operationForDisplayNineScale(&arrnum7,&bitColu7);
			operationForDisplayNineScale(&arrnum6,&bitColu6);
			operationForDisplayNineScale(&arrnum5,&bitColu5);
			operationForDisplayNineScale(&arrnum4,&bitColu4);
			operationForDisplayNineScale(&arrnum3,&bitColu3);
			operationForDisplayNineScale(&arrnum2,&bitColu2);
			operationForDisplayNineScale(&arrnum1,&bitColu1);	
		}	
/***/		
		else						
		{	
			for (int j = 0; j < 192; j++)	
			{
				if (pData[i-56]==(191-j)+1) //向右移动35 	
				{										
					sendOneBit(0x07,0xE0);//绿色    	
					if(i==56) 
					{
						flag_chart=1; 
						s_row=191-j;	//如果上面的判断语句改成大于或者小于会直接进入这里,则ROW实时变化,会造成线条实时生成
					}
				}	
				else					
				{	
					if((((j+1)&0x0f)==3)&&((i&0x0f)==0))
					{ 
						sendOneBit(0xff,0xff);// 
					}
					else
					{
						sendOneBit(0x00,0x00);
					}
				}
			}
			if((i==56)&&(flag_chart==0))
				moveLens=1; 
			else if((i==56)&&(flag_chart==1))
				moveLens=0; 
		}
/***/		
		displayKepTip(KepTip[arraynumEnd], arraynumEnd,bitColumnEnd,7,REALBLUE,REALBLUE); 				 
		operationForDisplayArray_tip_highpixel( &arraynumEnd,  &bitColumnEnd);
	} 
}

void displaySeventhInterface(void)
{
    uint8_t  arraynums_top = 0,arraynums_down = 0;
    uint8_t  bitColumn_top=0,bitColumn_down=0;	
	
	uint8_t  arraynums_left1 = 0,arraynums_left2 = 0,arraynums_left3 = 0;
    uint8_t  bitColumn_left1=0,bitColumn_left2=0,bitColumn_left3=0;
	
	uint8_t  arraynums_right1 = 0,arraynums_right2 = 0,arraynums_right3 = 0;
    uint8_t  bitColumn_right1=0,bitColumn_right2=0,bitColumn_right3=0;

	assignSelectChannel(7);
    for(int i=0;i<240;i++)		
    {	
		//第一行
		displaySelectChannel(SelectChannel[arraynums_top],arraynums_top,bitColumn_top); 
		operationForDisplayArray_tip_highpixel( &arraynums_top,  &bitColumn_top );
		
		if(i==0)
		{	
			sendAnyBlankBit(6);
			for(uint8_t n=0;n<180;n++)	
			{
				sendOneBit(0xff,0xff);
			}
			sendAnyBlankBit(6);
		}							
		else if(i==1)				
		{							
			for(uint8_t n=0;n<192;n++)	
			{						
				if((n==5)||(n==186))	
					sendOneBit(0xff,0xff);
				else
					sendOneBit(0x00,0x00);
			}
		}
		else if(i==56) //64
		{	
			sendAnyBlankBit(4);
			for(uint8_t n=0;n<184;n++)	
			{
				sendOneBit(0xff,0xff);
			}
			sendAnyBlankBit(4);
		} 
		else if(i==238)
		{	
			for(uint8_t n=0;n<192;n++)	
			{
				if((n==5)||(n==186))
					sendOneBit(0xff,0xff);
				else
					sendOneBit(0x00,0x00);
			}
		}				
		else if(i==239)	
		{				
			sendAnyBlankBit(6);
			for(uint8_t n=0;n<180;n++)	
			{	
				sendOneBit(0xff,0xff);
			}	
			sendAnyBlankBit(6);
		}	
 	
		else							
		{	
			if(i<56)//64					
			{							
				displaycolorPatternBigNum1(Pattern_Minutes[arraynums_left1],arraynums_left1,bitColumn_left1,RED);
				operationForDisplayArraybig(  &arraynums_left1,  &bitColumn_left1 );
				
				displaycolorPatternBigNum2(Pattern_Brightness[arraynums_left2],arraynums_left2,bitColumn_left2,GREEN); 
				operationForDisplayArraybig(&arraynums_left2, &bitColumn_left2);			
				
				displaycolorPatternBigNum3(Pattern_CentFahr[arraynums_left3],arraynums_left3,bitColumn_left3,YELLOW); 
				operationForDisplayArraybig(&arraynums_left3, &bitColumn_left3);	
			}	
		/***/		 
			else
			{	 
				sendAnyBlankBit(4);	
				if(2<=i&&i<=239)			
					sendOneBit(0xff,0xff);	
				else						
					sendOneBit(0x00,0x00);	
				sendAnyBlankBit(18);
				
				displayMinutes(Minutes[arraynums_right1],arraynums_right1,bitColumn_right1,8,RED,RED); //32
				operationSeventh_characterArray(&arraynums_right1, &bitColumn_right1); 
				sendAnyBlankBit(9);	
				sendAnyBlankBit(3);	
				if(2<=i&&i<=239)			
					sendOneBit(0xff,0xff);	
				else						
					sendOneBit(0x00,0x00);	
				sendAnyBlankBit(19);			
				displayBrightness(Brightness[arraynums_right2],arraynums_right2,bitColumn_right2,8,GREEN,GREEN); //32
				operationSeventh_characterArray(&arraynums_right2, &bitColumn_right2); 
				sendAnyBlankBit(9);
				
				sendAnyBlankBit(3);	
				if(2<=i&&i<=239)			
					sendOneBit(0xff,0xff);	
				else						
					sendOneBit(0x00,0x00);	
				sendAnyBlankBit(19);			
				displayTemperatureNum(CentFahr[arraynums_right3],arraynums_right3,bitColumn_right3,8,YELLOW,YELLOW); //32
				operationSeventh_characterArray(&arraynums_right3, &bitColumn_right3); 
				
				sendAnyBlankBit(4);
				if(2<=i&&i<=239)			
					sendOneBit(0xff,0xff);	
				else						
					sendOneBit(0x00,0x00);	
				sendAnyBlankBit(4);
			}	
		}	
		//第六行
		displayKepTip(KepTip[arraynums_down],arraynums_down,bitColumn_down,7,REALBLUE,REALBLUE); 	
		operationForDisplayArray_tip_highpixel( &arraynums_down,  &bitColumn_down );
    }	
}
void displayCaliForCustomInterface(void)
{	
	uint8_t  arraynums1 = 0,arraynums2 = 0,arraynums4 = 0,arraynums5 = 0;//arraynums3 = 0,
    uint8_t  bitColumn_smallData1=0,bitColumn_smallData2=0,bitColumn_smallData4=0,bitColumn_smallData5=0;//bitColumn_smallData3=0,
	GreenOptionCalibration();								
    for(int i=0;i<240;i++) 
    {
		displaycolorSmallNum_highPixel(Vref5V_andRealVol[arraynums1],arraynums1,bitColumn_smallData1,22,greenColor.color1,0xffff); 
		displaycolorSmallNum_highPixel(NoLoadCurrent[arraynums2],arraynums2,bitColumn_smallData2,22,greenColor.color2,0x0ffff); 
		displaycolorSmallNumMoreColor(Reset_Exit[arraynums4],arraynums4,bitColumn_smallData4,5,10,22,greenColor.color4,GREEN,greenColor.color5,0xffff);//需要向右移动8个像素
		displaycolorSmallNum_highPixel(Version[arraynums5],arraynums5,bitColumn_smallData5,22,WHITE,0xffff); 

		operationForDisplayArray_highPixel(&arraynums1,&bitColumn_smallData1);
		operationForDisplayArray_highPixel(&arraynums2,&bitColumn_smallData2);
		operationForDisplayArray_highPixel(&arraynums4,&bitColumn_smallData4);
		operationForDisplayArray_highPixel(&arraynums5,&bitColumn_smallData5);
		
		for ( uint8_t i = 0; i < 14; i++)
		{
			sendOneChar( 0x00,0xff,0xff);	
		}
    }
}

void displayCaliForFactoryInterface(void)	
{		
    uint8_t  arraynumsUp=0, arraynums1=0,arraynums2=0,arraynums3=0,arraynums4=0,arraynums5=0,arraynums6=0,arraynumsDown=0;											
    uint8_t  bitColumnUp=0, bitColumn1=0,bitColumn2=0,bitColumn3=0,bitColumn4=0,bitColumn5=0,bitColumn6=0,bitColumnDown=0;	
					 
    for(int i=0;i<240;i++)			
    {																		 
		displayKepTip(CaliKepTipUp[arraynumsUp],arraynumsUp,bitColumnUp,9,YELLOW,CYAN); 		
		operationForDisplayArray_tip_highpixel( &arraynumsUp,  &bitColumnUp);
		
		displaycolorSmallNum_highPixel(CaliFactory1[arraynums1],	arraynums1,bitColumn1,6,WHITE,WHITE); 
		displaycolorSmallNum_highPixel(CaliFactory2[arraynums2],	arraynums2,bitColumn2,7,RED,RED); 
		displaycolorSmallNum_highPixel(CaliFactory3[arraynums3],	arraynums3,bitColumn3,7,RED,RED); 
		displaycolorSmallNum_highPixel(CaliFactory4[arraynums4],	arraynums4,bitColumn4,7,YELLOW,YELLOW); 
		displaycolorSmallNum_highPixel(CaliFactory5[arraynums5],	arraynums5,bitColumn5,16,GREEN,GREEN); 
		displaycolorSmallNum_highPixel(CaliFactory6[arraynums6],	arraynums6,bitColumn6,16,CYAN,CYAN); 	
		
		operationForDisplayArray_highPixel( &arraynums1, &bitColumn1);
		operationForDisplayArray_highPixel( &arraynums2, &bitColumn2);
		operationForDisplayArray_highPixel( &arraynums3, &bitColumn3);
		operationForDisplayArray_highPixel( &arraynums4, &bitColumn4);
		operationForDisplayArray_highPixel( &arraynums5, &bitColumn5);
		operationForDisplayArray_highPixel( &arraynums6, &bitColumn6);

		displayKepTip(CaliKepTipDown[arraynumsDown],arraynumsDown,bitColumnDown,9,RED,GREEN); 	
		operationForDisplayArray_tip_highpixel( &arraynumsDown, &bitColumnDown);
    }
}	

void progressBar(void)
{
	NumberPowerOn_fill();
}

void displayPowerOn(const uint8_t picture1[],const uint8_t picture2[])	
{																
	unsigned int i;								
    BlockWrite();							
	Spi_SetCS(M0P_SPI0,   FALSE);			
	Gpio_ClrIO(GpioPortA, GpioPin15);	//cs
	Gpio_SetIO(GpioPortB, GpioPin4);	//rs
	
	uint8_t arraynums1=0, bitColumn1=0;
	
    for ( i = 0; i < 240; i++)	//一共240列		
    { 
		for (uint8_t j = 0; j < 15; j++)//一列30个字节
		{									
			unsigned char col8num;		
			unsigned char k;			
			col8num =picture1[30*i+j] ; 
			for (k = 0; k < 8; k++)					
			{											
				if (0x01 & col8num)
				{ 
					sendOneBit(0xC6,0x18);//sendOneBit(0xC6,0x18);//灰色//0x8410 #define REALBLUE 0x001F   0Xffe0
				}
				else
				{
					sendOneBit(0x00,0x00);
				}
				col8num >>= 1;
			}
		}
 		displaycolorPageBigNum_PowerOn(Partten_poweron[arraynums1],arraynums1,bitColumn1,8,WHITE,GREEN);
		operationForDisplayArraybig_PowerOn( &arraynums1,  &bitColumn1 );
		
		displayKepTipPowerOn();  

		for (unsigned char j = 0; j < 4; j++)
		{ 		
			for (unsigned char k = 0; k < 8; k++)
			{	
				sendOneBit(0x00,0x00);
			}	
		}
	}
	Spi_SetCS(M0P_SPI0, TRUE);
	Gpio_SetIO(GpioPortA, GpioPin15);//cs
}  

void getRECcolor(uint16_t color)
{
	REC_Color=color;
}
uint16_t returnRECcolor(void)
{
	return REC_Color;
}
int returngrow(void)
{
	return s_row;
} 
uint8_t returnLens(void)
{
	return moveLens;
}

void sendAnyBlankBit(uint8_t num)
{
	for(uint8_t i=0;i<num;i++)
		sendOneBit(0x00,0x00);
}

void assignSelectChannel(char page)
{
	static uint8_t i,polling=10;

	SelectChannel[6]=CHAR_HIGHPIXEL_CIRCLE;
	SelectChannel[7]=CHAR_HIGHPIXEL_CIRCLE;
	SelectChannel[8]=CHAR_HIGHPIXEL_CIRCLE;
	SelectChannel[9]=CHAR_HIGHPIXEL_CIRCLE;
	SelectChannel[10]=CHAR_HIGHPIXEL_CIRCLE;
	SelectChannel[11]=CHAR_HIGHPIXEL_CIRCLE;
	SelectChannel[12]=CHAR_HIGHPIXEL_CIRCLE; 

	SelectChannel[page+5]=CHAR_HIGHPIXEL_BIG_CIRCLE;
 
	if(Current_ldoub>5)
	{
		i++;
		if(i>=2)
		{
			i=0;
			polling++;
			if(polling>12)
			{
				polling=6;
			}
		}
		SelectChannel[polling]=CHAR_HIGHPIXEL_ARROWRIGHT;
	}
	
	else if(Current_ldoub<-5)
	{
		i++;
		if(i>=2)
		{
			i=0;
			polling--;
			if(polling<6)
			{
				polling=12;
			}
		}
		SelectChannel[polling]=CHAR_HIGHPIXEL_ARROWLEFT;
	}
	
} 
void temperatureObvious(void) 
{
	if(flash_Cali.flash_Cali_s.SventhState.Temperature==0)//c
	{
		Thermometer_ss[0]=CHAR_THERMOMETER_C1;
		Thermometer_ss[1]=CHAR_THERMOMETER_C2;
		Thermometer_ss[2]=CHAR_THERMOMETER_C3;
 
		if(getTempC_ld()>=0) 
		{ 
			Temperature_ss[0]= CHAR_NON;//CHAR_BCPU
			Temperature_ss[1]= (Temperature_u32/10)%10;
			Temperature_ss[2]= (Temperature_u32)%10;
		}
		else
		{ 
			Temperature_ss[0]=CHAR_Minus; 
			Temperature_ss[1]=(Temperature_u32/10)%10;
			Temperature_ss[2]=(Temperature_u32)%10;
		}
	}
	else if	(flash_Cali.flash_Cali_s.SventhState.Temperature==1)//f
	{
		if(Temperature_Fahr_u32<100)
		{
			Thermometer_ss[0]=CHAR_THERMOMETER_F1;
			Thermometer_ss[1]=CHAR_THERMOMETER_F2;
			Thermometer_ss[2]=CHAR_THERMOMETER_F3; 
				
			if(getTempF_ld()>=0)
			{ 
				Temperature_ss[0]=CHAR_NON;
				Temperature_ss[1]=(Temperature_Fahr_u32/10)%10;
				Temperature_ss[2]=(Temperature_Fahr_u32)%10;
			}
			else
			{ 
				Temperature_ss[0]=CHAR_Minus;
				Temperature_ss[1]=(Temperature_Fahr_u32/10)%10;
				Temperature_ss[2]=(Temperature_Fahr_u32)%10;
			}
		}
		else
		{
			Thermometer_ss[0]=CHAR_THERMOMETER_F1;
			Thermometer_ss[1]=CHAR_THERMOMETER_F2;
			Thermometer_ss[2]=CHAR_THERMOMETER_F3; 
			 
			Temperature_ss[0]=(Temperature_Fahr_u32/100)%10;
			Temperature_ss[1]=(Temperature_Fahr_u32/10)%10;
			Temperature_ss[2]=(Temperature_Fahr_u32)%10;
		}
	}
}

void temperaturehidden(void)
{
	Temperature_ss[0]=CHAR_NON;
	Temperature_ss[1]=CHAR_NON;	
	Temperature_ss[2]=CHAR_NON; 	
}

void temperatureTwinkle(void)
{
	static uint8_t timeblingcount=0;
	timeblingcount++;
	if(timeblingcount>=18)
		timeblingcount=0;
 
	TEMPERATURE_COLOR=RED;
	if(timeblingcount>9)
		temperatureObvious();
	else
		temperaturehidden();
	TEMPERATURE_COLOR=RED;
}

static long double Temperature_ldoub,Temperature_Fahr_ldoub;

void displayTemperature(void)
{
	static long double Temperature_offset;
	//static uint32_t Temperature_u32,Temperature_Fahr_u32;
	Temperature_ldoub=GetDisplayTemp();	
	
	if(Temperature_ldoub<0)
	{
		Temperature_offset=0.25*Temperature_ldoub;
		Temperature_ldoub+=Temperature_offset;
	}
	
	Temperature_u32=roundOffTemperature(Temperature_ldoub); 
	Temperature_Fahr_ldoub=Temperature_ldoub*1.8+32;		
	Temperature_Fahr_u32=Temperature_Fahr_ldoub;
	
	if(GetDisplayTemp()<45)	
	{	
		TEMPERATURE_COLOR=YELLOW;
		temperatureObvious();	
	}
	else	
	{	
		TEMPERATURE_COLOR=RED;		
		temperatureTwinkle();	
	}

}

long double getTempC_ld(void)
{
	return Temperature_ldoub;
}
long double getTempF_ld(void)
{
	return Temperature_Fahr_ldoub;
}
	
/***********************电压**************************************/
void voltagehidden(void)
{
	Volt_b[0]=CHAR_NON;
	Volt_b[1]=CHAR_NON;	
	Volt_b[2]=CHAR_NON;
	Volt_b[3]=CHAR_NON;	
	Volt_b[4]=CHAR_NON;	
	Volt_b[5]=CHAR_NON;
}

void voltageObvious(void)
{
	if(Voltage_u32>=10000) 	
	{
		Volt_b[0]=(Voltage_u32/10000)%10;
		Volt_b[1]=(Voltage_u32/1000)%10;
		Volt_b[2]=CHAR_POINT;
		Volt_b[3]=(Voltage_u32/100)%10;
		Volt_b[4]=(Voltage_u32/10)%10; 
		Volt_b[5]=CHAR_V;
	}			
	else		
	{		
		Volt_b[0]=(Voltage_u32/1000)%10;
		Volt_b[1]=CHAR_POINT;
		Volt_b[2]=(Voltage_u32/100)%10;
		Volt_b[3]=(Voltage_u32/10)%10;
		Volt_b[4]=(Voltage_u32/1)%10;  
		Volt_b[5]=CHAR_V;			
	}
}	

void voltageTwinkle(void)
{
	static uint8_t timeblingcount=0;
	timeblingcount++;
	if(timeblingcount>=18)
		timeblingcount=0;
 
	if(timeblingcount>9)
		voltageObvious();
	else
		voltagehidden();
}

void displayVoltage(void) 
{ 
	static uint8_t onece=0;
	onece++;
	if(onece>0xf0)
	{
		onece=0xf0;
	}
	
	
	if(((Voltage_u32<3800)||(Voltage_u32>30000))&&(onece>30))
	{	
		voltageTwinkle();	
	}
	else	
	{	
		voltageObvious();		
	}
}
/**********************end****************************/
/***************************电流***********************************/
void currentObvious(void)
{
	Curr_b[0]=(Current_u32/1000)%10;
	Curr_b[1]=CHAR_POINT;
	Curr_b[2]=(Current_u32/100)%10;
	Curr_b[3]=(Current_u32/10)%10;
	Curr_b[4]=(Current_u32)%10; 
	Curr_b[5]=CHAR_A;	
}	
void currenthidden(void)
{
	Curr_b[0]=CHAR_NON; 
	Curr_b[1]=CHAR_NON;
	Curr_b[2]=CHAR_NON;
	Curr_b[3]=CHAR_NON;	
	Curr_b[4]=CHAR_NON;	
	Curr_b[5]=CHAR_NON;
}
void currentTwinkle(void)
{
	static uint8_t timeblingcount=0;
	timeblingcount++;
	if(timeblingcount>=18)
		timeblingcount=0;
 
	if(timeblingcount>9)
		currentObvious();
	else
		currenthidden();
}

void displayCurrent(void) 
{ 
	static uint8_t onece=0;
	onece++;
	if(onece>0xf0)
	{
		onece=0xf0;
	}
	
	if((Current_u32>3000)&&(onece>30))
	{	
		currentTwinkle();	
	}
	else	
	{	
		currentObvious();		
	}
}
/***********************************************************/
