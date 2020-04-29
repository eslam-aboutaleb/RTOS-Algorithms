; Author : Eslam Aboutaleb

        ;C code compiler assumes that stack allocation is aligned on 8-byte boundaries.
		PRESERVE8 ;So ensure that the assembler interrupt handler has an 8-byte aligned stack.
		AREA |.text|, CODE, READONLY, ALIGN=2
        THUMB
		EXTERN  CurrentPtr
		EXPORT	SysTick_Handler
		EXPORT  OS_SchedulerLaunch
		IMPORT  Scheduler	


SysTick_Handler          ;save r0,r1,r2,r3,r12,lr,pc,psr      
    CPSID   I               ;Disable interrupts   
    PUSH    {R4-R11}        ;save r4,r5,r6,r7,r8,r9,r10,r11   
    LDR     R0, =CurrentPtr ; r0 points to CurrentPtr,old task       
    LDR     R1, [R0]        ; r1= CurrentPtr   
    STR     SP, [R1]        ;save old SP into TCB   
    PUSH    {R0,LR}         ;R0 was pointing to CurrentPtr,LR has interrupt return special address 0xFFFFFFF9
                            ;So Save LR before calling functionbecause BL uses LR register to save its return
							;Address
    BL      Scheduler       ;Call Scheduler function
	POP     {R0,LR}         ; Pop it back
	LDR     R1,[R0]         ;R1=CurrentPtr  =New task
	LDR     SP, [R1]        ;SP= .CurrentPtr->stackPtr 
    POP     {R4-R11}        ; restore regs r4-11   
    CPSIE   I                  
    BX      LR 
	
	

OS_SchedulerLaunch
    LDR     R0, =CurrentPtr         
    LDR     R2, [R0]       ; R2 =currentPt       
    LDR     SP, [R2]       ;SP = currentPt->stackPt    
    POP     {R4-R11}          
    POP     {R0-R3}            
    POP     {R12}
    ADD     SP,SP,#4       ;Discard LR reg       
    POP     {LR}           ;Pop PC into LR -> PC has the start of the thread  
    ADD     SP,SP,#4       ;Discard PSR reg  
    CPSIE   I               ;Enable interrupts  I bit in PSR=0      
    BX      LR              ;Return from sub routine   

    ALIGN
    END