/**
 @brief System Clock Configuaration
	The system clock iss configured as follow:
		System Clokc sourece 		= PLL (HSE)
		SYSCLK(Hz)			= 168Mhz
		HCLK(Hz)			= 168Mhz
		AHB Prescaler 			= 1
		APB1 Prescaler			= 4
		APB2 Prescaler			= 2
		HSE Frequency(hz)		= 8 Mhz
		PLL_M				= 4
		PLL_N				= 168
		PLL_P				= 2
		VDD(V)				= 3.3V
		Main regilator uotput voltage	= Scalel mode
		Flash Latence(WS)		= 5

*/

#include "stm32f407xx.h"
void SysClockConfig(void);
