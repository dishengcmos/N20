
#include "cachevalue.h" 

#include "adc_mid.h" 
#include "flash_mid.h" 
#include "rtcconfig.h"
#include "key.h"
#include <math.h>
#include "chart.h"
#include "displaydriver.h"
Interface_e Interface;//=CalibrationInterface;

 
uint8_t  hour_display;
bool Hadbeen;

long double  Quantity_ldoub,Work_ldoub;
long double Current_ldoub,Voltage_ldoub;
uint32_t Voltage_u32,Current_u32 ,Quantity_u32,Work_u32,Quantity_then_everyTenSeconds_u32;
 
/***********************第一页****************************/																																																													
 //uint8_t SelectChannel[20]={0,1,1,1,\
//CHAR_SMA_CIRCLE,CHAR_SMA_CIRCLE,CHAR_SMA_CIRCLE,CHAR_SMA_CIRCLE,CHAR_SMA_CIRCLE,CHAR_SMA_CIRCLE,CHAR_SMA_CIRCLE,\
//1,1,1,CHAR_MIDDLE_banyuan2
//};  

uint8_t SelectChannel[20]={CHAR_MIDDLE_banyuan1,CHAR_TURN_LEFT_TIP1,CHAR_TURN_LEFT_TIP2,1,CHAR_PLUS_TIP1,CHAR_PLUS_TIP2,CHAR_HIGHPIXEL_BIG_CIRCLE,		
CHAR_HIGHPIXEL_CIRCLE,CHAR_HIGHPIXEL_CIRCLE,CHAR_HIGHPIXEL_CIRCLE,\
CHAR_HIGHPIXEL_CIRCLE,CHAR_HIGHPIXEL_CIRCLE,CHAR_HIGHPIXEL_CIRCLE,\
CHAR_MINUS_TIP1,CHAR_MINUS_TIP2,1,1,CHAR_TURN_RIGHT_TIP1,CHAR_TURN_RIGHT_TIP2,CHAR_MIDDLE_banyuan2												
};	
uint8_t Partten_poweron[8]={0,1,2,3,4,5,6,7};	

uint8_t Volt_b[15]={0,CHAR_POINT,0,0,0,CHAR_V,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON};	
uint8_t Curr_b[15]={0,CHAR_POINT,0,0,0,CHAR_A,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON};	
uint8_t QuanResi_s[15]={0,0,0,0,0,CHAR_m,CHAR_A,CHAR_h,CHAR_NON,0,0,0,CHAR_POINT,0,CHAR_BIG_OHM};
uint8_t WorkPowe_s[15]={0,0,0,0,0,CHAR_m,CHAR_W,CHAR_h,CHAR_NON,0,0,CHAR_POINT,0,0,CHAR_BIG_W}; 
uint8_t KepTip[20]={CHAR_MIDDLE_banyuan1,CHAR_MIDDLE_0XFF,CHAR_MIDDLE_0XFF,CHAR_P_TIP_HP,CHAR_R_TIP_HP,CHAR_E_TIP_HP,CHAR_V_TIP_HP,CHAR_MIDDLE_0XFF,CHAR_MIDDLE_0XFF, CHAR_MIDDLE_banyuan2,\
CHAR_MIDDLE_banyuan1,CHAR_MIDDLE_0XFF,CHAR_MIDDLE_0XFF,CHAR_N_TIP_HP,CHAR_E_TIP_HP,CHAR_X_TIP_HP,CHAR_T_TIP_HP ,CHAR_MIDDLE_0XFF,CHAR_MIDDLE_0XFF,CHAR_MIDDLE_banyuan2};  						

//uint8_t KepTip_highpixel[20]={CHAR_MIDDLE_banyuan1,CHAR_MIDDLE_0XFF,CHAR_MIDDLE_0XFF,CHAR_H_TIP_HP,CHAR_E_TIP_HP,CHAR_L_TIP_HP,CHAR_P_TIP_HP,CHAR_MIDDLE_0XFF,CHAR_MIDDLE_0XFF, CHAR_MIDDLE_banyuan2,\
//CHAR_MIDDLE_banyuan1,CHAR_MIDDLE_0XFF,CHAR_MIDDLE_0XFF,CHAR_N_TIP_HP,CHAR_E_TIP_HP,CHAR_X_TIP_HP,CHAR_T_TIP_HP ,CHAR_MIDDLE_0XFF,CHAR_MIDDLE_0XFF,CHAR_MIDDLE_banyuan2};  						
uint8_t TTEMP[16]={0};  
uint8_t Thermometer_ss[3]={CHAR_THERMOMETER_C1,CHAR_THERMOMETER_C2,CHAR_THERMOMETER_C3}; 

//uint8_t Temperature_ss[4]={1,3,5,7};
uint8_t Temperature_ss[3]={CHAR_NON,2,7};
uint8_t Group_ss_highPixel[3] ={CHAR_GROUP_HIGHPIXEL1,CHAR_GROUP_HIGHPIXEL2,CHAR_GROUP_HIGHPIXEL3}; 
//uint8_t GroupNom_ss[4]	={CHAR_NON,CHAR_NON,CHAR_NON,0}; 

uint8_t GroupNom_ss_highPixel[3] = {CHAR_NON,0,1}; 
/***********************第二页****************************/
uint8_t Dplus_s_highPixel[15]=	   {CHAR_D_PATTERN,CHAR_D_CIRCLE,0,CHAR_POINT,0,0, CHAR_V,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_M,CHAR_o,CHAR_d,CHAR_e};  						
uint8_t DminusMode_s_highPixel[15]={CHAR_D_PATTERN,CHAR_D_CIRCLE,0,CHAR_POINT,0,0, CHAR_V,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_Q	,CHAR_C,CHAR_NON,CHAR_NON,CHAR_NON};  						
uint8_t Plus[1]={CHAR_PLUS};
uint8_t Minu[1]={CHAR_MINUS};
/***********************第三页****************************/																																																							
uint8_t Quan[9]={CHAR_NON,0,0,0,0,CHAR_BIG_m,CHAR_A,CHAR_BIG_h,CHAR_NON};
uint8_t Work[9]={CHAR_NON,0,0,0,0,CHAR_BIG_m,CHAR_BIG_W,CHAR_BIG_h,CHAR_NON};
uint8_t Timing[15]={CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,0,0,CHAR_COLONSMALL,0, 0,CHAR_COLONSMALL,0,0,CHAR_NON,CHAR_NON,CHAR_NON };  	
uint8_t CurrREC[16]={CHAR_NON,CHAR_NON,CHAR_NON,0,CHAR_POINT,0,0, CHAR_A,CHAR_NON,CHAR_R,CHAR_E,CHAR_C,CHAR_NON,CHAR_NON,CHAR_NON ,CHAR_NON};  	
			
/***********************第四页****************************/	
uint8_t VoltageNO1_sb[16]={1,CHAR_POINT,0,0,CHAR_POINT,0,0,CHAR_V,\
CHAR_BATTER_UP1,CHAR_BATTER_UP2,CHAR_BATTER_UP3,CHAR_BATTER_UP4,CHAR_BATTER_UP5,CHAR_BATTER_UP6,CHAR_BATTER_UP7};
uint8_t CurrentNO1_sb[16]={CHAR_NON,CHAR_NON,0,CHAR_POINT,0,0,0,CHAR_A,\
CHAR_BATTER_DOW1,CHAR_BATTER_DOW2,CHAR_BATTER_DOW3,CHAR_BATTER_DOW4,CHAR_BATTER_DOW5,CHAR_BATTER_DOW6,CHAR_BATTER_DOW7};

