

#ifndef __ADC_MID_H__
#define __ADC_MID_H__
 
#include "ddl.h"
#define MAXHIDDENCURRENT 5			
#define MINHIDDENCURRENT -5		
									
									
typedef struct
{
	long double AdcDplus;
	long double AdcDmin;
	long double AdcCurrentVIN;
	long double AdcCurrentOUT;
	long double AdcVoltage;
	long double AdcTemp;	
}ADC_SampleOriginalValue_s;

extern ADC_SampleOriginalValue_s Adc_Original;


void AdcSumm_Init(void);
void App_AdcPortInit(void);
void App_AdcInit_VrefInternal(void);
void App_AdcInit_VrefExternal(void);
void App_AdcSQRCfg(void);

void ReadAdcOriginalValue(void);

long double GetDisplayVoltage(void);
long double GetDisplayCurrent(void);
long double GetOriginalCurrent(void);

long double GetDisplayTemp(void);

 

long double GetDisplayDplus(void);
 
long double GetDisplayDmin(void);

long double generateVoltage(void);
uint8_t delayOnly(uint16_t delaytime);

void Currentcalibration(void); 

long double switchReferenceVoltageOfSupply(long double input);

long double stableData(long double input);
#endif /* __ADC_MID_H__ */

