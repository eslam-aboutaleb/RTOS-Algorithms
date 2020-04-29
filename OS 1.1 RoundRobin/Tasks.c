#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "Semaphore.h"
#include "Tasks.h"
#include "OS_Mailbox.h"

uint32 count0,count1,count2;

extern OS_BinarySemaphore Semaphore;

void Task0(void)
{
	uint8 x=0;
	while(1)
	{
		count0++;
		OS_SendMail((uint8)x);
		x++;
	}
	
}


void Task1(void)
{
	uint32 y=0;
	while(1)
	{
		count1++;
		y=OS_RecieveMail();
	}
	
}


void Task2(void)
{
	while(1)
	{
		count2++;
	}
	
}

