/*
* @file spi.h
* @brief SPI driver header file
* @details This module provides function prototypes for SPI communication.
*
* @author Aeron Lahoylahoy
* @date June 27, 2024
*/
#ifndef _SPI_H_
#define _SPI_H_

#include "stm32f4xx.h"
#include "stm32f446xx.h"
#include "port_pin_define.h"

extern void SPI1_Init(void);
extern void SPI1_Write_16bit(unsigned char MSB, unsigned char LSB);

#endif /* _SPI_H_ */