uint8_t VoltageNO2_sb[16]={2,CHAR_POINT,0,0,CHAR_POINT,0,0,CHAR_V,\
CHAR_BATTERWIRE_UP1,CHAR_BATTERWIRE_UP2,CHAR_BATTERWIRE_UP3,CHAR_BATTERWIRE_UP4,CHAR_BATTERWIRE_UP5,CHAR_BATTERWIRE_UP6,CHAR_BATTERWIRE_UP7};
uint8_t CurrentNO2_sb[16]={CHAR_NON,CHAR_NON,0,CHAR_POINT,0,0,0,CHAR_A,\
CHAR_BATTERWIRE_DOW1,CHAR_BATTERWIRE_DOW2,CHAR_BATTERWIRE_DOW3,CHAR_BATTERWIRE_DOW4,CHAR_BATTERWIRE_DOW5,CHAR_BATTERWIRE_DOW6,CHAR_BATTERWIRE_DOW7};

uint8_t Error_sb[16]={CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON};						
//uint8_t Error_sb[16]={CHAR_NON,CHAR_NON,CHAR_T,CHAR_e,CHAR_s,CHAR_t,CHAR_NON,CHAR_E,CHAR_r,CHAR_r,CHAR_o,CHAR_r,CHAR_NON,CHAR_Exclam,CHAR_NON,CHAR_NON};						

uint8_t Resistan_sb[16]={CHAR_NON,CHAR_NON,CHAR_NON,\
CHAR_R,CHAR_EQUAL,0,0,CHAR_POINT,0,0,0,CHAR_FOHM,CHAR_BOHM,\
CHAR_NON,CHAR_NON,CHAR_NON};						
/***********************第五页****************************/
uint8_t	scaleVolPlate1[16]={0,0,CHAR_POINT,0,0,CHAR_V,14,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON};
uint8_t	scaleVolPlate2[16]={0,0,CHAR_POINT,0,0,CHAR_V,14,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON};
uint8_t	scaleVolPlate3[16]={0,0,CHAR_POINT,0,0,CHAR_V,14,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON};
uint8_t	scaleVolPlate4[16]={0,0,CHAR_POINT,0,0,CHAR_V,14,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON};
uint8_t	scaleVolPlate5[16]={0,0,CHAR_POINT,0,0,CHAR_V,14,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON};
uint8_t	scaleVolPlate6[16]={0,0,CHAR_POINT,0,0,CHAR_V,14,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON};
uint8_t	scaleVolPlate7[16]={0,0,CHAR_POINT,0,0,CHAR_V,14,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON};
uint8_t	scaleVolPlate8[16]={0,0,CHAR_POINT,0,0,CHAR_V,14,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON};
uint8_t	scaleVolPlate9[16]={0,0,CHAR_POINT,0,0,CHAR_V,14,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON};
uint8_t scaleVolPlate10[16]={0,0,CHAR_POINT,0,0,CHAR_V,14,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON};
uint8_t scaleVolPlate11[16]={0,0,CHAR_POINT,0,0,CHAR_V,14,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON};
uint8_t scaleVolPlate12[16]={0,0,CHAR_POINT,0,0,CHAR_V,14,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON};
//uint8_t scaleVolPlate13[16]={0,0,CHAR_POINT,0,0,CHAR_V,14,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON};


/***********************第六页****************************/	
uint8_t	scaleCurPlate1[16]={0,CHAR_POINT,0,0,0,CHAR_A,14,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON};
uint8_t	scaleCurPlate2[16]={0,CHAR_POINT,0,0,0,CHAR_A,14,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON};
uint8_t	scaleCurPlate3[16]={0,CHAR_POINT,0,0,0,CHAR_A,14,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON};
uint8_t	scaleCurPlate4[16]={0,CHAR_POINT,0,0,0,CHAR_A,14,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON};
uint8_t	scaleCurPlate5[16]={0,CHAR_POINT,0,0,0,CHAR_A,14,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON};
uint8_t	scaleCurPlate6[16]={0,CHAR_POINT,0,0,0,CHAR_A,14,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON};
uint8_t	scaleCurPlate7[16]={0,CHAR_POINT,0,0,0,CHAR_A,14,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON};
uint8_t	scaleCurPlate8[16]={0,CHAR_POINT,0,0,0,CHAR_A,14,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON};
uint8_t	scaleCurPlate9[16]={0,CHAR_POINT,0,0,0,CHAR_A,14,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON};
uint8_t	scaleCurPlate10[16]={0,CHAR_POINT,0,0,0,CHAR_A,14,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON};
uint8_t	scaleCurPlate11[16]={0,CHAR_POINT,0,0,0,CHAR_A,14,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON};
uint8_t	scaleCurPlate12[16]={0,CHAR_POINT,0,0,0,CHAR_A,14,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON};
//uint8_t	scaleCurPlate13[16]={0,CHAR_POINT,0,0,0,CHAR_A,14,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON};
	
/***********************第七页****************************/	
uint8_t Minutes[16]   ={CHAR_NON,0,CHAR_NON,CHAR_M,CHAR_i,CHAR_n,CHAR_u,CHAR_t,CHAR_e, CHAR_s,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON};  	
uint8_t Brightness[16]={CHAR_NON,0,CHAR_NON,CHAR_B,CHAR_r,CHAR_i,CHAR_g,CHAR_h,CHAR_t, CHAR_n,CHAR_e,CHAR_s,CHAR_s,CHAR_NON,CHAR_NON };  	
uint8_t CentFahr[16]  ={CHAR_NON,0,CHAR_NON,CHAR_C,CHAR_e,CHAR_n,CHAR_t,CHAR_i,CHAR_i, CHAR_r,CHAR_a,CHAR_d,CHAR_e,CHAR_NON,CHAR_NON};  	

uint8_t Pattern_Minutes[2]  ={CHAR_BIG_CLOCK1,CHAR_BIG_CLOCK2};  	
uint8_t Pattern_Brightness[2]={CHAR_BIG_SUN1,CHAR_BIG_SUN2};  	
uint8_t Pattern_CentFahr[2]  ={CHAR_BIG_TEMPE1,CHAR_BIG_TEMPE2};  	
/***********************校准页****************************/		
uint8_t Vref5V_andRealVol[16]={CHAR_NON,CHAR_NON,CHAR_R,CHAR_e,CHAR_f,5,CHAR_V,CHAR_COLONBIG,0,CHAR_POINT,0,0,0,CHAR_V,CHAR_NON,CHAR_NON};  
uint8_t NoLoadCurrent[16]={CHAR_N,CHAR_o,CHAR_L,CHAR_o,CHAR_a,CHAR_d,CHAR_C,CHAR_u,CHAR_r,CHAR_COLONBIG,0,CHAR_POINT,0,0,CHAR_A };  
uint8_t Reset_Exit[16]={CHAR_R,CHAR_e,CHAR_s ,CHAR_e ,CHAR_t,CHAR_NON,CHAR_NON,CHAR_NON ,CHAR_NON ,CHAR_NON,CHAR_NON,CHAR_E ,CHAR_x ,CHAR_i ,CHAR_t };  
uint8_t Version[16]={CHAR_V,0,CHAR_POINT,1,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON};
//版本号为v0.1																																				 			
/***********************校准 for 工厂****************************/	
//uint8_t SelectChannel[20]={CHAR_MIDDLE_banyuan1,1,1,1,1,1,CHAR_HIGHPIXEL_BIG_CIRCLE,		
//CHAR_HIGHPIXEL_CIRCLE,CHAR_HIGHPIXEL_CIRCLE,CHAR_HIGHPIXEL_CIRCLE,\
//CHAR_HIGHPIXEL_CIRCLE,CHAR_HIGHPIXEL_CIRCLE,CHAR_HIGHPIXEL_CIRCLE,\
//1,1,1,1,1,1,CHAR_MIDDLE_banyuan2	

