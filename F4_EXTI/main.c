

#include "RccConfig.h"

void GPIO_Config (void)
{
	/*************>>>>>>> STEPS FOLLOWED <<<<<<<<************
	
	1. Enable GPIO Clock
	2. Set the required Pin in the INPUT Mode
	3. Configure the PULL UP/ PULL DOWN According to your requirement
	
	********************************************************/
	
	RCC->AHB1ENR |=  (1<<0);  // Enable GPIOA clock
	
	GPIOA->MODER &= ~(3<<2);  // Bits (3:2) = 0:0  --> PA1 in Input Mode
	
	GPIOA->PUPDR |=  (1<<3);  // Bits (3:2) = 1:0  --> PA1 is in Pull Down mode
}

void Interrupt_Config (void)
{
	/*************>>>>>>> STEPS FOLLOWED <<<<<<<<************
	
	1. Enable the SYSCNFG bit in RCC register 
	2. Configure the EXTI configuration Regiter in the SYSCNFG
	3. Enable the EXTI using Interrupt Mask Register (IMR)
	4. Configure the Rising Edge / Falling Edge Trigger
	5. Set the Interrupt Priority
	6. Enable the interrupt
	
	********************************************************/
	
	RCC->APB2ENR |= (1<<14);  // Enable SYSCNFG
	
	SYSCFG->EXTICR[0] &= ~(0xf<<4);  // Bits[7:6:5:4] = (0:0:0:0)  -> configure EXTI1 line for PA1
	
	EXTI->IMR |= (1<<1);  // Bit[1] = 1  --> Disable the Mask on EXTI 1
	
	EXTI->RTSR &= ~(1<<1);  // Disable Rising Edge Trigger for PA1
	
	EXTI->FTSR |= (1<<1);  // Enable Falling Edge Trigger for PA1
	
	NVIC_SetPriority (EXTI1_IRQn, 0);  // Set Priority
	
	NVIC_EnableIRQ (EXTI1_IRQn);  // Enable Interrupt
	
}

uint8_t count = 0;
int flag = 0;

void EXTI1_IRQHandler(void)
{
	/*************>>>>>>> STEPS FOLLOWED <<<<<<<<************
	
	1. Check the Pin, which trgerred the Interrupt
	2. Clear the Interrupt Pending Bit
	
	********************************************************/
	
	if (EXTI->PR & (1<<1))    // If the PA1 triggered the interrupt
	{
		flag = 1;
		EXTI->PR |= (1<<1);  // Clear the interrupt flag by writing a 1 
	}
}

void delay (uint32_t delay)
{
	while (delay--);
}

int main ()
{
	SysClockConfig ();
	GPIO_Config ();
	Interrupt_Config ();
	
	while (1)
	{
		if (flag)
		{
			delay (3000000);
			count++;
			flag = 0;
		}
	}
}
