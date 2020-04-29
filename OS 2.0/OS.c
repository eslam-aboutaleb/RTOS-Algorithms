#include <string.h>
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "SysTick_interface.h"
#include "NVIC_interface.h"
#include "Timer_interface.h"
#include "SCB_interface.h"
#include "Semaphore.h"
#include "OS.h"

#define F_CPU 16000000UL

uint32 MILLIS_PRESCALER=(F_CPU/1000);
uint32 Task_Delay_Tick_ms =1;

/*Local Functions Prototypes*/
static uint8 OS_InitTasks(void (*Task[OS_NUMBER_OF_TASKS])(void));



/*Functions defined in assembly files*/
void OS_SchedulerLaunch (void);

/*Array as TCBs for tasks*/
TCB_TypeDef TCBs[OS_NUMBER_OF_TASKS];

TCB_TypeDef *CurrentPtr;

/*Two dimensional array as stack*/
uint32 TCBs_Stack[OS_NUMBER_OF_TASKS][STACKSIZE];

void OS_TaskDelayTimer_Init(void)
{
	NVIC_Init();
	NVIC_EnableInterrupt(TIM1_UP_TIM10_IRQn);
	/*Set Timer to the lowest priority in NVIC*/
	NVIC_SetPeriority(TIM1_UP_TIM10_IRQn,0xF0);
	TIM_ConfigType Timer;
	memset(&Timer,0,sizeof(Timer));
	/*Enable Timer*/
	Timer.Counter_State=Enable_CNT;
	/*Initialize period value */
	Timer.PeriodVal=Task_Delay_Tick_ms;
	/*Initialize prescaler*/
	Timer.PrescalerVal=MILLIS_PRESCALER;
	/*Initialize Timer as Down counter*/
	Timer.Direction_State=UpCounter;
	/*Enable Timer interrupt*/
	Timer.UDI_State=Enable_TIM_Update_Interrupts;
	/*Initialize Timer*/
	TIM_Init(TIMER1,&Timer);
}

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

void OS_TaskCreate(void (*Task) (void))                
{                                                      
	static uint32 TaskIndex=0;                           
	TasksPtr[TaskIndex]=Task;                            
	TaskIndex++;                                         
}                                                      

static uint8 OS_InitTasks(void (*Task[OS_NUMBER_OF_TASKS])(void))
{
	uint16 index=0;
	/*Disable interrupts as it is critical section*/
	__disable_irq();
	
	/*Make circular linked list as each NextPtr of every task control 
	  block will point to the next TCB then the last NextPtr will point
	  to the first TCB                                                     */
	for(index=0;index<OS_NUMBER_OF_TASKS;index++)
	{
		TCBs[index].NextPtr=&TCBs[index+1];
	}
	
	index--;
	TCBs[index].NextPtr=&TCBs[0];
	
	/*Make PC reg points to the thread*/
	for(index=0;index<OS_NUMBER_OF_TASKS;index++)
	{
		OS_StackInit(index);
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
	/*Choose clock source*/
	SysTick_Init(AHB_DIVIDED_BY_8,ENABLE_SYSTICK_EXCEPTION);
	/*Initialize tasks*/
	OS_InitTasks(TasksPtr);
	/*Put Systick exeption as lowest priority 7*/
	SHRP3 = (SHRP3 & 0x00FFFFFF) | 0xE0000000; 
	/*Initialize task delay Timer*/
	OS_TaskDelayTimer_Init();
  /*Start SysTick */
	SysTick_Start(((TickTime* MILLIS_PRESCALER)-1),0);

	OS_SchedulerLaunch();
}

void Scheduler(void)
{
	/*Round Robin Scheduler*/
	CurrentPtr=CurrentPtr->NextPtr;
	/*Skip Blocked tasks*/
	while(CurrentPtr->Blocked !=0 || CurrentPtr->Delayed !=0)
	{
		CurrentPtr=CurrentPtr->NextPtr;
	}
}

