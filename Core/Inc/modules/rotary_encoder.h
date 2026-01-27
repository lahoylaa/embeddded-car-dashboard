/*
 * @file rotary_encoder.h
 * @brief Rotary Encoder module for handling user input via rotary encoder
 * @details This module is the header file for the rotary_encoder.c module
 * 
 * @author: Aeron Lahoylahoy
 * @date: June 27, 2024
 */

#ifndef ROTARY_ENCODER_H_
#define ROTARY_ENCODER_H_

/* Rotary Encoder Global Variables */
extern int encoderSW_Flag;
extern int encoderCLK_Flag;
extern int CounterClockwise_Count;
extern int Clockwise_Count;

extern int count;
extern int turn;
extern int blink;
extern int CCW;
extern int CW;

/* Rotary Encoder Functions */
void rotaryEncoderInit(void);
void encoderSW_Init(void);
void encoderCLK_Init(void);
void encoderDT_Init(void);

/* Interrupt Functions */
extern void EXTI0_IRQHandler(void);
extern void EXTI1_IRQHandler(void);

#endif /* ROTARY_ENCODER_H_ */
