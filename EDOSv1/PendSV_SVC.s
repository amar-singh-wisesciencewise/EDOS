
/*
*	AMAR SINGH
*	15 AUG 2018;
*	Contact: www.wisesciencewise.wordpress.com
*/
	
	.thumb
	.syntax unified	

/**************************PendSV Handler***************************/	
	.global	PendSVHandler
	.global CurrentThread
	.global NextThread

	// PendSVhandler is pendSV interrupt routine  
	// it is used for contesct switch
	
PendSVHandler:

	CPSID	I //disable interrrupt
	
	//Since we are calling this function(intended to be an ISR) inside another function so: 
	//taking SP back to the ISR entering state and undoing the cover function effects
	POP		{R7,LR}
	
    PUSH	{R4-R11}    
	
	//CurrentThread->sp = SP 
    LDR		R1,=CurrentThread
    LDR		R2,[R1,#0x00] //"LDR  R1, [R1]" would have also worked here and below
    STR		SP,[R2,#0x00]

	//SP = NextThread->sp
	LDR		R3,=NextThread
    LDR		R3,[R3,#0x00]
    LDR		SP,[R3,#0x00]

    STR		R3,[R1,#0x00]  //CurrentTHread = NextThread
 
    POP		{R4-R11}    

    CPSIE	I
	NOP
    //return to the NextThread
    BX		LR 
	//MOV	PC, LR  



/************************SVC Handler Function******************************/
    
	.global SVCHandler
	.global SVCReturn

SVCHandler:

/*		
	TST		LR, #4 //testing LR for which statck is in use
	ITE		EQ
	MRSEQ	R0,	MSP 
	MRSNE	R0,	PSP
*/

	MRS		R0, MSP
		
	LDR		R0,	[R0, #32] //getting PC of SVC Command (24+8) +8 for PUSH {R7,LR} done by Cover Function
	LDRB	R0, [R0, #-2] //R0 holds SVC called value

	
	//Storing the SVC value in a Global Variable
	LDR		R1, =SVCReturn 
    STR		R0, [R1,#0x00]
//	BX		LR
	MOV	PC, LR
	
/******************EDOSRun******************************/
	.global EDOSRun
	//CurrentThread has been already declared above
	//System Call to start the EDOS Scheduling
	
EDOSRun:

	CPSID	I //disable interrupt
	
	MOV		R14, #0xFFFFFFF9 //To make the EDOSRun return look like ISR return 	//MSP is used	
	
	//SP = CurrentThread->sp	
	LDR		R1,=CurrentThread
    LDR		R1,[R1,#0x00]
    LDR		SP,[R1,#0x00]
    
    POP 	{R4-R11}
    
    CPSIE	I //enable interrupt
    NOP
    BX		LR
		
	
	