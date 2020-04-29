#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "GPIO_interface.h"
#include "Led_interface.h"
#include "Led_Port.h"


void LED_Init(tLED led, tLED_State state)
{
    /* Configure led pins as output */
    switch (led)
    {
        case LED_1:
            GPIO_InitPortPin(LED_1_PORT_CR, LED_1_PIN, GPIO_OUT);
            break;
				case LED_2:
				    GPIO_InitPortPin(LED_2_PORT_CR, LED_2_PIN, GPIO_OUT);
				break;
        default:
            /* No Thing */
            break;
    }
    /* Set led state */
    LED_SetState(led, state);
}

void LED_Toggle(tLED led)
{
    /* Toggle led */
    if (LED_GetState(led) == LED_ON)
    {
        LED_SetState(led, LED_OFF);
    } else
    {
        LED_SetState(led, LED_ON);
    }

}


void LED_SetState(tLED led, tLED_State state)
{
    /* Set led state */
    switch (led)
    {
        case LED_1:
            GPIO_WritePortPin(LED_1_PORT_DR, LED_1_PIN, state);
            break;
        default:
            /* No Thing */
            break;
    }
}

tLED_State LED_GetState(tLED led)
{
    tLED_State ret = LED_OFF;

    /* Set led state */
    switch (led)
    {
        case LED_1:
            ret = GPIO_ReadPortPin(LED_1_PORT_DR, LED_1_PIN);
            break;
        default:
            /* No Thing */
            break;
    }
    return ret;
}

