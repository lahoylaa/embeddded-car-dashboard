/*
 * @file sonar.h
 * @brief Sonar module for distance measurement
 * @details This module is the header file for the sonar.c module
 *
 * @author: Aeron Lahoylahoy
 * @date: June 27, 2024
 */

#ifndef SONAR_H_
#define SONAR_H_

#include "port_pin_define.h"

/* Definitions */
#define SONAR_TRIG_PORT PORTA
#define SONAR_ECHO_PORT PORTB
#define SONAR_TRIG_PIN PIN5
#define SONAR_ECHO_PIN PIN4
#define SONAR_TRIG_CLK Aclk
#define SONAR_ECHO_CLK Bclk

/* Sonar Global Variables */
extern double current;
extern double last;
extern double timeElapsed;
extern double distance;

extern int checkDistance;

/* Sonar Functions */
void sonarInit(void);
void sonarTrigger_Init(void);
void sonarEcho_Init(void);
void readEcho(void);
void calculateDistance(double time);
void checkWarningSignal(void);

#endif /* SONAR_H_ */
