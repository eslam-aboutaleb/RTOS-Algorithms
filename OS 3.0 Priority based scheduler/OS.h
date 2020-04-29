/* ////////////////////////////////////////////////////////////////////////////
/ Author :Eslam Aboutaleb
/
/
/////////////////////////////////////////////////////////////////////////// */

#ifndef OS_H
#define OS_H

#include "OS_Services.h"

#define OS_NUMBER_OF_TASKS     3
#define STACKSIZE              100

/*Task control block*/
struct TCB
{
	uint32 *StackPtr; /*Stack pointer*/
	struct TCB *NextPtr; /*Pointer to the next TCB*/
	uint32 Blocked; /*If its value != 0 means the current task is blocked*/
	uint32 Delayed; /*Delay can be from 0 to 2^32-1 ms*/
	uint8  Priority; /*Priority levels from 0 to 255 -> 0 is the highest*/
};

typedef struct TCB TCB_TypeDef;

void OS_TaskCreate(void (*Task) (void) ,uint8 Priority);             
void OS_Start(uint32 TickTime);


#endif /*OS_H*/

