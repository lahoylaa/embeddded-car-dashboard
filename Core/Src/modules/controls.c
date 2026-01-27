/*
 * @file 	controls.c
 * @brief 	Control module for button and switch initialization and interrupt handling
 * @details This module provides functions to initialize buttons and switches with EXTI interrupts for:
 * 			- Menu navigation button
 * 			- Turn signal switch (left/right)
 * 			- Reset button
 * 			- Bluetooth enable button
 * 
 * @author: Aeron Lahoylahoy
 * @date: June 27, 2024
 */
#include "stm32f4xx.h"
#include "controls.h"
#include "i2c_master.h"
#include "button_functions.h"
#include "Display.h"
#include "eeprom.h"
#include "speed_sensor.h"

/* Variables */
int bluetoothFlag = 0;
int bluetoothCounter = 0;

/*
 * @brief Function that initializes the menu button with EXTI interrupt.
 * @param None
 * @return None
 */
void menuButtonInit(void)
{
	buttonInit(MENU_BUTTON_PORT, MENU_BUTTON_PIN);
}

/*
 * @brief Function that initializes the turn signal switches with EXTI interrupts.
 * @param None
 * @return None
 */
void turnSignalSWInit(void)
{
	turnSignalButtonInit(TURN_SIGNAL_PORT, TURN_RIGHT_PIN);
	turnSignalButtonInit(TURN_SIGNAL_PORT, TURN_LEFT_PIN);
}

/*
 * @brief Function that initializes a turn signal switch GPIO pin as input with EXTI interrupt.
 * @param Port: GPIO port (e.g., PORTA, PORTB, etc.)
 * @param Pin: GPIO pin number (0-15)
 * @return None
 */
