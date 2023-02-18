
#include "key.h"
#include "gpio.h"
#include "cachevalue.h"
#include "flash_mid.h" 
#include "spi_mid.h" 
#include "adc_mid.h" 
#include "rtcconfig.h"  
#include <math.h>
#include "pwm_mid.h"
#include "wire.h"

bool StratUpKey3;
bool StratUpKey1;
uint32_t WarningObviTimcount=0;
uint32_t WarningObviTimcount_showWarning=0;
ChangeWay_e ChangeWay;
greenOption_e greenOption;
greenColor_s greenColor;
CalibrationOption_s CalibrationOption;

KeyPress_s KeyPress;
SettingState_s SettingState;
SventhState_whole_e		SventhState_whole;
LockScreen_s LockScreen;
uint8_t g_displayHelpPage;

void KeyProgram(void)
{
	getKeyState();
}
 
void getKeyState(void)
{	
	if(flash_Cali.flash_Cali_s.flip==NormalDirection)
	{	
		key1Detect(&KeyPress.FlipLeftKey);
		key2Detect(&KeyPress.FlipRightKey);
		key3Detect(&KeyPress.HelpKey);
		key4Detect(&KeyPress.NextKey);
	}		
	else if(flash_Cali.flash_Cali_s.flip==TurnLeft)
	{
		key1Detect(&KeyPress.FlipRightKey);
		key2Detect(&KeyPress.NextKey);
		key3Detect(&KeyPress.FlipLeftKey);
		key4Detect(&KeyPress.HelpKey);
	} 	
	else if(flash_Cali.flash_Cali_s.flip==TurnRight)
	{
		key1Detect(&KeyPress.HelpKey);
		key2Detect(&KeyPress.FlipLeftKey);
		key3Detect(&KeyPress.NextKey);
		key4Detect(&KeyPress.FlipRightKey);
	} 	
	else if(flash_Cali.flash_Cali_s.flip==UpsideDown)	
	{
		key1Detect(&KeyPress.NextKey);
		key2Detect(&KeyPress.HelpKey);
		key3Detect(&KeyPress.FlipRightKey);
		key4Detect(&KeyPress.FlipLeftKey);
	} 
}	
	
void keyFirSecpageControl(void)
{	
	if((KeyPress.HelpKey==LongPress)&&(Interface==FirstInterface))
	{
		KeyPress.HelpKey = NoPress;
		allToZero();
	}
	else if(KeyPress.NextKey==LongPress)
	{	
		KeyPress.NextKey = NoPress;
			
		assignTQWtoFlash();	//时间 电荷 功 存到flash里
		 
		writeTQW_writebyte(Address_TQW);//写flash
		
		flash_Cali.flash_Cali_s.flashNum++; 
		
		if(flash_Cali.flash_Cali_s.flashNum>10) 
		{
			flash_Cali.flash_Cali_s.flashNum=1;	
		}
		writeCali();			//存num	
		assignFlashToVariaTQW();//从flash读取 时间 电荷 功		
		
		Quantity_u32=Quantity_ldoub;
		Work_u32=Work_ldoub;
		
		assignTQWtoFlash();		//时间 电荷 功 存到flash里
		writeTQW_writebyte(Address_TQW);//写flash
		//assignFlashNum();
	}
}
void keyThirdpageControl(void)
{		
	if((KeyPress.NextKey==LongPress)&&(SettingState.page3==0))
	{	
		KeyPress.NextKey = NoPress;
		SettingState.page3=1;
	}	
	else if((KeyPress.NextKey==LongPress)&&(SettingState.page3==1))
	{	
		KeyPress.NextKey = NoPress;
		SettingState.page3=0;
		writeCali();
	}
	
	if((KeyPress.NextKey==ShortPress)&&(SettingState.page3==1))
	{		
		KeyPress.NextKey = NoPress; 
		flash_Cali.flash_Cali_s.settingCurrentPage3+=10;
		if(flash_Cali.flash_Cali_s.settingCurrentPage3>300)
		{
			flash_Cali.flash_Cali_s.settingCurrentPage3=0;
		}
	}
	CurrREC[5]=(flash_Cali.flash_Cali_s.settingCurrentPage3/100)%10;
	CurrREC[6]=(flash_Cali.flash_Cali_s.settingCurrentPage3/10)%10;
}


