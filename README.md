# RTOS-Algorithms
RTOS compatible with ARM processors (Round Robin, Priority based schedulers , Semaphores, Queues, OS services)

Implementation of different algorithims for real time operating systems.

1- OS 1.0 its scheduler all in assembly and work with equal time slice for every task and the implementation of task control block
   based on circular linked list.
   : -To use this OS First you have to define number of tasks in OS.h file
     -Use OS_TaskCreate to create tasks just that without priorities or periods
     -OS_Start to start OS with desired time tick
     

2- OS 1.1 its scheduler is the same but in C and thread switching in assembly and imported this C function in assembly code.
   :This allows me in future updates to modify this function
   -Added Spin lock semaphore.
   -Added Mail box.
   -Added OS services like OS_TaskYield
   
   : -To use this OS First you have to define number of tasks in OS.h file
     -Use OS_TaskCreate to create tasks just that without priorities or periods
     -OS_Start to start OS with desired time tick
   
   
3- OS 2.0 its scheduler skips blocked and delayed tasks 
   -Added delayed section in task control block
   -Added Blocked section in task control block
   -Implement Cooperative semaphores
   -Blocked section is decreased by 1 every semaphore (OS_Wait)
   -Added FIFO Buffer queue
   -Added OS serives OS_TaskDelay
   
 
4- OS 3.0 its scheduler is priority based scheduler decides which task to run if itsn't blocked or delayed and highest priority.
   -Added priority section in task control block
   -Moved portable files into one file OS_Portable so the user can use it on any ARM MCU, just make some changes in that file.
   
   
  Refrences: UC/OS II for ARM files and Jonathan Valvano's book: Real-Time Operating Systems for Arm Cortex M Microcontrollers.
  
  Email : eslamaboutaleb@yahoo.com
  Number : 01002044529
