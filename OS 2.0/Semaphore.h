#ifndef SEMAPHORE_H
#define SEMAPHORE_H

typedef sint32  OS_Semaphore;

void OS_SemaphoreInit(OS_Semaphore *Semaphore,uint8 Value);
void OS_CreateBinarySemaphore (OS_Semaphore *Semaphore);
void OS_Wait(OS_Semaphore *Semphore);
void OS_Signal(OS_Semaphore *Semphore);

#endif /*SEMAPHORE_H*/

