## EDOSv2: Embedded Deterministic Operating System version 2

#Older Features:
1. Floating Point Unit is Disabled
2. Main Stack Pointer is used for SP
3. SysTick ISR is used to call the Scheduler (Round Robin)
4. PendSV ISR is used to do the context switching
5. SVC ISR is used to start the EDOS i.e. put the first thread in execution and afterwards SysTick takes over. 
6. Threads must have while(1) i.e. threads completion not allowed.

# Additional Features:
1. Idle Thread has been added.....Idle Thread puts the CPU to sleep 
2. EDOSDelay function has been implemented

NOTES:
-Idle Thread is initialized inside EDOSInit function.
-SVC Startup function (SVC #1) now, starts the Idle Thread instead of starting the first Thread 
