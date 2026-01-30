/*
* @file i2c_slave.h
* @brief I2C Slave driver for STM32F4xx
* @details This module provides functions for I2C communication in slave mode using the STM32F4xx I2C peripheral
*
* @author: Aeron Lahoylahoy
* @date:   June 27, 2024
*/
#ifndef _I2C_H_
#define _I2C_H_

#include "port_pin_define.h"

#define SLAVE PORTB
#define CLOCK_SLAVE Bclk
#define SDA PIN9 /*Slave SDA*/
#define SCL PIN8 /*Slave SCL*/

extern char data; /* I2C Variables */
extern int temp;  /*Temporary Value - I2C*/
extern int dataFlag;

void slaveConfig(void); /* I2C Slave Functions */
void get_data(void);	/*Receive Data from Master*/

#endif /* _I2C_H_ */