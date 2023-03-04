
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
 
///< ADC �����˿ڳ�ʼ��
void App_AdcPortInit(void)
{    
    ///< ����GPIO����ʱ��
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
      
    Gpio_SetAnalogMode(GpioPortA, GpioPin4);        //PA04 (AIN4) D+
    Gpio_SetAnalogMode(GpioPortA, GpioPin5);        //PA05 (AIN5) D-
    Gpio_SetAnalogMode(GpioPortA, GpioPin6);        //PA06 (AIN6) VOL 
    Gpio_SetAnalogMode(GpioPortA, GpioPin7);        //PA07 (AIN7) TEMP
    Gpio_SetAnalogMode(GpioPortB, GpioPin0); 		//PB00 (AIN8) VcurrentVIN   	
    Gpio_SetAnalogMode(GpioPortB, GpioPin11);       //PB011(AIN18)CURRENT
}

///< ADC ģ���ʼ��
void App_AdcInit_VrefInternal(void)
{
    stc_adc_cfg_t              stcAdcCfg;

    DDL_ZERO_STRUCT(stcAdcCfg);
    ///< ����ADC/BGR����ʱ��
    Sysctrl_SetPeripheralGate(SysctrlPeripheralAdcBgr, TRUE); 
    
    Bgr_BgrEnable();        ///< ����BGR
    
    ///< ADC ��ʼ������
    stcAdcCfg.enAdcMode         = AdcScanMode;              ///<����ģʽ-ɨ��
    stcAdcCfg.enAdcClkDiv       = AdcMskClkDiv8;            ///<������Ƶ
    stcAdcCfg.enAdcSampCycleSel = AdcMskSampCycle12Clk;      ///<����������-12
    stcAdcCfg.enAdcRefVolSel    = AdcMskRefVolSelInBgr1p5;//AdcMskRefVolSelAVDD;      ///<�ο���ѹѡ��-VCC
    stcAdcCfg.enAdcOpBuf        = AdcMskBufDisable;         ///<OP BUF����-��
    stcAdcCfg.enInRef           = AdcMskInRefEnable;       ///<�ڲ��ο���ѹʹ��- 
    stcAdcCfg.enAdcAlign        = AdcAlignRight;            ///<ת��������뷽ʽ-��
    Adc_Init(&stcAdcCfg);
}

void App_AdcInit_VrefExternal(void)
{
    stc_adc_cfg_t              stcAdcCfg;

    DDL_ZERO_STRUCT(stcAdcCfg);
    ///< ����ADC/BGR����ʱ��
    Sysctrl_SetPeripheralGate(SysctrlPeripheralAdcBgr, TRUE); 
    
    Bgr_BgrEnable();        ///< ����BGR
    
    ///< ADC ��ʼ������
    stcAdcCfg.enAdcMode         = AdcScanMode;              ///<����ģʽ-ɨ��
    stcAdcCfg.enAdcClkDiv       = AdcMskClkDiv8;            ///<������Ƶ-1 
    stcAdcCfg.enAdcSampCycleSel = AdcMskSampCycle12Clk;      ///<����������-12
    stcAdcCfg.enAdcRefVolSel    = AdcMskRefVolSelAVDD;		//AdcMskRefVolSelAVDD;      ///<�ο���ѹѡ��-VCC
    stcAdcCfg.enAdcOpBuf        = AdcMskBufDisable;         ///<OP BUF����-��
    stcAdcCfg.enInRef           = AdcMskInRefDisable;       ///<�ڲ��ο���ѹʹ��-��
    stcAdcCfg.enAdcAlign        = AdcAlignRight;            ///<ת��������뷽ʽ-��
    Adc_Init(&stcAdcCfg);
}


///< ADC ˳��ɨ�� ��������
void App_AdcSQRCfg(void)
{
    stc_adc_sqr_cfg_t          stcAdcSqrCfg;
    
    DDL_ZERO_STRUCT(stcAdcSqrCfg);
        
    ///< ˳��ɨ��ģʽ���ܼ�ͨ������
    ///< ע�⣺ɨ��ģʽ�£�������ת������Ϊnʱ��ת��ͨ�������÷�Χ����Ϊ[SQRCH(0)MUX,SQRCH(n-1)MUX]
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
    
    ///< ADC �ж�ʧ��
    Adc_DisableIrq();
    EnableNvic(ADC_IRQn, IrqLevel3, TRUE);
    
    ///< ����˳��ɨ�����
    Adc_SQR_Start();

}        
 
