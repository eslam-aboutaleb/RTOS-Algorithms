#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "OS.h"
#include "Semaphore.h"

extern TCB_TypeDef *CurrentPtr;

void OS_SemaphoreInit(OS_Semaphore *Semaphore,uint8 Value)
{
	*Semaphore=Value;
}

void OS_CreateBinarySemaphore (OS_Semaphore *Semaphore)
{
	*Semaphore=0;
}

void OS_Wait(OS_Semaphore *Semaphore)
{
	/*Disable interrupts*/
  __disable_irq();
	
	(*Semaphore) = (*Semaphore) - 1;
	
  if((*Semaphore) < 0)
	{
		/*Put this task in blocked state*/
		CurrentPtr->Blocked=(uint32)Semaphore;
		/*Enable Interrupts*/
    __enable_irq();
    /*Force SysTick interrupt*/
    OS_TaskYield();		
  }
  
	/*Enable interrupts*/
  __enable_irq();
}

void OS_Signal(OS_Semaphore *Semaphore)
{
	TCB_TypeDef *Temp;
	/*Disable interrupts*/
  __disable_irq();
  (*Semaphore) = (*Semaphore) + 1;
	if(*Semaphore<=0)
	{
		Temp=CurrentPtr->NextPtr;
		/*Traverse the linked list*/
		while(Temp->Blocked!=(uint32)Semaphore)
		{
			Temp=Temp->NextPtr;
		}
		/*Wake up this task*/
		Temp->Blocked=0;
	}
	/*Enable interrupts*/
  __enable_irq();
}

