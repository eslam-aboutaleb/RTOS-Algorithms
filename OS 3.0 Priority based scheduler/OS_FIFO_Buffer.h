/* ////////////////////////////////////////////////////////////////////////////
/ Author :Eslam Aboutaleb
/
/
/////////////////////////////////////////////////////////////////////////// */

#ifndef OS_FIFO_BUFFER_H
#define OS_FIFO_BUFFER_H

#define FIFO_BUF_SIZE  20

void OS_FIFO_Init(void);
void OS_FIFO_Write(uint32 Data);
uint32 OS_FIFO_Read(void);

#endif 

