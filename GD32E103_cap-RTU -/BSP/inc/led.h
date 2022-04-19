#ifndef _LED_H_
#define _LED_H_

#include "gd32e10x.h"
//#include  <includes.h>

void LED_Init(void);
void LED_Open(void);
void LED_Close(void);

//extern OS_TMR TMR_LED_Short_Light;
//extern OS_TMR TMR_LED_Double_Light;

extern unsigned long ulLedStatue;
void LED_Double_Light(void* parameter);
void LED_Short_Light(void* parameter);


#endif


