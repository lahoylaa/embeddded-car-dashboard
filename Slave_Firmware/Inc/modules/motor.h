/*
* @file motor.h
* @brief Motor module header file
* @details This module provides function prototypes for controlling stepper motors.
*
* @author Aeron Lahoylahoy
* @date June 27, 2024
*/
#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "port_pin_define.h"

/*Clocks Define*/
#define CLOCK_MOTOR Bclk
#define CLOCK_MOTOR1 Cclk
#define MOTOR_PORT1 PORTB
#define MOTOR_PORT2 PORTC

/* PIN Define */
/*Motor 1*/
#define MOTOR1_IN1 PIN4
#define MOTOR1_IN2 PIN5
#define MOTOR1_IN3 PIN6
#define MOTOR1_IN4 PIN7
/*Motor 2*/
#define MOTOR2_IN1 PIN4
#define MOTOR2_IN2 PIN5
#define MOTOR2_IN3 PIN6
#define MOTOR2_IN4 PIN7

/* Define Stepper Motor*/
#define STEP1 0x1
#define STEP2 0x2
#define STEP3 0x4
#define STEP4 0x8

/* Motor */
extern int speedMPH[20];
extern int speedRPM[20];
extern int speedIndex;
extern int speedCount1;
extern int speedCount2;
extern int current;
extern int forwardFlag1;
extern int reverseFlag1;
extern int forwardFlag2;
extern int reverseFlag2;
extern int mph;
extern int rpm;
extern int prevMph;
extern int prevRpm;

/* Motor Functions */
extern void moveSpeedometer(int speed);
extern void forwardControl_M1(int steps);
extern void reverseControl_M1(int steps);
extern void forwardControl_M2(int steps);
extern void reverseControl_M2(int steps);
extern void motorInit(void);
extern void SysTick_Init_Interrupt(int n);
extern void SysTick_Handler(void);

#endif /* _MOTOR_H_ */