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
	#define PLL_M 25
	#define PLL_N 168
	#define PLL_P 0 // PLL_P = 0
	
	//1. ENABLE HSE and wait for the HSE to become Ready
	//7.3.1 RCC clock control register (RCC_CR)
	RCC->CR |= RCC_CR_HSEON; //1<<16
	while(!(RCC->CR & RCC_CR_HSERDY)); //1<<17
	
	//2. Set the POWER ENABLE CLOCK and VOLTAGE REGULATOR
	//7.3.13 RCC APB1 peripheral clock enable register
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;// 1<<28 Power interface clock enable
	//5.4.1 PWR power control register 
	RCC->CR |= PWR_CR_VOS; //3<<14
	
	//3. Configure the FLASH PREFETCH and the LATENCY Related Settings
	//3.9.1 Flash access control register (FLASH_ACR)
	FLASH->ACR |= FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_5WS;//1<<8|1<<9|1<<10|5<<0
	
	// 4. Configure the PRESCALARS HCLK, PCLK1, PCLK2
	//7.3.3 RCC clock configuration register (RCC_CFGR)
	//AHB
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;// &= ~(1<<4)
	
	//APB1 PR
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV4; //(5<<10)
	
	//APB2 PR
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV2; //(4<<13)
	
	//5. Configure the MAIN PLL (P,M,N & source)
	//7.3.2 RCC PLL configuration register (RCC_PLLCFGR)
	RCC->PLLCFGR |= (PLL_M<<0) | (PLL_N<<6) | (PLL_P <<16) | (RCC_PLLCFGR_PLLSRC_HSE);
	
	//6. Enable the PLL and wait for it to become ready
	//7.3.1 RCC clock control register (RCC_CR)
	RCC->CR |= RCC_CR_PLLON; // 1<<24
	while( !(RCC->CR & RCC_CR_PLLRDY)); //1<<25
	
	//7. Select the Clock Source and wait for it to be set
	//7.3.3 RCC clock configuration register (RCC_CFGR)
	RCC->CFGR |= RCC_CFGR_SW_PLL; //2<<0
	while( (RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL); //(2<<2)
}

void GPIO_Config(void)
{
	/************** STEPS TO FOLLOW *****************
	1. Enable GPIO clock
	2. Set the PIN as output
	3. Configure the output mode i.e state, speed, and pull
	************************************************/
	
	//1. Enable GPIO clock
	//7.3.10 RCC AHB1 peripheral clock enable register (RCC_AHB1ENR)
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; //(4<<0)
	
	// 2. Set the PIN as output
	//8.4.1 GPIO port mode register (GPIOx_MODER) (x = A..I/J/K)
	GPIOD->MODER |= (1<<24); // pin PD12 (bits 24:25) as 
	// 00: Input
	// 01: Output
	// 10: Aternate funtion mode - che do chuc nang thay the
	// 11: Analoge mode
	
	//8.4 GPIO register
	//3. Configure the output mode i.e state, speed, and pull
	GPIOD->OTYPER = 0;
	GPIOD->OSPEEDR = 0;
	GPIOD->PUPDR = 0;	
}
void delay(uint32_t time)
{
	while(time--);	
}
int main(void)
{
	//SysClockConfig();
	GPIO_Config();
	while(1)
	{
		//8.4.7 GPIO Port set/reset register
		GPIOD->BSRR |= (1<<12);//set
		delay(10000000);
		GPIOD->BSRR |= (1<<12)<<16;	//reset	
		delay(10000000);
		/*
		You should not be setting bits in the BSRR using an OR command. 
		The BSRR register is meant to be atomic and therefore use EQUALS to set and reset bits. 
		It should not use Read-Modify-Write command such as OR/AND/XOR etc. 
		If you want to use these then use them on the ODR.
		*/
		
		/* 8.4.6 GPIO port output data register 
			GPIOD->ODR |= (1<<12);
			GPIOD->ODR &= ~(1<<12);
		To set the entire PORT HIGH
			GPIOD->ODR = OxFFF;
			GPIO->ODR = 0x00;
		*/		
	}
	
}
