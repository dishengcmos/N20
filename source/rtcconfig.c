
#include "rtcconfig.h"
#include "gpio.h"
#include "adc_mid.h"
#include "flash_mid.h"
#include "displaydriver.h" 
#include "key.h" 
#include "pwm_mid.h"
#include "spi_mid.h"
uint8_t flag;
uint8_t second, minute, hour, week, day, month, year;
uint16_t hourIRQ;
uint32_t TimeTotal,Timethen,TimeForAutoSaved, TimetotalThirdpage;
uint32_t TimethenThirdpage,Timetotaltemporary;


uint32_t TimeTotal_lockScreen;
#define LOCKSCREEN_ONEMIN_FORSEC 60
void Rtc_IRQHandler(void)
{
    if(Rtc_GetPridItStatus() == TRUE)
    {
        hourIRQ++ ; 
		if(hourIRQ>=99)
		{
			hourIRQ=99;
		}
        Rtc_ClearPrdfItStatus();             //清除中断标志位
    }
}

void RTC_Cfg(void)
{
    stc_rtc_initstruct_t RtcInitStruct;
	 
    RtcInitStruct.rtcAmpm = RtcPm;				//24小时制 
    RtcInitStruct.rtcClksrc = RtcClkRcl;		//内部低速时钟
//	RtcInitStruct.rtcClksrc = RtcClkXth256;	 	
    RtcInitStruct.rtcPrdsel.rtcPrdsel =RtcPrds;// RtcPrds;//周期中断类型PRDS
    RtcInitStruct.rtcPrdsel.rtcPrds = Rtc1H;	//Rtc1H;		//周期中断事件间隔
    RtcInitStruct.rtcTime.u8Second = 0x00;	
    RtcInitStruct.rtcTime.u8Minute = 0x00;		//minute;	
    RtcInitStruct.rtcTime.u8Hour   = 0x00;		
    RtcInitStruct.rtcTime.u8Day    = 0x00;		
    RtcInitStruct.rtcTime.u8DayOfWeek = 0x00;	
    RtcInitStruct.rtcTime.u8Month  = 0x00;		
    RtcInitStruct.rtcTime.u8Year   = 0x00;		
    RtcInitStruct.rtcCompen = RtcCompenDisable;	
    RtcInitStruct.rtcCompValue = 0 ;		//补偿值根据实际情况进行补偿
    Rtc_Init(&RtcInitStruct);

}			  

void Display_RtcConfig(void)								
{															
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);	//GPIO外设时钟打开
    Sysctrl_SetPeripheralGate(SysctrlPeripheralRtc,TRUE);	//RTC模块时钟打开  
	Sysctrl_ClkSourceEnable(SysctrlClkRCL, TRUE);			//系统时钟源使能
	
    RTC_Cfg();                                         		//配置RTC
	
    EnableNvic(RTC_IRQn, IrqLevel3, TRUE);                	//开启RTC周期中断向量
	Rtc_AlmIeCmd(TRUE);										//开启闹钟中断
    Rtc_Cmd(TRUE);                                        	//使能RTC开始计数
}			

void timerun(void)
{
	static stc_rtc_time_t readtime;						
	Rtc_ReadDateTime(&readtime); 
	
	second = readtime.u8Second;
	minute = readtime.u8Minute;
	hour   = readtime.u8Hour;
								
//	day    = readtime.u8Day;
//	week   = readtime.u8DayOfWeek;
//	month  = readtime.u8Month;
//	year   = readtime.u8Year;
										
	TimeTotal=hourIRQ*3600+BCDToInt(minute)*60+BCDToInt(second); 
//	if(Current_u32!=0)  
//		 Rtc_Cmd(TRUE); 
//	else if(Current_u32==0)
//		 Rtc_Cmd(FALSE); 
}

uint8_t BCDToInt(uint8_t value)
{
	uint8_t temp = 0;
	temp = (value>>4)*10;
	temp += value&0x0f;
	return temp;
}

void timinglockScreen(void)
{
	if((KeyPress.FlipLeftKey!=NoPress)||(KeyPress.FlipRightKey!=NoPress)\
		||(KeyPress.HelpKey!=NoPress)||(KeyPress.NextKey!=NoPress))
	{
		TimeTotal_lockScreen=TimeTotal;
		
		LockScreen.lock=0;
		LockScreen.unlock=0; 
	}																					//60
	else if((((int)TimeTotal-(int)TimeTotal_lockScreen)>=flash_Cali.flash_Cali_s.SventhState.Minutes*60)&&(flash_Cali.flash_Cali_s.SventhState.Minutes!=0)&&(Interface!=BlackScreen))
	{					
		LockScreen.lock=1;	
		LockScreen.unlock=0;		
 
		compareSet(500);
	}
}

