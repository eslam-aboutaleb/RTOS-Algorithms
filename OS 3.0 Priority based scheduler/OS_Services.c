/* ////////////////////////////////////////////////////////////////////////////
/ Author :Eslam Aboutaleb
/
/
/////////////////////////////////////////////////////////////////////////// */

#include <string.h>
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "SysTick_interface.h"
#include "NVIC_interface.h"
#include "Timer_interface.h"
#include "OS.h"
#include "OS_Services.h"

extern TCB_TypeDef *CurrentPtr;

extern TCB_TypeDef TCBs[OS_NUMBER_OF_TASKS];

/*Local functions prototypes*/
static void OS_CheckTaskDelay(void);


void OS_TaskYield(void)
{
	/*This to set Systick exception to give up the CPU to other task*/
	SET_BIT(SCB->ICSR,26);
}	

void OS_TaskDelay(uint32 DelayInMS)
{
	/*Put in the current TCB delay value*/
	CurrentPtr->Delayed=DelayInMS;
}


static void OS_CheckTaskDelay(void)
{
	uint32 Index=0;
	/*Decrease Delayed value of each task if is it bigger than 0*/
	for(Index=0;Index<OS_NUMBER_OF_TASKS;Index++)
	{
		if(TCBs[Index].Delayed > 0)
		{
			TCBs[Index].Delayed--;
		}
	}
}


void TIM1_UP_TIM10_IRQHandler (void)
{
	CLEAR_BIT(TIMER1->SR,0);
	/*Call Task delay routine*/
	OS_CheckTaskDelay();
} 


