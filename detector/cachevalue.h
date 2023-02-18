
#ifndef _CACHEVALUE_H_
#define _CACHEVALUE_H_

#include "ddl.h"
#include <stdbool.h>

extern long double Voltage_ldoub,Current_ldoub ,Quantity_ldoub,Work_ldoub;
extern uint32_t Voltage_u32,Current_u32 ,Quantity_u32,Work_u32,Quantity_then_everyTenSeconds_u32;
 
typedef enum{
FirstInterface,
SecondInterface,
ThirdInterface,
UsbFourLineInterface,
chartVoltageInterface,
chartCurrentInterface,
setOptionInterface,
EndNoExistface,
	
BlackScreen,	
CaliForCustomInterface,
CaliForFactoryInterface, 
 
}Interface_e;
extern Interface_e Interface; 

typedef enum {
    CHAR_BIG_POINT = 10,
    CHAR_BIG_V,
    CHAR_BIG_A,
    CHAR_BIG_NON,
    CHAR_BIG_m,
    CHAR_BIG_h,
    CHAR_BIG_W,
    CHAR_BIG_CLOCK1,
    CHAR_BIG_CLOCK2,
    CHAR_BIG_SUN1,
    CHAR_BIG_SUN2,
	CHAR_BIG_TEMPE1,
	CHAR_BIG_TEMPE2,
	
}CHAR_symbol_Big_e;


typedef enum {
    CHAR_POINT = 10,
	
    CHAR_V,
    CHAR_A,
    CHAR_NON,
    CHAR_m,
    CHAR_h,
    CHAR_W,
	CHAR_THERMOMETER_C1,
	CHAR_THERMOMETER_C2,
	CHAR_THERMOMETER_C3,
	CHAR_THERMOMETER_F1,
	CHAR_THERMOMETER_F2,
	CHAR_THERMOMETER_F3,
	CHAR_GROUP_HIGHPIXEL1,	
	CHAR_GROUP_HIGHPIXEL2,	
	CHAR_GROUP_HIGHPIXEL3,	
	CHAR_BIG_OHM,
    CHAR_D_PATTERN_DOWN1,
	CHAR_D_PATTERN_DOWN2,
    CHAR_D_PATTERN_UP1,
	CHAR_D_PATTERN_UP2,
	CHAR_D_CIRCLE,
	CHAR_PLUS,
	CHAR_MINUS,
	CHAR_M,
	CHAR_o,
	CHAR_d,
 	CHAR_e,
	CHAR_Q,
	CHAR_C, 
	CHAR_COLONSMALL,
    CHAR_R,
    CHAR_E,
	
	CHAR_BATTER_UP1,
	CHAR_BATTER_UP2,
	CHAR_BATTER_UP3 ,
	CHAR_BATTER_UP4 ,
	CHAR_BATTER_UP5 ,
	CHAR_BATTER_UP6 ,
	CHAR_BATTER_UP7 ,

	CHAR_BATTER_DOW1 ,
	CHAR_BATTER_DOW2 ,
	CHAR_BATTER_DOW3 ,
	CHAR_BATTER_DOW4 ,
	CHAR_BATTER_DOW5 ,
	CHAR_BATTER_DOW6 ,
	CHAR_BATTER_DOW7 ,
	
	CHAR_BATTERWIRE_UP1,
	CHAR_BATTERWIRE_UP2,
	CHAR_BATTERWIRE_UP3 ,
	CHAR_BATTERWIRE_UP4 ,
	CHAR_BATTERWIRE_UP5 ,
	CHAR_BATTERWIRE_UP6 ,
	CHAR_BATTERWIRE_UP7 ,
	CHAR_BATTERWIRE_DOW1 ,
	CHAR_BATTERWIRE_DOW2 ,
	CHAR_BATTERWIRE_DOW3 ,
	CHAR_BATTERWIRE_DOW4 ,
	CHAR_BATTERWIRE_DOW5 ,
	CHAR_BATTERWIRE_DOW6 ,
	CHAR_BATTERWIRE_DOW7 ,
	CHAR_i,
	CHAR_n,
	CHAR_u,
	CHAR_t, 
	CHAR_s,
	CHAR_B,
	CHAR_r, 
	CHAR_g,
	CHAR_a,
	CHAR_k,
	CHAR_x,
	CHAR_U,
    CHAR_Plus,
    CHAR_Minus,
	CHAR_f,
	CHAR_COLONBIG,
	CHAR_N,
	CHAR_L,
	CHAR_w,
	CHAR_S,
	CHAR_F,
	CHAR_P,
	CHAR_D,
	CHAR_T,
	CHAR_Exclam,
	CHAR_EQUAL ,
	CHAR_FOHM,
	CHAR_BOHM,
	CHAR_O,
	CHAR_G,
	CHAR_D_PATTERN,
}CHAR_symbol_small_e;


