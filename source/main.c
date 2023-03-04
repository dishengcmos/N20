#include "lcd_api.h" 
#include "displaydriver.h" 
#include "sysconfig.h"
#include "rtcconfig.h"
#include "key.h"
#include "adc_mid.h"
#include "flash_mid.h"
#include "pwm_mid.h" 
#include "flash.h" 
#include "chart.h" 
#include "timer3.h" 
#include "main.h" 	
//出货版本  		


int32_t main(void)	
{					
	App_ClkCfg();	
 	pwmStart();		
	//ledInit();	
	Lcd_SpiGpioInit();	
						
	AdcSumm_Init();		
						
	KeyInit();			
	checkStratUpKey3Press();	
	checkStratUpKey1Press();	
	Timethen=TimeTotal;			
	TimeForAutoSaved=TimeTotal; 
	Quantity_then_everyTenSeconds_u32=Quantity_u32;
									
	Flash_Init(6,TRUE);				
	readCali();						
	assignFlashToVariaCali(); 		
	findTQWaddress();				
									
	readTimeQW(Address_TQW); 		
	assignFlashNum(); 					
	assignFlashToVariaTQW();			
	writeTQW_writebyte(Address_TQW);	
	Lcd_Init();							//cs 引脚需要配置，否则会每次的颜色不一致  
	Display_RtcConfig();				
	displayLogoPreview();				
	sleepOut(); 						
	displayLogo();						
										
	delay1ms(10);						
	initInterfaceNum();					
 	while(1)							
	{									
		ReadAdcOriginalValue();		
 		AssignToCacheValue();		
									
		timerun();					
		KeyProgram(); 			//	
									
 		lcdDisplay();				
		timinglockScreen();			// 这个函数要放在按键检测的赋值和按键功能执行之间 
		KeyGeneral();					
		checkStratKey3LiftUp();			
		checkStratKey1LiftUp();			
										
		displayTemperature();			
		displayCurrent();
		displayVoltage();
		writeTQW_EveryTenSeconds(); 
	}								
}	


