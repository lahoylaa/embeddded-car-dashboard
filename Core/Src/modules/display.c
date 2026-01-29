/*
 * @file  	display.c
 * @brief 	Display module for menu display and user interface logic
 * @details Handles displaying:
 * 			- Bluetooth status
 * 			- Menu options (Time, Date, Temperature selection)
 * 			- Menu item blinking/highlighting based on rotary encoder input
 * 			- Real-time clock, date, and temperature display
 *
 * @author: Aeron Lahoylahoy
 * @date:   June 27, 2024
 */
#include "stm32f4xx.h"
#include "port_pin_define.h"
#include "RTC.h"
#include "Display.h"
#include "iLI9341.h"
#include "font_freemono_mono_bold_24.h"
#include "rotary_encoder.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "speed_sensor.h"
#include "controls.h"
#include "eeprom.h"
#include "i2c_master.h"

/* Time, Date, Temp Variables */
int arrayTimePos[50];
int arrayDatePos[50];
int arrayTempPos[50];

int timeFlag = 0;
int dateFlag = 0;
int tempFlag = 0;

int changeTimeFlag = 0;
int changeDateFlag = 0;
int changeTimeCount = 0;
int changeDateCount = 0;

/* Menu Variables */
int menuFlag = 0;
int blinkMenuFlag = 0;
int chooseMenu = 0;
int chooseTime = 0;
int chooseDate = 0;
int chooseTemp = 0;
int resetFlag = 0;
int displayFlag = 0;
int timeCount = 0;

int menuScreen = 0;

int trigger = 0;
int mileCounter = 0;

int state = MENUSTATE;

/*
 * @brief Function that displays Bluetooth status on the LCD.
 * @param n: 0 to display "Bluetooth", 1 to clear it
 * @return None
 */
void displayBluetooth(int n)
{
	char msg[50] = "Bluetooth";
	int offsetX = 0;

	switch (n)
	{
	case 0:
		for (int i = 0; i < strlen(msg); i++)
		{
			DrawCharS(30 + offsetX, (320 / 2) + 50, msg[i], WHITE, BLACK, 3);
			offsetX = offsetX + 20;
		}
		break;

	case 1:
		for (int i = 0; i < strlen(msg); i++)
		{
			DrawCharS(30 + offsetX, (320 / 2) + 50, msg[i], BLACK, BLACK, 3);
			offsetX = offsetX + 20;
		}
		break;
	}
}

/*
 * @brief Function that displays the menu options on the LCD.
 * @param None
 * @return None
 */
void displayMenu(void)
{
	char menu[50];
	int posInitial = (240 / 2) - 30;
	int posX = posInitial;
	int posY = 20;

	/* Display Time Menu */
	// Draw_String(posX, posY, "Time", WHITE, BLACK, &font_freemono_mono_bold_24);
	sprintf(menu, "Time");

	for (int i = 0; i < strlen(menu); i++)
	{
		DrawCharS(posX, posY, menu[i], WHITE, BLACK, 3);
		arrayTimePos[i] = posX;
		posX = posX + 25;
	}

	posX = posInitial;
	posY = posY + 50;

	/* Display Date Menu */
	sprintf(menu, "Date");

	for (int i = 0; i < strlen(menu); i++)
	{
		DrawCharS(posX, posY, menu[i], WHITE, BLACK, 3);
		arrayDatePos[i] = posX;
		posX = posX + 25;
	}

	posX = posInitial;
	posY = posY + 50;

	/* Display Temperature Menu */
	sprintf(menu, "Temp");

	for (int i = 0; i < strlen(menu); i++)
	{
		DrawCharS(posX, posY, menu[i], WHITE, BLACK, 3);
		arrayTempPos[i] = posX;
		posX = posX + 25;
	}

	posX = posInitial;
	posY = posY + 50;
}

/*
 * @brief Function that blinks the selected menu option on the LCD.
 * @param n: 0 for Time, 1 for Date, 2 for Temp
 * @return None
 */
