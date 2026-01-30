/*
* @file led.h
* @brief LED module header file
* @details This module provides function prototypes for initializing and controlling the LEDs.
*
* @author Aeron Lahoylahoy
* @date June 27, 2024
*/
#ifndef _LED_H_
#define _LED_H_

#include "port_pin_define.h"

/*Clock Define*/
#define CLOCK_LED Aclk

/*PORT Define*/
#define LED_PORT PORTA

/*PIN Define*/
#define TURN_LEFT PIN8	/*Turn Left Signal*/
#define TURN_RIGHT PIN9 /*Turn Right Signal*/
#define WARNING PIN10	/*Warning Signal*/

#define LEFT_LED 0
#define RIGHT_LED 1
#define WARNING_LED 3

extern int turnLeftFlag;
extern int turnRightFlag;
extern int turnOffFlag;
extern int blinkCount;
extern int warningFlag;
extern int warningCount;

extern void ledInit(void); /*LEDs Initialization*/
extern void turnSignalOff(void);
extern void ledOn(int n);
extern void turnSignalOn(void);
extern void turnSignal_Init(void);
extern void warningOff(void);
extern void warningOn(void);

extern void TIM7_Init(void);
extern void TIM7_IRQHandler(void);


#endif /* _LED_H_ */