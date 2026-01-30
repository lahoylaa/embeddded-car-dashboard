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
int bluetoothCountFlag = 0;
int bluetoothEnable = 0;
int bluetoothDisplay = 0;

int turnSignal = NO_TURN;
int turnFlag = 0;

/* Watch Dog */
int watchDogFlag = 0;
int flag = 0;

/* Photosensor */
int adcVal;
float voltage;
const int max_adc_value = 4095; /*ADC 12-bit resolution*/

/* Menu Display */
int menuButtonFlag = 0;

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
 * @brief Function that reads the photosensor value using ADC, calculates voltage, and adjusts LED brightness.
 * @param None
 * @return None
 */
void read_photosensor(void)
{

	ADC1->CR2 |= 0x40000000; /*Starts Conversion*/
	while (!(ADC1->SR & 2))
		;
	adcVal = ADC1->DR;

	/* watch dog check */
	watchDogCheck();

	/*Calculate Voltage*/
	voltage = (adcVal * 3.3) / max_adc_value;
	ledControl();
}

/*
 * @brief Function that initializes the photosensor GPIO pin and ADC.
 * @param None
 * @return None
 */
void photosensorInit(void)
{
	RCC->AHB1ENR |= PHOTOSENSOR_CLK;
	PHOTOSENSOR_PORT->MODER &= ~(0b11 << (PHOTOSENSOR_PIN * 2)); /*Clear*/
	PHOTOSENSOR_PORT->MODER |= (0b11 << (PHOTOSENSOR_PIN * 2));	 /*Analog Mode*/

	/*ADC1*/
	RCC->APB2ENR |= 0x100; /*Enable ADC1*/
	ADC1->CR1 |= 0x1;	   /*0b0001 AIN1 - 12-bit resolution*/
	ADC1->CR2 |= 0x0;	   /*Single Conversion Mode, Right Alignment*/
	ADC1->SQR3 |= 0x1;	   /*Conversion Sequence at CH1*/
	ADC1->CR2 |= 1;		   /*Enable ADC*/
}

/*
 * @brief Function that initializes the LED GPIO pin and Timer for PWM control.
 * @param None
 * @return None
 */
void ledInit(void)
{

	/* GPIO Port A Pin 5 PWM Output Initilization */
	RCC->AHB1ENR |= LED_CLK;
	LED_PORT->MODER &= ~(0b11 << (LED_PIN * 2)); // Clear
	LED_PORT->MODER |= (0b10 << (LED_PIN * 2));	 // Set AF

	/* Initilize TIM5 CH 1 */
	// AFR[0] = includes Pins 0-7
	// AFR[1] = includes PIns 8-15
	// Each Pin has 4 bits to set which AFR 0-15
	LED_PORT->AFR[0] &= ~0x00F00000;
	LED_PORT->AFR[0] |= 0x00200000; /*0b0001 = 1 (AF1) on Pin 5*/

	/* Enable TIM2 Clock */
	RCC->APB1ENR |= (0b1 << 1); /*TIM3 Clock*/
	TIM3->PSC = 16 - 1;			/*Scale down to 1MHz*/
	TIM3->ARR = 10000 - 1;		/*Reload count 10000*/

	/* Set PWM */
	TIM3->CNT = 0;			/*Clear*/
	TIM3->CCMR1 |= 0x6000;	/*PWM Mode 1 (0b110)*/
	TIM3->CCER |= (1 << 4); /*Enable PWM output*/
	TIM3->CCR2 = 10000;		/*Pulse width (CCR1/ARR) = Duty Cycle*/
	TIM3->CR1 |= (1 << 0);	/*Enable Timer*/
}

/*
 * @brief Function that initializes the Watchdog Timer.
 * @param None
 * @return None
 */
void watchDogInit(void)
{

	IWDG->KR |= 0x5555;	   // config key for watchdog timer
	IWDG->PR |= 0x01;	   // set 8 divider (1 second)
	IWDG->RLR |= 1024 - 1; // resets 1 second
	IWDG->KR |= 0xCCCC;	   // enable watchdog timer
}

/*
 * @brief Function that initializes the Watchdog button GPIO pin.
 * @param None
 * @return None
 */
void watchDogButtonInit(void)
{
	buttonInit(WATCH_DOG_PORT, WATCH_DOG_PIN);
}

/*
 * @brief Function that checks the watchdog flag and resets the watchdog timer.
 * @param None
 * @return None
 */
void watchDogCheck(void)
{
	if (watchDogFlag)
	{
		I2C1_byteWrite(slave, 0, 0x91);
		if (!flag)
		{
			IWDG->KR |= 0xAAAA; // enable watchdog timer
			flag = 1;
		}
	}
	else
	{
		IWDG->KR |= 0xAAAA; // enable watchdog timer
	}
}

/*
 * @brief Interrupt handler for EXTI lines 15 to 10.
 * @param None
 * @return None
 */
void EXTI15_10_IRQHandler(void)
{

	/* BLUETOOTH */
	if (EXTI->PR & (0b1 << BLUETOOTH_BUTTON_PIN))
	{
		bluetoothFlag = 1;
		bluetoothCountFlag = 1;
		EXTI->PR |= (0b1 << BLUETOOTH_BUTTON_PIN);
	}

	/* MENU On Display */
	if (EXTI->PR & (0b1 << MENU_BUTTON_PIN))
	{
		menuButtonFlag = 1;
		EXTI->PR |= (0b1 << MENU_BUTTON_PIN);
	}

	/* Watch Dog */
	if (EXTI->PR & (0b1 << WATCH_DOG_PIN))
	{
		if (debounceButton(WATCH_DOG_PORT, WATCH_DOG_PIN))
		{
			watchDogFlag = 1;
		}

		EXTI->PR |= (0b1 << WATCH_DOG_PIN);
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
		turnFlag = 1;
		EXTI->PR |= (0b1 << TURN_RIGHT_PIN);
	}

	if (EXTI->PR & (0b1 << TURN_LEFT_PIN))
	{
		turnFlag = 1;
		EXTI->PR |= (0b1 << TURN_LEFT_PIN);
	}

	/* Reset Button to Reset the odometer accumulated miles */
	if (EXTI->PR & (0b1 << RESET_BUTTON_PIN))
	{
		resetMilesFlag = 1;
		EXTI->PR |= (0b1 << RESET_BUTTON_PIN);
	}

	/* Hall Effect Sensor */
	if (EXTI->PR & (0b1 << 8))
	{

		if (debounceButton(PORTA, PIN8))
		{
			hallEffectFlag++;
			if (hallEffectFlag > 2)
				hallEffectFlag = 2;
		}
		EXTI->PR |= (0b1 << 8);
	}

	/* Miles */
	if (EXTI->PR & (0b1 << 8))
	{
		speedSensorHallDetected();
		EXTI->PR |= (0b1 << 8);
	}
}