void blinkDisplay(int n)
{
	char menu[50];

	switch (n)
	{
	case 0:
		sprintf(menu, "Time");
		if (blink == 1)
		{
			// Fill_Rect(arrayTimePos[0], 20, 100, 30, WHITE);
			for (int i = 0; i < strlen(menu); i++)
			{
				DrawCharS(arrayTimePos[i], 20, menu[i], WHITE, BLACK, 3);
			}
		}
		else
			Fill_Rect(arrayTimePos[0], 20, 100, 30, BLACK);
		break;

	case 1:
		sprintf(menu, "Date");
		if (blink == 1)
		{
			// Fill_Rect(arrayTimePos[0], 20, 100, 30, WHITE);
			for (int i = 0; i < strlen(menu); i++)
			{
				DrawCharS(arrayTempPos[i], 70, menu[i], WHITE, BLACK, 3);
			}
		}
		else
			Fill_Rect(arrayDatePos[0], 70, 100, 30, BLACK);
		break;

	case 2:
		sprintf(menu, "Temp");
		if (blink == 1)
		{
			// Fill_Rect(arrayTimePos[0], 20, 100, 30, WHITE);
			for (int i = 0; i < strlen(menu); i++)
			{
				DrawCharS(arrayTempPos[i], 120, menu[i], WHITE, BLACK, 3);
			}
		}
		else
			Fill_Rect(arrayTempPos[0], 120, 100, 30, BLACK);
		break;
	}
}

/*
 * @brief Function that prints the selected menu item's data on the LCD.
 * @param menu: Menu item to display (TIME, DATE, TEMP)
 * @return None
 */
void printToLCD(int menu)
{
	char msg[50];
	char timeMsg[50] = "TIME";
	char dateMsg[50] = "DATE";
	char tempMsg[50] = "TEMP";
	int offsetX = 0;

	switch (menu)
	{
	case TIME:

		/* Display Time Text */
		for (int i = 0; i < strlen(timeMsg); i++)
		{
			DrawCharS(50 + offsetX, (320 / 2) - 50, timeMsg[i], WHITE, BLACK, 5);
			offsetX = offsetX + 35;
		}

		offsetX = 0;

		/* Displays Time */
		if (ampmFlag)
		{
			sprintf(msg, "%d%d:%d%d PM", hourArray[0], hourArray[1], minArray[0], minArray[1]);
		}
		else
			sprintf(msg, "%d%d:%d%d AM", hourArray[0], hourArray[1], minArray[0], minArray[1]);

		for (int i = 0; i < strlen(msg); i++)
		{
			DrawCharS(50 + offsetX, 320 / 2, msg[i], WHITE, BLACK, 3);
			offsetX = offsetX + 20;
		}

		break;

	case DATE:

		/* Displays Date Text */
		for (int i = 0; i < strlen(dateMsg); i++)
		{
			DrawCharS(55 + offsetX, (320 / 2) - 50, dateMsg[i], WHITE, BLACK, 5);
			offsetX = offsetX + 35;
		}

		offsetX = 0;

		/* Display Date */
		sprintf(msg, "%d%d/%d%d/%d%d", monthArray[0], monthArray[1], dateArray[0], dateArray[1], yearArray[0], yearArray[1]);
		for (int i = 0; i < strlen(msg); i++)
		{
			DrawCharS(40 + offsetX, (320 / 2), msg[i], WHITE, BLACK, 3);
			offsetX = offsetX + 20;
		}
		break;

	case TEMP:

		/* Display Temp Text */
		for (int i = 0; i < strlen(tempMsg); i++)
		{
			DrawCharS(55 + offsetX, (320 / 2) - 50, tempMsg[i], WHITE, BLACK, 5);
			offsetX = offsetX + 35;
		}

		offsetX = 0;

		/* Display Temp  */
		sprintf(msg, "%d C", rtcTempArray[0]);
		for (int i = 0; i < strlen(msg); i++)
		{
			DrawCharS(80 + offsetX, (320 / 2), msg[i], WHITE, BLACK, 3);
			offsetX = offsetX + 25;
		}
		break;
	}
}

/*
 * @brief Function that manages blinking menu based on rotary encoder input.
 * @param None
 * @return None
 */
void blinkMenu(void)
{
	count = abs(CCW - CW);
	if (count > 3)
		count = 0;

	if (count == 0)
	{
		timeFlag = 1;
		if (tempFlag || dateFlag)
		{
			displayMenu();
			dateFlag = 0;
			tempFlag = 0;
		}

		blinkDisplay(0);
	}

	if (count == 1)
	{
		dateFlag = 1;
		if (timeFlag || tempFlag)
		{
			displayMenu();
			tempFlag = 0;
			timeFlag = 0;
		}
		blinkDisplay(1);
	}

	if (count == 2)
	{
		tempFlag = 1;
		if (dateFlag || timeFlag)
		{
			displayMenu();
			timeFlag = 0;
			dateFlag = 0;
		}
		blinkDisplay(2);
	}

	/* Reset */
	if (count == 3)
	{
		displayMenu();
	}
}

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

