
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
        Rtc_ClearPrdfItStatus();             //����жϱ�־λ
    }
}

void RTC_Cfg(void)
{
    stc_rtc_initstruct_t RtcInitStruct;
	 
    RtcInitStruct.rtcAmpm = RtcPm;				//24Сʱ�� 
    RtcInitStruct.rtcClksrc = RtcClkRcl;		//�ڲ�����ʱ��
//	RtcInitStruct.rtcClksrc = RtcClkXth256;	 	
    RtcInitStruct.rtcPrdsel.rtcPrdsel =RtcPrds;// RtcPrds;//�����ж�����PRDS
    RtcInitStruct.rtcPrdsel.rtcPrds = Rtc1H;	//Rtc1H;		//�����ж��¼����
    RtcInitStruct.rtcTime.u8Second = 0x00;	
    RtcInitStruct.rtcTime.u8Minute = 0x00;		//minute;	
    RtcInitStruct.rtcTime.u8Hour   = 0x00;		
    RtcInitStruct.rtcTime.u8Day    = 0x00;		
    RtcInitStruct.rtcTime.u8DayOfWeek = 0x00;	
    RtcInitStruct.rtcTime.u8Month  = 0x00;		
    RtcInitStruct.rtcTime.u8Year   = 0x00;		
    RtcInitStruct.rtcCompen = RtcCompenDisable;	
    RtcInitStruct.rtcCompValue = 0 ;		//����ֵ����ʵ��������в���
    Rtc_Init(&RtcInitStruct);

}			  

void Display_RtcConfig(void)								
{															
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);	//GPIO����ʱ�Ӵ�
    Sysctrl_SetPeripheralGate(SysctrlPeripheralRtc,TRUE);	//RTCģ��ʱ�Ӵ�  
	Sysctrl_ClkSourceEnable(SysctrlClkRCL, TRUE);			//ϵͳʱ��Դʹ��
	
    RTC_Cfg();                                         		//����RTC
	
    EnableNvic(RTC_IRQn, IrqLevel3, TRUE);                	//����RTC�����ж�����
	Rtc_AlmIeCmd(TRUE);										//���������ж�
    Rtc_Cmd(TRUE);                                        	//ʹ��RTC��ʼ����
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