void keyFourthpageControl(void)
{
	static long double vol_diffvaue, cur_aver;
		
	switch(flash_Cali.flash_Cali_s.WireRes_e)
	{
		case withoutWireData:  
			if(KeyPress.NextKey==LongPress) 
			{
				KeyPress.NextKey = NoPress;
				flash_Cali.flash_Cali_s.WireRes_e=withWireData;
				writeCali();
			}
		break;
		case withWireData:   
			if(KeyPress.NextKey==LongPress) 
			{
				KeyPress.NextKey = NoPress;  
				vol_diffvaue=flash_Cali.flash_Cali_s.VolCurwire_s.Vol_withoutwire - flash_Cali.flash_Cali_s.VolCurwire_s.Vol_withwire;
				cur_aver=(flash_Cali.flash_Cali_s.VolCurwire_s.Cur_withwire+flash_Cali.flash_Cali_s.VolCurwire_s.Cur_withoutwire)/2;
				
				if(vol_diffvaue<0||cur_aver==0)
				{
					flash_Cali.flash_Cali_s.WireRes_e=obviousError;
				}
				else
				{
					flash_Cali.flash_Cali_s.VolCurwire_s.Resis_wire=vol_diffvaue*1000/cur_aver	;
					flash_Cali.flash_Cali_s.WireRes_e=obviousResults;
				}
				writeCali();
			}
		break;
		case obviousResults:    
			
		break;
		case obviousError:   
 
		break;    
		default:
		break;
	}		
}			
			
void keyMinutesControl(void)
{			
	switch(flash_Cali.flash_Cali_s.SventhState.Minutes)
	{		
        case zero: 
			if(KeyPress.NextKey==ShortPress)	
			{
				KeyPress.NextKey=NoPress;
				flash_Cali.flash_Cali_s.SventhState.Minutes=one; 
			}
			break;
        case one:  
			if(KeyPress.NextKey==ShortPress)	
			{
				KeyPress.NextKey=NoPress;
				flash_Cali.flash_Cali_s.SventhState.Minutes=two; 
			}
			break;
        case two:  
			if(KeyPress.NextKey==ShortPress)	
			{
				KeyPress.NextKey=NoPress;
				flash_Cali.flash_Cali_s.SventhState.Minutes=three; 
			}
			break;
        case three: 
			if(KeyPress.NextKey==ShortPress)	
			{
				KeyPress.NextKey=NoPress;
				flash_Cali.flash_Cali_s.SventhState.Minutes=four; 
			}
			break;
        case four: 
			if(KeyPress.NextKey==ShortPress)	
			{
				KeyPress.NextKey=NoPress;
				flash_Cali.flash_Cali_s.SventhState.Minutes=five; 
			}
			
			break;
        case five: 
			if(KeyPress.NextKey==ShortPress)	
			{
				KeyPress.NextKey=NoPress;
				flash_Cali.flash_Cali_s.SventhState.Minutes=six; 
			}
			break;
        case six: 
			if(KeyPress.NextKey==ShortPress)	
			{
				KeyPress.NextKey=NoPress;
				flash_Cali.flash_Cali_s.SventhState.Minutes=seven; 
			}
			break;
        case seven: 
			if(KeyPress.NextKey==ShortPress)	
			{
				KeyPress.NextKey=NoPress;
				flash_Cali.flash_Cali_s.SventhState.Minutes=eight; 
			}
			
			break;
        case eight: 
			if(KeyPress.NextKey==ShortPress)	
			{
				KeyPress.NextKey=NoPress;
				flash_Cali.flash_Cali_s.SventhState.Minutes=nine; 
			}
			break;
        case nine: 
			if(KeyPress.NextKey==ShortPress)	
			{
				KeyPress.NextKey=NoPress;
				flash_Cali.flash_Cali_s.SventhState.Minutes=zero; 
			}
			break;
		default: 
		break;
	}
}

void keyBrightnessControl(void)
{
	switch(flash_Cali.flash_Cali_s.SventhState.Brightness)
	{
        case zero: 
			if(KeyPress.NextKey==ShortPress)	
			{
				KeyPress.NextKey=NoPress;
				flash_Cali.flash_Cali_s.SventhState.Brightness=one; 
			}
			break;
        case one:  
			if(KeyPress.NextKey==ShortPress)	
			{
				KeyPress.NextKey=NoPress;
				flash_Cali.flash_Cali_s.SventhState.Brightness=two; 
			}
			break;
        case two:  
			if(KeyPress.NextKey==ShortPress)	
			{
				KeyPress.NextKey=NoPress;
				flash_Cali.flash_Cali_s.SventhState.Brightness=three; 
			}
			break;
        case three: 
			if(KeyPress.NextKey==ShortPress)	
			{
				KeyPress.NextKey=NoPress;
				flash_Cali.flash_Cali_s.SventhState.Brightness=four; 
			}
			break;
        case four: 
			if(KeyPress.NextKey==ShortPress)	
			{
				KeyPress.NextKey=NoPress;
				flash_Cali.flash_Cali_s.SventhState.Brightness=five; 
			}
			
			break;
        case five: 
			if(KeyPress.NextKey==ShortPress)	
			{
				KeyPress.NextKey=NoPress;
				flash_Cali.flash_Cali_s.SventhState.Brightness=zero; 
			}
			break;
		default: 
		break;
	}
}

