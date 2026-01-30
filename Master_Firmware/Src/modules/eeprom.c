/*
 * @file 	eeprom.c
 * @brief 	EEPROM module for storing and retrieving cumulative mileage data
 * @details This module provides the functions to read, convert, and store cumulative
 * 	 		mileage data in an external EEPROM via I2C.
 *
 * @note 	Uses BCD format for storing mileage values (tens and units)
 * 			Stores mileage in increments, with rollover handling
 * 			EEPROM device address: 0x57
 * 
 * @author: Aeron Lahoylahoy
 * @date:  June 27, 2024
 */
#include "stm32f4xx.h"
#include "stm32f446xx.h"
#include "i2c_master.h"
#include "eeprom.h"
#include "controls.h"
#include "display.h"
#include "signal.h"
#include "port_pin_define.h"
#include "speed_sensor.h"
#include "ili9341.h"

/* Variables for EEPROM operations */
char mileEEPROM = 0x57; // address
char mileMemory = 0;
char mileSaved = 0;
int traveledMiles = 0;

/*
 * @brief Function that retrieves cumulative mileage from EEPROM and converts it to integer miles.
 * @param None
 * @return None
*/
void getCumulativeMiles(void){

	I2C1_byteRead(mileEEPROM, 0, &mileSaved);
	traveledMiles = traveledMiles + convertToMiles();

}

/*
 * @brief Function that converts BCD format mileage to integer miles.
 * @param None
 * @return miles: Converted mileage in integer format
 */
int convertToMiles(void){
	int miles;

	miles = (((mileSaved & 0xF0) >> 4) * 10) + (mileSaved & 0x0F);

	return miles;
}

/*
 * @brief Function that stores the current mileage in EEPROM.
 * @param None
 * @return None
 */
void storeMiles(void){
	static char increment;
	static char memory;

	increment++;

	if(increment > 255){
		memory++;
	}

	I2C1_byteWrite(mileEEPROM, memory, increment);
}

/*
 * @brief Function that reads saved settings from EEPROM and applies them.
 * @param None
 * @return None
 */
void readSavedData(void)
{
    char prevData;

    for (int i = 0; i < 5; i++)
    {
        I2C1_byteRead(mileEEPROM, i, &prevData);

        /* Turn Signal */
        if (i == 0)
        {
            I2C1_byteWrite(slave, 0, prevData);
        }

        /* Display */
        if (i == 1)
        {
            state = prevData;
            if (state == MENUSTATE)
            {
                displayMenu();
            }
        }

        /* Bluetooth */
        if (i == 2)
        {
            BLUETOOTH_ENABLE_PORT->ODR &= ~prevData;
            bluetoothEnable = prevData;
            bluetoothDisplay = prevData;
        }

        /* MILES LOG */
        if (i == 4)
        {
            traveledMiles = traveledMiles + convertToMiles();
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
        }
    }
}

/*
* @brief Function that reads the current mileage from the speed sensor and updates RPM and MPH.
 * @param None
 * @return None
 */
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
        { // half a second 32500 , 16250
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

/*
 * @brief Function that sends the current mileage index to the motor controller.
 * @param None
 * @return None
 */
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

/*
 * @brief Function that sends control messages based on button states.
 * @param None
 * @return None
 */
void sendMessages(void)
{

    /* Turn Signal */
    if (turnFlag)
    {
        if (debounceButton(PORTA, TURN_RIGHT_PIN))
        {

            I2C1_byteWrite(slave, 0, 0x41);
            delayMS(1);
            /* Save this to EEPROM */
            I2C1_byteWrite(mileEEPROM, 0, 0x41);
        }

        else if (debounceButton(PORTA, TURN_LEFT_PIN))
        {

            I2C1_byteWrite(slave, 0, 0x42);
            delayMS(1);
            I2C1_byteWrite(mileEEPROM, 0, 0x42);
        }
        else
        {
            I2C1_byteWrite(slave, 0, 0x43);
        }
        turnFlag = 0;
    }

    /* Mile Reset */
    if (resetMilesFlag)
    {
        if (debounceButton(RESET_BUTTON_PORT, RESET_BUTTON_PIN))
        {
            // I2C1_byteWrite(slave, 0, 0x21);
            I2C1_byteWrite(mileEEPROM, 4, 0);
            I2C1_byteWrite(slave, 0, 0x10); // sends the reset command
            traveledMiles = 0;
        }
        resetMilesFlag = 0;
    }

    /* Bluetooth */
    if (bluetoothFlag)
    {
        if (debounceButton(BLUETOOTH_BUTTON_PORT, BLUETOOTH_BUTTON_PIN))
        {
            BLUETOOTH_ENABLE_PORT->ODR ^= (0b1 << PIN11);
            bluetoothEnable++;

            if (bluetoothEnable == 2)
            {
                bluetoothEnable = 0;
            }
        }
        bluetoothFlag = 0;
    }

    /* Bluetooth Display */
    if (bluetoothEnable == 1)
    {

        if (bluetoothCounter == 1)
        {
            displayBluetooth(DISPLAY);
            /* Saves the value to the eeprom */
            I2C1_byteWrite(mileEEPROM, 2, 0x01);
        }
        else if (bluetoothCounter > 5)
        {
            displayBluetooth(CLEAR);
            bluetoothCounter = 0;
            bluetoothCountFlag = 0;
            bluetoothDisplay = 1;
        }
    }
    else if (bluetoothEnable == 0)
    {
        Fill_Rect((240 / 2) - 25, 225, 50, 55, BLACK);
        I2C1_byteWrite(mileEEPROM, 2, 0x00);
        bluetoothCountFlag = 0;
        bluetoothDisplay = 0;
    }

    /* Menu Button */
    if (debounceButton(MENU_BUTTON_PORT, MENU_BUTTON_PIN))
    {
        if (state != MENUSTATE)
            menuScreen = 1;
        // state = MENUSTATE;
    }

    if (mileFlag)
    {
        mileCounter++;
        /* Motor */
        if (mileCounter == 3)
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
        mileFlag = 0;
    }
}

