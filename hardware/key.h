
#ifndef _KEY_H_
#define _KEY_H_

#include "ddl.h"
#include <stdbool.h>
/**************
key1	key2
	N10
key3	key4
***************/
#define READKEY1   (!Gpio_GetInputIO(KEY1_PORT, KEY1_PIN))
#define READKEY2   (!Gpio_GetInputIO(KEY2_PORT, KEY2_PIN))
#define READKEY3   (!Gpio_GetInputIO(KEY3_PORT, KEY3_PIN))
#define READKEY4   (!Gpio_GetInputIO(KEY4_PORT, KEY4_PIN))

typedef enum
{
	NoPress,
	ShortPress,
	LongPress,
}KeyPressWay_e;
 
typedef struct
{
	KeyPressWay_e FlipLeftKey;
	KeyPressWay_e FlipRightKey;
	KeyPressWay_e HelpKey;
	KeyPressWay_e NextKey;
}KeyPress_s;
extern KeyPress_s KeyPress;

typedef enum
{
	zero,
	one,
	two,
	three,
	four,
	five,
	six,
	seven,
	eight,
	nine,
}setnumber_e;

//typedef struct
//{
//	setnumber_e Minutes; 
//	setnumber_e Brightness;
//	setnumber_e Temperature;
//}SventhState_detail_s;
//extern SventhState_detail_s		SventhState_detail;
		
	
typedef enum
{
	None_e,
	Minutes_e,
	Brightness_e,
	Temperature_e,
}SventhState_whole_e;

extern SventhState_whole_e		SventhState_whole;


typedef enum{
Increase,
Decrease,
}ChangeWay_e;
extern ChangeWay_e ChangeWay;

typedef struct
{
uint8_t page1;
uint8_t page2;
uint8_t page3;
uint8_t page4;
	
}SettingState_s ;
extern SettingState_s SettingState;
 
typedef enum{
	verf5v,
	noLoadCurr,
	curr275A,
	reset,
	exit,
	noexist,
}greenOption_e;
extern greenOption_e greenOption;

typedef struct {
uint16_t color1;
uint16_t color2;
uint16_t color3;
uint16_t color4;
uint16_t color5;
}greenColor_s ;
extern greenColor_s greenColor;


typedef struct {
bool Voltage_fiveVolt;
bool Current_zeroAmpere;
bool Current_twePSevenFiveAmpere;
bool into_Reset;
bool into_Exit;
}CalibrationOption_s;

extern CalibrationOption_s CalibrationOption;


typedef struct {
uint8_t lock;
uint8_t unlock;
}LockScreen_s;
extern LockScreen_s LockScreen;

 
extern bool StratUpKey3;
extern bool StratUpKey1;
extern uint32_t WarningObviTimcount;
extern uint32_t WarningObviTimcount_showWarning;

extern uint8_t g_displayHelpPage;


void KeyInit(void); 
bool GetTimerDelta(uint32_t FixTimeForCompare,uint32_t timer_tick);

void checkStratUpKey3Press(void);
void checkStratKey3LiftUp(void);
 
void checkStratUpKey1Press(void);
void checkStratKey1LiftUp(void);

void plusVolCurr(void);//shuang ji
void minusVolCurr(void);//sanji
void continuityPlus(void);
void continuityMinus(void);
void continuityPlusMinu(void);
void GreenOptionCalibration(void);

void pressCalibrationOption(void);
void calibrationOptionAssignZero(void);

void Calibrationhandling(void);
void clearQuantity(void);
void clearWork(void);
void clearTime(void); 
 
void keyCaliForFactoryInterface(void);

long double filter_GetDisplayCurrent(void) ;
void getKeyState(void);

void KeyProgram(void);
void KeyGeneral(void);


void key1Detect(KeyPressWay_e *pressway);
void key2Detect(KeyPressWay_e *pressway);
void key3Detect(KeyPressWay_e *pressway);
void key4Detect(KeyPressWay_e *pressway);

void normalRotate(void);
void leftRotate(void);
void rightRotate(void);
void upsidedownRotate(void);


void towardsLeft(void);
void towardsRight(void);

void keyThirdpageControl(void);

void keyFirSecpageControl(void);
void keySeventhpageControl(void);

void keyFourthpageControl(void);

void keyMinutesControl(void);
void keyBrightnessControl(void);
void keyTemperatureControl(void);

void brightnessControl(void);
void keyCalipageControl(void);
void delayLockScreen(void);
void keyCalpage(void);
#endif //_KEY_H_

