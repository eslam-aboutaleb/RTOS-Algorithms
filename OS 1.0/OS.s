		AREA |.text|, CODE, READONLY, ALIGN=2
        THUMB
		EXTERN  CurrentPtr
		EXPORT	SysTick_Handler
		EXPORT  OS_SchedulerLaunch


SysTick_Handler          ;save r0,r1,r2,r3,r12,lr,pc,psr      
    CPSID   I               ;Disable interrupts   
    PUSH    {R4-R11}        ;save r4,r5,r6,r7,r8,r9,r10,r11   
    LDR     R0, =CurrentPtr  ; r0 points to CurrentPtr,old task       
    LDR     R1, [R0]        ; r1= CurrentPtr   
    STR     SP, [R1]        ;save old SP into TCB   
    LDR     R1, [R1,#4]     ; r1 =CurrentPtr->next   
    STR     R1, [R0]        ;CurrentPtr =r1 =NextPtr =StackPtr of New task   
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