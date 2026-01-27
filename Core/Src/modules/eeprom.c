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
#include "i2c_master.h"
#include "eeprom.h"

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