void ReadAdcOriginalValue(void)
{
	Adc_Original.AdcDplus   	= (long double)Adc_GetSqrResult(AdcSQRCH0MUX);		//��ȡ˳��ɨ��ͨ��0
	Adc_Original.AdcDmin   		= (long double)Adc_GetSqrResult(AdcSQRCH1MUX);  	//��ȡ˳��ɨ��ͨ��1
	Adc_Original.AdcVoltage		= (long double)Adc_GetSqrResult(AdcSQRCH2MUX);  	//��ȡ˳��ɨ��ͨ��2 
	Adc_Original.AdcTemp		= (long double)Adc_GetSqrResult(AdcSQRCH3MUX);  	//��ȡ˳��ɨ��ͨ��3
	Adc_Original.AdcCurrentVIN	= (long double)Adc_GetSqrResult(AdcSQRCH4MUX);  	//��ȡ˳��ɨ��ͨ��4
	Adc_Original.AdcCurrentOUT	= (long double)Adc_GetSqrResult(AdcSQRCH5MUX);  	//��ȡ˳��ɨ��ͨ��5 
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
	DisplayDmin = AdcDmin_f* flash_Cali.flash_Cali_s.V3300volt /4095;//VREF��Ϊ3432
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
	R_adcdata=10000 * Vol_adcdata/(3.3 - Vol_adcdata);//100000 ��Ϊ10000
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
																					
	return Voltage_in * 11 ;//11ΪӲ�������ѹ flash_Cali.flash_Cali_s.G_V1500volt  
}

//���vrefȻ��õ���ѹֵ
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
			flash_Cali.flash_Cali_s.calibration=Doing_cali;	//����У׼	
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
			case 0x00:	//�ⲿ��5v adcʹ��1.5v��Ϊ�ο���ѹ ���ϵ��					
				for(int i=0;i<40;i++)												
				{																	
					V3300Ffloat_single = 5000*4095/(Adc_Original.AdcVoltage*11);	
					V3300Ffloat_sum += V3300Ffloat_single;							
				}																	
				flash_Cali.flash_Cali_s.V3300volt =V3300Ffloat_sum/40;						
				V3300Ffloat_sum=0;												
				//����ط���������ֱ�Ӱ�Curdata_originalIn ��Ϊ53.18681318681 //1#53.50915750916//2#51.73626373626//3#53.18681318681	
				Curdata_originalIn = 53.34798534799;
				K_magni_single=Curdata_originalOut/Curdata_originalIn;			//�����Ϊ0.005	
				flash_Cali.flash_Cali_s.K_magni=K_magni_single;
				//flash_Cali.flash_Cali_s.G_V1500volt=11;		
				step=0x01;				
				break;																								
			case 0x01:	