void keyTemperatureControl(void)
{
	switch(flash_Cali.flash_Cali_s.SventhState.Temperature)
	{
        case zero: 
			if(KeyPress.NextKey==ShortPress)	
			{
				KeyPress.NextKey=NoPress;
				flash_Cali.flash_Cali_s.SventhState.Temperature=one; 
			}
			break;
        case one:  
			if(KeyPress.NextKey==ShortPress)	
			{
				KeyPress.NextKey=NoPress;
				flash_Cali.flash_Cali_s.SventhState.Temperature=zero; 
			}
			break;
		default: 
		break;
	}
}

void keySeventhpageControl(void)
{
	static uint8_t onece=0;
	switch(SventhState_whole)
	{
        case None_e: 
			if(KeyPress.NextKey==LongPress)	
			{
				KeyPress.NextKey=NoPress;
				SventhState_whole=Minutes_e; 
			}
			if(onece==0)
			{
				onece=1;	 
				writeCali(); 
			}
			break;
        case Minutes_e: 
			keyMinutesControl();
			if(KeyPress.NextKey==LongPress)	
			{ 
				KeyPress.NextKey=NoPress; 
				SventhState_whole=Brightness_e; 
			} 
		break;
        case Brightness_e:
			keyBrightnessControl();
			if(KeyPress.NextKey==LongPress)	
			{
				KeyPress.NextKey=NoPress; 
				SventhState_whole=Temperature_e; 
			}   
		break;
        case Temperature_e:  
			
			keyTemperatureControl();
			if(KeyPress.NextKey==LongPress)	
			{
				KeyPress.NextKey=NoPress; 
				SventhState_whole=None_e; 
				onece=0;
			} 
			
		break;
		default: 
		break;
	}
}

void brightnessControl(void)
{	
	delayLockScreen();		
	if((LockScreen.lock==0)&&(Interface!=BlackScreen))	
 	{														
		if(flash_Cali.flash_Cali_s.SventhState.Brightness==zero)
			compareSet(450); 
		if(flash_Cali.flash_Cali_s.SventhState.Brightness==one)
			compareSet(350);
		if(flash_Cali.flash_Cali_s.SventhState.Brightness==two)
			compareSet(250);
		if(flash_Cali.flash_Cali_s.SventhState.Brightness==three)
			compareSet(150);
		if(flash_Cali.flash_Cali_s.SventhState.Brightness==four)
			compareSet(50);
		if(flash_Cali.flash_Cali_s.SventhState.Brightness==five)
			compareSet(0);//之前这个值是0
	}						
}						
						
