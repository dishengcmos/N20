
#include "chart.h"
#include "lcd_api.h"
#include "spi.h"
#include "gpio.h"
#include "adc_mid.h"
#include <stdlib.h>
#include "displaydriver.h" 
		 
 
/*曲线显示区域，即相对于LCD的高度,Y轴*/
#define PLOT_DISPLAY_AREA_Y  192  
#define LCD_X 160
#define LCD_Y 80

#define VOL_RANGE 600 //480
#define CUR_RANGE 600 //480
#define CALCULATE_RANGE 16 //超过这个范围则计算曲线位置，不超过则直接向右平移曲线

 
/*曲线处理*/

int pVoltline[DATA_SIZE];		
//0xffffffff 00000000			
int pCurrline[DATA_SIZE];		

int voltageOrigiChart[DATA_SIZE];
int currentOrigiChart[DATA_SIZE];

FilterValue_s FilterValue;

/** 	
1.根据商讨，将电压显示曲线页面暂时修改为从左向右行进。改进显示范围完善曲线长度质量。
		
问题：
1.已知竞品精度为0.01V，但是实际上精度无法达到此精度，在0.01v电压变化时并无波动。这样看起来似乎曲线比较平，但是精确度根本达不到
2.已知竞品精度为0.01V，但实际上百分之一伏特的刻度永远为0。无意义，暂时将其修改为0.01v的数值是实时变化的，但是会出现数值经常变动，看起来不稳定，显示曲线比较符合实际不是像竞品那样平滑的
3.竞品曲线比较平滑。一是由于精度低，不能很好的反应电压变化。二是由于在计算过程中，跟多的依赖于线条的平移而不是实时生成，这会造成电压数值变化时不能很好起伏，表现为一卡一卡的。且竞品电压范围小，加剧卡顿
***/
		
void bufferSampling(void) 					
{	 
	for(int i = 0 ; i < DATA_SIZE ; i++)
	{	
		voltageOrigiChart[i] = (uint16_t)0.0f;	
	} 
}

void additionalData_vol(int *chartdata,int input,int *input_filter)	//voltageOrigiChart 
{		
	static uint8_t onlyoncetimes=0;
	int	VoltageOrigina; 
	VoltageOrigina= input;
	
	 *input_filter+= (VoltageOrigina -*input_filter) * 0.2;// 系数小 滤波程度较大
 
	/******这部分用来删除前几个值，防止前几个值是点，连不上线******/
//	if(onlyoncetimes++<0x09)
//		return;
//	else
//		onlyoncetimes=0x09;
	/********end********/
	chartdata[0] = *input_filter;	
	for(int i = DATA_SIZE-1 ;i>=0;i--)  	//DATA_SIZE 
	{   
		chartdata[i + 1] =chartdata[i]; 
	}	
}	
void additionalData_cur(int *chartdata,int input,int *input_filter)	//voltageOrigiChart 
{		
	static uint8_t onlyoncetimes=0;
	int	VoltageOrigina; 
	VoltageOrigina= input;
																	
	*input_filter+= (VoltageOrigina -*input_filter) * 0.2;//0.6系数小 滤波程度较大
	//*input_filter=VoltageOrigina;
	/******这部分用来删除前几个值，防止前几个值是点，连不上线******/
//	if(onlyoncetimes++<0x09)
//		return;	
//		//chartdata[0]=0;
//	else	
//		onlyoncetimes=0x09;
	/***********end*****/	
	chartdata[0] = *input_filter;	
	for(int i = DATA_SIZE-1 ; i >=  0 ; i--)  	 
	{   
		chartdata[i + 1] = chartdata[i];
	}	
}		
/******								
实际宽度是96个像素，对应0 ~~~ 960mA
相当于每个像素10ma的测量范围		
*****/	

