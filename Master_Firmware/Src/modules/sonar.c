/*
 * @file sonar.c
 * @brief Sonar module for distance measurement using ultrasonic sensor
 * @details This module provides functions to initialize the sonar sensor,
 * 
 * @author: Aeron Lahoylahoy
 * @date: June 27, 2024
 */
#include "stm32f4xx.h"
#include "sonar.h"
#include "i2c_master.h"

/* Sonar Global Variables */
double current = 0;
double last = 0;
double timeElapsed = 0;
double distance = 0;

int checkDistance = 0;

/*
 * @brief Function to check warning signal based on sonar distance
 * @param None
 * @return None
 */
void checkWarningSignal(void){
	readEcho();
	calculateDistance(timeElapsed);
	if (distance < 10){
		I2C1_byteWrite(slave, 0, 0x61);
	}
	else I2C1_byteWrite(slave, 0, 0x60);
}

/*
* @brief Function to read the echo from the sonar sensor
 * @param None
 * @return None
 */
void readEcho(void)
{
    // Wait for a rising edge
    while ((TIM3->SR & 2) == 0);
    current = TIM3->CCR1;         // Log the current timestamp of the rising edge
    timeElapsed = current - last; // difference from the last recorded reading
    last = current;               // set the current reading as the last reading
}

/*
 * @brief Function to calculate the distance based on the time elapsed
 * @param time Time elapsed in microseconds
 * @return None
 */
void calculateDistance(double time)
{
    double prev = distance;

    // Convert pulse width from time to distance
    distance = (0.034 * time) / 2; // cm

    // convert to inches
    distance = distance / 2.5;

    // Taking into account that the sonar has corrupted values below 1 inches
    distance = distance - 1;

    // limit the distance reading to positive distance
    if (distance < 0)
        distance = prev;

    // limit corrupted readings
    if (distance > 100)
        distance = prev;
}

/*
 * @brief Function to initialize the sonar sensor
 * @param None
 * @return None
 */
void sonarInit(void){

	sonarTrigger_Init();
	sonarEcho_Init();

}

/*
 * @brief Function to initialize the sonar trigger pin
 * @param None
 * @return None
 */
void sonarTrigger_Init(void){

	    /* GPIO PA5 PWM Output Initilization */
    RCC->AHB1ENR |= SONAR_TRIG_CLK;
    SONAR_TRIG_PORT->MODER &= ~(0b11 << (SONAR_TRIG_PIN * 2)); // Clear
    SONAR_TRIG_PORT->MODER |= (0b10 << (SONAR_TRIG_PIN * 2));  // Set Pin 5 to alternate

    /* Initilize TIM2 CH 1 */
    // AFR[0] = includes Pins 0-7
    // AFR[1] = includes PIns 8-15
    // Each Pin has 4 bits to set which AFR 0-15
    SONAR_TRIG_PORT->AFR[0] &= ~0x00F00000; // Clear
    SONAR_TRIG_PORT->AFR[0] |= 0x00100000;  // 0b0001 = 1 (AF1) on Pin 5

    /* Enable TIM3 Clocks */
    RCC->APB1ENR |= SONAR_TRIG_CLK;     // TIM2 Clock
    TIM2->PSC = 16 - 1;    // Scale down to 1Mhz
    TIM2->ARR = 65000 - 1; // Reload with a count of 650000 (65ms)

    /* Set PWM */
    TIM2->CNT = 0;          // Clear
    TIM2->CCMR1 |= 0x60;    // PWM Mode 1 (0b110)
    TIM2->CCER |= (1 << 0); // enable PWM output
    TIM2->CCR1 = 65000 - 5; // Pulse width (CCR1/ARR) = Duty Cycle
    TIM2->CR1 |= (1 << 0);  // Enable timer


}

/*
 * @brief Function to initialize the sonar echo pin
 * @param None
 * @return None
 */
void sonarEcho_Init(void){

	RCC->AHB1ENR |= SONAR_ECHO_CLK;		// Set Clock
	SONAR_ECHO_PORT->MODER &= ~(0b11 << (SONAR_ECHO_PIN * 2));		// Clear
	SONAR_ECHO_PORT->MODER |= (0b10 << (SONAR_ECHO_PIN * 2));			// Set Pin 4 Alternate Function

	/* Initilize TIM3 Channel 1 */
	SONAR_ECHO_PORT->AFR[0] &= ~0x000F0000;							// Clear
	SONAR_ECHO_PORT->AFR[0] |= 0x00020000;								// 0b0010 = 2 (AF2) on pin

	RCC->APB1ENR |= SONAR_ECHO_CLK;		// TIM3 Clock
	TIM3->PSC = 16 - 1;		// Scale down to 1Mhz
	TIM3->CCMR1 = 0xC1;		// CH1 Input Capture, sample/16 N = 8
	TIM3->CCER = 0x0B;		// Enable Capture both edges
	TIM3->CR1 = 1; 				// Enable TIM3

}
