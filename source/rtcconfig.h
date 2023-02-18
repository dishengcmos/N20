

#ifndef	_RTCCONFIG_
#define	_RTCCONFIG_
#include "rtc.h"

extern	uint8_t second, minute, hour, week, day, month, year;
extern	uint16_t hourIRQ;
extern	uint32_t TimeTotal,Timethen,TimeForAutoSaved;
extern	uint32_t TimethenThirdpage,Timetotaltemporary;

//  	uint8_t g_LockScreen;

extern	uint32_t TimeTotal_lockScreen;
void Display_RtcConfig(void);

void RTC_Cfg(void);
uint8_t BCDToInt(uint8_t value);

void timerun(void); 

void timinglockScreen(void);
#endif //_RTCCONFIG_

