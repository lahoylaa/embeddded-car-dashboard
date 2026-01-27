/*
 * @file eeprom.h
 * @brief EEPROM module for storing and retrieving cumulative miles
 * @details This module is the header file for the eeprom.c module 
 * 
 * @author: Aeron Lahoylahoy
 * @date: June 27, 2024
 */

#ifndef EEPROM_H_
#define EEPROM_H_

extern char mileEEPROM;
extern char mileSaved;
extern int traveledMiles;
extern char mileMemory;

void getCumulativeMiles(void);
int convertEEPROM(void);
int convertToMiles(void);
void storeMiles(void);

#endif /* EEPROM_H_ */
