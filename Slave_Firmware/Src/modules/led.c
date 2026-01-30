/*
* @file led.c
* @brief LED module for turn signals and warning lights
* @details This module provides functions to initialize and control LEDs for turn signals and warning lights
*
* @author: Aeron Lahoylahoy
* @date:   June 27, 2024
*/
#include "stm32f4xx.h"
#include "led.h"

int turnLeftFlag = 0;
int turnRightFlag = 0;
int turnOffFlag = 0;
int warningFlag = 0;
int warningCount = 0;
int blinkCount = 0;

/*
* @brief Function that turns on the warning LED
* @param None
* @return None
*/
void warningOn(void){
	if(warningCount == 1){
		if(warningFlag) ledOn(WARNING_LED);
	}
	
	if(warningCount == 2){
		GPIOA->ODR &= ~(0b1 << 10);
	}
}

/*
* @brief Function that turns off the warning LED
* @param None
* @return None
*/
void warningOff(void){
	GPIOA->ODR &= ~(0b1 << 10);
}

/*
* @brief Function that turns on the turn signal LEDs
* @param None
* @return None
*/
void turnSignalOn(void)
{
	if(blinkCount == 1){
		if(turnRightFlag){
		GPIOA->ODR &= ~(0b1 << PIN9);
		ledOn(RIGHT_LED);
		}
		
		if(turnLeftFlag){
		GPIOA->ODR &= ~(0b1 << PIN8);
		ledOn(LEFT_LED);
		}
	}
	
	if(blinkCount == 2){
		if(turnRightFlag) GPIOA->ODR &= ~(0b1 << PIN8);
		if(turnLeftFlag) GPIOA->ODR &= ~(0b1 << PIN9);
	}
	
}

/*
* @brief Function that turns on a specified LED
* @param n: LED number to turn on
* @return None
*/
void ledOn(int n)
{

	switch (n)
	{

	case RIGHT_LED:
		GPIOA->ODR |= 1 << 8;
		break;

	case LEFT_LED:
		GPIOA->ODR |= 1 << 9;
		break;
	
	case WARNING_LED:
		GPIOA->ODR |= 1 << 10;
		break;
	}
}

/*
* @brief Function that turns off the turn signal LEDs
* @param None
* @return None
*/
void turnSignalOff(void)
{
	/* Turn off timer */
	//TIM7->CR1 &= ~0b1;

	/* Turn off turning LEDs */
	GPIOA->ODR &= ~((1 << 8) | (1 << 9));
}

/*
* @brief Function that initializes Timer 7 for LED blinking
* @param None
* @return None
*/
void TIM7_Init(void)
{
	RCC->APB1ENR |= 1 << 5;
	TIM7->CR1 &= ~0b1;

	TIM7->DIER |= 1U;

	TIM7->CNT = 0;
	TIM7->PSC = 16000 - 1;
	TIM7->ARR = 250 - 1;

	 TIM7->CR1 |= 0b1;

	NVIC_EnableIRQ(TIM7_IRQn);
}

/*
* @brief Timer 7 Interrupt Handler for Turn Signal and Warning LED
* @param None
* @return None
*/
void TIM7_IRQHandler(void)
{
	
	if(turnRightFlag || turnLeftFlag){
	blinkCount++;
	if(blinkCount > 2) blinkCount = 0;
	}
	
	if(warningFlag){
		warningCount++;
		if(warningCount > 2) warningCount = 0;
	}

	TIM7->SR &= ~0b1;
}

/*
* @brief Function that initializes the LED GPIO pins
* @param None
* @return None
*/
void ledInit(void)
{
	RCC->AHB1ENR |= CLOCK_LED;
	GPIOA->MODER &= ~((0b11 << (TURN_LEFT * 2)) | (0b11 << (TURN_RIGHT * 2)) | (0b11 << (WARNING * 2))); // Clear
	GPIOA->MODER |= (0b01 << (TURN_LEFT * 2)) | (0b01 << (TURN_RIGHT * 2)) | (0b01 << (WARNING * 2));	 // Output
}