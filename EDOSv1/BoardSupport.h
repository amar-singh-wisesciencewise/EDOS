/*
 * BoardSupport.h
 *
 *  Created on: 02-Jun-2018
 *      Author: AMAR SINGH
 */

#ifndef BOARDSUPPORT_H_
#define BOARDSUPPORT_H_

#define LED_ONE (1U << 0)
#define LED_TWO (1U << 1)
#define CLOCK_GATE_ENABLE  (*((volatile unsigned int*)0x400fe608))/*THIS ENABLE THE GPIO ADDRESSS SECTION*/
#define GPIO_N_DIR  (*((volatile unsigned int*) 0x40064400))/*THIS IS USED TO SET THE DIRECTION OF GPIO PORTS*/
#define GPIO_N_DEN  (*((volatile unsigned int*) 0x4006451c))/*THIS IS TO MAKE OUTPUT OF GPIO PORT DIGITAL*/
#define GPIO_N_DATA_BASE  ((volatile unsigned int*)0x40064000)/*THIS IS BASE ADDRESS OF GPIO DATA REGISTER*/
////
#define SYS_TICK_BASE 0xE000E000 /*base address of systick registers*/
#define STCTRL (*(volatile unsigned int*)(SYS_TICK_BASE + 0x010))
#define STRELOAD (*(volatile unsigned int*)(SYS_TICK_BASE + 0x014))
#define STCURRENT (*(volatile unsigned int*)(SYS_TICK_BASE + 0x018))



extern void SomeDelay(unsigned int);
extern void ParllelThread1();
extern void ParllelThread2();
extern int BoardInit();


#endif /* BOARDSUPPORT_H_ */
