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
