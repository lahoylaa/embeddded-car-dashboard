/*
 * @file display.h
 * @brief Display module for LCD screen
 * @details This module is the header file for the display.c module
 *
 * @author: Aeron Lahoylahoy
 * @date: June 27, 2024
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#define MENUSTATE 0
#define TIMESTATE 1
#define DATESTATE 2
#define TEMPSTATE 3

#define TIME 0
#define DATE 1
#define TEMP 2

#define DISPLAY 0
#define CLEAR 1
#define ICON 2

/* Time, Date, Temp */
extern int arrayTimePos[50];
extern int arrayDatePos[50];
extern int arrayTempPos[50];

extern int timeFlag;
extern int dateFlag;
extern int tempFlag;

extern int changeTimeFlag;
extern int changeDateFlag;
extern int changeTimeCount;
extern int changeDateCount;

/* Menu */
extern int state;
extern int menuFlag;
extern int blinkMenuFlag;
extern int chooseMenu;
extern int chooseTime;
extern int chooseDate;
extern int chooseTemp;
extern int resetFlag;
extern int displayFlag;
extern int timeCount;
extern int trigger;
extern int mileCounter;
extern int menuScreen;

/* Function Prototypes */
void displayBluetooth(int n);
void blinkHour(void);
void displayMenu(void);
void blinkDisplay(int n);
void printToLCD(int menu);
void blinkMenu(void);
void TIM7_Init(void);
void TIM7_IRQHandler(void);
void changeDate(void);
void printDateToLCD(int date);
void sendChangeDate(void);
void changeTime(void);
void sendChangeTime(void);
void decimalBinary(void);
void printTimeToLCD(int time);

#endif /* DISPLAY_H_ */
