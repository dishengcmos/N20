
#include "adc_mid.h"
#include "adc.h" 
#include "gpio.h"
#include "bgr.h"
#include "key.h"

#include "flash_mid.h"
#include "cachevalue.h"
#include <math.h>
#include <stdbool.h>

#include "main.h"

//#define VREF flash_Cali.flash_Cali_s.V3300volt
#define VREF 3300
/**

**/
ADC_SampleOriginalValue_s Adc_Original;

void AdcSumm_Init(void)
{
	App_AdcPortInit();
	App_AdcInit_VrefExternal();
	//App_AdcInit_VrefInternal();//1.5
	App_AdcSQRCfg();
}
 
///< ADC 采样端口初始化
void App_AdcPortInit(void)
{    
    ///< 开启GPIO外设时钟
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
      
    Gpio_SetAnalogMode(GpioPortA, GpioPin4);        //PA04 (AIN4) D+
    Gpio_SetAnalogMode(GpioPortA, GpioPin5);        //PA05 (AIN5) D-
    Gpio_SetAnalogMode(GpioPortA, GpioPin6);        //PA06 (AIN6) VOL 
    Gpio_SetAnalogMode(GpioPortA, GpioPin7);        //PA07 (AIN7) TEMP
    Gpio_SetAnalogMode(GpioPortB, GpioPin0); 		//PB00 (AIN8) VcurrentVIN   	
    Gpio_SetAnalogMode(GpioPortB, GpioPin11);       //PB011(AIN18)CURRENT
}

///< ADC 模块初始化
void App_AdcInit_VrefInternal(void)
{
    stc_adc_cfg_t              stcAdcCfg;

    DDL_ZERO_STRUCT(stcAdcCfg);
    ///< 开启ADC/BGR外设时钟
    Sysctrl_SetPeripheralGate(SysctrlPeripheralAdcBgr, TRUE); 
    
    Bgr_BgrEnable();        ///< 开启BGR
    
    ///< ADC 初始化配置
    stcAdcCfg.enAdcMode         = AdcScanMode;              ///<采样模式-扫描
    stcAdcCfg.enAdcClkDiv       = AdcMskClkDiv8;            ///<采样分频
    stcAdcCfg.enAdcSampCycleSel = AdcMskSampCycle12Clk;      ///<采样周期数-12
    stcAdcCfg.enAdcRefVolSel    = AdcMskRefVolSelInBgr1p5;//AdcMskRefVolSelAVDD;      ///<参考电压选择-VCC
    stcAdcCfg.enAdcOpBuf        = AdcMskBufDisable;         ///<OP BUF配置-关
    stcAdcCfg.enInRef           = AdcMskInRefEnable;       ///<内部参考电压使能- 
    stcAdcCfg.enAdcAlign        = AdcAlignRight;            ///<转换结果对齐方式-右
    Adc_Init(&stcAdcCfg);
}

void App_AdcInit_VrefExternal(void)
{
    stc_adc_cfg_t              stcAdcCfg;

    DDL_ZERO_STRUCT(stcAdcCfg);
    ///< 开启ADC/BGR外设时钟
    Sysctrl_SetPeripheralGate(SysctrlPeripheralAdcBgr, TRUE); 
    
    Bgr_BgrEnable();        ///< 开启BGR
    
    ///< ADC 初始化配置
    stcAdcCfg.enAdcMode         = AdcScanMode;              ///<采样模式-扫描
    stcAdcCfg.enAdcClkDiv       = AdcMskClkDiv8;            ///<采样分频-1 
    stcAdcCfg.enAdcSampCycleSel = AdcMskSampCycle12Clk;      ///<采样周期数-12
    stcAdcCfg.enAdcRefVolSel    = AdcMskRefVolSelAVDD;		//AdcMskRefVolSelAVDD;      ///<参考电压选择-VCC
    stcAdcCfg.enAdcOpBuf        = AdcMskBufDisable;         ///<OP BUF配置-关
    stcAdcCfg.enInRef           = AdcMskInRefDisable;       ///<内部参考电压使能-关
    stcAdcCfg.enAdcAlign        = AdcAlignRight;            ///<转换结果对齐方式-右
    Adc_Init(&stcAdcCfg);
}


