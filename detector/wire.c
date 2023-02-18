
#include "wire.h"
#include "cachevalue.h"
#include "flash_mid.h"

//WireRes_e WireRes;
//VolCurwire_s VolCurwire;
 
void batteryWire1Obvious(void)
{
	VoltageNO1_sb[8]=CHAR_BATTER_UP1; 
	VoltageNO1_sb[9]=CHAR_BATTER_UP2; 
	VoltageNO1_sb[10]=CHAR_BATTER_UP3;
	VoltageNO1_sb[11]=CHAR_BATTER_UP4;
	VoltageNO1_sb[12]=CHAR_BATTER_UP5;
	VoltageNO1_sb[13]=CHAR_BATTER_UP6;
	VoltageNO1_sb[14]=CHAR_BATTER_UP7;
	
	CurrentNO1_sb[8]=CHAR_BATTER_DOW1; 
	CurrentNO1_sb[9]=CHAR_BATTER_DOW2; 
	CurrentNO1_sb[10]=CHAR_BATTER_DOW3;
	CurrentNO1_sb[11]=CHAR_BATTER_DOW4;
	CurrentNO1_sb[12]=CHAR_BATTER_DOW5;
	CurrentNO1_sb[13]=CHAR_BATTER_DOW6;
	CurrentNO1_sb[14]=CHAR_BATTER_DOW7;
}

void batteryWire1Hidden(void)
{	
	VoltageNO1_sb[8]=CHAR_NON;
	VoltageNO1_sb[9]=CHAR_NON;
	VoltageNO1_sb[10]=CHAR_NON;
	VoltageNO1_sb[11]=CHAR_NON;
	VoltageNO1_sb[12]=CHAR_NON;
	VoltageNO1_sb[13]=CHAR_NON;
	VoltageNO1_sb[14]=CHAR_NON;
	VoltageNO1_sb[15]=CHAR_NON;
	
	CurrentNO1_sb[8]=CHAR_NON;
	CurrentNO1_sb[9]=CHAR_NON;
	CurrentNO1_sb[10]=CHAR_NON;
	CurrentNO1_sb[11]=CHAR_NON;
	CurrentNO1_sb[12]=CHAR_NON;
	CurrentNO1_sb[13]=CHAR_NON;
	CurrentNO1_sb[14]=CHAR_NON;
	CurrentNO1_sb[15]=CHAR_NON;
}

void batteryWire1Twinkle(void)
{
	static uint8_t blingcount=0;
	blingcount++;		
	if(blingcount>=18)	
		blingcount=0;	
						
	if(blingcount>9)	
		batteryWire1Obvious();
	else
		batteryWire1Hidden();
}
								
//这里的图像是和上面的一样的 ,图像需要修改
void BatteryWire2Obvious(void)	
{
	VoltageNO2_sb[8]=CHAR_BATTERWIRE_UP1; 
	VoltageNO2_sb[9]=CHAR_BATTERWIRE_UP2; 
	VoltageNO2_sb[10]=CHAR_BATTERWIRE_UP3;
	VoltageNO2_sb[11]=CHAR_BATTERWIRE_UP4;
	VoltageNO2_sb[12]=CHAR_BATTERWIRE_UP5;
	VoltageNO2_sb[13]=CHAR_BATTERWIRE_UP6;
	VoltageNO2_sb[14]=CHAR_BATTERWIRE_UP7;
	
	CurrentNO2_sb[8]=CHAR_BATTERWIRE_DOW1; 
	CurrentNO2_sb[9]=CHAR_BATTERWIRE_DOW2; 
	CurrentNO2_sb[10]=CHAR_BATTERWIRE_DOW3;
	CurrentNO2_sb[11]=CHAR_BATTERWIRE_DOW4;
	CurrentNO2_sb[12]=CHAR_BATTERWIRE_DOW5;
	CurrentNO2_sb[13]=CHAR_BATTERWIRE_DOW6;
	CurrentNO2_sb[14]=CHAR_BATTERWIRE_DOW7;
}
	
void BatteryWire2Hidden(void)
{
	VoltageNO2_sb[8]=CHAR_NON;
	VoltageNO2_sb[9]=CHAR_NON;
	VoltageNO2_sb[10]=CHAR_NON;
	VoltageNO2_sb[11]=CHAR_NON;
	VoltageNO2_sb[12]=CHAR_NON;
	VoltageNO2_sb[13]=CHAR_NON;
	VoltageNO2_sb[14]=CHAR_NON;
	VoltageNO2_sb[15]=CHAR_NON;
	
	CurrentNO2_sb[8]=CHAR_NON;
	CurrentNO2_sb[9]=CHAR_NON;
	CurrentNO2_sb[10]=CHAR_NON;
	CurrentNO2_sb[11]=CHAR_NON;
	CurrentNO2_sb[12]=CHAR_NON;
	CurrentNO2_sb[13]=CHAR_NON;
	CurrentNO2_sb[14]=CHAR_NON;
	CurrentNO2_sb[15]=CHAR_NON;
}
void BatteryWire2Twinkle(void)
{
	static uint8_t blingcount=0;
	blingcount++;		
	if(blingcount>=18)	
		blingcount=0;	
						
	if(blingcount>9)	
		BatteryWire2Obvious();
	else
		BatteryWire2Hidden();
}
void errorHidden(void)
{
	Error_sb[2]=CHAR_NON;
	Error_sb[3]=CHAR_NON;
	Error_sb[4]=CHAR_NON;
	Error_sb[5]=CHAR_NON;
	
 	Error_sb[7]=CHAR_NON;
	Error_sb[8]=CHAR_NON;
	Error_sb[9]=CHAR_NON;
	Error_sb[10]=CHAR_NON;
	Error_sb[11]=CHAR_NON;
	
	Error_sb[12]=CHAR_NON;
}

