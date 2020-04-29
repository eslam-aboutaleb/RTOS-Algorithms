#ifndef LED_INTERFACE_H
#define LED_INTERFACE_H

#include "Led_Port.h"

typedef enum
{
    LED_1,
	LED_2
}tLED;

typedef enum
{
   LED_OFF = 0,
   LED_ON  = 1
}tLED_State;


void LED_Init(tLED led, tLED_State state);
void LED_SetState(tLED led, tLED_State state);
void LED_Toggle(tLED led);
tLED_State LED_GetState(tLED led);

#endif /*LED_INTERFACE_H*/

