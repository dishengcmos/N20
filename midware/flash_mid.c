#include <stdio.h>
#include "flash_mid.h"
#include "flash.h"
#include "key.h" 
#include "cachevalue.h"
#include "rtcconfig.h" 

#include "adc_mid.h" 
#include <math.h>
uint16_t Address_TQW;

flash_TQW_u		flash_TQW;
flash_Cali_u	flash_Cali;


void assignFlashNum(void)
{
	if(flash_Cali.flash_Cali_s.flashNum	== 0xff)
		flash_Cali.flash_Cali_s.flashNum=1;	
}
												
void assignFlashToVariaTQW(void)				
{												
	if(flash_TQW.flash_tqrid.flash_No_s[flash_Cali.flash_Cali_s.flashNum].second!=0xff)   
		second=flash_TQW.flash_tqrid.flash_No_s[flash_Cali.flash_Cali_s.flashNum].second; 
	else 										
		second=0; 
	
	if(flash_TQW.flash_tqrid.flash_No_s[flash_Cali.flash_Cali_s.flashNum].minute!=0xff)
		minute=flash_TQW.flash_tqrid.flash_No_s[flash_Cali.flash_Cali_s.flashNum].minute;
	else
		minute=0;
	
	if(flash_TQW.flash_tqrid.flash_No_s[flash_Cali.flash_Cali_s.flashNum].hourIRQ!=0xffff)
		hourIRQ=flash_TQW.flash_tqrid.flash_No_s[flash_Cali.flash_Cali_s.flashNum].hourIRQ;
	else
		hourIRQ=0;
	
	if(flash_TQW.flash_tqrid.flash_No_s[flash_Cali.flash_Cali_s.flashNum].Quantity!=0xffffffff)
		Quantity_ldoub=flash_TQW.flash_tqrid.flash_No_s[flash_Cali.flash_Cali_s.flashNum].Quantity;
	else
		Quantity_ldoub=0;
		
	if(flash_TQW.flash_tqrid.flash_No_s[flash_Cali.flash_Cali_s.flashNum].Work!=0xffffffff)
		Work_ldoub=flash_TQW.flash_tqrid.flash_No_s[flash_Cali.flash_Cali_s.flashNum].Work; 
	else
		Work_ldoub=0;
								
 	hourIRQ=0;
// 	minute=0;
// 	second=0;
 	TimeTotal=hourIRQ*3600+BCDToInt(minute)*60+BCDToInt(second); 	
	TimeTotal=0;
	Timethen=TimeTotal;			
	TimeForAutoSaved=TimeTotal; 
	
//	Display_RtcConfig();
	//Quantity_f=Quantity_ldoub;
	//Work_f=Work_ldoub; 
}	

//uint32_t Cali_K_fiveVolt;
//float Cali_currentTurnZero;		
void assignFlashToVariaCali(void)
{	
//	Cali_K_fiveVolt=flash_Cali.flash_Cali_s.Cali_K_fiveVolt;
	if(isnan(flash_Cali.flash_Cali_s.Cali_K_fiveVolt))
		flash_Cali.flash_Cali_s.Cali_K_fiveVolt=1;	
	
//	Cali_currentTurnZero=flash_Cali.flash_Cali_s.Cali_currentTurnZero;
 	if(isnan(flash_Cali.flash_Cali_s.Cali_currentTurnZero))
		flash_Cali.flash_Cali_s.Cali_currentTurnZero=0; 
	
	if(flash_Cali.flash_Cali_s.calibration == blank_cal)
		flash_Cali.flash_Cali_s.calibration=No_cali;	
	
	if(flash_Cali.flash_Cali_s.flip == 0xff)	
		flash_Cali.flash_Cali_s.flip =NormalDirection;						
	if(flash_Cali.flash_Cali_s.OnlyOnceForcurTwoPointSevenFiveFlag== 0xff)		
		flash_Cali.flash_Cali_s.OnlyOnceForcurTwoPointSevenFiveFlag=curTwoPointSevenFive_noFlag;	
		
	if(flash_Cali.flash_Cali_s.CurZeroCritialFlag== 0xff)		
		flash_Cali.flash_Cali_s.CurZeroCritialFlag=curZero_critial_noFlag;
								
	if((flash_Cali.flash_Cali_s.WireRes_e == 0xff)||(flash_Cali.flash_Cali_s.WireRes_e== obviousResults)\
		||(flash_Cali.flash_Cali_s.WireRes_e== obviousError))
		flash_Cali.flash_Cali_s.WireRes_e=withoutWireData;
	if(flash_Cali.flash_Cali_s.settingCurrentPage3==0xFFFFFFFF)
		flash_Cali.flash_Cali_s.settingCurrentPage3=0;

	if(flash_Cali.flash_Cali_s.SventhState.Brightness==0xFF)
	{
		flash_Cali.flash_Cali_s.SventhState.Brightness=five;
	} 
	if(flash_Cali.flash_Cali_s.SventhState.Minutes==0xFF)
	{
		flash_Cali.flash_Cali_s.SventhState.Minutes=zero;
	}
	if(flash_Cali.flash_Cali_s.SventhState.Temperature==0xFF)
	{
		flash_Cali.flash_Cali_s.SventhState.Temperature=zero;
	}
 		
}				
				
