/*
 * BoardSupport.c
 *
 *  Created on: 02-Jun-2018
 *      Author: AMAR SINGH
 *      Contact: www.wisesciencewise.wordpress.com
 */
#include "BoardSupport.h"
#include "EDOS.h"

volatile int SysTickCount; //SystickCount is incremented upon every Systick interrupt


void ParllelThread1(){
//blinks an LED inside while(1)

    while (1){

           GPIO_N_DATA_BASE[LED_ONE] = LED_ONE;
           SomeDelay(10000);
           GPIO_N_DATA_BASE[LED_ONE] = ~LED_ONE;
           SomeDelay(10000);
    }

}

void ParllelThread2(){
//blinks an LED inside while(1)

    while (1){

           GPIO_N_DATA_BASE[LED_TWO] = LED_TWO;
           SomeDelay(20000);
           GPIO_N_DATA_BASE[LED_TWO] = ~LED_TWO;
           SomeDelay(20000);

    }

}

int BoardInit(){
// This function initializes the hardware like GPIO and SYSTICK

    (CLOCK_GATE_ENABLE) |= 0x1000 ;  //GPIO PORT CLOCK
     SomeDelay(100);
    GPIO_N_DIR |= (LED_ONE |LED_TWO); //setting direction of GPIO port as output
    GPIO_N_DEN |= (LED_ONE| LED_TWO); //digital enable


    STRELOAD = 0x7FFFFFu;/*max value*/
    STCURRENT = 0u;
    STCTRL |= 0x7; /*CLK_SRC =1(system clock); INTEN = 1(int enable ); ENABLE = 1(start counter)*/

    return 0;
}


void SysTickHandler(void){ //register this function in STARTUP CODE as SysTick ISR
    SysTickCount++; //just an counter does not used in present version of EDOS
    EDOSScheduler(); //scheduler
}


void SomeDelay(unsigned int duration){

    while(duration)
        --duration;
}
