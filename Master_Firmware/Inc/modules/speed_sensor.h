/*
 * @file speed_sensor.h
 * @brief Speed sensor module for hall effect sensor and RPM calculation
 * @details This module handles the initialization and reading of hall effect sensors to calculate RPM and MPH
 * 
 * @author: Aeron Lahoylahoy
 * @date: June 27, 2024
 */

#ifndef SPEED_SENSOR_H_
#define SPEED_SENSOR_H_

#include "stm32f4xx.h"
#include "stm32f446xx.h"
#include "port_pin_define.h"
#include "math.h"

/* Mile Input */
extern int hallEffectFlag;
extern float rpm;
extern float mph;
extern double startTime;
extern double endTime;
extern float cumulativeMiles;
extern float rpmAverage;
extern float rpmSum;
extern int rpmSumCounter;
extern int resetMilesFlag = 0;

extern int mileCounter;
extern int mileFlag;

extern int speedVal[20];
extern int speedIndex;

/* Hall Effect */
void hallSensorInit(void);
void rpmReaderInit(void);
void calculateRPM(void);
void calculateMPH(void);
void updateCumulativeMiles(void);
void readMiles(void);
void sendMiles(void);
void clearAccumulatedMiles(void);

#endif /* SPEED_SENSOR_H_ */
