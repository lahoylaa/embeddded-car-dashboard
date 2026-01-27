/*
 * @file speed_sensor.h
 * @brief Speed Sensor module for RPM and MPH calculation using Hall Effect sensor
 * @details This module provides functions to initialize the speed sensor,
 * 
 * @author: Aeron Lahoylahoy
 * @date: June 27, 2024
 */

#include "speed_sensor.h"

/* Variables */
int hallEffectFlag = 0;
float rpm;
float mph;
float cumulativeMiles = 0;
float rpmAverage = 0;
float rpmSum = 0;
int rpmSumCounter = 0;

/*
 * @brief Function to detect hall effect sensor signal
 * @param None
 * @return None
 */
void speedSensorHallDetected(void){
	hallEffectFlag++;
	if(hallEffectFlag > 2) hallEffectFlag = 2;
}

/*
 * @brief Function to initialize the RPM reader
 * @param None
 * @return None
 */
void rpmReaderInit(void)
{

	RCC->AHB1ENR |= Bclk;
	GPIOB->MODER &= ~(0b11 << (6 * 2));
	GPIOB->MODER |= (0b01 << (6 * 2));

	GPIOB->AFR[0] &= ~0x0F000000;
	GPIOB->AFR[0] |= 0x02000000;

	RCC->APB1ENR |= 4;
	TIM4->PSC = 1600;
	// TIM4->ARR = 250;
	TIM4->CCMR1 = 0x30;
	TIM4->CCR1 = 0;
	TIM4->CCER |= 1;
	TIM4->CNT = 0;
	TIM4->CR1 = 0;
}

/*
 * @brief Function to calculate RPM
 * @param None
 * @return None
 */
void calculateRPM(void){
	double timeElapsed = 0;
	float rpmPrev = rpm;
	rpmSumCounter++;

	timeElapsed = endTime;

	if (timeElapsed == 0)
	{
		rpmAverage = 0;
		calculateMPH();
	}
	else
	{

		rpm = 60 / ((timeElapsed / 10000) * 2);
	}
	rpmSum = rpm + rpmSum;

	if (rpmSumCounter == 7)
	{
		rpmAverage = rpmSum / rpmSumCounter;
		calculateMPH();
		rpmSum = 0;
		rpmSumCounter = 0;
	}
}

/*
 * @brief Function to calculate MPH
 * @param None
 * @return None
 */
void calculateMPH(void){
	static int sumCounter;
	float mphPrev = mph;

	mph = ((rpmAverage * 2 * 3.145 * 1.5 * 60) / 5280);

	if (mph > 100)
		mph = mphPrev;
}

/*
 * @brief Function to update cumulative miles
 * @param None
 * @return None
 */
void updateCumulativeMiles(void){
	cumulativeMiles = ((mph / 3600) + cumulativeMiles);
}

