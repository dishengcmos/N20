
#ifndef __CHART_H__
#define __CHART_H__

#include "ddl.h" 

#define DATA_SIZE   184// 	

extern int pVoltline[DATA_SIZE];	//0xffffffff 00000000
extern int pCurrline[DATA_SIZE];	
extern int voltageOrigiChart[DATA_SIZE];
extern int currentOrigiChart[DATA_SIZE];

typedef struct{
int VoltageFilter ;				
int CurrentFilter ;				
}FilterValue_s;
extern FilterValue_s FilterValue;

void bufferSampling(void) ;

void displayVolChart(void);

void displayCurChart(void);

void delayShowPlot(uint16_t delaytime);
 
void chartvolProcessing( int pData[],uint8_t Ob_chart) 	;
void chartcurrProcessing(int pData[],uint8_t Ob_chart) 	;

 
//extern const uint8_t BackGround[33984] ;
void lcdPlotRemap_cur(int *intput_chart, int *output_chart,int range);	
void lcdPlotRemap_vol(int *intput_chart, int *output_chart,int range);

void additionalData_vol(int *chartdata,int input,int *input_filter); 
void additionalData_cur(int *chartdata,int input,int *input_filter); 
#endif /* __CHART_H__ */
