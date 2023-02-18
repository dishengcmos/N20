

#include "displaydriver.h" 
#include "lcd_api.h"   
#include "dishelp.h"


void displayHelp1Interface(unsigned char pagenumber,const uint8_t Help_up_r1[],const uint8_t Help_up_r2[],const uint8_t Help_up_r3[],\
	const uint8_t Help_down_r1[],const uint8_t Help_down_r2[],const uint8_t Help_down_r3[],const uint8_t Help_down_r4[])
{
    uint8_t  arraynums_top = 0,arraynums_down = 0;
    uint8_t  bitColumn_top=0,bitColumn_down=0;	
	uint8_t  arraynums1 = 0,arraynums2 = 0,arraynums3 = 0,arraynums4 = 0,arraynums5 = 0,arraynums6 = 0,arraynums7 = 0,arraynums_non = 0;
    uint8_t  bitColumn1=0,bitColumn2=0,bitColumn3=0,bitColumn4=0,bitColumn5=0,bitColumn6=0,bitColumn7=0,bitColumn_non=0;
	
	assignSelectChannel(pagenumber); 
    for(int i=0;i<240;i++)		
    {	
		arraynums_non=0;
		bitColumn_non=0;
		//第一行
		displaySelectChannel(SelectChannel[arraynums_top],arraynums_top,bitColumn_top); 
		operationForDisplayArray_tip_highpixel( &arraynums_top,  &bitColumn_top );
 
		if(i==0)
		{
			sendAnyBlankBit(4);
			for(uint8_t n=0;n<184;n++)	
			{
				sendOneBit(0xff,0xff);
			}
			sendAnyBlankBit(4);
		}
		else if(i==1)
		{
			for(uint8_t n=0;n<192;n++)	
			{
				if((n==3)||(n==188))
					sendOneBit(0xff,0xff);
				else
					sendOneBit(0x00,0x00);
			}
		}
		else if(i==120)
		{
			sendAnyBlankBit(2);
			for(uint8_t n=0;n<188;n++)	
			{
				sendOneBit(0xff,0xff);
			}
			sendAnyBlankBit(2);
		}
		else if(i==238)
		{	
			for(uint8_t n=0;n<192;n++)	
			{
				if((n==3)||(n==188))
					sendOneBit(0xff,0xff);
				else
					sendOneBit(0x00,0x00);
			}
		}	
		else if(i==239)
		{	
			sendAnyBlankBit(4);
			for(uint8_t n=0;n<184;n++)	
			{
				sendOneBit(0xff,0xff);
			}
			sendAnyBlankBit(4);
		}		
	
		else//文字高76 宽8 12个 
		{	
				//line
				sendOneBit(0x00,0x00);	
				sendOneBit(0x00,0x00);	 
				if(2<=i&&i<=238)			
					sendOneBit(0xff,0xff);	
				else						
					sendOneBit(0x00,0x00);		
				sendOneBit(0x00,0x00);	
				/***************** line1 top ***************************/
				displayHelpchar(Help_up_r1[arraynums1],bitColumn1,CYAN); 		
 				operationHelp(&arraynums1, &bitColumn1); 						
				displayHelpchar(Help_up_r2[arraynums2],bitColumn2,CYAN); 		
 				operationHelp(&arraynums2, &bitColumn2); 	
				displayHelpchar(Help_up_r3[arraynums3],bitColumn3,CYAN); 		
 				operationHelp(&arraynums3, &bitColumn3); 
 				displayHelpchar(Help_p1r_non[arraynums_non],bitColumn_non,CYAN); 		
  				operationHelp(&arraynums_non, &bitColumn_non); 
 				displayHelpchar(Help_p1r_non[arraynums_non],bitColumn_non,CYAN); 		
  				operationHelp(&arraynums_non, &bitColumn_non); 		 
				/*****************line**************************/
				sendOneBit(0x00,0x00);
				sendOneBit(0x00,0x00);
				sendOneBit(0x00,0x00);
				sendOneBit(0x00,0x00);
				if(2<=i&&i<=238)
					sendOneBit(0xff,0xff);
				else
					sendOneBit(0x00,0x00);
				sendOneBit(0x00,0x00);
				sendOneBit(0x00,0x00);
				sendOneBit(0x00,0x00);
				/*****************end***************************/		 
				displayHelpchar(Help_down_r1[arraynums4],bitColumn4,CYAN); 		
 				operationHelp(&arraynums4, &bitColumn4); 
				displayHelpchar(Help_down_r2[arraynums5],bitColumn5,CYAN); 		
 				operationHelp(&arraynums5, &bitColumn5); 
				displayHelpchar(Help_down_r3[arraynums6],bitColumn6,CYAN); 		
 				operationHelp(&arraynums6, &bitColumn6); 
				displayHelpchar(Help_down_r4[arraynums7],bitColumn7,CYAN); 		
 				operationHelp(&arraynums7, &bitColumn7);  	
 				displayHelpchar(Help_p1r_non[arraynums_non],bitColumn_non,CYAN); 		
  				operationHelp(&arraynums_non, &bitColumn_non); 	
 				displayHelpchar(Help_p1r_non[arraynums_non],bitColumn_non,CYAN); 		
  				operationHelp(&arraynums_non, &bitColumn_non); 		
				/*****************line3 end***************************/
				sendOneBit(0x00,0x00);	
				if(2<=i&&i<=238)
					sendOneBit(0xff,0xff);
				else
					sendOneBit(0x00,0x00);
				
				sendOneBit(0x00,0x00);	
				sendOneBit(0x00,0x00);	
		}
		//第六行
		displayKepTip(KepTip[arraynums_down],arraynums_down,bitColumn_down,7,REALBLUE,REALBLUE); 	
		operationForDisplayArray_tip_highpixel( &arraynums_down,  &bitColumn_down );
    }	
}

















