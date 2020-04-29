#ifndef OS_H
#define OS_H

#include "OS_Services.h"

#define OS_NUMBER_OF_TASKS     3
#define STACKSIZE              100

/*Task control block*/
struct TCB
{
	uint32 *StackPtr;
	struct TCB *NextPtr;
	uint32 Blocked;
	uint32 Delayed;
};

typedef struct TCB TCB_TypeDef;

void OS_TaskCreate(void (*Task) (void));
void OS_Start(uint32 TickTime);


#endif /*OS_H*/

