/*
 * EDOS.c
 *
 *  Edited on: 22Sep-2018
 *      Author: AMAR SINGH
 *      Contact: www.wisesciencewise.wordpress.com
 */
#include "EDOS.h"

ThreadD* volatile CurrentThread ; //pointer to the Current running thread
ThreadD* volatile NextThread; //pointer to the Next thread to be run
unsigned int volatile SVCReturn; // it is used here to return SVC calling ID from SVC ISR

ThreadD IdleThreadD;
unsigned int IdleThreadStack[IDLE_THREAD_STACK_SIZE];

unsigned int ThreadReadyState; //a bit for each 32 threads- a set bit means thread is ready to execute;

ThreadD* ThreadDList[NO_OF_THREADS_SUPPORTED]; //array of threads started so far;
unsigned int ThreadsRunning; //Number of Threads running
unsigned int CurrentThreadID; //Current Thread Index for Round-Robin Scheduling

int ThreadStart(ThreadD* self, ThreadHandler threadHandler, void* stackStart, unsigned int stackSize)
{
    unsigned int* sp;

    if(ThreadsRunning >= NO_OF_THREADS_SUPPORTED)
        return 1; //cannot create any more thread as all NO_OF_THREADS_SUPPORTED are in use
    else{
       self->ThreadID = ThreadsRunning++;
    }

    //register the thread with the EDOS
    ThreadDList[self->ThreadID] = self;

    if(stackStart != NULL){
        sp = (unsigned int*)((((unsigned int)stackStart + stackSize) >> 3) << 3);
    }else{
        return 1;
    }

    //stack preparation
    *(--sp) = (1U << 24);  // xPSR: thumb instruction mode set; ARM Cortex M cannot be in any other mode
    *(--sp) = (unsigned int)threadHandler; //Program Counter

    //saving the SP top in the Thread Descriptor
    self->sp = (sp - 14);// -14 for other Registers

    //Mark the Thread Ready
    ThreadReadyState |= (1U << (self->ThreadID));


    return 0; //success
}

void EDOSInit(){

    // set the priority of PendSV Interrupt to the lowest i.e. 0xFF and priority of SysTick interrupt to the highest i.e. 0x0
    *(unsigned int volatile* )0xE000ED20 &= ~(0xE0000000); //bit 31:30:29
    *(unsigned int volatile* )0xE000ED20 |= (0xFFU << 16); //bit23:22:21

    // prepare Stack for Idle Thread
    unsigned int* sp;

    sp = (unsigned int*)((((unsigned int)IdleThreadStack + sizeof(IdleThreadStack)) >> 3) << 3);
    //stack preparation
    *(--sp) = (1U << 24);  // xPSR: thumb instruction mode set; ARM Cortex M cannot be in any other mode
    *(--sp) = (unsigned int)IdleThreadFunction; //Program Counter

    //saving the SP top in the Thread Descriptor
    IdleThreadD.sp = (sp - 14);// -14 for other Registers
    IdleThreadD.ThreadID = NO_OF_THREADS_SUPPORTED; // ThreadID of Idle Thread is 0
    IdleThreadD.DelayTick = 0; //cannot be other than 0 as Idle Thread cannot be in waiting state;

}


void EDOSScheduler(){
//must be called from interrupt disabled section
//Schedules the different running Threads for the CPU as per the Round Robin Scheduling

//    __asm(" CPSID   I");

    unsigned int i = 0;

    if(ThreadReadyState != 0){
        do{
            if (CurrentThreadID == ThreadsRunning) {
                CurrentThreadID = 0; // loop over once, all Threads have been serviced once
            }
            //go to next thread
            NextThread = ThreadDList[CurrentThreadID++];
            i++;

        }while(!(ThreadReadyState & (1U << (NextThread->ThreadID))));
    }else
    {
        NextThread = &IdleThreadD; //i.e. Idle Thread if ThreadReadyState == 0
    }

    //set the PendSV interrupt
    *(unsigned int volatile *)0xE000ED04 |= (1U << 28);

//    __asm("  CPSIE  I"); //enabling interrupts
}


void SysTickHandler(void){ //register this function in STARTUP CODE as SysTick ISR
    unsigned int i;

    for(i = 0; i< ThreadsRunning; i++){
        if(!(ThreadReadyState & (1U<<i))) //i.e thread is not ready
        {
            if(--ThreadDList[i]->DelayTick == 0){ //decrease the delay Tick remaining
                ThreadReadyState |= (1U << i);
            }
        }
    }
    __asm(" CPSID   I"); //disable interrupt
    EDOSScheduler(); //scheduler
    __asm("  CPSIE  I"); //enabling interrupts
}

void PendSVCoverFunction(void)//PendSV ISR
{
    PendSVHandler(); //real PendSV ISR, written in Assembly, but could not be registered in Startup file so used cover function
}

void SVCCoverFunction(void){
    SVCHandler();//actual SVC ISR, written in Assembly, but could not be registered in Startup file so used cover function

    if(SVCReturn == 1)//i.e SVC call was to start the Idle Thread
    {
        CurrentThread = &IdleThreadD;
        EDOSRun();
        //code should never come here
    }
    //Similarly other SVC Services can be implemented here depending on SVCReturn Value
}

void IdleThreadFunction(){

    while(1){

        __asm(" WFI");
    }
}

void EDOSDelay(unsigned int delay){
    __asm(" CPSID   I");
    CurrentThread -> DelayTick = delay;
    ThreadReadyState &= ~(1U << (CurrentThread->ThreadID));
    EDOSScheduler();
    __asm("  CPSIE  I");

}
