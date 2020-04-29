/* ////////////////////////////////////////////////////////////////////////////
/ Author :Eslam Aboutaleb
/
/
/////////////////////////////////////////////////////////////////////////// */

#include <string.h>
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "SysTick_interface.h"
#include "SCB_interface.h"
#include "Semaphore.h"
#include "OS_Portable.h"
#include "OS.h"

uint32 MILLIS_PRESCALER=(F_CPU/1000);

/*Local Functions Prototypes*/
static uint8 OS_InitTasks(void (*Task[OS_NUMBER_OF_TASKS])(void));



/*Functions defined in assembly files*/
void OS_SchedulerLaunch (void);

/*Array as TCBs for tasks*/
TCB_TypeDef TCBs[OS_NUMBER_OF_TASKS];
/*Pointer to current running task*/
TCB_TypeDef *CurrentPtr;
/*Two dimensional array as stack*/
uint32 TCBs_Stack[OS_NUMBER_OF_TASKS][STACKSIZE];



void OS_StackInit(uint16 TaskNUM)                                                          
{                                                                                          
	/*Stack pointer points to the top of the stack*/                                         
	 TCBs[TaskNUM].StackPtr = &TCBs_Stack[TaskNUM][STACKSIZE - 16]; /* Minus (16 Regs)*/     
	                                                                                         
	/*Set T bit in XPR register to turn on thumb mode*/                                      
	 TCBs_Stack[TaskNUM][STACKSIZE-1]  = 0x01000000; /* Thumb bit in PSR */  
  /*Initial values of Regs dont matter but to  help in debugging*/
   TCBs_Stack[TaskNUM][STACKSIZE-3]  = 0x14141414; /* R14              */
   TCBs_Stack[TaskNUM][STACKSIZE-4]  = 0x12121212; /* R12              */
   TCBs_Stack[TaskNUM][STACKSIZE-5]  = 0x03030303; /* R3               */
   TCBs_Stack[TaskNUM][STACKSIZE-6]  = 0x02020202; /* R2               */
   TCBs_Stack[TaskNUM][STACKSIZE-7]  = 0x01010101; /* R1               */
   TCBs_Stack[TaskNUM][STACKSIZE-8]  = 0x00000000; /* R0               */
   TCBs_Stack[TaskNUM][STACKSIZE-9]  = 0x11111111; /* R11              */
   TCBs_Stack[TaskNUM][STACKSIZE-10] = 0x10101010; /* R10              */
   TCBs_Stack[TaskNUM][STACKSIZE-11] = 0x09090909; /* R9               */
   TCBs_Stack[TaskNUM][STACKSIZE-12] = 0x08080808; /* R8               */
   TCBs_Stack[TaskNUM][STACKSIZE-13] = 0x07070707; /* R7               */
   TCBs_Stack[TaskNUM][STACKSIZE-14] = 0x06060606; /* R6               */
   TCBs_Stack[TaskNUM][STACKSIZE-15] = 0x05050505; /* R5               */
   TCBs_Stack[TaskNUM][STACKSIZE-16] = 0x04040404; /* R4	             */
}                                                                                          
																																												                                                                                     
                                                                                        
/*Static global Array of functions pointer*/           
static void (*TasksPtr[OS_NUMBER_OF_TASKS]) (void);

/*Static global array as priroty levels for OS*/
static uint8 Priorities[255];


void OS_TaskCreate(void (*Task) (void) ,uint8 Priority)                
{                                                      
	static uint32 TaskIndex=0;
  static uint8 priorty_Index=0;	
	/*Put the task as its index in array of function pointers*/
	TasksPtr[TaskIndex]=Task;                            
	TaskIndex++;
   /*put the priority level as its index in priorities array*/
  Priorities[priorty_Index]=Priority;
  priorty_Index++;
}                                                      

static uint8 OS_InitTasks(void (*Task[OS_NUMBER_OF_TASKS])(void))
{
	uint16 index=0;
	/*Disable interrupts as it is critical section*/
	__disable_irq();
	
	for(index=0;index<OS_NUMBER_OF_TASKS;index++)
	{
		/*Make circular linked list as each NextPtr of every task control 
	  block will point to the next TCB then the last NextPtr will point
	  to the first TCB                                                     */
		TCBs[index].NextPtr=&TCBs[index+1];
		/*Assign each task to its priority*/
		TCBs[index].Priority=Priorities[index];
	}
	/*To make circular linked list point the last task to the first one*/
	index--;
	TCBs[index].NextPtr=&TCBs[0];
	
	
	for(index=0;index<OS_NUMBER_OF_TASKS;index++)
	{
		/*Initalize the stack of each TCB*/
		OS_StackInit(index);
		/*Make PC reg points to the thread*/
		TCBs_Stack[index][STACKSIZE-2]=(uint32)(Task[index]);
	}
	
	/*Points to the head of linked list, Task 0 will run first*/
	CurrentPtr=&TCBs[0];
	
	/*Enable interrupts*/
	__enable_irq();

	return 1; /*Succesful */
}

void OS_Start(uint32 TickTime)
{
	/*First stop the SysTick */
	SysTick_Stop();
	/*Choose clock source & Enable systick exception*/
	SysTick_Init(AHB_DIVIDED_BY_8,ENABLE_SYSTICK_EXCEPTION);
	/*Initialize tasks*/
	OS_InitTasks(TasksPtr);
	/*Put Systick exeption as lowest priority 7*/
	SHRP3 = (SHRP3 & 0x00FFFFFF) | 0xE0000000; 
	/*Initialize task delay Timer*/
	OS_TaskDelayTimer_Init();
  /*Start SysTick */
	SysTick_Start(((TickTime* MILLIS_PRESCALER)-1),0);
	/*Start the scheduler*/
	OS_SchedulerLaunch();
}

void Scheduler(void)
{
	/*Lowest priority level*/
	uint32 Lowst_Priority=255;
	/*Temp pointer to TCB*/
	TCB_TypeDef *Temp;
	/*Pointer to determine which task will run*/
	TCB_TypeDef *Highest_Task;
	
	Temp = CurrentPtr;
	
	do
	{
		 /*Go to the next TCB*/
	   Temp=Temp->NextPtr;
	   /*Skip Blocked tasks & Delayed tasks & choose the highest priorities*/
	   if((Temp->Blocked ==0) && (Temp->Delayed ==0) && (Temp->Priority < Lowst_Priority))
	   {
			 /*If the Current priority is the smaller (Smallest num = Highest priority)
			   Assgin it as Lowest prioity and if there smaller one next iteration it 
			   will be smaller until it reaches the smallest number (highest priority) */
			 Lowst_Priority = Temp->Priority;
			 Highest_Task=Temp;
	   }
	 
	}while(CurrentPtr != Temp); /*Traverse all tasks but the current task*/
	
	CurrentPtr = Highest_Task;
}

