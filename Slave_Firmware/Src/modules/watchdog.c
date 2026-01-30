#include "stm32f4xx.h"
#include "stm32f446xx.h"
#include "watchdog.h"

/* Watch Dog Timers */
/* Function: Initilizes the watchdog timer for a 1 second */
void watchDogInit(void)
{

	IWDG->KR |= 0x5555; // config key for watchdog timer
	IWDG->PR |= 0x01;	// set 8 divider (1 second)
	IWDG->RLR |= 1;		// resets 1 second
}

/* Function: Interrupt Handler | Turn Signal and Bluetooth Button, and Hall Effect Sensor*/
/*
* @brief EXTI15_10 Interrupt Handler for Watchdog Button
* @param None
* @return None
*/
extern void EXTI15_10_IRQHandler(void)
{
	/*Turn Signal Interrupt*/
	if (WDG_PIN)
	{
		if (debounceButton(WDG_PORT, WDG_PIN)) /*Debounce Check*/
		{
			IWDG->KR |= 0xCCCC;
			IWDG->KR |= 0xAAAA;
		}
		EXTI->PR |= (0b1 << WDG_PIN); /* Reset Interrupt flag */
	}
}