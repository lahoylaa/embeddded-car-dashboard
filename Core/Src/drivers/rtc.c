/*
 * @file 	rtc.c
 * @brief 	DS3231 Real-Time Clock (RTC) driver
 * @details Provides functions to read time, date and temperature from DS3231 via I2C. 
 * 			Handles BCD <-> binary conversions and day-of-week mapping.
 *
 * @note 	All time/date values are read in BCD format from the DS3231 and converted to normal integers.
 * 		 	Uses 12-hour format with AM/PM flag detection
 *
 * @author 	Aeron Lahoylahoy
 * @date   	June 27, 2024
 *
 */
#include "stm32f4xx.h"
#include "RTC.h"
#include "i2c_master.h"
#include "port_pin_define.h"

/* RTC Address */
char rtcAddress = 0b01101000; // 0b1101000

/* RTC Memory Address Variables */
char secM = 0x00;
char minM = 0x01;
char hourM = 0x02;
char dayM = 0x03;
char dateM = 0x04;
char monthM = 0x05;
char yearM = 0x06;
char upperTempM = 0x11;
char lowerTempM = 0x12;

/* I2C Received */
char secR;
char minR;
char hourR;
char dayR;
char dateR;
char monthR;
char yearR;
char upperTempR;
char lowerTempR;

/* Binary Decimal Converted */
int sec;
int min;
int hour;
int day;
int date;
int month;
int year;

int secArray[2];
int minArray[2];
int hourArray[2];
int dateArray[2];
int monthArray[2];
int yearArray[2];

char *dayS[10];
int ampmFlag = AM;

/* Temp Variables */
int rtcTempArray[2];

/*
 * @brief Function that reads the temperature from the DS3231 RTC and stores the values in rtcTempArray.
 * @param None
 * @return None
 *
 */
void readTemp(void)
{
	/* Upper byte shows integer temp values */
	I2C1_byteRead(rtcAddress, upperTempM, &upperTempR);

	/* Lower byte shows the fractional temp values */
	I2C1_byteRead(rtcAddress, lowerTempM, &lowerTempR);

	rtcTempArray[0] = (((upperTempR & 0x70) >> 4) * 10) + (upperTempR & 0x0F);
	rtcTempArray[1] = (((lowerTempR & 0xC0) >> 4) * 10);
}

/*
 * @brief Function that gets the day of the week from the DS3231 RTC.
 * @param None
 * @return None
 */
void getDay(void)
{

	switch (dayR)
	{
	case 1:
		*dayS = "Sunday\n";
		break;

	case 2:
		*dayS = "Monday\n";
		break;

	case 3:
		*dayS = "Tuesday\n";
		break;

	case 4:
		*dayS = "Wednesday\n";
		break;

	case 5:
		*dayS = "Thursday\n";
		break;

	case 6:
		*dayS = "Friday\n";
		break;

	case 7:
		*dayS = "Saturday\n";
		break;
	}
}

/*
 * @brief Function that reads the time and date from the DS3231 RTC.
 * @param None
 * @return None
 */
void getTime(void)
{
	I2C1_byteRead(rtcAddress, secM, &secR);
	I2C1_byteRead(rtcAddress, minM, &minR);
	I2C1_byteRead(rtcAddress, hourM, &hourR);
	I2C1_byteRead(rtcAddress, dayM, &dayR);
	I2C1_byteRead(rtcAddress, dateM, &dateR);
	I2C1_byteRead(rtcAddress, monthM, &monthR);
	I2C1_byteRead(rtcAddress, yearM, &yearR);

	binaryDecimal();
	getDay();
}

/*
 * @brief Function that converts BCD values to binary.
 * @param None
 * @return None
 */
void binaryDecimal(void)
{

	sec = (((secR & 0x70) >> 4) * 10) + (secR & 0x0F);
	min = (((minR & 0x70) >> 4) * 10) + (minR & 0x0F);
	hour = (((hourR & 0x10) >> 4) * 10) + (hourR & 0x0F);
	day = (((dayR & 0xF0) >> 4) * 10) + (dayR & 0x0F);
	date = (((dateR & 0x30) >> 4) * 10) + (dateR & 0x0F);
	month = (((monthR & 0x10) >> 4) * 10) + (monthR & 0x0F);
	year = (((yearR & 0x70) >> 4) * 10) + (yearR & 0x0F);

	secArray[0] = (secR & 0x70) >> 4;
	secArray[1] = (secR & 0x0F);

	minArray[0] = (minR & 0x70) >> 4;
	minArray[1] = (minR & 0x0F);

	hourArray[0] = (hourR & 0x10) >> 4;
	hourArray[1] = (hourR & 0x0F);

	dateArray[0] = (dateR & 0x30) >> 4;
	dateArray[1] = (dateR & 0x0F);

	monthArray[0] = (monthR & 0x10) >> 4;
	monthArray[1] = (monthR & 0x0F);

	yearArray[0] = (yearR & 0x70) >> 4;
	yearArray[1] = (yearR & 0x0F);

	/* Gets the hour for AM or PM */
	if (((hourR & 0x20) >> 4) == 2)
	{
		ampmFlag = PM;
	}
	else if (((hourR & 0x20) >> 4) == 0)
	{
		ampmFlag = AM;
	}
}
