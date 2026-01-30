/*
 * @file rtc.h
 * @brief RTC driver for STM32F4xx
 * @details This module provides functions for RTC communication using the STM32F4xx RTC peripheral
 * 
 * @author: Aeron Lahoylahoy
 * @date: June 27, 2024
 */

#ifndef RTC_H_
#define RTC_H_
#define AM 0
#define PM 1

/* RTC Address */
extern char rtcAddress;

/* RTC Memory Address Variables */
extern char secM;
extern char minM;
extern char hourM;
extern char dayM;
extern char dateM;
extern char monthM;
extern char yearM;
extern char upperTempM;
extern char lowerTempM;

/* I2C Received */
extern char secR;
extern char minR;
extern char hourR;
extern char dayR;
extern char dateR;
extern char monthR;
extern char yearR;
extern char upperTempR;
extern char lowerTempR;

/* Binary Decimal Converted */
extern int sec;
extern int min;
extern int hour;
extern int day;
extern int date;
extern int month;
extern int year;

extern int secArray[2];
extern int minArray[2];
extern int hourArray[2];
extern int dateArray[2];
extern int monthArray[2];
extern int yearArray[2];

/* Temp Variables */
extern int rtcTempArray[2];

/* Extra Variables */
extern char *dayS[10];
extern int ampmFlag;

/* Function Prototypes */
void readTemp(void);
void getTime(void);
void binaryDecimal(void);
void getDay(void);
void decimalBinary(void);

#endif /* RTC_H_ */