void turnSignalButtonInit(GPIO_TypeDef *Port, int Pin)
{
	/*Variables*/
	int array = 0;
	int line = 0;
	int shift = 0;

	if (Port == PORTA)
	{
		RCC->AHB1ENR |= Aclk; /*Set GPIOx Clock*/
		line = 0x0;
	}
	else if (Port == PORTB)
	{
		RCC->AHB1ENR |= Bclk; /*Set GPIOx Clock*/
		line = 0x1;
	}
	else if (Port == PORTC)
	{
		RCC->AHB1ENR |= Cclk; /*Set GPIOx Clock*/
		line = 0x2;
	}
	RCC->APB2ENR |= 0x4000; /*Set Interrupt Clock*/

	/*GPIO Initilization*/
	Port->MODER &= ~(0b11 << (Pin * 2)); /*Clear*/
	Port->MODER |= (0b00 << (Pin * 2));	 /*Input*/
	Port->PUPDR |= (0b01 << (Pin * 2));	 /*Pull up*/

	/*Define array based on Pin*/
	if (Pin < 4)
		array = 0;
	else if ((Pin < 8) && (Pin > 3))
		array = 1;
	else if ((Pin < 12) && (Pin > 7))
		array = 2;
	else if ((Pin < 16) && (Pin > 11))
		array = 3;

	/*Interrupts Initialization*/
	if ((Pin == 0) | (Pin == 4) | (Pin == 8) | (Pin == 12))
		shift = 0;
	else if ((Pin == 1) | (Pin == 5) | (Pin == 9) | (Pin == 13))
		shift = 1;
	else if ((Pin == 2) | (Pin == 6) | (Pin == 10) | (Pin == 14))
		shift = 2;
	else if ((Pin == 3) | (Pin == 7) | (Pin == 11) | (Pin == 15))
		shift = 3;

	SYSCFG->EXTICR[array] &= ~(0xF << (shift * 4));
	SYSCFG->EXTICR[array] |= (line << (shift * 4));
	EXTI->IMR |= (0b1 << Pin);
	EXTI->FTSR |= (0b1 << Pin);
	EXTI->RTSR |= (0b1 << Pin); /* Include for turn signal switch */

	/*Enable Interrupts*/
	if (Pin == 0)
		NVIC_EnableIRQ(EXTI0_IRQn);
	else if (Pin == 1)
		NVIC_EnableIRQ(EXTI1_IRQn);
	else if (Pin == 2)
		NVIC_EnableIRQ(EXTI2_IRQn);
	else if (Pin == 3)
		NVIC_EnableIRQ(EXTI3_IRQn);
	else if (Pin == 4)
		NVIC_EnableIRQ(EXTI4_IRQn);
	else if ((Pin < 10) && (Pin > 4))
		NVIC_EnableIRQ(EXTI9_5_IRQn);
	else if ((Pin < 16) && (Pin > 9))
		NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/*
 * @brief Function that initializes the reset button with EXTI interrupt.
 * @param None
 * @return None
 */
void resetButtonInit(void)
{

	buttonInit(RESET_BUTTON_PORT, RESET_BUTTON_PIN);
}

/*
 * @brief Function that initializes the Bluetooth enable button with EXTI interrupt.
 * @param None
 * @return None
*/
void bluetoothEnableInit(void)
{

	RCC->AHB1ENR |= BLUETOOTH_ENABLE_CLK;
	BLUETOOTH_ENABLE_PORT->MODER &= ~(0b11 << (BLUETOOTH_ENABLE_PIN * 2)); // clear
	BLUETOOTH_ENABLE_PORT->MODER |= (0b01 << (BLUETOOTH_ENABLE_PIN * 2));  // output

	BLUETOOTH_ENABLE_PORT->ODR |= (0b1 << BLUETOOTH_ENABLE_PIN); // PIN High (Bluetooth OFF)
}

/*
 * @brief Function that initializes the Bluetooth button with EXTI interrupt.
 * @param None
 * @return None
 */
void bluetoothButtonInit(void)
{
	buttonInit(BLUETOOTH_BUTTON_PORT, BLUETOOTH_BUTTON_PIN);
}

/*
 * @brief Interrupt handler for EXTI lines 15 to 10.
 * @param None
 * @return None
 */
void EXTI15_10_IRQHandler(void)
{

		/* Bluetooth */
	if (EXTI->PR & (0b1 << BLUETOOTH_BUTTON_PIN))
	{
		if (debounceButton(BLUETOOTH_BUTTON_PORT, BLUETOOTH_BUTTON_PIN))
		{
			BLUETOOTH_ENABLE_PORT->ODR ^= (0b1 << PIN11);
			// debugFlag = 1;
			// displayBluetooth();
			bluetoothFlag = 1;
		}

		EXTI->PR |= (0b1 << BLUETOOTH_BUTTON_PIN);
	}

		/* Menu button */
	if (EXTI->PR & (0b1 << MENU_BUTTON_PIN))
	{
		if (debounceButton(MENU_BUTTON_PORT, MENU_BUTTON_PIN))
		{
			if (state != MENUSTATE)
				menuScreen = 1;
		}
		EXTI->PR |= (0b1 << MENU_BUTTON_PIN);
	}
}

/*
 * @brief Interrupt handler for EXTI lines 9 to 5.
 * @param None
 * @return None
 */
void EXTI9_5_IRQHandler(void)
{

	/* Only want to trigger a blink on falling edge */
	if (EXTI->PR & (0b1 << TURN_RIGHT_PIN))
	{
		if (debounceButton(PORTA, TURN_RIGHT_PIN))
		{

			I2C1_byteWrite(slave, 0, 0x41);
		}
		else
		{
			I2C1_byteWrite(slave, 0, 0x43);
		}
		EXTI->PR |= (0b1 << TURN_RIGHT_PIN);
	}

	/* Turn signal left */
	if (EXTI->PR & (0b1 << TURN_LEFT_PIN))
	{
		if (debounceButton(PORTA, TURN_LEFT_PIN))
		{

			I2C1_byteWrite(slave, 0, 0x42);
		}
		else
		{
			I2C1_byteWrite(slave, 0, 0x43);
		}
		EXTI->PR |= (0b1 << TURN_LEFT_PIN);
	}

		/* Reset button */
	if (EXTI->PR & (0b1 << RESET_BUTTON_PIN))
	{
		if (debounceButton(RESET_BUTTON_PORT, RESET_BUTTON_PIN))
		{
			// I2C1_byteWrite(slave, 0, 0x21);
			I2C1_byteWrite(mileEEPROM, mileMemory, 0);
			I2C1_byteWrite(slave, 0, 0x10); // sends the reset command
			traveledMiles = 0;
		}

		EXTI->PR |= (0b1 << RESET_BUTTON_PIN);
	}

	/* Miles */
	if (EXTI->PR & (0b1 << 8))
	{

		speedSensorHallDetected();

		EXTI->PR |= (0b1 << 8);
	}
}
