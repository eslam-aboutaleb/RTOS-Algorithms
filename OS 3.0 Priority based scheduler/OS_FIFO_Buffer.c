/* ////////////////////////////////////////////////////////////////////////////
/ Author :Eslam Aboutaleb
/
/
/////////////////////////////////////////////////////////////////////////// */

#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "OS.h"
#include "Semaphore.h"
#include "OS_FIFO_Buffer.h"

OS_Semaphore FIFO_Mutex; /*Used as mutex between Send and Get*/
OS_Semaphore Available_Space; /*Used to indicate how much storage is left*/
OS_Semaphore CurDataSize; /*Used to indicate the current size of the data*/

uint32 FIFO_BUFFER[FIFO_BUF_SIZE];

volatile uint32 *ReadPtr;
volatile uint32 *WritePtr;


void OS_FIFO_Init(void)
{
	/*Make both pointers point to first address of
	  Buffer means that it is empty                      */ 
	ReadPtr=WritePtr=&FIFO_BUFFER[0];
	/*Initialize available space as equal to buffer size */
	OS_SemaphoreInit(&Available_Space,FIFO_BUF_SIZE);
	/*Initialize Current data size as 0 (No data)        */
	OS_SemaphoreInit(&CurDataSize,0);
	/*Initialize FIFO mutex as 1                         */
	OS_SemaphoreInit(&FIFO_Mutex,1);
}

void OS_FIFO_Write(uint32 Data)
{
	/*Decrease one from this semaphore to indicate there is data*/
	OS_Wait(&Available_Space);
  /*Take mutex*/
	OS_Wait(&FIFO_Mutex);

	*WritePtr=Data; /*Assign data to WritePtr*/
	WritePtr++;     /*Move to next space */
	
	/*Check if ptr has reached end of the buffer
	 if it is wrap it to the begining of buffer */
	if(WritePtr==&FIFO_BUFFER[FIFO_BUF_SIZE])
	{
		WritePtr=&FIFO_BUFFER[0];
	}
	
	/*Free mutex*/
	OS_Signal(&FIFO_Mutex);
	/*Add one to this semaphore to indicate that buffer increased*/
	OS_Signal(&CurDataSize);
}

uint32 OS_FIFO_Read(void)
{	
	uint32 ReadBuf;
	/*Decrease this semaphore 1 to indicate the space has decreased*/
	OS_Wait(&CurDataSize);
	/*Take the mutex*/
	OS_Wait(&FIFO_Mutex);
	
	ReadBuf=*ReadPtr;
	ReadPtr++;  /*Move to next space */
	
	/*Check if ptr has reached end of the buffer
	 if it is wrap it to the begining of buffer */
	if(ReadPtr==&FIFO_BUFFER[FIFO_BUF_SIZE])
	{
		ReadPtr=&FIFO_BUFFER[0];
	}
	OS_Signal(&FIFO_Mutex);
	OS_Signal(&Available_Space);
	
	return ReadBuf;
}