void KeyGeneral(void)	
{	
	if(Interface==BlackScreen)//||Interface==CaliForFactoryInterface)
	{	
		if((KeyPress.HelpKey==ShortPress)||(KeyPress.FlipLeftKey==ShortPress)||(KeyPress.FlipRightKey==ShortPress))
		{	
			KeyPress.HelpKey=NoPress; 
			KeyPress.FlipLeftKey=NoPress; 
			KeyPress.FlipRightKey=NoPress;
		}
	}
	
	if((KeyPress.NextKey==ShortPress)&&(SettingState.page3==0)&&(SventhState_whole==None_e)&&(Interface!=CaliForCustomInterface)&&(Interface!=CaliForFactoryInterface)) 
	{
		KeyPress.NextKey=NoPress;	
		g_displayHelpPage=0;	
		Interface++;	
		if(Interface>=EndNoExistface)    
		{	
			Interface=FirstInterface;									
			if(flash_Cali.flash_Cali_s.SventhState.Brightness==zero)	
				compareSet(450); 
			if(flash_Cali.flash_Cali_s.SventhState.Brightness==one)
				compareSet(350);
			if(flash_Cali.flash_Cali_s.SventhState.Brightness==two)
				compareSet(250);
			if(flash_Cali.flash_Cali_s.SventhState.Brightness==three)
				compareSet(150);
			if(flash_Cali.flash_Cali_s.SventhState.Brightness==four)
				compareSet(50);
			if(flash_Cali.flash_Cali_s.SventhState.Brightness==five)
				compareSet(0);
		}	
		if ((Interface==FirstInterface)||(Interface==SecondInterface)||(Interface==ThirdInterface)||(Interface==UsbFourLineInterface)\
			||(Interface==chartVoltageInterface)||(Interface==chartCurrentInterface)||(Interface==setOptionInterface)
		)	
		{	
			flash_Cali.flash_Cali_s.interfaceNum=Interface;
			writeCali();
		}	
		else
		{	
			flash_Cali.flash_Cali_s.interfaceNum=FirstInterface;
			writeCali();
		}	
		refresh_TimeQW();
	}		
	
	if((KeyPress.HelpKey==ShortPress)&&(SettingState.page3==0)&&(SventhState_whole==None_e)&&(Interface!=CaliForCustomInterface)&&(Interface!=CaliForFactoryInterface)) 
	{
		KeyPress.HelpKey=NoPress;	
		g_displayHelpPage=0;	
		if(Interface==FirstInterface)    
		{	
			Interface=setOptionInterface;									
			if(flash_Cali.flash_Cali_s.SventhState.Brightness==zero)	
				compareSet(450); 
			if(flash_Cali.flash_Cali_s.SventhState.Brightness==one)
				compareSet(350);
			if(flash_Cali.flash_Cali_s.SventhState.Brightness==two)
				compareSet(250);
			if(flash_Cali.flash_Cali_s.SventhState.Brightness==three)
				compareSet(150);
			if(flash_Cali.flash_Cali_s.SventhState.Brightness==four)
				compareSet(50);
			if(flash_Cali.flash_Cali_s.SventhState.Brightness==five)
				compareSet(0);
		} 
		else
			Interface--;			
	
		if ((Interface==FirstInterface)||(Interface==SecondInterface)||(Interface==ThirdInterface)||(Interface==UsbFourLineInterface)\
			||(Interface==chartVoltageInterface)||(Interface==chartCurrentInterface)||(Interface==setOptionInterface)
		)	
		{	
			flash_Cali.flash_Cali_s.interfaceNum=Interface;
			writeCali();
		}	
		else
		{	
			flash_Cali.flash_Cali_s.interfaceNum=FirstInterface;
			writeCali();
		}	
		refresh_TimeQW();
	}
	
	if((KeyPress.FlipLeftKey==ShortPress)&&(Interface!=CaliForFactoryInterface)) 
	{
		KeyPress.FlipLeftKey=NoPress; 
		towardsLeft();
		writeCali();
	}
	
	if((KeyPress.FlipRightKey==ShortPress)&&(Interface!=CaliForFactoryInterface)) 
	{	
		KeyPress.FlipRightKey=NoPress;
		towardsRight();
		writeCali();
	}
	
	if(Interface==FirstInterface||Interface==SecondInterface \
	||Interface==ThirdInterface ||Interface==UsbFourLineInterface \
	||Interface==chartVoltageInterface||Interface==chartCurrentInterface ||Interface==setOptionInterface)
	{
		if((KeyPress.FlipLeftKey==LongPress)&&(g_displayHelpPage==0))
		{
			KeyPress.FlipLeftKey=NoPress;
			g_displayHelpPage=1;
			
			KeyPress.FlipLeftKey=NoPress;
			KeyPress.FlipRightKey=NoPress;
			KeyPress.HelpKey=NoPress;
			KeyPress.NextKey=NoPress;
			
		}
		else if((KeyPress.FlipLeftKey==LongPress)&&(g_displayHelpPage==1))
		{	
			KeyPress.FlipLeftKey=NoPress;
			g_displayHelpPage=0;
			
			KeyPress.FlipLeftKey=NoPress;
			KeyPress.FlipRightKey=NoPress;
			KeyPress.HelpKey=NoPress;
			KeyPress.NextKey=NoPress;
		}	
	}
	brightnessControl();
	
	//空白的按键状态清除
	if(Interface==FirstInterface||Interface==SecondInterface \
	||Interface==ThirdInterface ||Interface==UsbFourLineInterface \
	||Interface==chartCurrentInterface ||Interface==setOptionInterface)//不包含电压曲线界面
	{
		if(Interface!=FirstInterface)//在除了第一个界面外，长按PREV没有任何作用
		{	
			if(KeyPress.HelpKey==LongPress)
			{
				KeyPress.HelpKey=NoPress;
			}
		}	 

		if(KeyPress.FlipRightKey==LongPress)//长按 FlipRightKey 没有任何作用
		{
			KeyPress.FlipRightKey=NoPress; 
		}

		if(Interface==chartCurrentInterface)//电流曲线界面长按 NEXT没有任何作用
		{	
			if(KeyPress.NextKey==LongPress)
			{
				KeyPress.NextKey=NoPress;
			}
		}
	}
}
 
void normalRotate(void)
{
	flash_Cali.flash_Cali_s.flip=NormalDirection;
	WriteComm(0x36);	//
	WriteData(0x94);	// 
}
void leftRotate(void)
{
	flash_Cali.flash_Cali_s.flip=TurnLeft;
	WriteComm(0x36);	//
	WriteData(0xe0);	//e0
}
void rightRotate(void)
{
	flash_Cali.flash_Cali_s.flip=TurnRight;
	WriteComm(0x36);	//
	WriteData(0x20);	// 
}
void upsidedownRotate(void)
{
	flash_Cali.flash_Cali_s.flip=UpsideDown;
	WriteComm(0x36);	//
	WriteData(0x40);	//
}
	
