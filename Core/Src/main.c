#include "stm32f4xx.h"
#include "stdio.h"
#include "string.h"
#include "port_pin_define.h"
#include "math.h"
#include "stdlib.h"

/* LCD */
#include "iLI9341.h"
#include "font_freemono_mono_bold_24.h"

/* Controls: turn signal, miles, bluetooth buttons, reset*/
#include "controls.h"

/* I2C */
#include "i2c_master.h"

/* General Button */
#include "button_functions.h"
#include "rtc.h"
#include "display.h"
#include "sonar.h"
#include "rotary_encoder.h"
#include "eeprom.h"


void TIM7_Init(void);
void TIM7_IRQHandler(void);

int trigger = 0;
int mileCounter = 0;

extern void TIM7_Init(void);
extern void TIM7_IRQHandler(void);

/* New shit */
void changeTime(void);
void decimalBinary(void);
void printTimeToLCD(int time);
void sendChangeTime(void);
void changeDate(void);
void printDateToLCD(int date);
void sendChangeDate(void);

/* Speed */
void readMiles(void);
void sendMiles(void);
void clearAccumulatedMiles(void);

int speedVal[20] = {0, 5, 10, 15, 20, 25, 30, 35, 40,
					50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100};
int speedIndex = 0;

int main(void)

{

	__disable_irq();

	masterConfig();
	turnSignalSWInit();
	SysTick_Init();
	Display_Init();
	sonarInit();
	rotaryEncoderInit();

	resetButtonInit();

	/* Mile Shit */
	rpmReaderInit();
	hallSensorInit();

	/* Time 7 */
	TIM7_Init();

	/* Bluetooth Shit */
	bluetoothButtonInit();
	bluetoothEnableInit();

	/* Display Shit */
	menuButtonInit();

	/* SONAR SHIT */
	sonarInit();

	NVIC_SetPriority(EXTI9_5_IRQn, 1);

	__enable_irq();

	displayMenu();

	/* Always get the value of miles beforehand */
	//	getCumulativeMiles;

	/* TEsting */
	// I2C1_byteWrite(mileEEPROM, 0, 0x05);

	for (int i = 0; i < 1000; i++)
		;
	// I2C1_byteRead(mileEEPROM, 0, 0x05);
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

	// I2C1_byteWrite(slave, 0, 0x61);
	//
	//					readEcho();
	//		calculateDistance(timeElapsed);
	//
	//		if(checkDistance){
	//				/* Debug Sonar */
	//		printf("Distance %g\n", distance);
	//			checkWarningSignal();
	//			//I2C1_byteWrite(slave, 0, 0x61);
	//
	//			checkDistance = 0;
	//		}

	while (1)
	{
		switch (state)
		{
		case MENUSTATE:

			/* Add Reset Shit */

			/* read SOnar */
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

/* FIX THIS */
// void clearAccumulatedMiles(void){
//
//	for(int i = 0; i < 0xFF; i++){
//		I2C1_byteWrite(mileEEPROM, memory
//	}
//
// }

void readMiles(void)
{

	if (hallEffectFlag == 0)
	{
		TIM4->CR1 = 0; // turn off timer
		TIM4->CNT = 0;
		rpmAverage = 0;
		endTime = 0;
		mph = 0;
		// calculateRPM();
	}

	if (hallEffectFlag == 1)
	{
		TIM4->CR1 = 1; // start timer

		if (TIM4->CNT > 9000)
		{	// half a second 32500 , 16250
			//			TIM4->CR1 = 0; // turn off timer
			//			TIM4->CNT = 0;
			//			//rpmAverage = 0;
			//			endTime = 0;
			//			calculateRPM();
			hallEffectFlag = 0;
		}
	}

	else if (hallEffectFlag == 2)
	{
		TIM4->CR1 = 0; // turn off timer
		endTime = TIM4->CNT;
		calculateRPM();
		// calculateMPH();
		TIM4->CR1 = 0; // turn off timer
		TIM4->CNT = 0;
		hallEffectFlag = 1;
	}
}

void sendMiles(void)
{

	for (int i = 0; i < 20; i++)
	{

		if (mph == 0)
		{
			speedIndex = 0;
		}
		else if ((mph <= speedVal[i]) && (mph > speedVal[i - 1]))
			speedIndex = i;
	}

	if (speedIndex > 9)
	{
		I2C1_byteWrite(slave, 0, 0x50 | (speedIndex - 10));
	}
	else
		I2C1_byteWrite(slave, 0, 0x30 | speedIndex);
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

void sendChangeDate(void)
{

	char msgMonth = (monthArray[0] << 4) | monthArray[1];
	char msgDate = (dateArray[0] << 4) | dateArray[1];
	char msgYear = (yearArray[0] << 4) | yearArray[1];

	I2C1_byteWrite(rtcAddress, monthM, msgMonth);
	I2C1_byteWrite(rtcAddress, dateM, msgDate);
	I2C1_byteWrite(rtcAddress, yearM, msgYear);
}

/* New shit */
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

void decimalBinary(void)
{

	hourArray[0] = hour / 10;
	hourArray[1] = hour % 10;

	minArray[0] = min / 10;
	minArray[1] = min % 10;

	dateArray[0] = date / 10;
	dateArray[1] = date % 10;

	monthArray[0] = month / 10;
	monthArray[1] = month % 10;

	yearArray[0] = year / 10;
	yearArray[1] = year % 10;
}