void lcdPlotRemap_cur(int *intput_chart, int *output_chart,int range)	
{	
	static	int min;//max,
    int g_row;
	uint8_t movelens;
	static float temp = 0; 
	movelens=returnLens();
	g_row=returngrow(); 
	float height = PLOT_DISPLAY_AREA_Y;			
	static float scale ;							
	scale =height/(2*range);									
	if((abs(g_row-103)>=CALCULATE_RANGE)||(movelens==1))//CURRENT	
	{								 				
		min =((intput_chart[0]/100)*100)-650; 				
		if(min<=-50)							
		{									
			min=-50;	 				
		}									
	}										 	
    for(uint32_t i = 0; i < DATA_SIZE; i ++)			
    {	
		if(intput_chart[i]<0)
		return; 
		temp =  intput_chart[i] - min;		
		output_chart[i] =  temp*scale; 
    }
}
void lcdPlotRemap_vol(int *intput_chart, int *output_chart,int range)	
{	
	static	int min;//max
	int g_row;
	uint8_t movelens;
	float temp = 0; 
	movelens=returnLens();
	g_row=returngrow();						
	float height = PLOT_DISPLAY_AREA_Y;
	static	float scale;		
    scale =  height/(2*range);									
	if((abs(g_row-103)>=CALCULATE_RANGE)||(movelens==1))//VOLTAGE
	{									 
		min =((intput_chart[0]/100)*100)-650; 
		if(min<=-50)
		{	
			min=-50; 
		}	
	}				 
    for(uint32_t i = 0; i < DATA_SIZE; i ++)
    {
		if(intput_chart[i]<0)
		return; 
        temp =  intput_chart[i] - min;
		output_chart[i]=temp*scale; 
    }
}
 
int VoltageObScale;
static float offset_voltemp,offset_voltemp_f; 
void fillVolScale(int *offset_10mv)//
{												
//	static float offset_voltemp,offset_voltemp_f; 
	int scale_row;
	scale_row=returngrow();
	
	if(scale_row>103)
	{
//		offset_voltemp_f=(float)FilterValue.VoltageFilter;
		offset_voltemp=(float)FilterValue.VoltageFilter;
		
	} 
 	VoltageObScale = offset_voltemp;//(int)roundOffVoltage((uint32_t)offset_voltemp);	
	
	scaleVolPlate7[0]=	 (VoltageObScale/10000)%10;
	scaleVolPlate7[1]=	 (VoltageObScale/1000)%10;
	scaleVolPlate7[3]=	 (VoltageObScale/100)%10; 
	
	VoltageObScale-=100;
	if(VoltageObScale<0)VoltageObScale=0;
	scaleVolPlate6[0]=	 (VoltageObScale/10000)%10;
	scaleVolPlate6[1]=	 (VoltageObScale/1000)%10;
	scaleVolPlate6[3]=	 (VoltageObScale/100)%10;
	
	VoltageObScale-=100;
	if(VoltageObScale<0)VoltageObScale=0;
	scaleVolPlate5[0]=	 (VoltageObScale/10000)%10;
	scaleVolPlate5[1]=	 (VoltageObScale/1000)%10;
	scaleVolPlate5[3]=	 (VoltageObScale/100)%10;
	
	VoltageObScale-=100;
	if(VoltageObScale<0)VoltageObScale=0;
	scaleVolPlate4[0]=	 (VoltageObScale/10000)%10;
	scaleVolPlate4[1]=	 (VoltageObScale/1000)%10;
	scaleVolPlate4[3]=	 (VoltageObScale/100)%10;
	
	VoltageObScale-=100;
	if(VoltageObScale<0)VoltageObScale=0;
	scaleVolPlate3[0]=	 (VoltageObScale/10000)%10;
	scaleVolPlate3[1]=	 (VoltageObScale/1000)%10;
	scaleVolPlate3[3]=	 (VoltageObScale/100)%10;
		
	VoltageObScale-=100;
	if(VoltageObScale<0)VoltageObScale=0;
	scaleVolPlate2[0]=	 (VoltageObScale/10000)%10;
	scaleVolPlate2[1]=	 (VoltageObScale/1000)%10;
	scaleVolPlate2[3]=	 (VoltageObScale/100)%10;
	
	VoltageObScale-=100;
	if(VoltageObScale<0)VoltageObScale=0;
	scaleVolPlate1[0]=	 (VoltageObScale/10000)%10;
	scaleVolPlate1[1]=	 (VoltageObScale/1000)%10;
	scaleVolPlate1[3]=	 (VoltageObScale/100)%10;
	
	VoltageObScale+=700;
	scaleVolPlate8[0]=	 (VoltageObScale/10000)%10;
	scaleVolPlate8[1]=	 (VoltageObScale/1000)%10;
	scaleVolPlate8[3]=	 (VoltageObScale/100)%10;
	
	VoltageObScale+=100;
	scaleVolPlate9[0]=	 (VoltageObScale/10000)%10;
	scaleVolPlate9[1]=	 (VoltageObScale/1000)%10;
	scaleVolPlate9[3]=	 (VoltageObScale/100)%10;
	
	VoltageObScale+=100;
	scaleVolPlate10[0]=	 (VoltageObScale/10000)%10;
	scaleVolPlate10[1]=	 (VoltageObScale/1000)%10;
	scaleVolPlate10[3]=	 (VoltageObScale/100)%10;
	
	VoltageObScale+=100;
	scaleVolPlate11[0]=	 (VoltageObScale/10000)%10;
	scaleVolPlate11[1]=	 (VoltageObScale/1000)%10;
	scaleVolPlate11[3]=	 (VoltageObScale/100)%10;	
	
	VoltageObScale+=100;
	scaleVolPlate12[0]=	 (VoltageObScale/10000)%10;
	scaleVolPlate12[1]=	 (VoltageObScale/1000)%10;
	scaleVolPlate12[3]=	 (VoltageObScale/100)%10;	
  
}