//uint8_t KepTip[20]={CHAR_MIDDLE_banyuan1,CHAR_MIDDLE_0XFF,CHAR_MIDDLE_0XFF,CHAR_H_TIP_HP,CHAR_E_TIP_HP,CHAR_L_TIP_HP,CHAR_P_TIP_HP,CHAR_MIDDLE_0XFF,CHAR_MIDDLE_0XFF, CHAR_MIDDLE_banyuan2,\
//CHAR_MIDDLE_banyuan1,CHAR_MIDDLE_0XFF,CHAR_MIDDLE_0XFF,CHAR_N_TIP_HP,CHAR_E_TIP_HP,CHAR_X_TIP_HP,CHAR_T_TIP_HP ,CHAR_MIDDLE_0XFF,CHAR_MIDDLE_0XFF,CHAR_MIDDLE_banyuan2};  						

uint8_t CaliKepTipUp[20]={CHAR_MIDDLE_banyuan1,CHAR_MIDDLE_0XFF,CHAR_MIDDLE_0XFF,CHAR_MIDDLE_Plus,CHAR_MIDDLE_2,CHAR_MIDDLE_Point,CHAR_MIDDLE_A,CHAR_MIDDLE_0XFF,CHAR_MIDDLE_0XFF, CHAR_MIDDLE_banyuan2,\
CHAR_MIDDLE_banyuan1,CHAR_MIDDLE_0XFF,CHAR_MIDDLE_0XFF,CHAR_MIDDLE_Minus,CHAR_MIDDLE_2,CHAR_MIDDLE_Point,CHAR_MIDDLE_A,CHAR_MIDDLE_0XFF,CHAR_MIDDLE_0XFF,CHAR_MIDDLE_banyuan2};  

uint8_t CaliFactory1[20]={CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_V,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_A,CHAR_NON,CHAR_NON};
uint8_t CaliFactory2[20]={CHAR_NON,5,CHAR_V,CHAR_COLONSMALL,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON};
uint8_t CaliFactory3[20]={CHAR_Plus,0,CHAR_A,CHAR_COLONSMALL,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON};
uint8_t CaliFactory4[20]={CHAR_Plus,2,CHAR_POINT,0,0,CHAR_A,CHAR_COLONSMALL,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON};
uint8_t CaliFactory5[20]={CHAR_Minus,0,CHAR_A,CHAR_COLONSMALL,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON};
uint8_t CaliFactory6[20]={CHAR_Minus,2,CHAR_POINT,0,0,CHAR_A,CHAR_COLONSMALL,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON,CHAR_NON};
uint8_t CaliKepTipDown[20]={CHAR_MIDDLE_banyuan1,CHAR_MIDDLE_0XFF,CHAR_MIDDLE_0XFF,CHAR_MIDDLE_5,CHAR_MIDDLE_V,CHAR_MIDDLE_Plus,CHAR_MIDDLE_0,CHAR_MIDDLE_0XFF,CHAR_MIDDLE_0XFF, CHAR_MIDDLE_banyuan2,\
CHAR_MIDDLE_banyuan1,CHAR_MIDDLE_0XFF,CHAR_MIDDLE_0XFF,CHAR_MIDDLE_Minus,CHAR_MIDDLE_0,CHAR_MIDDLE_Point,CHAR_MIDDLE_A,CHAR_MIDDLE_0XFF,CHAR_MIDDLE_0XFF,CHAR_MIDDLE_banyuan2};  
																																																																			
/***********************help1****************************/																																																										
const uint8_t Help_p1r1[30]={HELP_l, HELP_o ,HELP_n ,HELP_g ,HELP_non,HELP_p,HELP_r,HELP_e,HELP_s,HELP_s,HELP_non,HELP_t,HELP_o,HELP_non,HELP_non,HELP_l,HELP_o,HELP_n,HELP_g,HELP_non,HELP_p,HELP_r,HELP_e,HELP_s,HELP_s,HELP_non,HELP_t,HELP_o,HELP_non};		
const uint8_t Help_p1r2[30]={HELP_r ,HELP_o ,HELP_t ,HELP_a ,HELP_t ,HELP_e ,HELP_non ,HELP_t,HELP_h,HELP_e,HELP_non ,HELP_non,HELP_non,HELP_non,HELP_non,HELP_r ,HELP_o ,HELP_t ,HELP_a ,HELP_t ,HELP_e ,HELP_non ,HELP_t,HELP_h,HELP_e,HELP_non ,HELP_non,HELP_non,HELP_non};
const uint8_t Help_p1r3[30]={HELP_s ,HELP_c ,HELP_r ,HELP_e ,HELP_e ,HELP_n,HELP_non,HELP_l ,HELP_e ,HELP_f ,HELP_t,HELP_point,HELP_non,HELP_non,HELP_non ,HELP_s ,HELP_c ,HELP_r ,HELP_e ,HELP_e ,HELP_n,HELP_non,HELP_r ,HELP_i ,HELP_g ,HELP_h ,HELP_t,HELP_point};																									
const uint8_t Help_p1r4[30]={HELP_l, HELP_o ,HELP_n ,HELP_g ,HELP_non,HELP_p,HELP_r,HELP_e,HELP_s,HELP_s,HELP_non,HELP_t,HELP_o,HELP_non,HELP_non,HELP_l,HELP_o,HELP_n,HELP_g,HELP_non,HELP_p,HELP_r,HELP_e,HELP_s,HELP_s,HELP_non,HELP_t,HELP_o,HELP_non};		
const uint8_t Help_p1r5[30]={HELP_c ,HELP_l ,HELP_e ,HELP_a ,HELP_r ,HELP_non ,HELP_t ,HELP_h ,HELP_e ,HELP_non,HELP_d ,HELP_a ,HELP_t ,HELP_a ,HELP_non ,HELP_c ,HELP_h ,HELP_a ,HELP_n ,HELP_g ,HELP_e ,HELP_non ,HELP_t ,HELP_h ,HELP_e ,HELP_non,HELP_non ,HELP_non ,HELP_non ,HELP_non};
const uint8_t Help_p1r6[30]={HELP_g ,HELP_r ,HELP_o ,HELP_u ,HELP_p ,HELP_non ,HELP_d ,HELP_a ,HELP_t ,HELP_a ,HELP_point,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_d ,HELP_a ,HELP_t ,HELP_a,HELP_non,HELP_g ,HELP_r ,HELP_o ,HELP_u ,HELP_p ,HELP_point,HELP_non ,HELP_non ,HELP_non ,HELP_non};
const uint8_t Help_p1r_non[30]={HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non, HELP_non,HELP_non,HELP_non,HELP_non,HELP_non,HELP_non,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non, HELP_non,HELP_non,HELP_non,HELP_non,HELP_non,HELP_non,HELP_non,HELP_non,HELP_non,HELP_non,HELP_non,HELP_non,HELP_non,HELP_non};
												
const uint8_t Help_p3r5[30]={HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_h ,HELP_o ,HELP_l ,HELP_d ,HELP_non ,HELP_f ,HELP_o ,HELP_r ,HELP_non ,HELP_non};
const uint8_t Help_p3r6[30]={HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_s ,HELP_e ,HELP_t ,HELP_t ,HELP_i ,HELP_n ,HELP_g ,HELP_s ,HELP_point ,HELP_non};
const uint8_t Help_p3r7[30]={HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_p ,HELP_r ,HELP_e ,HELP_s ,HELP_s ,HELP_non ,HELP_t ,HELP_o ,HELP_non ,HELP_non};
const uint8_t Help_p3r8[30]={HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_a ,HELP_d ,HELP_j ,HELP_u ,HELP_s ,HELP_t ,HELP_point ,HELP_non ,HELP_non ,HELP_non};
																																																					 				