///< ADC 顺序扫描 功能配置
void App_AdcSQRCfg(void)
{
    stc_adc_sqr_cfg_t          stcAdcSqrCfg;
    
    DDL_ZERO_STRUCT(stcAdcSqrCfg);
        
    ///< 顺序扫描模式功能及通道配置
    ///< 注意：扫描模式下，当配置转换次数为n时，转换通道的配置范围必须为[SQRCH(0)MUX,SQRCH(n-1)MUX]
    stcAdcSqrCfg.bSqrDmaTrig = FALSE;
    stcAdcSqrCfg.enResultAcc = AdcResultAccDisable;
    stcAdcSqrCfg.u8SqrCnt    = 6;
    Adc_SqrModeCfg(&stcAdcSqrCfg);

    //Adc_CfgSqrChannel(AdcSQRCH0MUX, AdcExInputCH0);
    Adc_CfgSqrChannel(AdcSQRCH0MUX, AdcExInputCH4) ;
    Adc_CfgSqrChannel(AdcSQRCH1MUX, AdcExInputCH5) ;
    Adc_CfgSqrChannel(AdcSQRCH2MUX, AdcExInputCH6) ;
    Adc_CfgSqrChannel(AdcSQRCH3MUX, AdcExInputCH7) ;
    Adc_CfgSqrChannel(AdcSQRCH4MUX, AdcExInputCH8) ;
    Adc_CfgSqrChannel(AdcSQRCH5MUX, AdcExInputCH18);
    
    ///< ADC 中断失能
    Adc_DisableIrq();
    EnableNvic(ADC_IRQn, IrqLevel3, TRUE);
    
    ///< 启动顺序扫描采样
    Adc_SQR_Start();

}        
 
void ReadAdcOriginalValue(void)
{
	Adc_Original.AdcDplus   	= (long double)Adc_GetSqrResult(AdcSQRCH0MUX);		//获取顺序扫描通道0
	Adc_Original.AdcDmin   		= (long double)Adc_GetSqrResult(AdcSQRCH1MUX);  	//获取顺序扫描通道1
	Adc_Original.AdcVoltage		= (long double)Adc_GetSqrResult(AdcSQRCH2MUX);  	//获取顺序扫描通道2 
	Adc_Original.AdcTemp		= (long double)Adc_GetSqrResult(AdcSQRCH3MUX);  	//获取顺序扫描通道3
	Adc_Original.AdcCurrentVIN	= (long double)Adc_GetSqrResult(AdcSQRCH4MUX);  	//获取顺序扫描通道4
	Adc_Original.AdcCurrentOUT	= (long double)Adc_GetSqrResult(AdcSQRCH5MUX);  	//获取顺序扫描通道5 
	Adc_SQR_Start();
}
 

long double complementary_filter(long double input1,long double newinput,long double filter)
{
	return ((input1 * filter) + (newinput * (1.0f - filter)));
}

long double level_filter(long double input1,long double newinput)
{
	if(newinput-input1>=10)
	{
		input1=newinput;
		return input1;
	}	
	else if(newinput-input1>=0.5)
	{		
		return complementary_filter(input1,  newinput,  0.3);
	}		
	else if((newinput-input1>0.08)&&(newinput-input1<0.5))
	{	
		return complementary_filter(input1,  newinput,  0.81);
	}	
	else 
	{	
		return complementary_filter(input1,  newinput,  0.9);
	}	
}


long double level_3V3_filter(long double input1,long double newinput)
{
	if(newinput-input1>=2100)
	{
		input1=newinput;
		return input1;
	}
	else if(newinput-input1>=210)
	{		
		return complementary_filter(input1,  newinput,  0.3);
	}		
	else if((newinput-input1>6)&&(newinput-input1<210))
	{	
		return complementary_filter(input1,  newinput,  0.8);
	}	
	else 
	{	
		return complementary_filter(input1,  newinput,  0.91);
	}	
}


//long double filter(long double input)  
//{  
//	#define N 5
//	static long double value_buf[N];
//	static char i=0; 
//	char count;  
//	long double  sum=0;	
//
//	for(i=0;i<N;i++)
//	{
//		value_buf[i++] = input;  
//		
//		sum += value_buf[count];  
//	}
//	return (long double)(sum/N);  
//} 
//  
//#define	N  101
//long double filter(long double get_ad)  
//{  
//	long double value_buf[N],temp;  
//	int  count,i,j;  
//	for ( count=0;count<N;count++)  
//	{								
//	  value_buf[count] = get_ad;  
//	}  

//	for (j=0;j<N-1;j++)  
//	{  
//	  for (i=0;i<N-j;i++)  
//	  {  
//		 if ( value_buf[i]>value_buf[i+1] )  
//		 {  
//			temp = value_buf[i];  
//			value_buf[i] = value_buf[i+1];   
//			value_buf[i+1] = temp;  
//		 }  
//	  }  
//	}  
//	return value_buf[(N-1)/2];  
//}  



