/*
 * @file port_pin_define.h
 * @brief Port and Pin definitions for GPIO configuration
 * @details This header file defines macros for GPIO ports and pins
 * 			to standardize pin referencing across the project.
 *
 * @author: Aeron Lahoylahoy
 * @date: June 27, 2024
 *
 */

#ifndef INC_PORT_PIN_DEFINE_H_
#define INC_PORT_PIN_DEFINE_H_

/* Ports */
#define PORTA GPIOA
#define PORTB GPIOB
#define PORTC GPIOC
#define PORTD GPIOD
#define PORTE GPIOE
#define PORTF GPIOF
#define PORTG GPIOG
#define PORTH GPIOH
#define PORTI GPIOI
#define PORTJ GPIOJ
#define PORTK GPIOK

/* Pins */
#define PIN0 0
#define PIN1 1
#define PIN2 2
#define PIN3 3
#define PIN4 4
#define PIN5 5
#define PIN6 6
#define PIN7 7
#define PIN8 8
#define PIN9 9
#define PIN10 10
#define PIN11 11
#define PIN12 12
#define PIN13 13
#define PIN14 14
#define PIN15 15

/* GPIO */
#define Aclk 0x01
#define Bclk 0x02
#define Cclk 0x04
#define Dclk 0x08
#define Eclk 0x10
#define Fclk 0x20
#define Gclk 0x40
#define Hclk 0x80

#endif /* INC_PORT_PIN_DEFINE_H_ */