const uint8_t Help_p4r5[30]={HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_l,HELP_o,HELP_n,HELP_g,HELP_non,HELP_p,HELP_r,HELP_e,HELP_s,HELP_s,HELP_non,HELP_t,HELP_o,HELP_non};		
const uint8_t Help_p4r6[30]={HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_r ,HELP_e ,HELP_c ,HELP_o,HELP_r,HELP_d,HELP_non,HELP_d,HELP_a,HELP_t,HELP_a,HELP_point ,HELP_non ,HELP_non ,HELP_non};																		
const uint8_t Help_p4r7[30]={HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non,HELP_non,HELP_non,HELP_non,HELP_non,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non};
const uint8_t Help_p4r8[30]={HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non,HELP_non,HELP_non,HELP_non,HELP_non,HELP_non,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non ,HELP_non};
																																																																																			
	
																																																																				
#define CURRENT_N 31
void AssignToCacheValue(void)
{ 
	static uint32_t Pwr_u32,Resistan_u32,DisplayDplus_u32,DisplayDmin_u32;

	static long double Voltage_temp,Voltage_judgeTemp,Pwr_ldoub,Resistan_ldoub,\
		DisplayDplus_ldoub,DisplayDmin_ldoub,dQuantity_ldoub,dWork_ldoub;
	static long double  Pwr_ldoub_tmp;
 	static long double	Current_ldoub_tmp;
	static long double dQuantity_Thirdpageldoub,Quantity_Thirdpageldoub,dWork_Thirdpageldoub,Work_Thirdpageldoub;

	static uint32_t	Quantity_Thirdpageu32,Work_Thirdpageu32;
	static uint32_t hour_3page,minute_3page,second_3page;
 	/**********************第一页*********************/ 
 	Voltage_temp=GetDisplayVoltage(); 					
	Voltage_judgeTemp=stableData(Voltage_temp);			
	if(Voltage_judgeTemp>=0) 							
	{													
		Voltage_ldoub=Voltage_judgeTemp;				
		Voltage_u32=roundOffVoltage(Voltage_ldoub);		
	}		
	
 	Current_ldoub_tmp=GetDisplayCurrent(); 
	
 	static long double sum_cur[CURRENT_N];
	static long double output_tmp1;
	static long double output_tmp2;
	static long double temp;
	static uint8_t	i_cur=0;
 
	sum_cur[i_cur]=Current_ldoub_tmp;
	
	i_cur++;
	
	if(i_cur==CURRENT_N)
	{
		for(uint8_t j = 0;j<(CURRENT_N-1);j++)
		{
			for(uint8_t i = 0;i<(CURRENT_N-j);i++)
			if(sum_cur[i]>sum_cur[i+1])
			{
				temp = sum_cur[i];
				sum_cur[i] = sum_cur[i+1];
				sum_cur[i+1] = temp;
			}
		}
		i_cur=0;
	}
	
	output_tmp1=  sum_cur[(CURRENT_N-1)/2];
	
	//output_tmp2 += (output_tmp1 - output_tmp2) * 0.5f;
	if((output_tmp1>20)||(output_tmp1<-20)) 
		Current_ldoub=output_tmp1; 
	else
		Current_ldoub=0;
 	
		
 	
	
// 	static long double sum_cur;
//	static long double output_tmp1;
//	static long double output_tmp2;
// 
//	for(uint8_t i_cur=0;i_cur<14;i_cur++)
//	{
//		sum_cur+=GetDisplayCurrent(); 
//		output_tmp1=sum_cur/14;
//	} 
//	//output_tmp2 += (output_tmp1 - output_tmp2) * 0.5f;
//	if((output_tmp1>20)||(output_tmp1<-20)) 
//		Current_ldoub=output_tmp1; 
//	else
//		Current_ldoub=0;
// 	 
//	sum_cur=0;

	
	Current_u32=Current_ldoub; 
//	Curr_b[0]=(Current_u32/1000)%10;
//	Curr_b[2]=(Current_u32/100)%10;
//	Curr_b[3]=(Current_u32/10)%10;
//	Curr_b[4]=(Current_u32)%10; 
	
	showCharge_protocol(Voltage_ldoub,Current_ldoub);		
	//test_protocol(Voltage_ldoub,Current_ldoub);
	
	Pwr_ldoub_tmp=Voltage_ldoub*Current_ldoub/1000;
//	Pwr_ldoub_tmp=filter_powr(Pwr_ldoub_tmp);
//	if(Pwr_ldoub_tmp!=0)
	Pwr_ldoub=Pwr_ldoub_tmp; 
	Pwr_u32 =Pwr_ldoub;
	
	if(Pwr_u32<100000)
	{
		WorkPowe_s[9]=(Pwr_u32/10000)%10;
		WorkPowe_s[10]=(Pwr_u32/1000)%10;
		WorkPowe_s[11]=CHAR_POINT;
		WorkPowe_s[12]=(Pwr_u32/100)%10;
		WorkPowe_s[13]=(Pwr_u32/10)%10;
//		WorkPowe_s[14]=(Pwr_u32/1)%10; 
	}
	else
	{
		WorkPowe_s[9]=(Pwr_u32/100000)%10;
		WorkPowe_s[10]=(Pwr_u32/10000)%10;
		WorkPowe_s[11]=(Pwr_u32/1000)%10;
		WorkPowe_s[12]=CHAR_POINT;
		WorkPowe_s[13]=(Pwr_u32/100)%10;
//		WorkPowe_s[14]=(Pwr_u32/10)%10; 
	}
	if(Current_ldoub!=0)
	{
		Resistan_ldoub=Voltage_ldoub*1000/Current_ldoub;
		Resistan_u32=Resistan_ldoub;
	}
	else if(Current_ldoub==0)		
	{								
		Resistan_u32=9999900;		
	}								
	
	QuanResi_s[9] =(Resistan_u32/100000)%10;	
	QuanResi_s[10]=(Resistan_u32/10000)%10; 	
	QuanResi_s[11]=(Resistan_u32/1000)%10; 	
	QuanResi_s[13]=(Resistan_u32/100)%10; 		
//	QuanResi_s[14]=(Resistan_u32/100)%10; 		
	
	if((TimeTotal>Timethen)||((TimeTotal==0)&&(Timethen==0)&&(Hadbeen==0)))
	{	
		Hadbeen=1; 							
		Timethen=TimeTotal; 				
											
		dQuantity_ldoub=fabs(Current_ldoub/3600); //  
		Quantity_ldoub+=dQuantity_ldoub;
		
		dWork_ldoub= fabs(dQuantity_ldoub*(Voltage_ldoub/1000));//voltage是以mv为单位所以除以1000
		Work_ldoub+=dWork_ldoub; 
		
		if(TimeTotal!=0)
		{
			Hadbeen=0;
		}
	}
	Quantity_u32=Quantity_ldoub; 
	Work_u32=Work_ldoub;
	
	if(Quantity_u32<99999)
	{
		QuanResi_s[0]=(Quantity_u32/10000)%10;
		QuanResi_s[1]=(Quantity_u32/1000)%10;
		QuanResi_s[2]=(Quantity_u32/100)%10;
		QuanResi_s[3]=(Quantity_u32/10)%10;
		QuanResi_s[4]=(Quantity_u32)%10;	
		
		QuanResi_s[5]=CHAR_m;
		QuanResi_s[6]=CHAR_A;
		QuanResi_s[7]=CHAR_h;
	}
	else
	{	
		if(Quantity_u32>99999999)
		{
			Quantity_u32=99999999;
		}
		uint32_t Quantity_Ah_u32=Quantity_u32/1000;
		QuanResi_s[0]=(Quantity_Ah_u32/10000)%10;
		QuanResi_s[1]=(Quantity_Ah_u32/1000)%10;
		QuanResi_s[2]=(Quantity_Ah_u32/100)%10;
		QuanResi_s[3]=(Quantity_Ah_u32/10)%10;
		QuanResi_s[4]=(Quantity_Ah_u32)%10;	
		
		QuanResi_s[5]=CHAR_A;
		QuanResi_s[6]=CHAR_h;
		QuanResi_s[7]=CHAR_NON; 
	}
	
	if(Work_u32<99999)
	{
		WorkPowe_s[0]=(Work_u32/10000)%10;	
		WorkPowe_s[1]=(Work_u32/1000)%10;	
		WorkPowe_s[2]=(Work_u32/100)%10;	
		WorkPowe_s[3]=(Work_u32/10)%10;		
		WorkPowe_s[4]=(Work_u32)%10; 
		
		WorkPowe_s[5]=CHAR_m; 
		WorkPowe_s[6]=CHAR_W; 
		WorkPowe_s[7]=CHAR_h; 		
 	}
	else
	{
		if(Work_u32>99999999)
		{
			Work_u32=99999999;
		}
		uint32_t Work_Wh_u32=Work_u32/1000;
		
		WorkPowe_s[0]=(Work_Wh_u32/10000)%10;	
		WorkPowe_s[1]=(Work_Wh_u32/1000)%10;	
		WorkPowe_s[2]=(Work_Wh_u32/100)%10;	
		WorkPowe_s[3]=(Work_Wh_u32/10)%10;		
		WorkPowe_s[4]=(Work_Wh_u32)%10; 
		WorkPowe_s[5]=CHAR_W; 
		WorkPowe_s[6]=CHAR_h; 
		WorkPowe_s[7]=CHAR_NON; 	
	}
	
	GroupNom_ss_highPixel[1]=(flash_Cali.flash_Cali_s.flashNum/10)%10;			   	
	GroupNom_ss_highPixel[2]=(flash_Cali.flash_Cali_s.flashNum)%10;	

	/******************第二页***********************/
	DisplayDplus_ldoub=GetDisplayDplus();	
	DisplayDplus_u32=DisplayDplus_ldoub;	
	DisplayDmin_ldoub=GetDisplayDmin();		
	DisplayDmin_u32 = DisplayDmin_ldoub;	
											
	Dplus_s_highPixel[2]=(DisplayDplus_u32/1000)%10; 
	Dplus_s_highPixel[4]=(DisplayDplus_u32/100)%10; 
	Dplus_s_highPixel[5]=(DisplayDplus_u32/10)%10; 	 

	DminusMode_s_highPixel[2]=(DisplayDmin_u32/1000)%10; 
	DminusMode_s_highPixel[4]=(DisplayDmin_u32/100)%10; 
	DminusMode_s_highPixel[5]=(DisplayDmin_u32/10)%10;   
	/****************** 第三页 ***********************/
	if((TimeTotal>TimethenThirdpage)&&(Current_u32>flash_Cali.flash_Cali_s.settingCurrentPage3))
	{
		TimethenThirdpage=TimeTotal;
		Timetotaltemporary++;
		
		getRECcolor(GREEN);//green 开始第三页面的计时
		dQuantity_Thirdpageldoub=Current_ldoub/3600; 
		Quantity_Thirdpageldoub+=dQuantity_Thirdpageldoub;  
		
		dWork_Thirdpageldoub=dQuantity_Thirdpageldoub*(Voltage_ldoub/1000);//voltage是以mv为单位所以除以1000
		Work_Thirdpageldoub+=dWork_Thirdpageldoub; 
	}
	else if(TimeTotal>TimethenThirdpage) 
	{	
		getRECcolor(RED);//red 
	}	
	
	Quantity_Thirdpageu32=Quantity_Thirdpageldoub; 
	Work_Thirdpageu32=Work_Thirdpageldoub;
	/***************************/
	if(Quantity_Thirdpageu32<9999)
	{
		//Quan[1]=(Quantity_Thirdpageu32/10000)%10;
		Quan[1]=(Quantity_Thirdpageu32/1000)%10;
		Quan[2]=(Quantity_Thirdpageu32/100)%10;
		Quan[3]=(Quantity_Thirdpageu32/10)%10;	
		Quan[4]=(Quantity_Thirdpageu32)%10;		
		Quan[5]=CHAR_BIG_m;
		Quan[6]=CHAR_A;
		Quan[7]=CHAR_BIG_h;
	}
	else
	{
		if(Quantity_Thirdpageu32>99999999)
		{
			Quantity_Thirdpageu32=99999999; 
		}
		uint32_t Quantity_Ah_Thirdpageu32=Quantity_Thirdpageu32/1000;
		
		Quan[1]=(Quantity_Ah_Thirdpageu32/10000)%10;
		Quan[2]=(Quantity_Ah_Thirdpageu32/1000)%10;
		Quan[3]=(Quantity_Ah_Thirdpageu32/100)%10;
		Quan[4]=(Quantity_Ah_Thirdpageu32/10)%10;	
		Quan[5]=(Quantity_Ah_Thirdpageu32)%10;		
		Quan[6]=CHAR_A;
		Quan[7]=CHAR_BIG_h; 
	}
		
	if(Work_Thirdpageu32<9999)	
	{
		//Work[0]=(Work_Thirdpageu32/10000)%10;	
		Work[1]=(Work_Thirdpageu32/1000)%10;	
		Work[2]=(Work_Thirdpageu32/100)%10;		
		Work[3]=(Work_Thirdpageu32/10)%10;		
		Work[4]=(Work_Thirdpageu32)%10; 
		Work[5]=CHAR_BIG_m;
		Work[6]=CHAR_BIG_W;
		Work[7]=CHAR_BIG_h;
		
	}
	else
	{
		if(Work_Thirdpageu32>99999999)
		{
			Work_Thirdpageu32=99999999;
		}
		uint32_t Work_Wh_Thirdpageu32=Work_Thirdpageu32/1000;
		
		Work[1]=(Work_Wh_Thirdpageu32/10000)%10;	
		Work[2]=(Work_Wh_Thirdpageu32/1000)%10;	
		Work[3]=(Work_Wh_Thirdpageu32/100)%10;		
		Work[4]=(Work_Wh_Thirdpageu32/10)%10;		
		Work[5]=(Work_Wh_Thirdpageu32)%10; 
		Work[6]=CHAR_BIG_W;
		Work[7]=CHAR_BIG_h; 
 	}
 	hour_3page=Timetotaltemporary/3600; 
	minute_3page=Timetotaltemporary/60%60;
	second_3page=Timetotaltemporary-3600*hour_3page-60*minute_3page;
	
	Timing[4]=(hour_3page/10)%10;	
	Timing[5]=(hour_3page)%10; 
	Timing[7]=(minute_3page/10)%10;	
	Timing[8]=(minute_3page)%10; 
	Timing[10]=(second_3page/10)%10;		
	Timing[11]=(second_3page)%10; 
	/******************第四页*空**********************/
	
	/******************第七页***********************/
	Minutes[1]   = flash_Cali.flash_Cali_s.SventhState.Minutes;
	Brightness[1]=flash_Cali.flash_Cali_s.SventhState.Brightness;
	CentFahr[1]=flash_Cali.flash_Cali_s.SventhState.Temperature;
	if(flash_Cali.flash_Cali_s.SventhState.Temperature==zero)
	{
		 CentFahr[3]=CHAR_C;
		 CentFahr[4]=CHAR_e;
		 CentFahr[5]=CHAR_n;
		 CentFahr[6]=CHAR_t;
		 CentFahr[7]=CHAR_i;
		 CentFahr[8]=CHAR_g;
		 CentFahr[9]=CHAR_r;
		 CentFahr[10]=CHAR_a;
		 CentFahr[11]=CHAR_d;
		 CentFahr[12]=CHAR_e; 
	}
	else if(flash_Cali.flash_Cali_s.SventhState.Temperature==one)
	{		
		 CentFahr[3]=CHAR_F;
		 CentFahr[4]=CHAR_a;
		 CentFahr[5]=CHAR_h;
		 CentFahr[6]=CHAR_r;
		 CentFahr[7]=CHAR_e;
		 CentFahr[8]=CHAR_n;
		 CentFahr[9]=CHAR_h;
		 CentFahr[10]=CHAR_e;
		 CentFahr[11]=CHAR_i;
		 CentFahr[12]=CHAR_t; 
	}
	/******************校准页***********************/
	if(Interface==CaliForCustomInterface)
	{
		if(Voltage_u32>=10000)
		{
			Vref5V_andRealVol[8]=(Voltage_u32/10000)%10;
			Vref5V_andRealVol[9]=(Voltage_u32/1000)%10;
			Vref5V_andRealVol[10]=CHAR_POINT;
			Vref5V_andRealVol[11]=(Voltage_u32/100)%10;
			Vref5V_andRealVol[12]=(Voltage_u32/100)%10;
		}
		else
		{
			Vref5V_andRealVol[8]=(Voltage_u32/1000)%10;
			Vref5V_andRealVol[9]=CHAR_POINT;
			Vref5V_andRealVol[10]=(Voltage_u32/100)%10;
			Vref5V_andRealVol[11]=(Voltage_u32/10)%10;
			Vref5V_andRealVol[12]=(Voltage_u32/10)%10;
		}
		
		NoLoadCurrent[10]=(Current_u32/1000)%10;
		NoLoadCurrent[12]=(Current_u32/100)%10;
		NoLoadCurrent[13]=(Current_u32/10)%10;
		NoLoadCurrent[14]=(Current_u32)%10;
			
		if((CalibrationOption.Voltage_fiveVolt==1)||\
		(CalibrationOption.Current_zeroAmpere==1)||\
		(CalibrationOption.Current_twePSevenFiveAmpere==1)||\
		(CalibrationOption.into_Reset==1)||\
		(CalibrationOption.into_Exit==1))
		{
			Reset_Exit[6]=CHAR_o;
			Reset_Exit[7]=CHAR_k;
			Reset_Exit[8]=CHAR_Exclam ; 
		}
		else
		{
			Reset_Exit[6]=CHAR_NON;
			Reset_Exit[7]=CHAR_NON;
			Reset_Exit[8]=CHAR_NON;
		}
	}
	
}
			
