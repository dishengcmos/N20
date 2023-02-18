
#ifndef _FLASH_MID_
#define _FLASH_MID_

#include "ddl.h"
#include <stdbool.h>

#include "key.h" 
#define FLASH_BLOCK_SIZE	512

#define SECTOR120 0xf000	//#define SECTOR126 0xfc00
#define SECTOR121 0xf200
#define SECTOR122 0xf400
#define SECTOR127 0xfe00
#define ADDRESS_NEXT_SECTOR 0x0200	
								
								
								
#define CALIBRATIONSIZE 256//88+40
#define	NUMTQWSIZE 134

extern uint16_t Address_TQW;

en_result_t writeTimeQW(uint16_t address);
void readTimeQW(uint16_t address);	
void refresh_TimeQW(void);
void assignFlashToVariaTQW(void);
void assignFlashToVariaCali(void);
void allToZero(void);  
void assignFlashNum(void);
en_result_t writeTQW_writebyte(uint16_t address);
void assignTQWtoFlash(void); 
void findTQWaddress(void);
typedef union  
{
	struct{
		struct{
			
			uint8_t 	second;  
			uint8_t		minute;  
			uint16_t	hourIRQ; 
			uint32_t Quantity;
			uint32_t Work;
		}flash_No_s[11];	
								
		uint8_t sectorID;	 
	}flash_tqrid;
	
	uint8_t GBuffer[NUMTQWSIZE];
}flash_TQW_u;

extern flash_TQW_u flash_TQW;



typedef union  
{	
	struct
	{
		enum {Done_cali,No_cali,Doing_cali,blank_cal=0XFF}calibration;		// 默认是1.//0表示已校准 即1表示未校准，,2表示正在校准
		enum {NormalDirection,TurnLeft,TurnRight,UpsideDown} flip;			//前8个字节的第2位。默认是1.即1表示已翻转，					
		uint8_t interfaceNum; 					
		enum {curTwoPointSevenFive_alldoneFlag,curTwoPointSevenFive_done_negativeFlag,curTwoPointSevenFive_done_positiveFlag,curTwoPointSevenFive_noFlag} OnlyOnceForcurTwoPointSevenFiveFlag;	//默认是 ThreeStri_no	
		enum {curZero_critial_alldoneFlag,curZero_critial_negativeFlag,curZero_critial_positiveFlag,curZero_critial_noFlag} CurZeroCritialFlag;	//默认是 ThreeStri_no	
		
		int settingCurrentPage3;
		long double  Cali_curTwoPointSevenFive_positiveValue; 	
		long double  Cali_curTwoPointSevenFive_negativeValue; 	
																
		long double  CurLocal_Zero_critial_positiveValue; 		
		long double  CurLocal_Zero_critial_negativeValue; 		
		
		long double  V3300volt;   	
		long double  K_magni; 		
		long double  Cali_K_fiveVolt;
		long double  Cali_currentTurnZero;
		long double  Cur_outofCaliEquation_zeroOffset_positiveValue;
		long double  Cur_outofCaliEquation_zeroOffset_negativeValue;
		uint16_t 	 Address_FlashTQW;
		uint8_t flashNum;  
		
		enum{
			withoutWireData,
			withWireData,
			obviousResults,
			obviousError,
		}WireRes_e;
		
		struct{
			long double	Vol_withoutwire;
			long double Cur_withoutwire;
			long double Vol_withwire;
			long double Cur_withwire;
			long double Resis_wire;
		}VolCurwire_s;
		
		struct
		{
			setnumber_e Minutes; 
			setnumber_e Brightness;
			setnumber_e Temperature;
		}SventhState;
		
	}flash_Cali_s;
								
	uint8_t CaBuffer[CALIBRATIONSIZE];
}flash_Cali_u;
												
extern	flash_Cali_u	flash_Cali;						 				
										
void readCali(void)	; 
void writeCali(void); 
void writeTQW_EveryTenSeconds(void); 
  

#endif	//_FLASH_MID_