void allToZero(void)
{	
	second=0;
	minute=0;
	hourIRQ=0;
	Quantity_ldoub=0;
	Work_ldoub=0;	
	Quantity_u32=0;
	Work_u32=0;	
	Timethen=0;	
//	TimeTotal=0;					
//	Display_RtcConfig(); 		
	writeTimeQW(Address_TQW);				
}

//unsigned long long readnumber1=0;	
//unsigned long long readnumber2=0;			
//bool whetherSwitchSectors(en_result_t result) 
//{	 
//	static char i=0;	
//	if((result!=Ok)&&(i++<=3))//4次
//	{
//		return 1;	
//	}
//	else if(((result!=Ok)&&(i>3)) && (flash_Cali.flash_Cali_s.Address_FlashTQW!=0xf000))//实际是第5次		
//	{		
//		flash_Cali.flash_Cali_s.Address_FlashTQW += ADDRESS_NEXT_SECTOR;
//		i=0;
//		writeCali();
//		return 1;	
//	}	
//	else	
//		return 0;	 
//}	
//		
		
//void refresh_TimeQW(void)	
//{
//	en_result_t		enResult = Ok;	
//						
//	do{
//		readTimeQW(flash_Cali.flash_Cali_s.Address_FlashTQW);					
//		enResult=writeTimeQW(flash_Cali.flash_Cali_s.Address_FlashTQW);
// 
//	}while(whetherSwitchSectors(enResult));		//满足条件则循环				
//}		
/************			 
*************/			
void switchoverAddressWriteTQW(void)
{	
	if(Address_TQW==SECTOR121)	
		Address_TQW=SECTOR122;	
	else 			
		Address_TQW=SECTOR121;	
								
	
	writeTimeQW(Address_TQW);	
} 	
	
void refresh_TimeQW(void)
{	
	flash_TQW.flash_tqrid.sectorID++;		
	
	if(flash_TQW.flash_tqrid.sectorID<254)	
	{										
		switchoverAddressWriteTQW();		
	}			
	else if(flash_TQW.flash_tqrid.sectorID>=254)	
	{		
		flash_TQW.flash_tqrid.sectorID=0;	
		switchoverAddressWriteTQW();
	}		
}	
	
/**********寻找tqw地址*************/
//从小到大排列
void bubble_sort(uint32_t arr[], int len) 
{
	uint32_t i, j, temp;
	for (i = 0; i < len - 1; i++)
		for (j = 0; j < len - 1 - i; j++)
		if (arr[j] > arr[j + 1]) 
		{
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
		}
}

uint32_t assgnTime(void)
{
	uint32_t TimeTot;  
	if(flash_TQW.flash_tqrid.flash_No_s[flash_Cali.flash_Cali_s.flashNum].second!=0xff)
		second=flash_TQW.flash_tqrid.flash_No_s[flash_Cali.flash_Cali_s.flashNum].second;
	else
		second=0;

	if(flash_TQW.flash_tqrid.flash_No_s[flash_Cali.flash_Cali_s.flashNum].minute!=0xff)
		minute=flash_TQW.flash_tqrid.flash_No_s[flash_Cali.flash_Cali_s.flashNum].minute;
	else
		minute=0;

	if(flash_TQW.flash_tqrid.flash_No_s[flash_Cali.flash_Cali_s.flashNum].hourIRQ!=0xffff)
		hourIRQ=flash_TQW.flash_tqrid.flash_No_s[flash_Cali.flash_Cali_s.flashNum].hourIRQ;
	else
		hourIRQ=0;
	TimeTot=hourIRQ*3600+BCDToInt(minute)*60+BCDToInt(second); 
	return 	TimeTot;
}

uint32_t totalIdforCompare[47];

void findTQWaddress(void)
{			
	uint16_t address_begin=SECTOR121; 
	uint8_t num_id=0; 			 					 
										
	for(address_begin=SECTOR121;address_begin<=SECTOR122;address_begin+=ADDRESS_NEXT_SECTOR)	//只有两个
	{		
		readTimeQW(address_begin);
		if(flash_TQW.flash_tqrid.sectorID==0xff)	
			totalIdforCompare[num_id]=0;
		else
			totalIdforCompare[num_id]=flash_TQW.flash_tqrid.sectorID;
		num_id++;
	}
		
		
	if((totalIdforCompare[0]==0)&&(totalIdforCompare[1]==254)) 
		Address_TQW=SECTOR121;
	else if	((totalIdforCompare[0]==254)&&(totalIdforCompare[1]==0))
		Address_TQW=SECTOR122;
	else
	{
		if(totalIdforCompare[0]>totalIdforCompare[1])
			Address_TQW=SECTOR121;
		else if(totalIdforCompare[0]<totalIdforCompare[1])
			Address_TQW=SECTOR122;
		else
			Address_TQW=SECTOR121;
	}
		
	/**
	memcpy(totalId_temp,totalIdforCompare,sizeof(int)*2);
	bubble_sort(totalId_temp, 47);
	
	for(i=0;i<46;i++)
	{
		if(totalIdforCompare[i]==totalId_temp[46])
		break;
	}
	Address_TQW=(uint16_t) SECTOR81 + i*ADDRESS_NEXT_SECTOR;
 **/
} 		
		