void towardsLeft(void)
{
	if(flash_Cali.flash_Cali_s.flip==NormalDirection)
	{
		leftRotate();
	}
	else if(flash_Cali.flash_Cali_s.flip==TurnLeft)
	{
		upsidedownRotate();
	}
	else if(flash_Cali.flash_Cali_s.flip==UpsideDown)
	{
		rightRotate();
	}
	else if(flash_Cali.flash_Cali_s.flip==TurnRight)
	{ 
		normalRotate();
	}
}
void towardsRight(void)
{
	if(flash_Cali.flash_Cali_s.flip==NormalDirection)
	{ 
		rightRotate();
	}
	else if(flash_Cali.flash_Cali_s.flip==TurnRight)
	{ 
		upsidedownRotate();
	}
	else if(flash_Cali.flash_Cali_s.flip==UpsideDown)
	{ 
		leftRotate();
	}
	else if(flash_Cali.flash_Cali_s.flip==TurnLeft)
	{ 
		normalRotate();
	}
}
void KeyInit(void)
{	
	//pb7输入(硬件上拉)
    stc_gpio_cfg_t GpioInitStruct;
    
	DDL_ZERO_STRUCT(GpioInitStruct);
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);
    ///< 端口方向配置->输入 
    GpioInitStruct.enDir = GpioDirIn;
    ///< 端口驱动能力配置->高驱动能力
    GpioInitStruct.enDrv = GpioDrvL;
    ///< 端口上下拉配置->无
    GpioInitStruct.enPu = GpioPuDisable;
    GpioInitStruct.enPd = GpioPdDisable;
    ///< 端口开漏输出配置->开漏输出关闭
    GpioInitStruct.enOD = GpioOdDisable;
    ///< 端口输入/输出值寄存器总线控制模式配置->AHB
    GpioInitStruct.enCtrlMode = GpioAHB;
    ///< GPIO IO USER KEY初始化
	Gpio_Init(KEY1_PORT, KEY1_PIN, &GpioInitStruct);
 	Gpio_Init(KEY2_PORT, KEY2_PIN, &GpioInitStruct);
	Gpio_Init(KEY3_PORT, KEY3_PIN, &GpioInitStruct);
	Gpio_Init(KEY4_PORT, KEY4_PIN, &GpioInitStruct);
}
   
