/**
  ******************************************************************************

  CLOCK Setup For STM32F446RE
  Author:   ControllersTech
  Updated:  2nd Aug 2020

  ******************************************************************************
  Copyright (C) 2017 ControllersTech.com

  This is a free software under the GNU license, you can redistribute it and/or modify it under the terms
  of the GNU General Public License version 3 as published by the Free Software Foundation.
  This software library is shared with public for educational purposes, without WARRANTY and Author is not liable for any damages caused directly
  or indirectly by this software, read more about this on the GNU General Public License.

  ******************************************************************************
*/

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 4
  *            PLL_N                          = 180
  *            PLL_P                          = 2
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */

#include "stm32f446xx.h"



void SysClockConfig (void)
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
	
	
	#define PLL_M 	4
	#define PLL_N 	180
	#define PLL_P 	0  // PLLP = 2

	// 1. ENABLE HSE and wait for the HSE to become Ready
	RCC->CR |= RCC_CR_HSEON;  // RCC->CR |= 1<<16; 
	while (!(RCC->CR & RCC_CR_HSERDY));  // while (!(RCC->CR & (1<<17)));
	
	// 2. Set the POWER ENABLE CLOCK and VOLTAGE REGULATOR
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;  // RCC->APB1ENR |= 1<<28;
	PWR->CR |= PWR_CR_VOS;  // PWR->CR |= 3<<14; 
	
	
	// 3. Configure the FLASH PREFETCH and the LATENCY Related Settings
	FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_5WS;  // FLASH->ACR = (1<<8) | (1<<9)| (1<<10)| (5<<0);
	
	// 4. Configure the PRESCALARS HCLK, PCLK1, PCLK2
	// AHB PR
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;  // RCC->CFGR &= ~(1<<4);
	
	// APB1 PR
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;  // RCC->CFGR |= (5<<10);
	
	// APB2 PR
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;  // RCC->CFGR |= (4<<13);
	
	
	// 5. Configure the MAIN PLL
	RCC->PLLCFGR = (PLL_M <<0) | (PLL_N << 6) | (PLL_P <<16) | (RCC_PLLCFGR_PLLSRC_HSE);  // (1<<22);

	// 6. Enable the PLL and wait for it to become ready
	RCC->CR |= RCC_CR_PLLON;  // RCC->CR |= (1<<24);
	while (!(RCC->CR & RCC_CR_PLLRDY));  // while (!(RCC->CR & (1<<25)));
	
	// 7. Select the Clock Source and wait for it to be set
	RCC->CFGR |= RCC_CFGR_SW_PLL;  // RCC->CFGR |= (2<<0);
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);  // while (!(RCC->CFGR & (2<<2)));
}


void GPIO_Config (void)
{
	/************ STEPS FOLLOWED ***********
	1. Enable the GPIO CLOCK
	2. Set the Pin as OUTPUT
	3. Configure the OUTPUT MODE
	***************************************/
	
	// 1. Enable the GPIO CLOCK
	RCC->AHB1ENR |= (1<<0);  
	
	// 2. Set the Pin as OUTPUT
	GPIOA->MODER |= (1<<10);  // pin PA5(bits 11:10) as Output (01)
	
	// 3. Configure the OUTPUT MODE
	GPIOA->OTYPER = 0;
	GPIOA->OSPEEDR = 0;
}

void delay (uint32_t time)
{
	while (time--);
}

int main (void)
{
	
	SysClockConfig ();
	GPIO_Config ();
	
	while (1)
	{
		GPIOA->BSRR |= (1<<5);  // Set the pin PA5
		delay (20000000); 
		GPIOA->BSRR |= ((1<<5) <<16);  // Reset pin PA5
		delay (20000000); 
	}
}

