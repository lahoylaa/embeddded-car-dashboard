/*
* @file watchdog.h
* @brief Watchdog module header file
* @details This module provides function prototypes for initializing and handling the watchdog timer.
*
* @author Aeron Lahoylahoy
* @date June 27, 2024
*/
#ifndef WATCHDOG_H
#define WATCHDOG_H

#include "stm32f4xx.h"
#include "spi.h"
#include "seven_segment.h"

#define WDG_PORT PORTA
#define WDG_PIN PIN12

extern void watchDogInit(void);
extern void EXTI15_10_IRQHandler(void);

#endif /* WATCHDOG_H */