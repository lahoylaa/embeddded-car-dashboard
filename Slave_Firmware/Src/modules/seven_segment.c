/*
* @file seven_segment.c
* @brief Seven Segment Display driver
* @details This module provides functions to initialize and control a seven-segment display
*
* @author: Aeron Lahoylahoy
* @date:   June 27, 2024
*/
#include "stm32f446xx.h"
#include "spi.h"
#include "seven_segment.h"

uint8_t seg1, seg2, seg3, seg4, seg5, seg6, seg7, seg8;
int val[10] = {0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B};

/*
* @brief Function that sets default values for all 8 seven segments
* @param None
* @return None
*/
void setDefaultVal(void)
{
	/* Digit value 0 */
	seg1 = 0x7E;
	seg2 = 0x7E;
	seg3 = 0x7E;
	seg4 = 0x7E;
	seg5 = 0x7E;
	seg6 = 0x7E;
	seg7 = 0x7E;
	seg8 = 0x7E;
}

/*
* @brief Function that writes values to all 8 seven segments
* @param None
* @return None
*/
void Seven_Segment_Write(void){
	SPI1_Write(seg1Address, seg1);
	SPI1_Write(seg2Address, seg2);
	SPI1_Write(seg3Address, seg3);
	SPI1_Write(seg4Address, seg4);
	SPI1_Write(seg5Address, seg5);
	SPI1_Write(seg6Address, seg6);
	SPI1_Write(seg7Address, seg7);
	SPI1_Write(seg8Address, seg8);
}

/*
* @brief Function that sets values for segments 5-8
* @param None
* @return None
*/
void setSegCounter(void){
    SPI1_Write(seg5Address, seg5);
	SPI1_Write(seg6Address, seg6);
	SPI1_Write(seg7Address, seg7);
	SPI1_Write(seg8Address, seg8);
}

/*
* @brief Function that initializes the seven segment display
* @param None
* @return None
*/
void Seven_Segment_Init(void){
    SPI1_Write(0x09, 0x00); // No decode mode
	SPI1_Write(0x0A, 0x02); // 2/16 intensity
	SPI1_Write(0x0B, 0x07); // Scan limit set to digit 0-7
	SPI1_Write(0x0C, 0x01); // Normal operations
	// SPI1_Write(0x0F, 0x0F);	// turns test register on
	// SPI1_Write(0x0F, 0x00); 	// normal operations

}