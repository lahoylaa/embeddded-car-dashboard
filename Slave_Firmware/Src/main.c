/*
* @file main.c
* @brief Main file for Slave MCU
* @details This is the main file for the Slave MCU that handles I2C communication,
* 
* @author:	Aeron Lahoylahoy
* @date:	June 27, 2024
*/
#include "stm32f4xx.h"
#include "port_pin_define.h"
#include "i2c_slave.h"
#include "motor.h"
#include "seven_segment.h"
#include "watchdog.h"
#include "led.h"
#include "math.h"
#include "stdlib.h"

int main(void)
{
	__disable_irq();

	/*SLAVE INITIALIZATION*/
	slaveConfig(); /*Slave Initialization*/
	ledInit();	   /*LEDs Initialization*/

	/*MASTER INITIALIZATION*/
	/* Initilization */
	motorInit();			   // Motor intilization with GPIOB as outputs
	SysTick_Init_Interrupt(0); // SysTick initilization as interrupt set to 10

	/* SEVEN SEG */
	SPI1_Init();
	Seven_Segment_Init(); // Initiliazes the Seven Segment Display
	setDefaultVal();	  // Sets the default values for all 8 seven segments (0)
	Seven_Segment_Write();

	buttonInit(WDG_PORT, WDG_PIN);

	TIM7_Init();

	__enable_irq();

	SysTick_Init_Interrupt(5);

	speedCount1 = 400;
	speedCount2 = 400;

	while (speedCount1 > 0)
	{
		reverseFlag1 = 1;
	}

	reverseFlag1 = 0;
	speedCount2 = 400;

	while (speedCount2 > 0)
	{
		reverseFlag2 = 1;
	}

	reverseFlag2 = 0;
	speedCount1 = 0;
	speedCount2 = 0;

	while (1)
	{
		get_data();

		if (dataFlag == 1)
		{
			/* MESSAGE # 3 and 5: Motor 1 and 2 */
			if ((data & 0xF0) == 0x30)
			{
				for (int i = 0; i < 10; i++)
				{
					if ((data & 0x0F) == i)
					{
						// SysTick_Init_Interrupt(40);	/*Set Speed Movement*/
						/*Display Seven Segment*/
						SPI1_Write(seg1Address, val[i]);
						SPI1_Write(seg2Address, val[5]);
						SPI1_Write(seg5Address, val[speedMPH[i] & 0xF0 >> 4]);
						SPI1_Write(seg6Address, val[speedMPH[1] & 0x0F]);
						/*Set Position of Motors*/
						speedIndex = i;
						mph = speedMPH[speedIndex] * 4;
						rpm = speedMPH[speedIndex] * 3.5;

						SysTick_Init_Interrupt(5);

						/*Speedometer - RPM*/

						if (abs(prevRpm - rpm) == 0)
						{
							forwardFlag1 = 0;
							reverseFlag1 = 0;
						}
						else
						{
							if ((rpm > prevRpm))
								forwardFlag1 = 1;
							else
								reverseFlag1 = 1;
						}

						prevMph = mph;
						prevRpm = rpm;
						break;
					}
				}
			}
			if ((data & 0xF0) == 0x50)
			{
				for (int i = 0; i < 10; i++)
				{
					if ((data & 0x0F) == i)
					{
						SysTick_Init_Interrupt(40); /*Set Speed Movement*/
						/*Display Seven Segment*/
						SPI1_Write(seg1Address, val[i]);
						SPI1_Write(seg2Address, val[5]);
						SPI1_Write(seg5Address, val[speedMPH[i] & 0xF0 >> 4]);
						SPI1_Write(seg6Address, val[speedMPH[1] & 0x0F]);
						/*Set Position of Motors*/
						speedIndex = 10 + i;
						mph = speedMPH[speedIndex] * 4;
						rpm = speedMPH[speedIndex] * 3.5;

						SysTick_Init_Interrupt(5);

						if (abs(prevRpm - rpm) == 0)
						{
							forwardFlag1 = 0;
							reverseFlag1 = 0;
						}
						else
						{
							if ((rpm > prevRpm))
								forwardFlag1 = 1;
							else
								reverseFlag1 = 1;
						}
						break;
					}
				}
			}
			/*MESSAGE #4: Turn Signal */
			if ((data & 0xF0) == 0x40)
			{
				TIM7->CR1 |= 0b1;
				/* Turn Right */
				if ((data & 0x0F) == 0x01)
				{
					/* Debug Purposes */
					SPI1_Write(seg1Address, val[1]);
					SPI1_Write(seg2Address, val[4]);

					turnRightFlag = 1;
					turnLeftFlag = 0;
				}

				/* Turn Left */
				else if ((data & 0x0F) == 0x02)
				{
					/* Debug Purposes */
					SPI1_Write(seg1Address, val[2]);
					SPI1_Write(seg2Address, val[4]);
					turnLeftFlag = 1;
					turnRightFlag = 0;
				}

				else
				{
					SPI1_Write(seg1Address, val[3]);
					SPI1_Write(seg2Address, val[4]);
					turnRightFlag = 0;
					turnLeftFlag = 0;
					turnSignalOff();
				}
			}

			/* MESSAGE #6: Sonar */
			if ((data & 0xF0) == 0x60)
			{
				TIM7->CR1 |= 0b1;
				/* Warning Signal */
				if ((data & 0x0F) == 0x01)
				{
					warningFlag = 1;
				}

				if ((data & 0x0F) == 0x00)
				{
					warningOff();
					warningFlag = 0;
				}
			}

			dataFlag = 0;
			data = 0;
		}

		/* Turn Signal */
		turnSignalOn();

		/* Warning */
		warningOn();
	}
}
