/*
 * @file main.c
 * @brief Main application file for embedded car dashboard
 * @details This file contains the main application logic for the STM32F4xx-based car dashboard system.
 *
 * @author: Aeron Lahoylahoy
 * @date: June 27, 2024
 */
/* STM32 Core */
#include "stm32f4xx.h"

/* Standard Libraries */
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "stdlib.h"

/* Project Port and Pin Defines */
#include "port_pin_define.h"

/* Display */
#include "iLI9341.h"
#include "font_freemono_mono_bold_24.h"
#include "display.h"

/* Controls: turn signal, miles, bluetooth buttons, reset*/
#include "controls.h"

/* Peripherals and Drivers */
#include "i2c_master.h"
#include "button_functions.h"
#include "rtc.h"
#include "sonar.h"
#include "rotary_encoder.h"
#include "eeprom.h"
#include "speed_sensor.h"

int main(void)

{

	/* System Init */
	__disable_irq();

	masterConfig(); // I2C Master Config
	turnSignalSWInit(); // Turn Signal Switch Init
	SysTick_Init(); // SysTick Init
	Display_Init(); // LCD Display Init
	sonarInit(); // Sonar Init
	rotaryEncoderInit(); // Rotary Encoder Init
	resetButtonInit(); // Reset Button Init

	/* Mile */
	rpmReaderInit();
	hallSensorInit();

	/* Timer 7 */
	TIM7_Init();

	/* Bluetooth */
	bluetoothButtonInit();
	bluetoothEnableInit();

	/* Display */
	menuButtonInit();

	/* SONAR */
	sonarInit();

	/* Set interrupt priority */
	NVIC_SetPriority(EXTI9_5_IRQn, 1);

	__enable_irq();

	displayMenu();

	/* Wait for image to display on LCD */
	for (int i = 0; i < 1000; i++)
		;

	getCumulativeMiles();

	if (traveledMiles == 0)
	{
		I2C1_byteWrite(slave, 0, 0x10);
	}
	else
	{

		for (int i = 0; i < traveledMiles; i++)
		{
			I2C1_byteWrite(slave, 0, 0x11);
			for (int j = 0; j < 500; j++)
				;
		}
	}

	/* Main Loop */
	while (1)
	{
		switch (state)
		{
		case MENUSTATE:

			/* read Sonar */
			checkWarningSignal();

			/* read mph */
			readMiles();

			if (menuFlag)
			{
				Fill_Screen(BLACK);
				displayMenu();
				menuFlag = 0;
			}

			if (blinkMenuFlag)
			{
				// chooseMenu = 0;
				blinkMenu();

				/* Move to the chosen menu */
				if (blinkMenuFlag == 2)
				{
					displayFlag = 1;
					if (count == 0)
						state = TIMESTATE;
					if (count == 1)
						state = DATESTATE;
					if (count == 2)
						state = TEMPSTATE;
					blinkMenuFlag = 0;
				}
				else
					state = MENUSTATE;
			}
			break;

		case TIMESTATE:
			/* ADD RESET */
			if (resetFlag)
			{
				count = 0;
				resetFlag = 0;
			}

			/* Read Sonar */
			checkWarningSignal();

			/* read mph */
			readMiles();

			/* do this once  */
			if (displayFlag)
			{
				Fill_Screen(BLACK);
				getTime();
				printToLCD(TIME);
				displayFlag = 0;
			}

			/* Change time */
			if (changeTimeFlag)
			{
				changeTime();
			}
			else
			{

				/* Update time */
				if (trigger)
				{
					timeCount++;
					if (timeCount == 4)
					{
						getTime();
						printToLCD(TIME);
						timeCount = 0;
					}
					trigger = 0;
				}
			}

			/* Reset for Menu state */
			if (menuScreen)
			{
				state = MENUSTATE;
				blinkMenuFlag = 0;
				chooseMenu = 0;
				menuScreen = 0;
				count = 0;
				menuFlag = 1;
			}
			else
				state = TIMESTATE;

			break;

		case DATESTATE:
			/* ADD RESET */
			if (resetFlag)
			{
				count = 0;
				resetFlag = 0;
			}

			/* Check Sonar */
			checkWarningSignal();

			/* read mph */
			readMiles();

			if (displayFlag)
			{
				Fill_Screen(BLACK);
				getTime();
				printToLCD(DATE);
				displayFlag = 0;
			}

			/* Change time */
			if (changeDateFlag)
			{
				changeDate();
			}
			else
			{

				/* Update Date */
				if (trigger)
				{
					timeCount++;
					if (timeCount == 4)
					{
						getTime();
						printToLCD(DATE);
						timeCount = 0;
					}
					trigger = 0;
				}
			}

			/* Reset for Menu state */
			if (menuScreen)
			{
				state = MENUSTATE;
				blinkMenuFlag = 0;
				chooseMenu = 0;
				count = 0;
				menuFlag = 1;
				menuScreen = 0;
			}
			else
				state = DATESTATE;

			break;

		case TEMPSTATE:
			/* ADD RESET */
			if (resetFlag)
			{
				count = 0;
				resetFlag = 0;
			}

			/* Read Sonar */
			checkWarningSignal();

			/* read mph */
			readMiles();

			if (displayFlag)
			{
				Fill_Screen(BLACK);
				getTime();
				printToLCD(TEMP);
				displayFlag = 0;
			}

			/* Update time */
			if (trigger)
			{
				timeCount++;
				if (timeCount == 4)
				{
					readTemp();
					printToLCD(TEMP);
					timeCount = 0;
				}
				trigger = 0;
			}

			/* Reset for Menu state */
			if (menuScreen)
			{
				CW = 0;
				CCW = 0;
				state = MENUSTATE;
				blinkMenuFlag = 0;
				chooseMenu = 0;
				count = 0;
				menuFlag = 1;
				menuScreen = 0;
			}
			else
				state = TEMPSTATE;
			break;
		}
	}
}