void TIM7_IRQHandler(void)
{
	/* DISPLAY */
	blink++;
	if (blink > 2)
		blink = 0;

	/* Update time */
	trigger = 1;

	/* bluetooth */
	if (bluetoothFlag)
	{
		displayBluetooth(0);

		bluetoothCounter++;
		if (bluetoothCounter == 4)
		{
			displayBluetooth(1);
			bluetoothCounter = 0;
			bluetoothFlag = 0;
		}
	}

	mileCounter++;
	/* Motor */
	if (mileCounter == 2)
	{
		sendMiles();
	}

	if (mileCounter == 4)
	{
		/* write to EEPROM */
		updateCumulativeMiles();
		if (cumulativeMiles > 1)
		{
			storeMiles();

			I2C1_byteWrite(slave, 0, 0x11);
			// I2C1_byteWrite(slave, 0, 0x11);
			cumulativeMiles = 0;
		}
		mileCounter = 0;
	}

	TIM7->SR &= ~0b1;
}

/*
 * @brief Function that changes the date on the RTC.
 * @param None
 * @return None
 */
void changeDate(void)
{
	char msg[50];
	int offsetX = 0;
	decimalBinary();

	/* change hour */
	switch (changeDateCount)
	{

	case 0:
		/* wait for something */
		break;

		/* HOUR */
	case 1:

		sprintf(msg, "%d%d", monthArray[0], monthArray[1]);

		if (blink == 1)
		{
			for (int i = 0; i < strlen(msg); i++)
			{
				DrawCharS(40 + offsetX, (320 / 2), msg[i], WHITE, BLACK, 3);
				offsetX = offsetX + 20;
			}
		}
		else
		{
			for (int i = 0; i < strlen(msg); i++)
			{
				DrawCharS(40 + offsetX, (320 / 2), msg[i], BLACK, BLACK, 3);
				offsetX = offsetX + 20;
			}
		}

		break;

	case 2:
		printDateToLCD(0);
		sprintf(msg, "%d%d", dateArray[0], dateArray[1]);

		if (blink == 1)
		{
			for (int i = 0; i < strlen(msg); i++)
			{
				DrawCharS(100 + offsetX, (320 / 2), msg[i], WHITE, BLACK, 3);
				offsetX = offsetX + 20;
			}
		}
		else
		{
			for (int i = 0; i < strlen(msg); i++)
			{
				DrawCharS(100 + offsetX, (320 / 2), msg[i], BLACK, BLACK, 3);
				offsetX = offsetX + 20;
			}
		}

		break;

	case 3:
		printDateToLCD(1);
		sprintf(msg, "%d%d", yearArray[0], yearArray[1]);

		if (blink == 1)
		{
			for (int i = 0; i < strlen(msg); i++)
			{
				DrawCharS(160 + offsetX, (320 / 2), msg[i], WHITE, BLACK, 3);
				offsetX = offsetX + 20;
			}
		}
		else
		{
			for (int i = 0; i < strlen(msg); i++)
			{
				DrawCharS(160 + offsetX, (320 / 2), msg[i], BLACK, BLACK, 3);
				offsetX = offsetX + 20;
			}
		}
		break;

	case 4:
		printDateToLCD(2);

		/* Send I2C message */
		sendChangeDate();
		changeDateCount = 0;
		changeDateFlag = 0;

		break;
	}
}

/*
 * @brief Function that prints the date to the LCD.
 * @param date The date to print (0 = month, 1 = day, 2 = year).
 * @return None
 */
void printDateToLCD(int date)
{
	char msg[50];
	int offsetX = 0;

	switch (date)
	{
	case 0:
		sprintf(msg, "%d%d", monthArray[0], monthArray[1]);
		for (int i = 0; i < strlen(msg); i++)
		{
			DrawCharS(40 + offsetX, 320 / 2, msg[i], WHITE, BLACK, 3);
			offsetX = offsetX + 20;
		}

		break;

	case 1:
		sprintf(msg, "%d%d", dateArray[0], dateArray[1]);
		for (int i = 0; i < strlen(msg); i++)
		{
			DrawCharS(100 + offsetX, 320 / 2, msg[i], WHITE, BLACK, 3);
			offsetX = offsetX + 20;
		}

		break;

	case 2:
		sprintf(msg, "%d%d", yearArray[0], yearArray[1]);

		for (int i = 0; i < strlen(msg); i++)
		{
			DrawCharS(160 + offsetX, 320 / 2, msg[i], WHITE, BLACK, 3);
			offsetX = offsetX + 20;
		}
		break;
	}
}

/*
 * @brief Function that sends the changed date to the RTC.
 * @param None
 * @return None
 */
