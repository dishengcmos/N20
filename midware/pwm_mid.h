

#ifndef _PWM_MID_H_
#define _PWM_MID_H_


#include "stdint.h" 

void ledInit(void);


void pwmStart(void);
void compareSet(uint16_t u16CHxACompare);






#endif 
