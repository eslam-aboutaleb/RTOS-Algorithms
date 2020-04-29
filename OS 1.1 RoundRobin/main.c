#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "OS.h"
#include "Semaphore.h"
#include "Tasks.h"


OS_BinarySemaphore Semaphore;

int main(void)
{
	OS_TaskCreate(Task0);
	OS_TaskCreate(Task1);
	OS_TaskCreate(Task2);
	OS_CreateBinarySimaphore(&Semaphore);
	OS_Start(10);
	while(1);
}

