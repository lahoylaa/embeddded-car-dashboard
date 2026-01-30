/*
* @file    seven_segment.h
* @brief   Seven Segment Display driver
* @details This module provides functions to initialize and control a seven-segment display
*
* @author: Aeron Lahoylahoy
* @date:   June 27, 2024
*/
#ifndef _SEVENSEGMENT_H_
#define _SEVENSEGMENT_H_

#include "stm32f4xx.h"
#include "stm32f446xx.h"
#include "port_pin_define.h"

/* SEVEN SEG */
#define seg1Address 0x01
#define seg2Address 0x02
#define seg3Address 0x03
#define seg4Address 0x04
#define seg5Address 0x05
#define seg6Address 0x06
#define seg7Address 0x07
#define seg8Address 0x08

#define shutdownAddress 0x0C;
#define decodeAddress 0x09;
#define intensityAddress 0x0A;
#define scanLimitAddress 0x0B;

extern uint8_t seg1, seg2, seg3, seg4, seg5, seg6, seg7, seg8;
extern int val[10];

extern void setDefaultVal(void);
extern void Seven_Segment_Init(void);
extern void Seven_Segment_Write(void);

#endif /* _SEVENSEGMENT_H_ */