/************ tqw读取 *************/
void readTimeQW(uint16_t address)					
{	
//	do 
//	{
		for(int i=0;i<NUMTQWSIZE;i++)		
		{													
			flash_TQW.GBuffer[i] = *(uint8_t*)(address+i);
		}
//	}
//	while(flash_TQW.GBuffer[133]!=sumCheck(flash_TQW.GBuffer,133));
}													
/***************end***************/		
										
/************ tqw 写入**************/						
en_result_t writeTimeQW(uint16_t address)					
{	
	en_result_t		enResult = Ok;	
					
	assignTQWtoFlash();						
	enResult = writeTQW_writebyte(address);
	return enResult;
}		
		
void assignTQWtoFlash(void)													
{		
//	flash_TQW.GBuffer[0+flash_Cali.flash_Cali_s.flashNum*12] = second;		
//	flash_TQW.GBuffer[1+flash_Cali.flash_Cali_s.flashNum*12] = minute;		
//	flash_TQW.GBuffer[2+flash_Cali.flash_Cali_s.flashNum*12] = hourIRQ&0xff;//hourIRQ
//	flash_TQW.GBuffer[3+flash_Cali.flash_Cali_s.flashNum*12] = (hourIRQ&0xff00)>>8;//hourIRQ
		
	flash_TQW.GBuffer[4+flash_Cali.flash_Cali_s.flashNum*12] =  Quantity_u32&0xff; 
	flash_TQW.GBuffer[5+flash_Cali.flash_Cali_s.flashNum*12] = (Quantity_u32&0xff00)>>8;
	flash_TQW.GBuffer[6+flash_Cali.flash_Cali_s.flashNum*12] = (Quantity_u32&0xff0000)>>16;
	flash_TQW.GBuffer[7+flash_Cali.flash_Cali_s.flashNum*12] = (Quantity_u32&0xff000000)>>24;
		
	flash_TQW.GBuffer[8+flash_Cali.flash_Cali_s.flashNum*12] =  Work_u32&0xff; 
	flash_TQW.GBuffer[9+flash_Cali.flash_Cali_s.flashNum*12] = (Work_u32&0xff00)>>8;
	flash_TQW.GBuffer[10+flash_Cali.flash_Cali_s.flashNum*12] = (Work_u32&0xff0000)>>16;
	flash_TQW.GBuffer[11+flash_Cali.flash_Cali_s.flashNum*12] = (Work_u32&0xff000000)>>24;
}		
		
en_result_t writeTQW_writebyte(uint16_t address)	
{		
	en_result_t		enResult = Ok;
		
 	enResult	=	Flash_SectorErase(address);
	if(enResult!= Ok )	
		return enResult;
		
	for(int i=0;i<NUMTQWSIZE;i++)  		
	{		
		enResult=Flash_WriteByte(address+i, flash_TQW.GBuffer[i]); 
		if(enResult!=Ok) 
			return enResult; 
	}	
	return 	enResult; 
}		
		
/*******************end*************************/
void readCali(void)   
{	
	for(int i=0;i<CALIBRATIONSIZE;i++)
	{
		flash_Cali.CaBuffer[i]=*(uint8_t*)(SECTOR120+i); 
	}
}	
void writeCali(void)   
{	
	Flash_SectorErase(SECTOR120);
	for(int i=0;i<CALIBRATIONSIZE;i++)
	{
		Flash_WriteByte(SECTOR120+i , flash_Cali.CaBuffer[i]);
	}
}
/***************************		
	10改为1	
	屏蔽	
****************************/

void writeTQW_EveryTenSeconds(void)				
{												
	static bool Hadbeen_EveryTenSeconds; 		
 	if((TimeTotal-TimeForAutoSaved > 10)||((TimeTotal==0)&&(TimeForAutoSaved==0)&&(Hadbeen_EveryTenSeconds==0)) )
	{	
		if((fabs(GetDisplayCurrent())>30)&&(Quantity_u32!=Quantity_then_everyTenSeconds_u32))
		{	 	 					
			Hadbeen_EveryTenSeconds=1; 
			TimeForAutoSaved=TimeTotal; 
			Quantity_then_everyTenSeconds_u32=Quantity_u32;
			/***********************/	
			refresh_TimeQW();		
			/***********************/
			if(TimeTotal!=0)
			{
				Hadbeen_EveryTenSeconds=0;
			}
		}	
	}	
}	

