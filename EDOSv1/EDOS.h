/*
 * EDOS.h
 *
 *  Created on: 02-Jun-2018
 *      Author: AMAR SINGH
 */

#ifndef EDOS_H_
#define EDOS_H_

#define NO_OF_THREADS_SUPPORTED 32
#define STACK_SIZE 64

#define NULL 0x00

// Thread Control Block (TCB)
typedef struct {
    void* sp; //Stack Pointer
    unsigned int ThreadID; // thread ID
}ThreadD; //thread descriptor

typedef void (*ThreadHandler)();

void EDOSInit(void);

void EDOSScheduler(void);

extern void EDOSRun(void);

int ThreadStart(ThreadD* self, ThreadHandler threadHandler, void* stackStart, unsigned int stackSize);

extern void PendSVHandler(void); //assembly function actual ISR

void PendSVCoverFunction(void); //Cover function for ISR

extern void SVCHandler(void); //assembly function actual ISR

void SVCCoverFunction(void); //Cover function for ISR



#endif /* EDOS_H_ */
