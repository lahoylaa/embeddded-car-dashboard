/*
 * @file i2c_master.h
 * @brief I2C master driver for STM32F4xx
 * @details This module provides functions for I2C communication using the STM32F4xx I2C peripheral
 *
 * @author: Aeron Lahoylahoy
 * @date: June 27, 2024
 */

#ifndef I2C_MASTER_H_
#define I2C_MASTER_H_

#include "stm32f4xx.h"
#include "port_pin_define.h"

/* I2C Variables */
extern char slave;

/* I2C Master Functions */
void masterConfig(void);
int I2C1_byteRead(char saddr, char maddr, char *data);
int I2C1_byteWrite(char saddr, char maddr, char data);

#endif /* I2C_MASTER_H_ */
