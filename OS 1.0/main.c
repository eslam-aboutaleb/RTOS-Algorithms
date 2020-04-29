#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "Tasks.h"
#include "OS.h"


int main(void)
{
	OS_TaskCreate(Task0);
	OS_TaskCreate(Task1);
	OS_TaskCreate(Task2);
	OS_Start(10);
	while(1);
}