typedef enum {
	CHAR_MIDDLE_banyuan1,
	CHAR_MIDDLE_0XFF,
	CHAR_H_TIP_HP,
	CHAR_E_TIP_HP,
	CHAR_L_TIP_HP,
	CHAR_P_TIP_HP,
	CHAR_MIDDLE_banyuan2,
	CHAR_N_TIP_HP,
	CHAR_X_TIP_HP,
	CHAR_T_TIP_HP,	
		
	CHAR_HIGHPIXEL_BIG_CIRCLE, 
	CHAR_HIGHPIXEL_CIRCLE, 
	CHAR_HIGHPIXEL_ARROWRIGHT,
	CHAR_HIGHPIXEL_ARROWLEFT,
	CHAR_MIDDLE_0,
	CHAR_MIDDLE_2,
	CHAR_MIDDLE_5,
	CHAR_MIDDLE_V,
	CHAR_MIDDLE_A,
	CHAR_MIDDLE_Plus,
	CHAR_MIDDLE_Minus,
	CHAR_MIDDLE_Point,
	CHAR_R_TIP_HP, 
	CHAR_V_TIP_HP,
	CHAR_TURN_LEFT_TIP1, 
	CHAR_TURN_LEFT_TIP2,
	CHAR_TURN_RIGHT_TIP1, 
	CHAR_TURN_RIGHT_TIP2,
	
	CHAR_PLUS_TIP1,
	CHAR_PLUS_TIP2,
	CHAR_MINUS_TIP1,
	CHAR_MINUS_TIP2,
	
}CHAR_tip_middle_e;

typedef enum {
	HELP_non,
	HELP_l,
	HELP_o,
	HELP_n,
	HELP_g,
	HELP_p,
	HELP_r,
	HELP_e,
	HELP_s, 
	HELP_t,
	HELP_a, 
	HELP_f,
	HELP_i,
	HELP_h, 
	HELP_point, 
	HELP_c, 
	HELP_d, 
	HELP_u,
	HELP_j, 
}CHAR_help_e;
/***********************flash保存页面****************************/
//extern uint8_t Flash_Save_Time12[23];
//extern uint8_t Flash_Save_Time34[23];
//extern uint8_t Flash_Save_Time56[23];
//extern uint8_t Flash_Save_Time78[23];
//extern uint8_t Flash_Save_Time9a[23];
/***********************end****************************/

extern uint8_t Partten_poweron[8];	
/***********************第一页****************************/
extern uint8_t	SelectChannel[20];
extern uint8_t	SelectRainbowChannel[16]; 
extern uint8_t 	Volt_b[15]; 
extern uint8_t 	Curr_b[15];
extern uint8_t 	QuanResi_s[15];
extern uint8_t 	WorkPowe_s[15];
 
extern uint8_t	KepTip[20];

extern uint8_t	TTEMP[16];
extern uint8_t Thermometer_ss[3]; 
extern uint8_t Temperature_ss[3]; 


extern uint8_t Group_ss_highPixel[3]; 
//extern uint8_t GroupNom_ss[4]; 
extern uint8_t GroupNom_ss_highPixel[3]; 
 

/***********************第二页****************************/
extern uint8_t VoNo_bb[20];
extern uint8_t CuQ_bb[20];
extern uint8_t PW_bb[20];



extern uint8_t Dplus_s_highPixel[15];
extern uint8_t DminusMode_s_highPixel[15];
extern uint8_t Plus[1];
extern uint8_t Minu[1];
/***********************第三页****************************/
extern uint8_t Quan[9];
extern uint8_t Work[9];
extern uint8_t Timing[15];  	
extern uint8_t CurrREC[16];  	