int roundOffChart(int inputdata,int localrow)
{
	if((((inputdata/10)%10)>=5)&&(localrow<=48))
		return inputdata+100;
	
	else if((((inputdata/10)%10)<5)&&(localrow>48))
		return inputdata;
}

int scalec_row;
static float offset_curtemp_f; 
void fillCurScale(int *offset_10ma)  // 
{
	static int CurrentObScale;
//	static float offset_curtemp,offset_curtemp_f; 
	
	scalec_row=returngrow();
	
	if(scalec_row>103)
	{
		offset_curtemp_f = (float)FilterValue.CurrentFilter;
	}
//	offset_curtemp =offset_curtemp_f;// 系数小 滤波程度较大
	
	CurrentObScale =(int)offset_curtemp_f;
	
	if(CurrentObScale<=600){	
		scaleCurPlate1[0]= 0;  
		scaleCurPlate1[2]=0;
		scaleCurPlate2[0]= 0; 
		scaleCurPlate2[2]=1;
		scaleCurPlate3[0]= 0;  
		scaleCurPlate3[2]=2;
		scaleCurPlate4[0]= 0;  
		scaleCurPlate4[2]=3;
		scaleCurPlate5[0]= 0;
		scaleCurPlate5[2]= 4;
		scaleCurPlate6[0]= 0;  
		scaleCurPlate6[2]= 5;
		scaleCurPlate7[0]= 0;  
		scaleCurPlate7[2]=6;
		scaleCurPlate8[0]= 0;  
		scaleCurPlate8[2]=7;
		scaleCurPlate9[0]= 0; 
		scaleCurPlate9[2]=8;
		scaleCurPlate10[0]= 0; 
		scaleCurPlate10[2]=9;
		scaleCurPlate11[0]= 1; 
		scaleCurPlate11[2]=0;
		scaleCurPlate12[0]= 1; 
		scaleCurPlate12[2]=1;
	}
	else
	{	 
		scaleCurPlate7[0]=	 (CurrentObScale/1000)%10;	
		scaleCurPlate7[2]=	 (CurrentObScale/100)%10; 	
		CurrentObScale-=100;
		if(CurrentObScale<0)CurrentObScale=0;
		scaleCurPlate6[0]=	 (CurrentObScale/1000)%10;	
		scaleCurPlate6[2]=	 (CurrentObScale/100)%10;	
		CurrentObScale-=100;
		if(CurrentObScale<0)CurrentObScale=0;
		scaleCurPlate5[0]=	 (CurrentObScale/1000)%10;
		scaleCurPlate5[2]=	 (CurrentObScale/100)%10;
		CurrentObScale-=100;
		if(CurrentObScale<0)CurrentObScale=0;
		scaleCurPlate4[0]=	 (CurrentObScale/1000)%10;
		scaleCurPlate4[2]=	 (CurrentObScale/100)%10;
		CurrentObScale-=100;
		if(CurrentObScale<0)CurrentObScale=0;
		scaleCurPlate3[0]=	 (CurrentObScale/1000)%10;
		scaleCurPlate3[2]=	 (CurrentObScale/100)%10;
		CurrentObScale-=100;
		if(CurrentObScale<0)CurrentObScale=0;
		scaleCurPlate2[0]=	 (CurrentObScale/1000)%10;
		scaleCurPlate2[2]=	 (CurrentObScale/100)%10;
		CurrentObScale-=100;
		if(CurrentObScale<0)CurrentObScale=0;
		scaleCurPlate1[0]=	 (CurrentObScale/1000)%10;
		scaleCurPlate1[2]=	 (CurrentObScale/100)%10;
		CurrentObScale+=700;
		scaleCurPlate8[0]=	 (CurrentObScale/1000)%10;
		scaleCurPlate8[2]=	 (CurrentObScale/100)%10; 
		CurrentObScale+=100;
		scaleCurPlate9[0]=	 (CurrentObScale/1000)%10;
		scaleCurPlate9[2]=	 (CurrentObScale/100)%10;
		CurrentObScale+=100;
		scaleCurPlate10[0]=	 (CurrentObScale/1000)%10;
		scaleCurPlate10[2]=	 (CurrentObScale/100)%10;	
		CurrentObScale+=100;
		scaleCurPlate11[0]=	 (CurrentObScale/1000)%10;
		scaleCurPlate11[2]=	 (CurrentObScale/100)%10;
		CurrentObScale+=100;
		scaleCurPlate12[0]=	 (CurrentObScale/1000)%10;
		scaleCurPlate12[2]=	 (CurrentObScale/100)%10;
	}
}