void errorObvious(void)
{
	Error_sb[2]=CHAR_T;
	Error_sb[3]=CHAR_e;
	Error_sb[4]=CHAR_s;
	Error_sb[5]=CHAR_t;

	Error_sb[7]=CHAR_E;
	Error_sb[8]=CHAR_r;
	Error_sb[9]=CHAR_r;
	Error_sb[10]=CHAR_o;
	Error_sb[11]=CHAR_r;
	
	Error_sb[12]=CHAR_Exclam;
	
}
void obVolCurrwithoutWire(void)
{	//23.56
	VoltageNO1_sb[2]=(Voltage_u32/10000)%10;
	VoltageNO1_sb[3]=(Voltage_u32/1000)%10;
	VoltageNO1_sb[5]=(Voltage_u32/100)%10;
	VoltageNO1_sb[6]=(Voltage_u32/10)%10; 
	//2.456
	CurrentNO1_sb[2]=(Current_u32/1000)%10;
	CurrentNO1_sb[4]=(Current_u32/100)%10;
	CurrentNO1_sb[5]=(Current_u32/10)%10;
	CurrentNO1_sb[6]=(Current_u32)%10;  
	flash_Cali.flash_Cali_s.VolCurwire_s.Vol_withoutwire = Voltage_ldoub;
	flash_Cali.flash_Cali_s.VolCurwire_s.Cur_withoutwire = Current_ldoub;
}

void obVolCurrwithoutWire_Inflash(void)
{
	static uint32_t Vol_u32,Cur_u32;
	
	Vol_u32=flash_Cali.flash_Cali_s.VolCurwire_s.Vol_withoutwire ;
	Cur_u32=flash_Cali.flash_Cali_s.VolCurwire_s.Cur_withoutwire ;
	
	VoltageNO1_sb[2]=(Vol_u32/10000)%10;
	VoltageNO1_sb[3]=(Vol_u32/1000)%10;
	VoltageNO1_sb[5]=(Vol_u32/100)%10;
	VoltageNO1_sb[6]=(Vol_u32/10)%10; 
	//2.456
	CurrentNO1_sb[2]=(Cur_u32/1000)%10;
	CurrentNO1_sb[4]=(Cur_u32/100)%10;
	CurrentNO1_sb[5]=(Cur_u32/10)%10;
	CurrentNO1_sb[6]=(Cur_u32)%10; 
}

void obVolCurrwithWire(void)
{
	VoltageNO2_sb[2]=(Voltage_u32/10000)%10;
	VoltageNO2_sb[3]=(Voltage_u32/1000)%10;
	VoltageNO2_sb[5]=(Voltage_u32/100)%10;
	VoltageNO2_sb[6]=(Voltage_u32/10)%10; 
	//2.456	
	CurrentNO2_sb[2]=(Current_u32/1000)%10;
	CurrentNO2_sb[4]=(Current_u32/100)%10;
	CurrentNO2_sb[5]=(Current_u32/10)%10;
	CurrentNO2_sb[6]=(Current_u32)%10; 
			
	flash_Cali.flash_Cali_s.VolCurwire_s.Vol_withwire = Voltage_ldoub;
	flash_Cali.flash_Cali_s.VolCurwire_s.Cur_withwire = Current_ldoub;
}

void obResisWire(void)
{	
	uint32_t Resiswire_u32;
	Resiswire_u32=flash_Cali.flash_Cali_s.VolCurwire_s.Resis_wire;
	
	Resistan_sb[5]=(Resiswire_u32/10000)%10;
	Resistan_sb[6]=(Resiswire_u32/1000)%10;
	//Resistan_sb[7]
	Resistan_sb[8]=(Resiswire_u32/100)%10;
	Resistan_sb[9]=(Resiswire_u32/10)%10; 
	Resistan_sb[10]=(Resiswire_u32/1)%10; 
}

void obResisWireZero(void)
{
	Resistan_sb[5]=0;
	Resistan_sb[6]=0; 
	Resistan_sb[8]=0;
	Resistan_sb[9]=0;
	Resistan_sb[10]=0;
}

void displaybatteryWire(void)	
{	
	switch(flash_Cali.flash_Cali_s.WireRes_e)
	{
		case withoutWireData:  
			batteryWire1Twinkle();
			BatteryWire2Obvious();
			obVolCurrwithoutWire();
			errorHidden();
			obResisWireZero();
		break;
		case withWireData:  
			batteryWire1Obvious();
			BatteryWire2Twinkle();
			obVolCurrwithoutWire_Inflash();
			obVolCurrwithWire();  
			errorHidden();  
			obResisWireZero();

		break;
		case obviousResults:   
			batteryWire1Obvious();
			BatteryWire2Obvious();
			errorHidden();  
			obResisWire();
		break;
		case obviousError:   
			batteryWire1Obvious();
			BatteryWire2Obvious();
			errorObvious();  
			obResisWireZero(); 
			 
		break;
	} 
}
