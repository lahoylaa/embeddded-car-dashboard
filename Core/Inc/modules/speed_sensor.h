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
#include "math.h"

/* Mile Input */
extern int hallEffectFlag;
extern float rpm;
extern float mph;

/* Hall Effect */
void hallSensorInit(void);
void rpmReaderInit(void);
void calculateRPM(void);
void calculateMPH(void);
void updateCumulativeMiles(void);

#endif /* SPEED_SENSOR_H_ */
