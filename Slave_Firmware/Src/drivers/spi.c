#include "stm32f4xx.h"
#include "stm32f446xx.h"
#include "spi.h"

/*
 * @brief Function that initializes the SPI1 peripheral
 * @param None
 * @return None
 */
void SPI1_Init(void)
{
    /* Enable GPIOA Clock */
    RCC->AHB1ENR |= 1;

    /* Enable SPI Clock */
    RCC->APB2ENR |= 0x1000;

    /* Port A Pin 5 (SCLK) and Pin 7 (MOSI) */
    GPIOA->MODER &= ~0xCC00;      // Clear
    GPIOA->MODER |= 0x8800;       // set alternate mode
    GPIOA->AFR[0] &= ~0xF0F00000; // clear alternate mode
    GPIOA->AFR[0] |= 0x50500000;  // set SPI mode SPI1

    /* Port A  Pin 1 */
    GPIOA->MODER &= ~0x00C; // Clear
    GPIOA->MODER |= 0x004;  // set pin as output

    /* SPI Initilization */
    SPI1->CR1 = 0x31C; // set baud rate, 8 bit data frame (CLK dile @ 1)
    SPI1->CR2 = 0;
    SPI1->CR1 |= 0x40; // Enable SPI
}

/*
* @brief Function that writes data to the SPI1 peripheral
* @param MSB: Most Significant Byte (Address)
* @param LSB: Least Significant Byte (Data)
* @return None
*/
void SPI1_Write_16Bit(unsigned char MSB, unsigned char LSB)
{
    while (!(SPI1->SR & 2))
        ;               // wait until transfer buffer is empty
    GPIOA->ODR &= ~0x2; // set CS low (enable transfer)
    SPI1->DR = MSB;     // memory address
    // while(!(SPI1->SR & 2));			// wait for transmission to be done
    while (SPI1->SR & 0x80)
        ;
    SPI1->DR = LSB; // data information
    while (SPI1->SR & 0x80)
        ;              // wait for transmission to be done
    GPIOA->ODR |= 0x2; // set CS high (stop transfer)
}

/*
* @brief Function that writes 8-bit data to the SPI1 peripheral
* @param data: 8-bit data to be sent
* @return None
*/
void SPi1_Write_8bit(unsigned char data)
{
    while (!(SPI1->SR & 2))
        ;               // wait until transfer buffer is empty
    GPIOA->ODR &= ~0x2; // set CS low (enable transfer)
    SPI1->DR = data;    // data information
    while (SPI1->SR & 0x80)
        ;              // wait for transmission to be done
    GPIOA->ODR |= 0x2; // set CS high (stop transfer)
}
