#ifndef SEMAPHORE_H
#define SEMAPHORE_H

typedef uint8  OS_BinarySemaphore;

void OS_CreateBinarySimaphore (OS_BinarySemaphore *Semaphore);
void OS_Wait(OS_BinarySemaphore *Semphore);
void OS_Signal(OS_BinarySemaphore *Semphore);

#endif /*SEMAPHORE_H*/