uint32_t roundOffVoltage(uint32_t inputdata) 
{
	if(inputdata>=10000)
	{
		if(((inputdata/10)%10)>=5)
			return  inputdata+10;
		else if(((inputdata/10)%10)<5)
			return inputdata;
	}	
	else	
	{ 
		if(((inputdata)%10)>=5)
			return inputdata+1;
		else if(((inputdata)%10)<5)
			return inputdata;
	}
 }

uint32_t roundOffTemperature(long double inputdata)
{
	
	inputdata*=10;
	uint32_t data=inputdata;
	if((data%10)>=5)
		return  (data+10)/10;
	
	else if((data%10)<5)
		return data/10; 
}

 
void disCaliSign(void)
{
	CaliOperatForFactory(Voltage_ldoub,Current_ldoub);
	disCurrCaliFactory(Current_ldoub);
}

void disCurrCaliFactory(long double Cur)
{
	if(Cur>0)
		CaliFactory1[7]= CHAR_Plus;
	else if(Cur<0)
		CaliFactory1[7]= CHAR_Minus;
	CaliFactory1[8]=(Current_u32/1000)%10;
	CaliFactory1[9]=CHAR_POINT;
	CaliFactory1[10]=(Current_u32/100)%10;
	CaliFactory1[11]=(Current_u32/10)%10;
	CaliFactory1[12]=(Current_u32)%10; 
}		
			
