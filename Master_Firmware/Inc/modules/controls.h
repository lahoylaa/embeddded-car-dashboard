/*
 * @file controls.h
 * @brief Control module for button and switch initialization and interrupt handling
 * @details This module is the header file for the controls.c module
 * 
 * @author: Aeron Lahoylahoy
 * @date: June 27, 2024
 */

#ifndef CONTROLS_H_
#define CONTROLS_H_

#include "stm32f4xx.h"
#include "port_pin_define.h"

/* Turn Signal */
#define TURN_SIGNAL_PORT PORTA
#define TURN_RIGHT_PIN PIN6
#define TURN_LEFT_PIN PIN7

#define RIGHT 0
#define NO_TURN 1
#define LEFT 2

/* Reset and Bluetooth Button */
#define BLUETOOTH_BUTTON_PORT PORTA
#define RESET_BUTTON_PORT PORTA

#define BLUETOOTH_ENABLE_CLK Aclk
#define BLUETOOTH_ENABLE_PORT PORTA

#define RESET_BUTTON_PIN PIN9
#define BLUETOOTH_BUTTON_PIN PIN10
#define BLUETOOTH_ENABLE_PIN PIN11

/* Display Menu */
#define MENU_BUTTON_PORT PORTA
#define MENU_BUTTON_PIN PIN12

/* Watch Dog Timer */
#define WATCH_DOG_PORT PORTA
#define WATCH_DOG_PIN PIN4

/* Photo Sensor */
#define PHOTOSENSOR_CLK Aclk
#define PHOTOSENSOR_PORT PORTA
#define PHOTOSENSOR_PIN PIN1

/* LED Brightness */
#define LED_CLK Bclk
#define LED_PORT PORTB
#define LED_PIN PIN5

/* Photo Sensor */
extern  int adcVal;
extern float voltage;
extern const int max_adc_value;

/* Bluetooth */
extern int bluetoothFlag;
extern int bluetoothCounter;
extern int bluetoothEnable;
extern int bluetoothCountFlag;
extern int bluetoothDisplay;

/* Turn Signal */
extern int turnSignal;
extern int turnFlag;

/* Watch Dog Variables */
extern int watchDogFlag;
extern int flag;

/** Function Prototypes **/
/* Turn Signal */
void turnSignalSWInit(void);
void turnSignalButtonInit(GPIO_TypeDef *Port, int Pin);

/* Reset Miles */
void resetButtonInit(void);

/* Bluetooth */
void bluetoothButtonInit(void);
void bluetoothEnableInit(void);

/* Display */
void menuButtonInit(void);

/* Watch Dog Reset */
void watchDogInit(void);
void watchDogButtonInit(void);
void watchDogCheck(void);

/* Photor Sensor */
void photosensorInit(void);
void ledInit(void);
void read_photosensor(void);
void ledControl(void);

/* Handler */
void EXTI9_5_IRQHandler(void);
void EXTI15_10_IRQHandler(void);

#endif /* CONTROLS_H_ */
