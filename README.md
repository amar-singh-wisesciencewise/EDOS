## EDOS
# EDOS: Embedded Deterministic Operating System


EDOS is a rudimentary RTOS written for TM4C1294XL Controller using Code Composer Studio IDE. 

Requirement:
1. TM4C1294XL
2. CCS v7 (GNU Compiler)
3. Ubuntu 18.04 

Feature:
1. Floating Point Unit is Disabled
2. Main Stack Pointer is used for STACK
3. SysTick ISR is used to call the Scheduler (Round Robin)
4. PendSV ISR is used to do the context switching
5. SVC ISR is used to start the EDOS
NOTE: New versions will add new features.

API:
ThreadStart(ThreadD* self,ThreadHandler threadHandler, void* stackStart, unsigned int stackSize)


[link] https://wisesciencewise.wordpress.com/2018/09/30/context-switching-in-arm-cortex-m3-m4/


