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
void SysClockConfig(void)
{

	/*************>>>>>>> STEPS FOLLOWED <<<<<<<<************
	
	1. ENABLE HSE and wait for the HSE to become Ready
	2. Set the POWER ENABLE CLOCK and VOLTAGE REGULATOR
	3. Configure the FLASH PREFETCH and the LATENCY Related Settings
	4. Configure the PRESCALARS HCLK, PCLK1, PCLK2
	5. Configure the MAIN PLL
	6. Enable the PLL and wait for it to become ready
	7. Select the Clock Source and wait for it to be set
	
	********************************************************/
	#define PLL_M 4
	#define PLL_N 160
	#define PLL_P 0 // PLL_P = 2
	
	//1. ENABLE HSE and wait for the HSE to become Ready
	//7.3.1 RCC clock control register (RCC_CR)
	RCC->CR =0x00;
	RCC->CR |= RCC_CR_HSEON; //1<<16
	while(!(RCC->CR & RCC_CR_HSERDY)); //1<<17
	
	//2. Set the POWER ENABLE CLOCK and VOLTAGE REGULATOR
	//7.3.13 RCC APB1 peripheral clock enable register
	RCC->APB1ENR = 0x00;
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;// 1<<28 Power interface clock enable
	//5.4.1 PWR power control register 
	RCC->CR |= PWR_CR_VOS; //1<<14
	
	//3. Configure the FLASH PREFETCH and the LATENCY Related Settings
	//3.9.1 Flash access control register (FLASH_ACR)
	FLASH->ACR |= FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_5WS;//1<<8|1<<9|1<<10|5<<0
	
	// 4. Configure the PRESCALARS HCLK, PCLK1, PCLK2
	//7.3.3 RCC clock configuration register (RCC_CFGR)
	RCC->CFGR = 0x00;
	//AHB
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;// &= ~(1<<4)
	
	//APB1 PR
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV4; //(5<<10)
	
	//APB2 PR
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV2; //(4<<13)
	
	//5. Configure the MAIN PLL (P,M,N & source)
	//7.3.2 RCC PLL configuration register (RCC_PLLCFGR)
	RCC->PLLCFGR = 0;
	RCC->PLLCFGR |= (PLL_M<<0) | (PLL_N<<6) | (RCC_PLLCFGR_PLLSRC_HSE);
	RCC->PLLCFGR &= ~(1<<16);
	
	//6. Enable the PLL and wait for it to become ready
	//7.3.1 RCC clock control register (RCC_CR)
	RCC->CR |= RCC_CR_PLLON; // 1<<24
	while( !(RCC->CR & RCC_CR_PLLRDY)); //1<<25
	
	//7. Select the Clock Source and wait for it to be set
	//7.3.3 RCC clock configuration register (RCC_CFGR)
	RCC->CFGR |= RCC_CFGR_SW_PLL; //2<<0
	while( (RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL); //(2<<2)
}