#First Version of EDOS:

##Feature:
1. Floating Point Unit is Disabled
2. Main Stack Pointer is used for SP
3. SysTick ISR is used to call the Scheduler (Round Robin)
4. PendSV ISR is used to do the context switching
5. SVC ISR is used to start the EDOS i.e. put the first thread in execution and afterwards SysTick takes over. 
6. Threads must have while(1) i.e. threads completion not allowed.


##Steps to create and start a Thread/Task: (main.c should give an idea)
1. create a stack i.e. an integer array.
2. create a ThreadD object.
3. call the ThreadStart API with proper arguments.
4. do not forget to write your regitered thread function ;)

NOTE: To overcome some shortcomings of CCS around "C and Assembly mixing" has made me do idiosyncratic things in the code.
