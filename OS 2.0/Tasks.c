#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "Semaphore.h"
#include "Tasks.h"
#include "OS_Mailbox.h"
#include "OS_FIFO_Buffer.h"
#include "OS.h"
uint32 count0,count1,count2;

extern OS_Semaphore Semaphore;

void Task0(void)
{
	uint8 x=0;
	while(1)
	{
		count0++;
		x++;
		OS_TaskDelay(1000);
	}
}


void Task1(void)
{
	uint32 y=0;
	while(1)
	{
		count1++;
	}
}


void Task2(void)
{
	while(1)
	{
		count2++;
	}
}

