#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "OS.h"
#include "Semaphore.h"

void OS_CreateBinarySimaphore (OS_BinarySemaphore *Semaphore)
{
	*Semaphore=0;
}

void OS_Wait(OS_BinarySemaphore *Semphore)
{
	/*Disable interrupts*/
  __disable_irq();
  while((*Semphore) == 0)
	{
		/*If semaphore is 0 allow other tasks to preempt this task
		  and halt this task until it becomes available         */
    __enable_irq();
    /*Allow interrupts in this period*/		
    __disable_irq();
  }
  (*Semphore) = (*Semphore) - 1;
	/*Enable interrupts*/
  __enable_irq();
}

void OS_Signal(OS_BinarySemaphore *Semphore)
{
	/*Disable interrupts*/
  __disable_irq();
  (*Semphore) = (*Semphore) + 1;
	/*Enable interrupts*/
  __enable_irq();
}

