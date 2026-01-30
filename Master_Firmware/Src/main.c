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
#include "date.h"
#include "time.h"
#include "temp.h"
#include "logo.h"

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

	/* Sonar */
	sonarInit();

	/* Watch Dog Reset */
	watchDogInit();

	/* Photosensor */
	photosensorInit();
	ledInit();

	/* Set interrupt priority */
	NVIC_SetPriority(TIM7_IRQn, 1);
    NVIC_SetPriority(EXTI9_5_IRQn, 2);
    NVIC_SetPriority(EXTI15_10_IRQn, 3);

	__enable_irq();

	Rotate_Display(2);
	displayLogo();

	/* Read Previous Data */
	readSavedData();

	/* Watch Dog Reset */
	watchDogInit();

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

			/* watch dog check */
			watchDogCheck();

			/* Photo Sensor */
			read_photosensor();

			/* Bluetooth */
			displayBluetooth(ICON);

			if (menuFlag)
			{
				// Fill_Screen(BLACK);
				Fill_Rect(0, 0, 240, 225, BLACK);
				displayMenu();
				menuFlag = 0;
			}

			if (blinkMenuFlag)
			{
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

			/* Save state into EEPROM */
			I2C1_byteWrite(mileEEPROM, 1, state);
			sendMessages();

			break;

		case TIMESTATE:

			/* Read sonar */
			checkWarningSignal();

			/* Read Mph */
			readMiles();

			/* Watch dog check */
			watchDogCheck();

			/* Photo Sensor */
			read_photosensor();

			/* Bluetooth */
			displayBluetooth(ICON);

			/* Read Sonar */
			checkWarningSignal();

			/* read mph */
			readMiles();

			if (displayFlag)
			{
				Fill_Rect(0, 0, 240, 225, BLACK);
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

			/* Save State into EEPROM */
			I2C1_byteWrite(mileEEPROM, 1, state);
			sendMessages();

			break;

		case DATESTATE:

			/* Check Sonar */
			checkWarningSignal();

			/* Read mph */
			readMiles();

			/* Watch dog cehck */
			watchDogCheck();

			/* Photo Sensor */
			read_photosensor();

			/* Bluetooth */
			displayBluetooth(ICON);

			if (displayFlag)
			{
				Fill_Rect(0, 0, 240, 225, BLACK);
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

			/* Save State into EEPROM */
			I2C1_byteWrite(mileEEPROM, 1, state);
			sendMessages();

			break;

		case TEMPSTATE:

			/* Read Sonar */
			checkWarningSignal();

			/* Read mph */
			readMiles();

			/* Watch dog check */
			watchDogCheck();

			/* Photo Sensor */
			read_photosensor();

			/* Bluetooth */
			displayBluetooth(ICON);

			if (displayFlag)
			{
				Fill_Rect(0, 0, 240, 225, BLACK);
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

			/* Save State into EEPROM */
			I2C1_byteWrite(mileEEPROM, 1, state);	
			sendMessages();
			
			break;
		}
	}
}