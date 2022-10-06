

#include "Delay_F103.h"


void GPIO_Config (void)
{
	/*************>>>>>>> STEPS FOLLOWED <<<<<<<<************
	
	1. Enable GPIO Clock
	2. Set the required Pin in the INPUT Pull-up/ Pull-down Mode
	3. Configure the ODR (1-> Pull UP, 0-> Pull down)
	
	********************************************************/
	
	RCC->APB2ENR |=  (1<<2);  // Enable GPIOA clock
	
	GPIOA->CRL &= ~(0xf<<4);  // clear bits (7:6:5:4)
	
	GPIOA->CRL |= (8<<4);  // Bits (7:6:5:4) = 1:0:0:0  --> PA1 in Input Mode in Pull-up/ Pull-down mode
	
	GPIOA->ODR |= (1<<1);  //  --> PA1 is in Pull UP mode
}


void Interrupt_Config (void)
{
	/*************>>>>>>> STEPS FOLLOWED <<<<<<<<************
	
	1. Enable the AFIO CLOCK bit in RCC register 
	2. Configure the EXTI configuration Regiter in the AFIO
	3. Disable the EXTI Mask using Interrupt Mask Register (IMR)
	4. Configure the Rising Edge / Falling Edge Trigger
	5. Set the Interrupt Priority
	6. Enable the interrupt
	
	********************************************************/
	
	RCC->APB2ENR |= (1<<0);  // Enable AFIO CLOCK
	
	AFIO->EXTICR[0] &= ~(0xf<<4);  // Bits[7:6:5:4] = (0:0:0:0)  -> configure EXTI1 line for PA1
	
	EXTI->IMR |= (1<<1);  // Bit[1] = 1  --> Disable the Mask on EXTI 1
	
	EXTI->RTSR |= (1<<1);  // Enable Rising Edge Trigger for PA1
	
	EXTI->FTSR &= ~(1<<1);  // Disable Falling Edge Trigger for PA1
	
	NVIC_SetPriority (EXTI1_IRQn, 1);  // Set Priority
	
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



int main ()
{
	SystemInit();
	GPIO_Config ();
	TIM2_Config();
	Interrupt_Config ();
	
	while (1)
	{
		if (flag)
		{
			Delay_ms(100);  // avoid debouncing
			count++;
			flag = 0;
		}
	}
}