void CaliOperatForFactory(long double Vel,long double Cur)//
{	 
	static int zeroOffset_positiveValue,zeroOffset_negativeValue;
	static int TwoPointSevenFive_static1,TwoPointSevenFive_static2;
	if(!isnan(flash_Cali.flash_Cali_s.Cur_outofCaliEquation_zeroOffset_positiveValue))
	{
		zeroOffset_positiveValue=(int)flash_Cali.flash_Cali_s.Cur_outofCaliEquation_zeroOffset_positiveValue;
		
		if(zeroOffset_positiveValue>0)
			CaliFactory3[5]=CHAR_Plus;
		else if (zeroOffset_positiveValue<0) 
		{
			CaliFactory3[5]=CHAR_Minus;
			zeroOffset_positiveValue=-zeroOffset_positiveValue;
		}
			
		CaliFactory3[6]=(zeroOffset_positiveValue/1000)%10;	
		CaliFactory3[7]=CHAR_POINT;
		CaliFactory3[8]=(zeroOffset_positiveValue/100)%10;	
		CaliFactory3[9]=(zeroOffset_positiveValue/10)%10;	
		CaliFactory3[10]=(zeroOffset_positiveValue)%10; 	
		CaliFactory3[11]=CHAR_o; 
		CaliFactory3[12]=CHAR_k; 
	}
	if(!isnan(flash_Cali.flash_Cali_s.Cur_outofCaliEquation_zeroOffset_negativeValue)) 
	{
		zeroOffset_negativeValue=(int)flash_Cali.flash_Cali_s.Cur_outofCaliEquation_zeroOffset_negativeValue;

		if(zeroOffset_negativeValue>0)
			CaliFactory5[5]=CHAR_Plus;
		else if (zeroOffset_negativeValue<0)
		{
			CaliFactory5[5]=CHAR_Minus;
			zeroOffset_negativeValue=-zeroOffset_negativeValue;
		}
			
		
		CaliFactory5[6]=(zeroOffset_negativeValue/1000)%10;
		CaliFactory5[7]=CHAR_POINT;
		CaliFactory5[8]=(zeroOffset_negativeValue/100)%10;
		CaliFactory5[9]=(zeroOffset_negativeValue/10)%10;
		CaliFactory5[10]=(zeroOffset_negativeValue)%10; 
		CaliFactory5[11]=CHAR_o; 
		CaliFactory5[12]=CHAR_k; 
	}
	
	CaliFactory1[0]=(Voltage_u32/1000)%10;
	CaliFactory1[1]=CHAR_POINT;
	CaliFactory1[2]=(Voltage_u32/100)%10;
	CaliFactory1[3]=(Voltage_u32/10)%10;
	CaliFactory1[4]=(Voltage_u32/1)%10;  
	
	if(flash_Cali.flash_Cali_s.calibration==No_cali)		 
	{	
		CaliFactory2[4]=CHAR_U;	
		CaliFactory2[5]=CHAR_x;	
	}	
	else if(flash_Cali.flash_Cali_s.calibration==Doing_cali)
	{	
		CaliFactory2[4]=CHAR_POINT;
		CaliFactory2[5]=CHAR_POINT;
	}	
	else if(flash_Cali.flash_Cali_s.calibration==Done_cali)
	{	
		CaliFactory2[4]=CHAR_o;
		CaliFactory2[5]=CHAR_k;
	}
		
	if(flash_Cali.flash_Cali_s.OnlyOnceForcurTwoPointSevenFiveFlag==curTwoPointSevenFive_noFlag)
	{
//		CaliFactory4[8]=CHAR_Plus; 
//		CaliFactory4[9]=CHAR_Minus;
//		CaliFactory6[8]=CHAR_Plus; 
//		CaliFactory6[9]=CHAR_Minus;
		
		CaliFactory4[8]=CHAR_NON; 
		CaliFactory4[9]=CHAR_NON;
		CaliFactory6[8]=CHAR_NON;
		CaliFactory6[9]=CHAR_NON;
	}
	else if(flash_Cali.flash_Cali_s.OnlyOnceForcurTwoPointSevenFiveFlag==curTwoPointSevenFive_done_positiveFlag)
	{	
 
		TwoPointSevenFive_static1= (int)(flash_Cali.flash_Cali_s.Cali_curTwoPointSevenFive_positiveValue*1000);
		
		if(TwoPointSevenFive_static1<0)
			TwoPointSevenFive_static1=-TwoPointSevenFive_static1;
		
		CaliFactory4[8]=(TwoPointSevenFive_static1/1000)%10;
		CaliFactory4[9]=CHAR_POINT;
		CaliFactory4[10]=(TwoPointSevenFive_static1/100)%10;
		CaliFactory4[11]=(TwoPointSevenFive_static1/10)%10;
		CaliFactory4[12]=(TwoPointSevenFive_static1)%10;
	
		
		CaliFactory4[13]=CHAR_o; 
		CaliFactory4[14]=CHAR_k; 
	}		
	else if(flash_Cali.flash_Cali_s.OnlyOnceForcurTwoPointSevenFiveFlag==curTwoPointSevenFive_done_negativeFlag)
	{

		TwoPointSevenFive_static2= (int)(flash_Cali.flash_Cali_s.Cali_curTwoPointSevenFive_negativeValue*1000);
		
		if(TwoPointSevenFive_static2<0)
			TwoPointSevenFive_static2=-TwoPointSevenFive_static2;
		
		CaliFactory6[8]=(TwoPointSevenFive_static2/1000)%10;
		CaliFactory6[9]=CHAR_POINT;
		CaliFactory6[10]=(TwoPointSevenFive_static2/100)%10;
		CaliFactory6[11]=(TwoPointSevenFive_static2/10)%10;
		CaliFactory6[12]=(TwoPointSevenFive_static2)%10;
		
		CaliFactory6[13]=CHAR_o;
		CaliFactory6[14]=CHAR_k;
	}
	else
	{
		TwoPointSevenFive_static1= (int)(flash_Cali.flash_Cali_s.Cali_curTwoPointSevenFive_positiveValue*1000);
		if(TwoPointSevenFive_static1<0)
			TwoPointSevenFive_static1=-TwoPointSevenFive_static1;
		
		CaliFactory4[8]=(TwoPointSevenFive_static1/1000)%10;
		CaliFactory4[9]=CHAR_POINT;
		CaliFactory4[10]=(TwoPointSevenFive_static1/100)%10;
		CaliFactory4[11]=(TwoPointSevenFive_static1/10)%10;
		CaliFactory4[12]=(TwoPointSevenFive_static1)%10;
		CaliFactory4[13]=CHAR_o; 
		CaliFactory4[14]=CHAR_k; 
		
		
		TwoPointSevenFive_static2= (int)(flash_Cali.flash_Cali_s.Cali_curTwoPointSevenFive_negativeValue*1000);
		if(TwoPointSevenFive_static2<0)
			TwoPointSevenFive_static2=-TwoPointSevenFive_static2;
		CaliFactory6[8]=(TwoPointSevenFive_static2/1000)%10;
		CaliFactory6[9]=CHAR_POINT;
		CaliFactory6[10]=(TwoPointSevenFive_static2/100)%10;
		CaliFactory6[11]=(TwoPointSevenFive_static2/10)%10;
		CaliFactory6[12]=(TwoPointSevenFive_static2)%10;
		CaliFactory6[13]=CHAR_o;
		CaliFactory6[14]=CHAR_k;
//		flash_Cali.flash_Cali_s.interfaceNum=FirstInterface;//改变默认画面
//		writeCali();
	}
}
/***/