long double delayShow(long double input)
{
	static long double input_settle,output;
	static uint8_t i=0;
	if( (i++)==0 )
		input_settle=input;
	else if(i>=20){
		output=input_settle;
		i=0;
	}
	return output;
}

long double GetDisplayDplus(void)
{
	static long double  DisplayDplus,AdcDplus_sum,AdcDplus_f; 
	static char i=0;
	AdcDplus_sum+=Adc_Original.AdcDplus;
	i++;
	
	if(i>=20)
	{
		AdcDplus_f=AdcDplus_sum/20;	
		AdcDplus_sum=0;				
		i=0;						
	}
	DisplayDplus = AdcDplus_f * flash_Cali.flash_Cali_s.V3300volt /4095;
	return  DisplayDplus;
}

long double GetDisplayDmin(void)
{
	static long double  DisplayDmin,DisplayDmin_sum,AdcDmin_f; 
	static char i=0;
	
	DisplayDmin_sum+=Adc_Original.AdcDmin;
	i++;
	
	if(i>=20)
	{
		AdcDmin_f=DisplayDmin_sum/20;
		DisplayDmin_sum=0;
		i=0;
	}
	DisplayDmin = AdcDmin_f* flash_Cali.flash_Cali_s.V3300volt /4095;//VREF改为3432
	return  DisplayDmin;

}

long double GetDisplayTemp(void)
{
	static long double  Vol_adcdata,R_adcdata,DisplayTemp; 
	static long double AdcTemp_sum,AdcTemp_f;
	static char i = 0;
	AdcTemp_sum+=Adc_Original.AdcTemp;
	i++;	
			
	if(i>=10)
	{
		AdcTemp_f=AdcTemp_sum/10;
		AdcTemp_sum=0;
		i=0;
	}
	
	Vol_adcdata = (AdcTemp_f*VREF/4095)/1000;
	R_adcdata=10000 * Vol_adcdata/(3.3 - Vol_adcdata);//100000 改为10000
	DisplayTemp=1/(log(R_adcdata/10000)/3380 + (1/298.15)) -272.65;//
	
	return DisplayTemp;
}

long double GetDisplayVcurrentVIN(void)
{
	long double  DisplayVcurrentVIN; 
	DisplayVcurrentVIN = Adc_Original.AdcCurrentVIN*VREF/4095;
	return  DisplayVcurrentVIN;
}

long double GetDisplayVcurrentOUT(void)	
{
	long double  DisplayVcurrentOUT; 
	DisplayVcurrentOUT = Adc_Original.AdcCurrentOUT*VREF/4095;
	return  DisplayVcurrentOUT;
} 
long double generateVoltage(void)
{
	static long double ADCdata_originalout,Voltage_in; 

	ADCdata_originalout=Adc_Original.AdcVoltage; 									
	Voltage_in=(ADCdata_originalout * flash_Cali.flash_Cali_s.V3300volt /4095);		
																					
	return Voltage_in * 11 ;//11为硬件电阻分压 flash_Cali.flash_Cali_s.G_V1500volt  
}

//求出vref然后得到电压值
long double GetDisplayVoltage(void)
{
//	static long double origVol,origVolOffset,finalVol;
//	origVol	=generateVoltage();
//	
//	if(origVol>10000)
//	{
//		origVolOffset=150.000000000001 -0.0225000000000001*origVol + (1.25E-06)*origVol*origVol;
//		finalVol=origVol+ origVolOffset;
//	}
//	else
//	{
//		finalVol=origVol;
//	}
//	
//	return finalVol * flash_Cali.flash_Cali_s.Cali_K_fiveVolt; 
 	return generateVoltage() * flash_Cali.flash_Cali_s.Cali_K_fiveVolt; 
}

static uint8_t step=0xff;