//				if(delayOnly(1)==1)//����ط�����������У׼��ʱ��								
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
	Currentcalibration();//��3v3	k	
								
	Currentout_sum += GetDisplayVcurrentOUT();
	i++;		
	if(i>=10)	
	{				
		Currentout_final=Currentout_sum/10; 
		Currentout_sum=0; 
		i=0;	
	} 
	if(!isnan(flash_Cali.flash_Cali_s.V3300volt)&&(Currentout_final!=0))	//��֤Vout��Kֵ��Ϊ��Чֵ
	{													
		Voltk=Currentout_final;							
		Voltk=Voltk/flash_Cali.flash_Cali_s.K_magni; 	
		Vdrop_R12=(flash_Cali.flash_Cali_s.V3300volt-Voltk)*681/42200;				
																					
		if((step==0x01)||(flash_Cali.flash_Cali_s.calibration==Done_cali))			
		{																			
			Current_local = (Voltk- Vdrop_R12)/0.005;	//005						
		}																								
	}															
	/************ȷ������**************/	
	if(Interface==CaliForFactoryInterface)	
	{
		if(KeyPress.HelpKey==ShortPress)													
		{	
			KeyPress.HelpKey=NoPress;												
			if((flash_Cali.flash_Cali_s.CurZeroCritialFlag&0x01)==0x01)//��һλ��1  
			{																		
				flash_Cali.flash_Cali_s.CurLocal_Zero_critial_positiveValue=Current_local;
				flash_Cali.flash_Cali_s.CurZeroCritialFlag&=0x02;//�ѵ�һλ��0
				
				CurMIN=-9.72197665527381 + 0.118838187015592 * Current_local + 0.00000233906377363875*(Current_local*Current_local) - 0.000000000259381731498375 *(Current_local*Current_local*Current_local);//��������0.005ŷķ��ʱ��																								
				Curr_CaliEquation=Current_local - CurMIN;		
				
				flash_Cali.flash_Cali_s.Cur_outofCaliEquation_zeroOffset_positiveValue = Curr_CaliEquation; 
				writeCali();																																
			}								
		}								
		else if(KeyPress.NextKey==ShortPress)			
		{
			KeyPress.NextKey=NoPress;														
			if((flash_Cali.flash_Cali_s.CurZeroCritialFlag&0x02)==0x02)//�ڶ�λ��1 
			{
				flash_Cali.flash_Cali_s.CurLocal_Zero_critial_negativeValue=Current_local;
				flash_Cali.flash_Cali_s.CurZeroCritialFlag&=0x01;//�ѵڶ�λ��0
				
				CurMIN=15.0108698392389 - 0.11314941372057 *Current_local - 0.0000029112702627779*(Current_local*Current_local)- 0.000000000555098502228613 *(Current_local*Current_local*Current_local);//��������0.005ŷķ��ʱ��
				Curr_CaliEquation=Current_local + CurMIN+5- (Voltage_u32/1000*0.5);//+30
					
				flash_Cali.flash_Cali_s.Cur_outofCaliEquation_zeroOffset_negativeValue = Curr_CaliEquation; 
		
				writeCali();	
			}
		}
	}
 	/************* *** У׼��ʽ *** *************/
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
		CurMIN=-9.72197665527381 + 0.118838187015592 *Current_local + 0.00000233906377363875*(Current_local*Current_local) - 0.000000000259381731498375 *(Current_local*Current_local*Current_local);//��������0.005ŷķ��ʱ��																								
		Curr_CaliEquation=Current_local - CurMIN;																																													
	}																																																															
	else if(Current_local<flash_Cali.flash_Cali_s.CurLocal_Zero_critial_negativeValue)																																														
	{
		//CurMIN=15.0108698392389 - 0.11314941372057 *Current_local - 0.0000029112702627779*(Current_local*Current_local)- 0.000000000555098502228613 *(Current_local*Current_local*Current_local);//��������0.005ŷķ��ʱ��		
		//Curr_CaliEquation=Current_local + CurMIN+5;
		CurMIN=15.0108698392389 - 0.11314941372057 *Current_local - 0.0000029112702627779*(Current_local*Current_local)- 0.000000000555098502228613 *(Current_local*Current_local*Current_local);//��������0.005ŷķ��ʱ��		
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
    //2.75Ϊ��׼������ϵ��											
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
	float a= 0.4f;//ԽС���˲�ǿ��Խ��
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
�������������ܻ�ȥ����������ҪӰ�������߼���
ʹ������������ĵ�ѹ�жϣ��������ѹ����4.2v��
���ѹ����Ϊʹ���ڲ�1.5v��Ϊ�ο���ѹ��
��ֻ��ȡ��ɺ���Ҫ�Ļ�����ֹӰ������ADC��ֵ�ļ��
**********************/
/************************
long double switchReferenceVoltageOfSupply(long double input)
{
	if(input<4200)
	{
		App_AdcInit_VrefInternal();//1.5
		App_AdcSQRCfg();
		
		//��ѹ���
		static long double Ffloat_sum,ADCdata_originalout,Voltage_in; 
		static char i_Ffloat; 
		Ffloat_sum+=Adc_Original.AdcVoltage; 
		i_Ffloat++; 
		if(i_Ffloat>=32)
		{            	   	
			i_Ffloat=0;								
			ADCdata_originalout=Ffloat_sum/32; 		
			Voltage_in=(ADCdata_originalout * 1500 /4095);//11ΪӲ�������ѹ 
			Ffloat_sum=0;
		}
		//��ѹ���end

		//App_AdcInit_VrefExternal();//3V3
		//App_AdcSQRCfg();
		
		return Voltage_in * flash_Cali.flash_Cali_s.G_V1500volt*flash_Cali.flash_Cali_s.Cali_K_fiveVolt;
	}
	else
		return input;
}
********************************/
