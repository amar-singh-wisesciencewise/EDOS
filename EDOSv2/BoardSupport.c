/*
 * BoardSupport.c
 *
 *  Edited on: 22Sep-2018
 *      Author: AMAR SINGH
 *      Contact: www.wisesciencewise.wordpress.com
 */
#include "BoardSupport.h"
#include "EDOS.h"

void ParllelThread1(){
//blinks an LED inside while(1)

    while (1){

           GPIO_N_DATA_BASE[LED_ONE] = LED_ONE;
           EDOSDelay(10);
           GPIO_N_DATA_BASE[LED_ONE] = ~LED_ONE;
           EDOSDelay(10);
    }

}

void ParllelThread2(){
//blinks an LED inside while(1)

    while (1){

           GPIO_N_DATA_BASE[LED_TWO] = LED_TWO;
           EDOSDelay(5);
           GPIO_N_DATA_BASE[LED_TWO] = ~LED_TWO;
           EDOSDelay(5);

    }

}

int BoardInit(){
// This function initializes the hardware like GPIO and SYSTICK

    (CLOCK_GATE_ENABLE) |= 0x1000 ;  //GPIO PORT CLOCK
     SomeDelay(100);
    GPIO_N_DIR |= (LED_ONE |LED_TWO); //setting direction of GPIO port as output
    GPIO_N_DEN |= (LED_ONE| LED_TWO); //digital enable

    STRELOAD = 0x0FFFFFU;/*max value = 0x7FFFFFU*/
    STCURRENT = 0u;
    STCTRL |= 0x7; /*CLK_SRC =1(system clock); INTEN = 1(int enable ); ENABLE = 1(start counter)*/

    return 0;
}


void SomeDelay(unsigned int duration){

    while(duration)
        --duration;
}
