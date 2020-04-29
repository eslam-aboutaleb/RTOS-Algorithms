#ifndef OS_H
#define OS_H

#define OS_NUMBER_OF_TASKS     3
#define STACKSIZE              100


void OS_TaskCreate(void (*Task) (void));
void OS_Start(uint32 TickTime);
void OS_TaskYield(void);

#endif /*OS_H*/