void sendChangeDate(void)
{

	char msgMonth = (monthArray[0] << 4) | monthArray[1];
	char msgDate = (dateArray[0] << 4) | dateArray[1];
	char msgYear = (yearArray[0] << 4) | yearArray[1];

	I2C1_byteWrite(rtcAddress, monthM, msgMonth);
	I2C1_byteWrite(rtcAddress, dateM, msgDate);
	I2C1_byteWrite(rtcAddress, yearM, msgYear);
}

/*
 * @brief Function that changes the time on the RTC.
 * @param None
 * @return None
 */
void changeTime(void)
{
	char msg[50];
	int offsetX = 0;
	decimalBinary();

	/* change hour */
	switch (changeTimeCount)
	{

	case 0:
		/* wait for something */
		break;

		/* HOUR */
	case 1:

		sprintf(msg, "%d%d", hourArray[0], hourArray[1]);

		if (blink == 1)
		{
			for (int i = 0; i < strlen(msg); i++)
			{
				DrawCharS(50 + offsetX, 320 / 2, msg[i], WHITE, BLACK, 3);
				offsetX = offsetX + 20;
			}
		}
		else
		{
			for (int i = 0; i < strlen(msg); i++)
			{
				DrawCharS(50 + offsetX, 320 / 2, msg[i], BLACK, BLACK, 3);
				offsetX = offsetX + 20;
			}
		}

		break;

	case 2:
		printTimeToLCD(0);
		sprintf(msg, "%d%d", minArray[0], minArray[1]);

		if (blink == 1)
		{
			for (int i = 0; i < strlen(msg); i++)
			{
				DrawCharS(110 + offsetX, 320 / 2, msg[i], WHITE, BLACK, 3);
				offsetX = offsetX + 20;
			}
		}
		else
		{
			for (int i = 0; i < strlen(msg); i++)
			{
				DrawCharS(110 + offsetX, 320 / 2, msg[i], BLACK, BLACK, 3);
				offsetX = offsetX + 20;
			}
		}

		break;

	case 3:
		printTimeToLCD(1);

		if (ampmFlag == 1)
		{
			sprintf(msg, "PM");
		}
		else
			sprintf(msg, "AM");

		if (blink == 1)
		{
			for (int i = 0; i < strlen(msg); i++)
			{
				DrawCharS(170 + offsetX, 320 / 2, msg[i], WHITE, BLACK, 3);
				offsetX = offsetX + 20;
			}
		}
		else
		{
			for (int i = 0; i < strlen(msg); i++)
			{
				DrawCharS(170 + offsetX, 320 / 2, msg[i], BLACK, BLACK, 3);
				offsetX = offsetX + 20;
			}
		}
		break;

	case 4:
		printTimeToLCD(2);

		/* Send I2C message */
		sendChangeTime();
		changeTimeCount = 0;
		changeTimeFlag = 0;

		break;
	}
}

/*
 * @brief Function that sends the changed time to the RTC.
 * @param None
 * @return None
 */
void sendChangeTime(void)
{
	char msgHour;
	char msgMin;

	if (ampmFlag == 1)
	{
		msgHour = 0b1100000 | (hourArray[0] << 4) | hourArray[1];
		I2C1_byteWrite(rtcAddress, hourM, msgHour);
	}
	else
	{
		msgHour = 0b1000000 | (hourArray[0] << 4) | hourArray[1];
		I2C1_byteWrite(rtcAddress, hourM, msgHour);
	}

	msgMin = (minArray[0] << 4) | minArray[1];
	I2C1_byteWrite(rtcAddress, minM, msgMin);
}

/*
 * @brief Function that prints the time to the LCD.
 * @param time The time to print (0 = hour, 1 = minute, 2 = AM/PM).
 * @return None
 */
void printTimeToLCD(int time)
{
	char msg[50];
	int offsetX = 0;

	switch (time)
	{
	case 0:
		sprintf(msg, "%d%d", hourArray[0], hourArray[1]);
		for (int i = 0; i < strlen(msg); i++)
		{
			DrawCharS(50 + offsetX, 320 / 2, msg[i], WHITE, BLACK, 3);
			offsetX = offsetX + 20;
		}

		break;

	case 1:
		sprintf(msg, "%d%d", minArray[0], minArray[1]);
		for (int i = 0; i < strlen(msg); i++)
		{
			DrawCharS(110 + offsetX, 320 / 2, msg[i], WHITE, BLACK, 3);
			offsetX = offsetX + 20;
		}

		break;

	case 2:
		if (ampmFlag == 1)
		{
			sprintf(msg, "PM");
		}
		else
			sprintf(msg, "AM");

		for (int i = 0; i < strlen(msg); i++)
		{
			DrawCharS(170 + offsetX, 320 / 2, msg[i], WHITE, BLACK, 3);
			offsetX = offsetX + 20;
		}
		break;
	}
}
