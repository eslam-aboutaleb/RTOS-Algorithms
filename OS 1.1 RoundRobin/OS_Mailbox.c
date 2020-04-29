#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "Semaphore.h"

OS_BinarySemaphore SendBusy=0;
uint32 Mail;


uint8 OS_SendMail(uint32 Data)
{
	if(SendBusy==1)
	{
		return 0; /*Failed to send there still data that is unread*/
	}
	else
	{
		Mail=Data;
		OS_Signal(&SendBusy);
	}
	return 1;
}


uint32 OS_RecieveMail(void)
{
	OS_Wait(&SendBusy);
	return Mail;
}