void keyCaliForFactoryInterface(void)
{
	if(flash_Cali.flash_Cali_s.calibration!=Done_cali)
	{
		if(
		  KeyPress.FlipLeftKey==ShortPress
		||KeyPress.FlipLeftKey==LongPress
		||KeyPress.FlipRightKey==ShortPress
		||KeyPress.FlipRightKey==LongPress
		||KeyPress.HelpKey==ShortPress
			
		||KeyPress.NextKey==ShortPress
		||KeyPress.NextKey==LongPress)
		{ 	
			KeyPress.FlipLeftKey=NoPress; 
			KeyPress.FlipRightKey=NoPress; 
			KeyPress.HelpKey=NoPress; 
			KeyPress.NextKey=NoPress;
		}	
		else if(KeyPress.HelpKey==LongPress)
		{ 	
//			flash_Cali.flash_Cali_s.a_cal=0x01;
//			writeCali();
		}	
	}		
			
	else if(isnan(flash_Cali.flash_Cali_s.CurLocal_Zero_critial_positiveValue))//无效值
	{
		if(KeyPress.FlipLeftKey==ShortPress
		||KeyPress.FlipLeftKey==LongPress
		||KeyPress.FlipRightKey==ShortPress
		||KeyPress.FlipRightKey==LongPress

		||KeyPress.HelpKey==LongPress
		||KeyPress.NextKey==ShortPress
		||KeyPress.NextKey==LongPress)
		{
			KeyPress.FlipLeftKey=NoPress; 
			KeyPress.FlipRightKey=NoPress; 
			KeyPress.HelpKey=NoPress; 
			KeyPress.NextKey=NoPress;
		}
		else if(KeyPress.HelpKey==ShortPress)
		{ 
//			flash_Cali.flash_Cali_s.a_cal=0x02;
//			writeCali();
		}
	}
 
	else if(isnan(flash_Cali.flash_Cali_s.Cali_curTwoPointSevenFive_positiveValue))//无效值
	{
			if(
			KeyPress.FlipLeftKey==LongPress
			||KeyPress.FlipRightKey==ShortPress
			||KeyPress.FlipRightKey==LongPress
			||KeyPress.HelpKey==ShortPress
			||KeyPress.HelpKey==LongPress
			||KeyPress.NextKey==ShortPress
			||KeyPress.NextKey==LongPress)
			{
				KeyPress.FlipLeftKey=NoPress; 
				KeyPress.FlipRightKey=NoPress; 
				KeyPress.HelpKey=NoPress; 
				KeyPress.NextKey=NoPress;
			}
			else if(KeyPress.FlipLeftKey==ShortPress)
			{
				KeyPress.FlipLeftKey=NoPress;
		
				if((flash_Cali.flash_Cali_s.OnlyOnceForcurTwoPointSevenFiveFlag&0x01)==0x01)			
				{																										
					flash_Cali.flash_Cali_s.Cali_curTwoPointSevenFive_positiveValue=2000/filter_GetDisplayCurrent(); 	
					flash_Cali.flash_Cali_s.OnlyOnceForcurTwoPointSevenFiveFlag&=0x02;// 10								
					
//					flash_Cali.flash_Cali_s.a_cal=0x03;
					writeCali();	
			
				}
			}
	}		
		
	else if(isnan(flash_Cali.flash_Cali_s.CurLocal_Zero_critial_negativeValue))// 
	{
		if(KeyPress.FlipLeftKey==ShortPress
		||KeyPress.FlipLeftKey==LongPress
		||KeyPress.FlipRightKey==ShortPress
		||KeyPress.FlipRightKey==LongPress
		||KeyPress.HelpKey==ShortPress
		||KeyPress.HelpKey==LongPress 
		||KeyPress.NextKey==LongPress)
		{
			KeyPress.FlipLeftKey=NoPress; 
			KeyPress.FlipRightKey=NoPress; 
			KeyPress.HelpKey=NoPress; 
			KeyPress.NextKey=NoPress;
		}
		else if(KeyPress.NextKey==ShortPress)
		{
//			flash_Cali.flash_Cali_s.a_cal=0x04;
//			writeCali();
		}
	}
			
	else if(isnan(flash_Cali.flash_Cali_s.Cali_curTwoPointSevenFive_negativeValue))// 
	{
		if(KeyPress.FlipLeftKey==ShortPress
		||KeyPress.FlipLeftKey==LongPress 
		||KeyPress.FlipRightKey==LongPress
		||KeyPress.HelpKey==ShortPress
		||KeyPress.HelpKey==LongPress
		||KeyPress.NextKey==ShortPress
		||KeyPress.NextKey==LongPress)
		{
			KeyPress.FlipLeftKey=NoPress; 
			KeyPress.FlipRightKey=NoPress; 
			KeyPress.HelpKey=NoPress; 
			KeyPress.NextKey=NoPress;
		}
		else if(KeyPress.FlipRightKey==ShortPress)
		{
			if((flash_Cali.flash_Cali_s.OnlyOnceForcurTwoPointSevenFiveFlag&0x02)==0x02)
			{				
				flash_Cali.flash_Cali_s.Cali_curTwoPointSevenFive_negativeValue=-2000/filter_GetDisplayCurrent(); 
				flash_Cali.flash_Cali_s.OnlyOnceForcurTwoPointSevenFiveFlag&=0x01;//0x01

				flash_Cali.flash_Cali_s.interfaceNum=FirstInterface;//改变默认画面
				 
				writeCali();
			}		
		}
	}
 
}
 
bool GetTimerDelta(uint32_t FixTimeForCompare,uint32_t timer_tick)
{	
    if (timer_tick <= FixTimeForCompare)
	{	
		return 1;
    }	
	else
	{
	    return 0;
	}
}	

void checkStratUpKey1Press(void)
{
	if(READKEY1)//按下
	{
		StratUpKey1=1;
	}	
}

void checkStratKey1LiftUp(void)
{
	if((StratUpKey1==1)&&(!READKEY1))//松手
	{
		StratUpKey1=0;
	}
}


void checkStratUpKey3Press(void)
{
	if(READKEY3)//按下
	{
		StratUpKey3=1;
	}	
}

void checkStratKey3LiftUp(void)
{
	if((StratUpKey3==1)&&(!READKEY3))//松手
	{
		StratUpKey3=0;
	}
}

void keyCalpage(void)
{
	if(
		(KeyPress.FlipLeftKey==LongPress)
	&&(KeyPress.FlipRightKey==LongPress)
	&&(KeyPress.HelpKey==LongPress)
	&&(KeyPress.NextKey==LongPress)
	)
	{
		KeyPress.FlipLeftKey=NoPress;
		KeyPress.FlipRightKey=NoPress;
		KeyPress.HelpKey=NoPress;
		KeyPress.NextKey=NoPress;
		Interface=CaliForFactoryInterface;
	}

	static uint8_t keyTime=0;
	if(
		(KeyPress.FlipLeftKey==LongPress)
	||(KeyPress.FlipRightKey==LongPress)
	||(KeyPress.HelpKey==LongPress)
	||(KeyPress.NextKey==LongPress)
	)
	{
		keyTime++;
	}
	if(keyTime>0x30)
	{
		KeyPress.FlipLeftKey=NoPress;
		KeyPress.FlipRightKey=NoPress;
		KeyPress.HelpKey=NoPress;
		KeyPress.NextKey=NoPress;
	}
	
}