void Currentcalibration(void)
{	
	static long double CurdataOut_sum,CurdataIn_sum,Curdata_originalOut,Curdata_originalIn;
	static char i_Courdata;
	static long double K_magni_single;					
	//static long double G_V1500volt_sum,G_V1500volt_single;	
	static long double V3300Ffloat_sum,V3300Ffloat_single;
	
	if((flash_Cali.flash_Cali_s.calibration==No_cali)||(flash_Cali.flash_Cali_s.calibration==Doing_cali))
	{
	CurdataIn_sum	+=GetDisplayVcurrentVIN(); 
	CurdataOut_sum	+=GetDisplayVcurrentOUT(); 
	i_Courdata++;
	if(i_Courdata>=20)	
	{
		Curdata_originalOut=CurdataOut_sum/20;  
		Curdata_originalIn =CurdataIn_sum /20; 
			
		CurdataOut_sum=0; 
		CurdataIn_sum=0; 
		i_Courdata=0; 
									
		if(KeyPress.HelpKey==LongPress)//if(StratUpKey3==1)//
		{	
			KeyPress.HelpKey=NoPress;
			flash_Cali.flash_Cali_s.calibration=Doing_cali;	//正在校准	
			step=0x00;					
		}	  
		/***********shan chu****5#*****/
#ifdef	DEBUG_MODE 
		flash_Cali.flash_Cali_s.V3300volt =3271.289343346;
		flash_Cali.flash_Cali_s.K_magni=16.80211480362; 
		
		flash_Cali.flash_Cali_s.calibration=Done_cali; 
		flash_Cali.flash_Cali_s.CurLocal_Zero_critial_positiveValue=319.793992493;  //
		flash_Cali.flash_Cali_s.CurLocal_Zero_critial_negativeValue=288.6030488693; 
		
		flash_Cali.flash_Cali_s.Cur_outofCaliEquation_zeroOffset_positiveValue=291.2815020417;
		flash_Cali.flash_Cali_s.Cur_outofCaliEquation_zeroOffset_negativeValue=275.7028246305;
		
		flash_Cali.flash_Cali_s.Cali_curTwoPointSevenFive_positiveValue=1.050685365021; 
		flash_Cali.flash_Cali_s.Cali_curTwoPointSevenFive_negativeValue=1.016474311915; 
		flash_Cali.flash_Cali_s.OnlyOnceForcurTwoPointSevenFiveFlag=curTwoPointSevenFive_alldoneFlag; 
#endif	
		/********************/
 		switch(step)
		{	
			case 0x00:	//外部接5v adc使用1.5v作为参考电压 求出系数					
				for(int i=0;i<40;i++)												
				{																	
					V3300Ffloat_single = 5000*4095/(Adc_Original.AdcVoltage*11);	
					V3300Ffloat_sum += V3300Ffloat_single;							
				}																	
				flash_Cali.flash_Cali_s.V3300volt =V3300Ffloat_sum/40;						
				V3300Ffloat_sum=0;												
				//这个地方再有问题直接把Curdata_originalIn 改为53.18681318681 //1#53.50915750916//2#51.73626373626//3#53.18681318681	
				Curdata_originalIn = 53.34798534799;
				K_magni_single=Curdata_originalOut/Curdata_originalIn;			//电阻改为0.005	
				flash_Cali.flash_Cali_s.K_magni=K_magni_single;
				//flash_Cali.flash_Cali_s.G_V1500volt=11;		
				step=0x01;				
				break;																								
			case 0x01:	
//				if(delayOnly(1)==1)//这个地方是留个归零校准的时间								
//				{	
					if(!isnan(flash_Cali.flash_Cali_s.V3300volt))
					{	
						step=0xff;										
						flash_Cali.flash_Cali_s.calibration=Done_cali;  
						writeCali();  
					}						
//				}	
				break; 	
			default:
				break;
		}					
	}
	}
}

