#include "stm32f4xx.h"
#include "i2c_slave.h"

char data; /* I2C Variables */
int temp;  /*Temporary Value - I2C*/
int dataFlag;

/*
* @brief Function that retrieves data from the I2C bus when addressed by the master
* @param None
* @return None
*/
void get_data(void)
{
	if ((I2C1->SR1 & 2))
	{
		temp = I2C1->SR1;
		temp = I2C1->SR2;
	}

	if ((I2C1->SR1 & (0b1 << 6)))
	{
	data = I2C1->DR;
	}

	if ((I2C1->SR1 & (0b1 << 4)))
	{
	temp = I2C1->SR1;
	I2C1->CR1 = 0x401; // Clear
	}

	if (data != 0)
		dataFlag = 1;
	else
		dataFlag = 0;
}

/*
* @brief Function that initializes the I2C1 peripheral in slave mode
* @param None
* @return None
*/
void slaveConfig(void)
{
	/* GPIOB Clock */
	RCC->AHB1ENR |= CLOCK_SLAVE;

	/* Alternate function on Port B Pin 8 and Pin 9*/
	SLAVE->MODER &= ~(0b11 << (SDA * 2)) | (0b11 << (SCL * 2)); /*Clear*/
	SLAVE->MODER |= 0xA0000;

	/* Configure alternate function */
	SLAVE->AFR[1] &= ~0x000000FF;
	SLAVE->AFR[1] |= 0x00000044; // AF4 = 0b0100 on Pin 8 and Pin 9

	SLAVE->OTYPER |= (0b1 << SDA) | (0b1 << SCL);				 /* Configure Open Drain */
	SLAVE->PUPDR |= (0b01 << (SDA * 2)) | (0b01 << (SCL * 2));	 /* If using internal resitors */
	SLAVE->OSPEEDR |= (0b11 << (SDA * 2)) | (0b11 << (SCL * 2)); /*High Speed (11)*/

	/* I2C Configurations */
	RCC->APB1ENR |= 0x200000; /*Bit 21 to enable I2C1 clock*/
	I2C1->CR1 = 0x8000;		  /*Reset*/
	I2C1->CR1 &= ~0x8000;	  /*Clear reset*/
	I2C1->CCR = 80;			  /*Standard mode*/
	I2C1->CR2 = 42U;
	I2C1->OAR1 |= (0x32 << 1); /*Slave Address*/
	I2C1->CR1 |= 0x1;		   /*Enable I2C*/
	I2C1->CR1 |= (1 << 10);
}