/*
 * @file button_functions.h
 * @brief GPIO button initialization and software debouncing utilities
 * @details Provides common functions for configuring GPIO pins as inputs with external interrupts (EXTI)
 * 
 * @author: Aeron Lahoylahoy
 * @date: June 27, 2024
 */

#ifndef BUTTON_FUNCTIONS_H_
#define BUTTON_FUNCTIONS_H_

#include "stm32f4xx.h"
#include "port_pin_define.h"

void buttonInit(GPIO_TypeDef *Port, int Pin);
int debounceButton(GPIO_TypeDef *Port, int Pin);

#endif /* BUTTON_FUNCTIONS_H_ */