long double GetOriginalCurrent(void)
{	
	static long double Curr_CaliEquation,Vdrop_R12;
 	static long double Voltk;
	static long double CurMIN;  
	static long double Current_local;
	static long double Currentout_sum;
	static long double Currentout_final;
	static char i; 
	Currentcalibration();//求3v3	k	
								
	Currentout_sum += GetDisplayVcurrentOUT();
	i++;		
	if(i>=10)	
	{				
		Currentout_final=Currentout_sum/10; 
		Currentout_sum=0; 
		i=0;	
	} 
	if(!isnan(flash_Cali.flash_Cali_s.V3300volt)&&(Currentout_final!=0))	//保证Vout和K值都为有效值
	{													
		Voltk=Currentout_final;							
		Voltk=Voltk/flash_Cali.flash_Cali_s.K_magni; 	
		Vdrop_R12=(flash_Cali.flash_Cali_s.V3300volt-Voltk)*681/42200;				
																					
		if((step==0x01)||(flash_Cali.flash_Cali_s.calibration==Done_cali))			
		{																			
			Current_local = (Voltk- Vdrop_R12)/0.005;	//005						
		}																								
	}															
	/************确定正负**************/	
	if(Interface==CaliForFactoryInterface)	
	{
		if(KeyPress.HelpKey==ShortPress)													
		{	
			KeyPress.HelpKey=NoPress;												
			if((flash_Cali.flash_Cali_s.CurZeroCritialFlag&0x01)==0x01)//第一位是1  
			{																		
				flash_Cali.flash_Cali_s.CurLocal_Zero_critial_positiveValue=Current_local;
				flash_Cali.flash_Cali_s.CurZeroCritialFlag&=0x02;//把第一位置0
				
				CurMIN=-9.72197665527381 + 0.118838187015592 * Current_local + 0.00000233906377363875*(Current_local*Current_local) - 0.000000000259381731498375 *(Current_local*Current_local*Current_local);//当电阻是0.005欧姆的时候																								
				Curr_CaliEquation=Current_local - CurMIN;		
				
				flash_Cali.flash_Cali_s.Cur_outofCaliEquation_zeroOffset_positiveValue = Curr_CaliEquation; 
				writeCali();																																
			}								
		}								
		else if(KeyPress.NextKey==ShortPress)			
		{
			KeyPress.NextKey=NoPress;														
			if((flash_Cali.flash_Cali_s.CurZeroCritialFlag&0x02)==0x02)//第二位是1 
			{
				flash_Cali.flash_Cali_s.CurLocal_Zero_critial_negativeValue=Current_local;
				flash_Cali.flash_Cali_s.CurZeroCritialFlag&=0x01;//把第二位置0
				
				CurMIN=15.0108698392389 - 0.11314941372057 *Current_local - 0.0000029112702627779*(Current_local*Current_local)- 0.000000000555098502228613 *(Current_local*Current_local*Current_local);//当电阻是0.005欧姆的时候
				Curr_CaliEquation=Current_local + CurMIN+5- (Voltage_u32/1000*0.5);//+30
					
				flash_Cali.flash_Cali_s.Cur_outofCaliEquation_zeroOffset_negativeValue = Curr_CaliEquation; 
		
				writeCali();	
			}
		}
	}
 	/************* *** 校准公式 *** *************/
	static uint8_t onlyonce_currdisp;
	if (onlyonce_currdisp<0x15)
	{	
		onlyonce_currdisp++;
		return 0;
	}
	else
	{
		onlyonce_currdisp=0xf0;
	}	
						
	if((Current_local>flash_Cali.flash_Cali_s.CurLocal_Zero_critial_positiveValue)\
		&&(!isnan(flash_Cali.flash_Cali_s.CurLocal_Zero_critial_positiveValue)))		 																																	
	{																									
		CurMIN=-9.72197665527381 + 0.118838187015592 *Current_local + 0.00000233906377363875*(Current_local*Current_local) - 0.000000000259381731498375 *(Current_local*Current_local*Current_local);//当电阻是0.005欧姆的时候																								
		Curr_CaliEquation=Current_local - CurMIN;																																													
	}																																																															
	else if(Current_local<flash_Cali.flash_Cali_s.CurLocal_Zero_critial_negativeValue)																																														
	{
		//CurMIN=15.0108698392389 - 0.11314941372057 *Current_local - 0.0000029112702627779*(Current_local*Current_local)- 0.000000000555098502228613 *(Current_local*Current_local*Current_local);//当电阻是0.005欧姆的时候		
		//Curr_CaliEquation=Current_local + CurMIN+5;
		CurMIN=15.0108698392389 - 0.11314941372057 *Current_local - 0.0000029112702627779*(Current_local*Current_local)- 0.000000000555098502228613 *(Current_local*Current_local*Current_local);//当电阻是0.005欧姆的时候		
		Curr_CaliEquation=Current_local + CurMIN+5 - (Voltage_u32/1000*0.5);//+30
	}																																										
	else 																																									
	{	
		Curr_CaliEquation=0;
	}	
															
	if(flash_Cali.flash_Cali_s.calibration==Done_cali)
	{		
		if((Current_local>flash_Cali.flash_Cali_s.CurLocal_Zero_critial_positiveValue)\
			&&(!isnan(flash_Cali.flash_Cali_s.Cur_outofCaliEquation_zeroOffset_positiveValue)))	
		{		
			Curr_CaliEquation-=flash_Cali.flash_Cali_s.Cur_outofCaliEquation_zeroOffset_positiveValue; 	
		}
		else if((Current_local<flash_Cali.flash_Cali_s.CurLocal_Zero_critial_negativeValue)\
			&&(!isnan(flash_Cali.flash_Cali_s.Cur_outofCaliEquation_zeroOffset_negativeValue)))
		{
			Curr_CaliEquation-=flash_Cali.flash_Cali_s.Cur_outofCaliEquation_zeroOffset_negativeValue; 
		}							
	}													
    //2.75为基准，乘以系数											
	if((Curr_CaliEquation>0)&&(!isnan(flash_Cali.flash_Cali_s.Cali_curTwoPointSevenFive_positiveValue)))		 
		Curr_CaliEquation *= flash_Cali.flash_Cali_s.Cali_curTwoPointSevenFive_positiveValue;								
	else if((Curr_CaliEquation<0)&&(!isnan(flash_Cali.flash_Cali_s.Cali_curTwoPointSevenFive_negativeValue)))								
		Curr_CaliEquation *= flash_Cali.flash_Cali_s.Cali_curTwoPointSevenFive_negativeValue;	
		
	/********/
	//Curr_CaliEquation=Current_local;
	return Curr_CaliEquation ; 
}