/***********************第四页****************************/	
 
extern uint8_t VoltageNO1_sb[16];
extern uint8_t CurrentNO1_sb[16];
extern uint8_t VoltageNO2_sb[16];
extern uint8_t CurrentNO2_sb[16];
extern uint8_t Error_sb[16];
extern uint8_t Resistan_sb[16]; 
/***********************第五页****************************/	
extern uint8_t	scaleVolPlate1[16];
extern uint8_t	scaleVolPlate2[16];
extern uint8_t	scaleVolPlate3[16];
extern uint8_t	scaleVolPlate4[16];
extern uint8_t	scaleVolPlate5[16];
extern uint8_t	scaleVolPlate6[16];
extern uint8_t	scaleVolPlate7[16];
extern uint8_t	scaleVolPlate8[16];
extern uint8_t	scaleVolPlate9[16];
extern uint8_t	scaleVolPlate10[16];
extern uint8_t	scaleVolPlate11[16];
extern uint8_t	scaleVolPlate12[16]; 
/***********************第六页****************************/	
extern uint8_t	scaleCurPlate1[16];
extern uint8_t	scaleCurPlate2[16];
extern uint8_t	scaleCurPlate3[16];
extern uint8_t	scaleCurPlate4[16];
extern uint8_t	scaleCurPlate5[16];
extern uint8_t	scaleCurPlate6[16];
extern uint8_t	scaleCurPlate7[16];
extern uint8_t	scaleCurPlate8[16];
extern uint8_t	scaleCurPlate9[16];
extern uint8_t	scaleCurPlate10[16];
extern uint8_t	scaleCurPlate11[16];
extern uint8_t	scaleCurPlate12[16];
//extern uint8_t	scaleCurPlate13[16];

/***********************第七页****************************/	
extern uint8_t Minutes[16];
extern uint8_t Brightness[16];
extern uint8_t CentFahr[16];

extern uint8_t Pattern_Minutes[2];  	
extern uint8_t Pattern_Brightness[2] ;  	
extern uint8_t Pattern_CentFahr[2] ;  
/***********************警告页****************************/	
extern uint8_t SetDis_Waring[15];	
/***********************校准页for用户****************************/		
extern uint8_t Vref5V_andRealVol[16];
extern uint8_t NoLoadCurrent[16]; 
extern uint8_t Reset_Exit[16];
extern uint8_t Version[16];

/***********************校准for工厂****************************/
extern uint8_t CaliKepTipUp[20];
extern uint8_t CaliFactory1[20];
extern uint8_t CaliFactory2[20];
extern uint8_t CaliFactory3[20];
extern uint8_t CaliFactory4[20];
extern uint8_t CaliFactory5[20];
extern uint8_t CaliFactory6[20];
extern uint8_t CaliKepTipDown[20];
/************************help******************************/
extern const uint8_t Help_p1r1[30];
extern const uint8_t Help_p1r2[30];
extern const uint8_t Help_p1r3[30];
extern const uint8_t Help_p1r4[30];
extern const uint8_t Help_p1r5[30];
extern const uint8_t Help_p1r6[30];
extern const uint8_t Help_p1r_non[30]; 

 
extern const uint8_t Help_p3r5[30];
extern const uint8_t Help_p3r6[30];
extern const uint8_t Help_p3r7[30];
extern const uint8_t Help_p3r8[30];
extern const uint8_t Help_p4r5[30];
extern const uint8_t Help_p4r6[30];
extern const uint8_t Help_p4r7[30];
extern const uint8_t Help_p4r8[30];
			

/************************end******************************/


void AssignToCacheValue(void);	

uint32_t roundOffVoltage(uint32_t inputdata);
uint32_t roundOffTemperature(long double inputdata);
 
	
void CaliOperatForFactory(long double Vel,long double Cur);

//void showflag_FASTPD(char trunSwitch);
void showCharge_protocol(long double Vel,long double Cur);

//void test_protocol(long double Vol,long double Cur)	;
void disCaliSign(void);

uint8_t filter_current(long double input,long double *output);

long double filter_powr(long double input);

void disCurrCaliFactory(long double Cur);
#endif /* _CACHEVALUE_H_ */