bool Dplus_BetweenMinMax(long double min,long double max)
{
	long double DisplayDplus_ldoub;
	DisplayDplus_ldoub=GetDisplayDplus();
	if((min<=DisplayDplus_ldoub)&&(DisplayDplus_ldoub<=max))
		return 1;
	else
		return 0;
}
bool Dminus_BetweenMinMax(long double min,long double max)
{
	long double DisplayDminus_ldoub;
	DisplayDminus_ldoub=GetDisplayDmin();
	if((min<=DisplayDminus_ldoub)&&(DisplayDminus_ldoub<=max))
		return 1;
	else
		return 0;
} 


//void test_protocol(long double Vol,long double Cur)	
//{
//		DminusMode_s_highPixel[9]=CHAR_S;
//		DminusMode_s_highPixel[10]=CHAR_A;
//		DminusMode_s_highPixel[11]=CHAR_M;
//		DminusMode_s_highPixel[12]=CHAR_S;
//		DminusMode_s_highPixel[13]=CHAR_U;
//		DminusMode_s_highPixel[14]=CHAR_N;
//		DminusMode_s_highPixel[15]=CHAR_G;
//		
//}
 

void showCharge_protocol(long double Vol,long double Cur)	
{																				
	long double Power = fabs(Vol * Cur/1000); 	
																				
	if(Dplus_BetweenMinMax(510 , 690)&&Dminus_BetweenMinMax(0 , 300))
	{
		if((4500<=Vol)&&(Vol<=5500))
		{										
			if(fabs(Cur)<3500)	
			{
				DminusMode_s_highPixel[8]=CHAR_NON;
				DminusMode_s_highPixel[9]=CHAR_NON;	
				DminusMode_s_highPixel[10]=CHAR_Q;
				DminusMode_s_highPixel[11]=CHAR_C; 
				DminusMode_s_highPixel[12]=2;
				DminusMode_s_highPixel[13]=CHAR_POINT;	
				DminusMode_s_highPixel[14]=0; 			 								
 	
			}										
			else
			{
				DminusMode_s_highPixel[8]=CHAR_NON;
				DminusMode_s_highPixel[9]=CHAR_NON;
				DminusMode_s_highPixel[10]=CHAR_NON;
				DminusMode_s_highPixel[11]=CHAR_S;
				DminusMode_s_highPixel[12]=CHAR_C; 
				DminusMode_s_highPixel[13]=CHAR_P;
				DminusMode_s_highPixel[14]=CHAR_NON; 
			}
		}
		else if(5500<GetDisplayVoltage())
		{
			DminusMode_s_highPixel[8]=CHAR_NON;
			DminusMode_s_highPixel[9]=CHAR_NON;
			DminusMode_s_highPixel[10]=CHAR_NON;
			DminusMode_s_highPixel[11]=CHAR_F;
			DminusMode_s_highPixel[12]=CHAR_C; 
			DminusMode_s_highPixel[13]=CHAR_P;
			DminusMode_s_highPixel[14]=CHAR_NON;
			//DminusMode_s_highPixel[15]=CHAR_NON; 		
			//DminusMode_s_highPixel[15]=CHAR_NON; 
		}
		else
		{
			DminusMode_s_highPixel[8]=CHAR_NON;
			DminusMode_s_highPixel[9]=CHAR_NON;
			DminusMode_s_highPixel[10]=CHAR_NON;
			DminusMode_s_highPixel[11]=CHAR_S;
			DminusMode_s_highPixel[12]=CHAR_C; 
			DminusMode_s_highPixel[13]=CHAR_P;
			DminusMode_s_highPixel[14]=CHAR_NON;
			//DminusMode_s_highPixel[15]=CHAR_NON; 		
			//DminusMode_s_highPixel[15]=CHAR_NON; 	
		}
		// showflag_FASTPD(1);
	}
	else if(
	(Dplus_BetweenMinMax(510  , 690)&&Dminus_BetweenMinMax(510  , 690))||	
	(Dplus_BetweenMinMax(2805 , 3795)&&Dminus_BetweenMinMax(510  , 690))||	
	(Dplus_BetweenMinMax(2805 , 3795)&&Dminus_BetweenMinMax(2805 , 3795))||	
	(Dplus_BetweenMinMax(510  , 690)&&Dminus_BetweenMinMax(0    , 300)) 	
	)
	{
			DminusMode_s_highPixel[8]=CHAR_NON;
			DminusMode_s_highPixel[9]=CHAR_NON;
			DminusMode_s_highPixel[10]=CHAR_Q;	
			DminusMode_s_highPixel[11]=CHAR_C; 	
			DminusMode_s_highPixel[13]=CHAR_POINT;	
			DminusMode_s_highPixel[14]=0; 			
			//DminusMode_s_highPixel[15]=CHAR_NON; 	
		if(Power<=18000)
			DminusMode_s_highPixel[12]=2;
		else if((Power>18000)&&(Power<=22000))
			DminusMode_s_highPixel[12]=3; 
		else
			DminusMode_s_highPixel[12]=4; 
 
		// showflag_FASTPD(1);
	}
	else if(Dplus_BetweenMinMax(510  , 690)&&Dminus_BetweenMinMax(2805 , 3795)) 
	{
		DminusMode_s_highPixel[8]=CHAR_NON;
		DminusMode_s_highPixel[9]=CHAR_NON;
		DminusMode_s_highPixel[10]=CHAR_Q;
		DminusMode_s_highPixel[11]=CHAR_C; 
		DminusMode_s_highPixel[12]=3; 
		DminusMode_s_highPixel[13]=CHAR_POINT;
		DminusMode_s_highPixel[14]=0; 
		//DminusMode_s_highPixel[15]=CHAR_NON;
		
		// showflag_FASTPD(1); 
	}
	else if(Dplus_BetweenMinMax(1900 , 2750)&&Dminus_BetweenMinMax( 1900,2750 ))
	{
		DminusMode_s_highPixel[8]=CHAR_NON;
		DminusMode_s_highPixel[9]=CHAR_NON;
		DminusMode_s_highPixel[10]=CHAR_A;
		DminusMode_s_highPixel[11]=CHAR_P;
		DminusMode_s_highPixel[12]=CHAR_P;
		DminusMode_s_highPixel[13]=CHAR_L;
		DminusMode_s_highPixel[14]=CHAR_E;
		//DminusMode_s_highPixel[15]=CHAR_NON;
		// showflag_FASTPD(1); 
	}
	else if(Dplus_BetweenMinMax(2750 , 2900)&&Dminus_BetweenMinMax( 2750,2900 ))
	{
		DminusMode_s_highPixel[8]=CHAR_NON;
		DminusMode_s_highPixel[9]=CHAR_NON;
		DminusMode_s_highPixel[10]=CHAR_NON;		
		DminusMode_s_highPixel[11]=CHAR_D;		
		DminusMode_s_highPixel[12]=CHAR_C;		
		DminusMode_s_highPixel[13]=CHAR_P;	
		DminusMode_s_highPixel[14]=CHAR_NON; 
		//DminusMode_s_highPixel[15]=CHAR_NON;
		//DminusMode_s_highPixel[15]=CHAR_NON;
		
		// showflag_FASTPD(1); 
	}					
	else if(Dplus_BetweenMinMax(1000 , 1500)&&Dminus_BetweenMinMax( 1000,1500 ))
	{
		
		
		DminusMode_s_highPixel[8]=CHAR_S;
		DminusMode_s_highPixel[9]=CHAR_A;
		DminusMode_s_highPixel[10]=CHAR_M;
		DminusMode_s_highPixel[11]=CHAR_S;
		DminusMode_s_highPixel[12]=CHAR_U;
		DminusMode_s_highPixel[13]=CHAR_N;
		DminusMode_s_highPixel[14]=CHAR_G;
		// showflag_FASTPD(1); 
	}
	else if(Dplus_BetweenMinMax(2815 , 3795)&&Dminus_BetweenMinMax( 1530,2070 ))
	{
		 
		DminusMode_s_highPixel[8]=CHAR_NON;
		DminusMode_s_highPixel[9]=CHAR_NON;
		DminusMode_s_highPixel[10]=CHAR_S;
		DminusMode_s_highPixel[11]=CHAR_V;
		DminusMode_s_highPixel[12]=CHAR_O;
		DminusMode_s_highPixel[13]=CHAR_O;
		DminusMode_s_highPixel[14]=CHAR_C; 
		// showflag_FASTPD(1); 
	}  
	else
	{
		DminusMode_s_highPixel[8]=CHAR_U;
		DminusMode_s_highPixel[9]=CHAR_n;
		DminusMode_s_highPixel[10]=CHAR_k;
		DminusMode_s_highPixel[11]=CHAR_n;
		DminusMode_s_highPixel[12]=CHAR_o;
		DminusMode_s_highPixel[13]=CHAR_w;
		DminusMode_s_highPixel[14]=CHAR_n;
		
//		if(Cur>1500)
//		// showflag_FASTPD(1); 
//		else
//		// showflag_FASTPD(0); 
	}
}
 						