void keyCalipageControl(void)
{
	if(KeyPress.HelpKey==ShortPress)
	{
		KeyPress.HelpKey=NoPress;
		greenOption++; 
	}
	
	if(KeyPress.NextKey==ShortPress)
	{
		KeyPress.NextKey=NoPress;
		pressCalibrationOption();
	}
	else if(KeyPress.NextKey==NoPress)
	{
		calibrationOptionAssignZero();
	}
}

void GreenOptionCalibration(void)
{
	greenColor.color1=0xffff;
	greenColor.color2=0xffff;
	greenColor.color3=0xffff;
	greenColor.color4=0xffff;
	greenColor.color5=0xffff;
	switch(greenOption)
	{
		case verf5v:
			greenColor.color1=0X07E0;
 		break;
		case noLoadCurr:
			greenColor.color2 =0X07E0;
		break;
		case curr275A:
			//greenColor.color3 =0x07e0;
			greenOption++;
		break;
		case reset:
			greenColor.color4 =0X07E0;
		 
		break;
		case exit:
			greenColor.color5 =0X07E0;
		break;
		case noexist:
			 greenOption=verf5v;
		break;
 
		default:
		break;
	}
}
 
void calibrationOptionAssignZero(void)
{
	CalibrationOption.Voltage_fiveVolt=0;
	CalibrationOption.Current_zeroAmpere=0;
	CalibrationOption.Current_twePSevenFiveAmpere=0;
	CalibrationOption.into_Reset=0;
	CalibrationOption.into_Exit=0;
}

void pressCalibrationOption(void)
{
	calibrationOptionAssignZero();
	switch(greenOption)
	{
		case verf5v:
			CalibrationOption.Voltage_fiveVolt=1; 
 		break;
		case noLoadCurr:
			CalibrationOption.Current_zeroAmpere=1;
		break;
		case curr275A:
			CalibrationOption.Current_twePSevenFiveAmpere=1;
		break;
		case reset:
			CalibrationOption.into_Reset=1;
		break;
		case exit:
			CalibrationOption.into_Exit=1;
		break;
		default:
		break;
	}
}

void Calibrationhandling(void)
{		
	if(CalibrationOption.Voltage_fiveVolt==1)
	{	
		flash_Cali.flash_Cali_s.Cali_K_fiveVolt=5000/generateVoltage();
		writeCali();	
	}					
	else if(CalibrationOption.Current_zeroAmpere==1) 
	{	
		flash_Cali.flash_Cali_s.Cali_currentTurnZero=GetOriginalCurrent();
		writeCali();
	}		
	else if(CalibrationOption.Current_twePSevenFiveAmpere==1)
	{	
		
	}	
	else if(CalibrationOption.into_Reset==1)
	{
		flash_Cali.flash_Cali_s.Cali_K_fiveVolt=1;
		flash_Cali.flash_Cali_s.Cali_currentTurnZero=0;
		writeCali();
	}
	else if(CalibrationOption.into_Exit==1)
	{
		Interface=FirstInterface;
		writeCali();
	}
}

void clearQuantity(void)
{	
	if((Interface==FirstInterface)||
		(Interface==SecondInterface)||
		(Interface==ThirdInterface)||
		(Interface==UsbFourLineInterface))
	{
		Quantity_u32=0;
		Quantity_ldoub=0;
		writeTimeQW(Address_TQW);
	}
}

void clearWork(void)
{
	if((Interface==FirstInterface)||
		(Interface==SecondInterface)||
		(Interface==ThirdInterface)||
		(Interface==UsbFourLineInterface))
	{
		Work_u32=0;
		Work_ldoub=0;
		writeTimeQW(Address_TQW);
	}
}

void clearTime(void)
{
	if((Interface==FirstInterface)||
		(Interface==ThirdInterface)||
		(Interface==UsbFourLineInterface))
	{
		second=0;
		minute=0;
		hourIRQ=0;
		Timethen=0;
		TimeTotal=0;
		writeTimeQW(Address_TQW);
//		Display_RtcConfig();
	}
}

long double filter_GetDisplayCurrent(void) 
{
    long double sum = 0;
	
    for(char i = 0; i < 100; i++) 
	{
        sum += GetDisplayCurrent();
    }
    return (sum / 100);
}