void displayVolChart(void)			
{	 							
 	static int offset_vol; 		
 	static int voltage; 		
	static uint8_t Onlyonce=0; 	// 当值为0xff 才开始显示曲线
															 
	if(Onlyonce++<0x12)										//0x07		
	{														
 		FilterValue.VoltageFilter=(int)GetDisplayVoltage();	//刚进入曲线，目的是让刻度直接到想要位置
		offset_voltemp=FilterValue.VoltageFilter;
		voltage=(int)GetDisplayVoltage();	
	}				
	else													//过了一段时间，可以取ADC作为电压曲线,这么做是为了直接取有效值，防止曲线从0上升
	{		
		Onlyonce=0xff;
		voltage=(int)GetDisplayVoltage();		
	}	
	
	if(Onlyonce==0xff)	//
	{		
		additionalData_vol(voltageOrigiChart,voltage,&FilterValue.VoltageFilter);	
		lcdPlotRemap_vol(voltageOrigiChart,pVoltline,VOL_RANGE);
	}		
 	fillVolScale(&offset_vol); 													
		
	chartvolProcessing(pVoltline,Onlyonce);	
}												
												
void displayCurChart(void) 		
{			
	static int offset_cur; 	
	static int current; 		
	static uint8_t OnlyonceCur=0; 

											
	if(OnlyonceCur++ < 0x12) 
	{ 										
		current=(int)GetDisplayCurrent();
		if(current<0)							
			current=-current;			
		FilterValue.CurrentFilter=current;	
			offset_curtemp_f=FilterValue.CurrentFilter;
	} 										
	else 		 											
	{												
		current=(int)GetDisplayCurrent();
		if(current<0)							
			current=-current;											
		OnlyonceCur=0xff;												
	}															 		

	if(OnlyonceCur==0xff)	//
	{
		additionalData_cur(currentOrigiChart,current,&FilterValue.CurrentFilter);	
		lcdPlotRemap_cur(currentOrigiChart,pCurrline,CUR_RANGE);	
	}			
	fillCurScale(&offset_cur);									
	chartcurrProcessing(pCurrline,OnlyonceCur);		
}