long double filter_powr(long double input)
{
	
	static long double output,sum;
	static uint8_t i=0;
	i++;
	sum += input;
	if(i>=30)
	{
		output=sum/30;
		i=0;
		sum=0;
		return output;
	}
	else
		return 0;
}
	
uint8_t filter_current(long double input,long double *output)
{
	static long double sum,output_tmp;
	static uint8_t i=0;
	i++;
	sum += input;
	if(i>=14)
	{
		output_tmp=sum/14;
		i=0;
		sum=0;
		
		if((output_tmp>5)||(output_tmp<-5)) 
			*output=output_tmp; 
		else
			*output=0;
		return 1;
	}
	else
		return 0;
}

//void showflag_FASTPD(char trunSwitch)
//{
//	if(trunSwitch==1)
//	{	
//		DplusW4page_sb[8]=CHAR_FASTPD_1;
//		DplusW4page_sb[9]=CHAR_FASTPD_2;
//		DplusW4page_sb[10]=CHAR_FASTPD_3;
//		DplusW4page_sb[11]=CHAR_FASTPD_4;
//		DplusW4page_sb[12]=CHAR_FASTPD_5;
//		DplusW4page_sb[13]=CHAR_FASTPD_6;
//	}
//	else if(trunSwitch==0)
//	{
//		DplusW4page_sb[8]=CHAR_NON;
//		DplusW4page_sb[9]=CHAR_NON;
//		DplusW4page_sb[10]=CHAR_NON;
//		DplusW4page_sb[11]=CHAR_NON;
//		DplusW4page_sb[12]=CHAR_NON;
//		DplusW4page_sb[13]=CHAR_NON;
//	}
//}
/***
#define N 11
char filter()
{
	char value_buf[N];
	char count,i,j,temp;
	for(count = 0;count < N;count++)	//获取采样值
	{
		value_buf[count] = get_ad(); 
		delay();
	}
	for(j = 0;j<(N-1);j++)
		for(i = 0;i<(N-j);i++)
		if(value_buf[i]>value_buf[i+1])
		{
			temp = value_buf[i];
			value_buf[i] = value_buf[i+1];
			value_buf[i+1] = temp;
		}
	return value_buf[(N-1)/2];
}


***/


