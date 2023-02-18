

#ifndef __WIRE_H__
#define __WIRE_H__


//typedef enum{
//	withoutWireData,
//	withWireData,
//	obviousResults,
//	obviousError,
//}WireRes_e;
//extern WireRes_e WireRes; 


//typedef struct{
//long double	Vol_withoutwire;
//long double Cur_withoutwire;
//long double Vol_withwire;
//long double Cur_withwire;
//long double Resis_wire;
//}VolCurwire_s;
//extern VolCurwire_s VolCurwire;


void displaybatteryWire(void);
void obVolCurrwithoutWire_Inflash(void);
void obVolCurrwithoutWire(void);

void obVolCurrwithWire(void);

void obResisWire(void);
void obResisWireZero(void);
#endif /* __WIRE_H__ */
