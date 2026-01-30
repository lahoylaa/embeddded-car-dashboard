/*
 * @file 	button_functions.c
 * @brief 	GPIO button initialization and software debouncing utilities
 * @details Provides common functions for configuring GPIO pins as inputs with external interrupts (EXTI)
 * 			and simple polling-based debouncing
 *
 * @note 	Interrupt configuration uses both falling and rising edges where needed, but
 * 		 	falling edge is default for most buttons
 * 		 	Debounce uses a simple polling counter (30 samples, threshold > 20 low)
 *
 * @author 	Aeron Lahoylahoy
 * @date   	June 27, 2024
 */


#include "button_functions.h"

/*
* @brief Function that initializes a GPIO pin as input with EXTI interrupt.
* @param Port: GPIO port (e.g., PORTA, PORTB, etc.)
* @param Pin: GPIO pin number (0-15)
*/
void buttonInit(GPIO_TypeDef *Port, int Pin)
{
	/*Variables*/
	int array = 0;
	int line = 0;
	int shift = 0;

	if(Port == PORTA){
		RCC->AHB1ENR |= Aclk;		/*Set GPIOx Clock*/
		line = 0x0;
	}
	else if(Port == PORTB){
		RCC->AHB1ENR |= Bclk;		/*Set GPIOx Clock*/
		line = 0x1;
	}
	else if(Port == PORTC){
		RCC->AHB1ENR |= Cclk;		/*Set GPIOx Clock*/
		line = 0x2;
	}
  RCC->APB2ENR |= 0x4000;		/*Set Interrupt Clock*/

	/*GPIO Initilization*/
	Port->MODER &= ~(0b11 << (Pin * 2));  /*Clear*/
	Port->MODER |= (0b00 << (Pin * 2));		/*Input*/
	Port->PUPDR |= (0b01 << (Pin * 2));		/*Pull up*/


	/*Define array based on Pin*/
	if(Pin < 4) array = 0;
	else if((Pin < 8) && (Pin > 3)) array = 1;
	else if((Pin < 12) && (Pin > 7)) array = 2;
	else if((Pin < 16) && (Pin > 11)) array = 3;

	/*Interrupts Initialization*/
	if ((Pin == 0) | (Pin == 4) | (Pin == 8) | (Pin == 12)) shift  = 0;
	else if ((Pin == 1) | (Pin == 5) | (Pin == 9) | (Pin == 13)) shift  = 1;
	else if ((Pin == 2) | (Pin == 6) | (Pin == 10) | (Pin == 14)) shift  = 2;
	else if ((Pin == 3) | (Pin == 7) | (Pin == 11) | (Pin == 15))  shift  = 3;

	SYSCFG->EXTICR[array] &= ~(0xF << (shift * 4));
	SYSCFG->EXTICR[array] |= (line << (shift * 4));
	EXTI->IMR |= (0b1 << Pin);
	EXTI->FTSR |= (0b1 << Pin);
	//EXTI->RTSR |= (0b1 << Pin);	/* Include for turn signal switch */

	/*Enable Interrupts*/
	if(Pin == 0) NVIC_EnableIRQ(EXTI0_IRQn);
	else if(Pin == 1) NVIC_EnableIRQ(EXTI1_IRQn);
	else if(Pin == 2) NVIC_EnableIRQ(EXTI2_IRQn);
	else if(Pin == 3) NVIC_EnableIRQ(EXTI3_IRQn);
	else if(Pin == 4) NVIC_EnableIRQ(EXTI4_IRQn);
	else if((Pin < 10) && (Pin > 4)) NVIC_EnableIRQ(EXTI9_5_IRQn);
	else if((Pin < 16) && (Pin > 9)) NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/*
 * @brief Function that debounces a button by checking the IDR of the inputted Port and Pin parameters.
 * @param Port: GPIO port (e.g., PORTA, PORTB, etc.)
 * @param Pin: GPIO pin number (0-15)
 * @return 1 if button is pressed, 0 otherwise
 */
int debounceButton(GPIO_TypeDef *Port, int Pin)
{
	int count = 0;

	/* Checks Port and Pin parameter IDR 30 times */
	for (int i = 0; i < 30; i++)
	{
		/* Checks if Port and Pin paramter is low  */
		if (!(Port->IDR & (0b1 << Pin)))
			count++;
		// delayMsCounter(1);
	}

	/* Valid reading only if 20 good IDR values */
	if (count > 20)
		return 1;
	return 0;
}
