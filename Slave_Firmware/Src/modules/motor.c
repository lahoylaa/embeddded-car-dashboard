/*
* @file motor.c
* @brief Motor module for controlling stepper motors
* @details This module provides functions to initialize and control stepper motors
*
*  @author: Aeron Lahoylahoy
*  @date:   June 27, 2024
*/
#include "stm32f4xx.h"
#include "motor.h"

//									0 1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 
int speedMPH[20] = {0,5,10,15,20,25,30,35,40,50,55,60,65,70,75,80,85,90,95,100};
//									0 1  2  3  4  5  6  7  8  9   10  11  12  13  14  15  16  17 18 19 				
int speedRPM[20] = {0,12,23,35,47,58,70,82,93,117,128,140,152,163,175,187,198,210,222,233};
int speedIndex = 0;
int speedCount1 = 0;
int speedCount2 = 0;
int forwardFlag1 = 0;
int reverseFlag1 = 0;
int forwardFlag2 = 0;
int reverseFlag2 = 0;
int mph;
int rpm;
int prevMph = 0;
int prevRpm = 0;
int current = STEP4;

/*
* @brief Function that moves the speedometer motor based on speed input
* @param speed: Speed value to move the motor to
* @return None
*/
void moveSpeedometer(int speed){
	static int lastSpeed;
	
	if(speed > lastSpeed){
		forwardControl_M1(speed);
	}
	
	else if(speed < lastSpeed){
		reverseControl_M1(speed);
	}
	else forwardControl_M1(0);
	
	lastSpeed = speed;
	
}

/*
* @brief Function that initializes the motor GPIO pins
* @param None
* @return None
*/
void motorInit(void)
{
	// Set GPIOx Clock
	RCC->AHB1ENR |= CLOCK_MOTOR | CLOCK_MOTOR1;
	/*Motor 1*/
	MOTOR_PORT1->MODER &= ~(0b11 << (MOTOR1_IN1 * 2)) | (0b11 << (MOTOR1_IN2 * 2)) | (0b11 << (MOTOR1_IN3 * 2)) | (0b11 << (MOTOR1_IN4 * 2)); /*Clear*/
	MOTOR_PORT1->MODER |= (0b01 << (MOTOR1_IN1 * 2)) | (0b01 << (MOTOR1_IN2 * 2)) | (0b01 << (MOTOR1_IN3 * 2)) | (0b01 << (MOTOR1_IN4 * 2));	 /*Output*/
	/*Motor 2*/
	MOTOR_PORT2->MODER &= ~(0b11 << (MOTOR2_IN1 * 2)) | (0b11 << (MOTOR2_IN2 * 2)) | (0b11 << (MOTOR2_IN3 * 2)) | (0b11 << (MOTOR2_IN4 * 2)); /*Clear*/
	MOTOR_PORT2->MODER |= (0b01 << (MOTOR2_IN1 * 2)) | (0b01 << (MOTOR2_IN2 * 2)) | (0b01 << (MOTOR2_IN3 * 2)) | (0b01 << (MOTOR2_IN4 * 2));	 /*Output*/
}

/*
* @brief Function that controls forward movement of Motor 1
* @param steps: Number of steps for the motor to move
* @return None
*/
void forwardControl_M1(int steps)
{
	//steps = steps * 40;
	
	for (int i = 0; i < steps; i++)
	{
		// Clear all INT pins on motor
		MOTOR_PORT1->ODR &= ~0x0F << MOTOR1_IN1;

		switch (current)
		{
		case STEP1:
			current = STEP2;
			break;

		case STEP2:
			current = STEP3;
			break;

		case STEP3:
			current = STEP4;
			break;

		case STEP4:
			current = STEP1;
			break;
		}

		// Set current step pins on
		MOTOR_PORT1->ODR |= current << MOTOR1_IN1;
	}
}

/*
* @brief Function that controls reverse movement of Motor 1
* @param steps: Number of steps for the motor to move
* @return None
*/
void reverseControl_M1(int steps)
{	
	for (int i = 0; i < steps; i++)
	{
		// Clear all INT pins on motor
		MOTOR_PORT1->ODR &= ~0x0F << MOTOR1_IN1;

		switch (current)
		{
		case STEP1:
			current = STEP4;
			break;

		case STEP2:
			current = STEP1;
			break;

		case STEP3:
			current = STEP2;
			break;

		case STEP4:
			current = STEP3;
			break;
		}

		// Set current step pins on
		MOTOR_PORT1->ODR |= current << MOTOR1_IN1;
	}
}

/*
* @brief Function that controls forward movement of Motor 2
* @param steps: Number of steps for the motor to move
* @return None
*/
void forwardControl_M2(int steps)
{	
	for (int i = 0; i < steps; i++)
	{
		// Clear all INT pins on motor
		MOTOR_PORT2->ODR &= ~0x0F << MOTOR2_IN1;

		switch (current)
		{
		case STEP1:
			current = STEP2;
			break;

		case STEP2:
			current = STEP3;
			break;

		case STEP3:
			current = STEP4;
			break;

		case STEP4:
			current = STEP1;
			break;
		}

		// Set current step pins on
		MOTOR_PORT2->ODR |= current << MOTOR2_IN1;
	}
}

/*
* @brief Function that controls reverse movement of Motor 2
* @param steps: Number of steps for the motor to move
* @return None
*/
void reverseControl_M2(int steps)
{

	for (int i = 0; i < steps; i++)
	{
		// Clear all INT pins on motor
		MOTOR_PORT2->ODR &= ~0x0F << MOTOR2_IN1;

		switch (current)
		{
		case STEP1:
			current = STEP4;
			break;

		case STEP2:
			current = STEP1;
			break;

		case STEP3:
			current = STEP2;
			break;

		case STEP4:
			current = STEP3;
			break;
		}

		// Set current step pins on
		MOTOR_PORT2->ODR |= current << MOTOR2_IN1;
	}

}

/*
* @brief Systick Interrupt Initilization (1 millisecond) takes a number (in ms) to set up systick 
* @param n: Number of milliseconds for the SysTick timer
* @return None
*/
void SysTick_Init_Interrupt(int n)
{
	SysTick->CTRL = 0;
	SysTick->LOAD = n * 16000; /* 1 MilliSeconds @ 16Mhz */
	SysTick->VAL = 0;
	SysTick->CTRL = 0x07; /* Enable Systick, interrupt, 16Mhz */
}

/*
* @brief Systick Interrupt Handler controls the motor to move one step
* @param None
* @return None
*/
void SysTick_Handler(void)
{
	
	/*Controlling First Motor*/
	if((speedCount1 > rpm) && forwardFlag1){	/*RPM*/
		forwardFlag1 = 0;
		forwardFlag2 = 1;
	}
	
	if((speedCount2 > mph) && forwardFlag2){	/*MPH*/
		forwardFlag2 = 0;
	}
	
	if((speedCount1 < rpm) && reverseFlag1){	/*RPM*/
		reverseFlag1 = 0;
		reverseFlag2 = 1;
	}
	
	if((speedCount2 < mph) && reverseFlag2){	/*MPH*/
		reverseFlag2 = 0;
	}
	
	if(forwardFlag1){
		speedCount1++;
		forwardControl_M1(1);
	}
	if(forwardFlag2){
		speedCount2++;
		forwardControl_M2(1);
	}
	
	if(reverseFlag1){
		speedCount1--;
		reverseControl_M1(1);
	}
	if(reverseFlag2){
		speedCount2--;
		reverseControl_M2(1);
	}
}