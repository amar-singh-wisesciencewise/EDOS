/*
 * EDOS.c
 *
 *  Created on: 02-Jun-2018
 *      Author: AMAR SINGH
 *      Contact: www.wisesciencewise.wordpress.com
 */
#include "EDOS.h"

ThreadD* volatile CurrentThread ; //pointer to the Current running thread
ThreadD* volatile NextThread; //pointer to the Next thread to be run
int volatile SVCReturn; // it is used here to return SVC calling ID from SVC ISR

ThreadD* ThreadDList[NO_OF_THREADS_SUPPORTED]; //array of threads started so far
unsigned int ThreadsRunning; //Number of Threads running
unsigned int CurrentThreadID; //Current Thread Index for Round-Robin Scheduling

unsigned int ThreadIDBitMap; //32 bits for 32 threads for ready threads management

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
    *(--sp) = (1U << 24);  // xPSR: thread mode set; ARM Cortex M cannot be in any other mode
    *(--sp) = (unsigned int)threadHandler; //Program Counter

    //saving the SP top in the Thread Descriptor
    self->sp = (sp - 14);// -14 for other Registers


    return 0; //success
}

void EDOSInit(){
    // set the priority of PendSV Interrupt to the lowest i.e. 0xFF and priority of SysTick interrupt to the highest i.e. 0x0
    *(unsigned int volatile* )0xE000ED20 &= ~(0xE0000000); //bit 31:30:29
    *(unsigned int volatile* )0xE000ED20 |= (0xFFU << 16); //bit23:22:21

}


void EDOSScheduler(){
//Schedules the different running Threads for the CPU as per the Round Robin Scheduling

    __asm(" CPSID   I");

    if (CurrentThreadID == ThreadsRunning) {
        CurrentThreadID = 0U; // loop over once, all Threads have been serviced once
    }
    //go to next thread
    NextThread = ThreadDList[CurrentThreadID++];

    //set the PendSV interrupt
    if (NextThread != CurrentThread) {
        *(unsigned int volatile *)0xE000ED04 |= (1U << 28);
    }

    __asm("  CPSIE  I"); //enabling interrupts
}


void PendSVCoverFunction(void)//PendSV ISR
{
    PendSVHandler(); //real PendSV ISR, written in Assembly, but could not be registered in Startup file so used cover function
}

void SVCCoverFunction(void){
    SVCHandler();//actual SVC ISR, written in Assembly, but could not be registered in Startup file so used cover function

    if(SVCReturn == 1)//i.e SVC call was to start the EDOS Threads
    {
        CurrentThread = ThreadDList[CurrentThreadID++];
        EDOSRun();
        //code should never come here
    }
    //Similarly other SVC Services can be implemented here depending on SVCReturn Value
}