long double GetDisplayCurrent(void)
{
	static long double	Curre; 
	static long double	Final_CurrentValue;		
		
	Curre=GetOriginalCurrent();			
	if(Curre>0)		
	{		
		Final_CurrentValue= Curre-flash_Cali.flash_Cali_s.Cali_currentTurnZero;
//		if(Final_CurrentValue<0)
//		{	
//			Final_CurrentValue=0;
//		}								
	}																
	else if(Curre<0) 	
	{		
		Final_CurrentValue= Curre-flash_Cali.flash_Cali_s.Cali_currentTurnZero;
//		Final_CurrentValue= Curre;			
	}		
	else	
	{		
		Final_CurrentValue=0;
	}
	return Final_CurrentValue;
	
}
uint8_t delayOnly(uint16_t delaytime)				
{												
	static uint32_t Timcount=0;				
	static uint8_t a=0x01;	
	switch(a) 
	{
		case 0x01:
			Timcount++; 
			if(Timcount==delaytime){
				a=0x02;
			}	
			break;
		case 0x02: 
			Timcount=0;
			a=0x03;
			break;
		default:
			break;			
	}
	
	if(a==0x03)
	{
		a=0x01;
		return 1;
	}
	else 
	{
		return 0;
	}
}

long double stableData(long double input)
{
	static long double inputdata_sum=0,outputfilter=0,output;
	output=input;
	static char i=0;
	float a= 0.4f;//越小则滤波强度越大
	i++;	
	inputdata_sum += input;
	if(i >=10)	
	{
		outputfilter=inputdata_sum/10;
		inputdata_sum=0;
		i=0;
		
		output=(a * outputfilter + (1 - a) * output);
		return output;
	}
	else
		return -1;
}

//long double stableData(long double input)
//{
//	
//	uint8_t num=0;
//	num++;	
//	if(num >=20)	
//	{	
//		Curre=Curre_sum/20;
//		i_curr=0;
//		Curre_sum=0;
//	}	
//	
//	
//	static long double output=0;
//	float a= 0.1f;
//	
//	output=(a * input + (1 - a) * output);
//    return output;
//}


/**
这个函数后面可能会去掉，尽量不要影响其他逻辑。
使用正常检测来的电压判断，如果检测电压低于4.2v，
则电压检测改为使用内部1.5v作为参考电压。
且只读取完成后需要改回来防止影响其他ADC数值的检测
**********************/
/************************
long double switchReferenceVoltageOfSupply(long double input)
{
	if(input<4200)
	{
		App_AdcInit_VrefInternal();//1.5
		App_AdcSQRCfg();
		
		//电压输出
		static long double Ffloat_sum,ADCdata_originalout,Voltage_in; 
		static char i_Ffloat; 
		Ffloat_sum+=Adc_Original.AdcVoltage; 
		i_Ffloat++; 
		if(i_Ffloat>=32)
		{            	   	
			i_Ffloat=0;								
			ADCdata_originalout=Ffloat_sum/32; 		
			Voltage_in=(ADCdata_originalout * 1500 /4095);//11为硬件电阻分压 
			Ffloat_sum=0;
		}
		//电压输出end

		//App_AdcInit_VrefExternal();//3V3
		//App_AdcSQRCfg();
		
		return Voltage_in * flash_Cali.flash_Cali_s.G_V1500volt*flash_Cali.flash_Cali_s.Cali_K_fiveVolt;
	}
	else
		return input;
}
********************************/
