/*
 * @file 	rotary_encoder.c
 * @brief 	Rotary Encoder module for handling user input via rotary encoder
 * @details This module provides functions to initialize the rotary encoder pins,
 *
 * @author: Aeron Lahoylahoy
 * @date:  June 27, 2024
 */
#include "stm32f4xx.h"
#include "port_pin_define.h"
#include "rotary_encoder.h"
#include "display.h"
#include "button_functions.h"
#include "iLI9341.h"
#include "rtc.h"

/* Rotary Encoder Global Variables */
int encoderSW_Flag = 0;
int encoderCLK_Flag;
int CounterClockwise_Count = 0;
int Clockwise_Count = 0;

int count = 3;
int turn;
int blink;
int CCW;
int CW;

/*
 * @brief Function that initializes the rotary encoder
 * @param None
 * @return None
 */
void rotaryEncoderInit(void)
{

	encoderSW_Init();
	encoderCLK_Init();
	encoderDT_Init();
}

/*
 * @brief Function that initializes the SW pin on the Rotary Encoder
 * @param None
 * @return None
 */
void encoderSW_Init(void)
{

	// Set GPIOC Clock
	RCC->AHB1ENR |= Bclk;

	// Set Interrupt Clock
	RCC->APB2ENR |= 0x4000;

	// GPIOC Initilization
	GPIOB->MODER &= ~(0b11 << (PIN0 * 2));
	GPIOB->MODER |= (0b00 << (PIN0 * 2));
	GPIOB->PUPDR |= (0b01 << (PIN0 * 2));

	// Interrupt Initilization
	SYSCFG->EXTICR[0] &= ~0x000F; // Clear
	SYSCFG->EXTICR[0] |= 0x0001;  // Port C on EXTI 0
	EXTI->IMR |= (0b1 << PIN0);	  // Interrupt Mask on PIN 0
	EXTI->FTSR |= (0b1 << PIN0);  // Falling edge on PIN 0

	// Enable Interrupt for Pin 0
	NVIC_EnableIRQ(EXTI0_IRQn);
}

/*
 * @brief Function that initializes the CLK pin on the Rotary Encoder
 * @param None
 * @return None
 */
void encoderCLK_Init(void)
{

	// Set GPIOC Clock
	RCC->AHB1ENR |= Bclk;

	// Set Interrupt Clock
	RCC->APB2ENR |= 0x4000;

	// GPIOC Initilization
	GPIOB->MODER &= ~(0b11 << (PIN1 * 2));
	GPIOB->MODER |= (0b00 << (PIN1 * 2));
	GPIOB->PUPDR |= (0b01 << (PIN1 * 2));

	// Interrupt Initilization
	SYSCFG->EXTICR[0] &= ~0x00F0; // Clear
	SYSCFG->EXTICR[0] |= 0x0010;  // Port C on EXTI 1
	EXTI->IMR |= (0b1 << PIN1);	  // Interrupt Mask on PIN 1
	EXTI->FTSR |= (0b1 << PIN1);  // Falling edge on PIN 1

	// Enable Interrupt for Pin 1
	NVIC_EnableIRQ(EXTI1_IRQn);
}

/*
 * @brief Function that initializes the DT pin on the Rotary Encoder
 * @param None
 * @return None
 */
void encoderDT_Init(void)
{

	// Set GPIOC Clock
	RCC->AHB1ENR |= Bclk;

	// GPIOC Initilization
	GPIOB->MODER &= ~(0b11 << (PIN2 * 2));
	GPIOB->MODER |= (0b00 << (PIN2 * 2));
	// GPIOC->PUPDR |= (0b01 << (PIN2 * 2));
}

/*
 * @brief EXTI0 Interrupt Handler for Rotary Encoder SW pin
 * @param None
 * @return None
 */
extern void EXTI0_IRQHandler(void)
{

	/* Debounce pin when interrupt occurs */
	if (debounceButton(PORTB, PIN0))
	{

		encoderSW_Flag = 1;
		blinkMenuFlag++;

		if (state == TIMESTATE)
		{
			changeTimeFlag = 1;
			changeTimeCount++;
		}

		if (state == DATESTATE)
		{
			changeDateFlag = 1;
			changeDateCount++;
		}
	}

	/* Reset EXTI0 interrupt flag  */
	EXTI->PR |= (0b1 << PIN0);
}

/*
 * @brief EXTI1 Interrupt Handler for Rotary Encoder CLK pin
 * @param None
 * @return None
 */
extern void EXTI1_IRQHandler(void)
{

	/* Determine the direction of the turn */
	/* Checks Rotary Encoder DT pin value is 0 (Port C Pin 2) */
	if (!(PORTB->IDR & (0b1 << PIN2)))
	{
		delayMS(10);

		if (blinkMenuFlag)
		{
			CCW++;
			if (CCW > 3)
				CCW = 0;
		}

		if (state == TIMESTATE)
		{
			if (changeTimeCount == 1)
			{
				hour--;
				if (hour < 1)
					hour = 12;
			}

			if (changeTimeCount == 2)
			{
				min--;
				if (min < 1)
					min = 59;
			}

			if (changeTimeCount == 3)
			{
				ampmFlag--;
				if (ampmFlag < 0)
					ampmFlag = 1;
			}
		}

		if (state == DATESTATE)
		{

			/* change month */
			if (changeDateCount == 1)
			{
				month--;
				if (month < 1)
					month = 12;
			}

			/* change date */
			if (changeDateCount == 2)
			{
				date--;
				if (date < 1)
					date = 31;
			}

			/* change year */
			if (changeDateCount == 3)
			{
				year--;
				if (year < 1)
					year = 99;
			}
		}
	}

	/* Checks Rotary Encoder DT pin value is 1 (Port C Pin 2) */
	else if (PORTB->IDR & (0b1 << PIN2))
	{
		delayMS(10);

		if (blinkMenuFlag)
		{
			CW++;
			if (CW > 3)
				CW = 0;
		}

		/* New shit */
		if (state == TIMESTATE)
		{
			if (changeTimeCount == 1)
			{
				hour++;
				if (hour > 12)
					hour = 1;
			}

			if (changeTimeCount == 2)
			{
				min++;
				if (min > 59)
					min = 1;
			}

			if (changeTimeCount == 3)
			{
				ampmFlag++;
				if (ampmFlag > 1)
					ampmFlag = 0;
			}
		}

		if (state == DATESTATE)
		{

			/* change month */
			if (changeDateCount == 1)
			{
				month++;
				if (month > 12)
					month = 1;
			}

			/* change date */
			if (changeDateCount == 2)
			{
				date++;
				if (date > 31)
					date = 1;
			}

			/* change year */
			if (changeDateCount == 3)
			{
				year++;
				if (year > 99)
					year = 1;
			}
		}
	}

	/* Reset EXTI0 interrupt flag  */
	EXTI->PR |= (0b1 << PIN1);
}
