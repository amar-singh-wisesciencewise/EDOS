/**
 * main.c
 * AMAR SINGH:
 *
 *  Edited on: 22Sep-2018
 *Contact: www.wisesciencewise.wordpress.com
 *
 */

#include "BoardSupport.h"

#include "EDOS.h"

ThreadD Thread1;
ThreadD Thread2;
unsigned int Stack1[STACK_SIZE]; //stack for thread 1
unsigned int Stack2[STACK_SIZE];


int main(void){

    __asm(" CPSID   I"); //disabling global interrupts so to avoid triggering EDOSScheduler before the thread's initialization

    BoardInit(); //Hardware initialization
    EDOSInit(); //Priority settings

    ThreadStart(&Thread1,ParllelThread1,(void*)Stack1,sizeof(Stack1));
    ThreadStart(&Thread2,ParllelThread2,(void*)Stack2,sizeof(Stack2));

    __asm("  CPSIE  I"); //enabling global interrupt

    __asm(" SVC   #1"); //SVC call to start the first thread.....EDOSRun

    //Code should never come back here;

	return 0;
}