void key1Detect(KeyPressWay_e *pressway)
{
	static uint8_t a=0x01;
	static uint32_t Timecount;
	 
	switch(a)
    {
        case 0x01: 
            if((READKEY1 ==1)&&(LockScreen.lock==1))    
            {
				TimeTotal_lockScreen=TimeTotal;		
				LockScreen.lock=0;						
				//WriteComm(0x11);					
				a=0x20;									
            }           							
			else if((READKEY1==1)&&(StratUpKey1!=1))  //第一次按下
            {
				a=0x02;
				Timecount=0;
            }
            break;
        case 0x02: 
			Timecount++; 
            if(READKEY1 ==0)//第一次按下后松手
            {		
				*pressway=ShortPress;
				Timecount=0;
				a=0x20;
				break;
			}		
            else//第一次按下后不松手 ，长按
            {
				if(Timecount>=6)
				{	
					*pressway=LongPress;
					a=0x20;	
					Timecount=0;	
					break;	
				}	
            }	
             break;
		case 0x20:
			if(READKEY1==0)// 松手
			{	 
				Timecount=0;
				a=0x01;
			}
			break;
        default:
			a=0x01;
          break;
    }
}

void key2Detect(KeyPressWay_e *pressway)
{
	static uint8_t a=0x01;
	static uint32_t Timecount; 
	 
	switch(a)
    {
        case 0x01: 
             
            if((READKEY2 ==1)&&(LockScreen.lock==1))    
            {
				TimeTotal_lockScreen=TimeTotal;
				LockScreen.lock=0;					
				//WriteComm(0x11);	
				a=0x20;
            }           
			else if(READKEY2 ==1)//第一次按下
            {
				a=0x02;
				Timecount=0;
            }
            break;
        case 0x02: 
			Timecount++; 
            if(READKEY2 ==0)//第一次按下后松手
            {		
				*pressway=ShortPress;
				Timecount=0;
				a=0x20;
				break;
			}		
            else//第一次按下后不松手 ，长按
            {
				if(Timecount>=6)
				{	
					*pressway=LongPress;
					a=0x20;	
					Timecount=0;	
					break;	
				}	
            }	
             break;
		case 0x20:
			if(READKEY2==0)// 松手
			{	 
				Timecount=0;
				a=0x01;
			}
			break;
        default:
			a=0x01;
          break;
    }
}
void key3Detect(KeyPressWay_e *pressway)
{
	static uint8_t a=0x01;
	static uint32_t Timecount; 
	 
	switch(a)
    {
        case 0x01: 
            if((READKEY3 ==1)&&(LockScreen.lock==1))    
            {
				TimeTotal_lockScreen=TimeTotal;
				LockScreen.lock=0;					
				//WriteComm(0x11);	
				a=0x20;
            }           
			else if(READKEY3 ==1)//第一次按下
            {	
				a=0x02;
				Timecount=0;
            }
            break;
        case 0x02: 
			Timecount++; 
            if(READKEY3 ==0)//第一次按下后松手
            {		
				*pressway=ShortPress;
				Timecount=0;
				a=0x20;
				break;
			}		
            else//第一次按下后不松手 ，长按
            {
				if(Timecount>=6)
				{	
					*pressway=LongPress;
					a=0x20;	
					Timecount=0;	
					break;	
				}	
            }	
             break;
		case 0x20:
			if(READKEY3==0)// 松手
			{	 
				Timecount=0;
				a=0x01;
			}
			break;
        default:
			a=0x01;
          break;
    }
}
 
void key4Detect(KeyPressWay_e *pressway)
{
	static uint8_t a=0x01;

	static uint32_t Timecount; 
	switch(a)
    {
        case 0x01: 
            if((READKEY4 ==1)&&(LockScreen.lock==1))    
            {
				TimeTotal_lockScreen=TimeTotal;
				LockScreen.lock=0; 
				//WriteComm(0x11);	
				a=0x20;
            }           
			else if(READKEY4 ==1)//第一次按下
            {
				a=0x02;
				Timecount=0;
            }
            break;
        case 0x02: 
			Timecount++; 
            if(READKEY4 ==0)//第一次按下后松手
            {		
				*pressway=ShortPress;
				Timecount=0;
				a=0x20;
				break;
			}		
            else//第一次按下后不松手 ，长按
            {
				if(Timecount>=6)
				{	
					*pressway=LongPress;
					a=0x20;	
					Timecount=0;	
					break;	
				}	
            }	
             break;
		case 0x20:
			if(READKEY4==0)// 松手
			{	 
				Timecount=0;
				a=0x01;
			}
			break;
        default:
			a=0x01;
          break;
    }
}
//睡眠后默认是白色的，所以逻辑是先唤醒屏幕，然后再发送pwm点亮屏幕
void delayLockScreen(void)				
{										
	static uint32_t Timcount=0;			
	static uint8_t a=0x01;	
	switch(a) 
	{
		case 0x01:
			if(LockScreen.lock==0)
			{
				Timcount++; 
				if(Timcount>0)	//2
				{
					a=0x02;
				}	
			}
			
			break;
		case 0x02: 					
			LockScreen.unlock=1;	
			Timcount=0;	
			a=0x01;
			break;
		default:
			a=0x01;
			break;			
	}
}
