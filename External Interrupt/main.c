#include "RccConfig.h"
void GPIO_Config(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER &= ~(3<<0);//PA0
	GPIOA->PUPDR &= 0<<0;//pull up	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER |= 1<<24;
}
void Interrupt_Config(void)
{
	/*** STEP ****
	1. Enable SYSCFG bit in RCC register
	2. Configure the EXTI configuration Register in the SYSCFG (IMR)
	3. Disable the EXTI using Interrupt Mask Regiter (IMR)
	4. Configure the Rising Edge/Fallin Edge Trigger
	5. Set the Interrupt Priority
	6. Enbale the interrupt
	*/
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	
	SYSCFG->EXTICR[0] &= ~(0xf<<0);//PA0
	
	EXTI->IMR |= 1<<0;//EXTI_IMR_IM0;
	
	EXTI->RTSR |= (1<<0);//enable rising EXTI_RTSR_TR0
	EXTI->FTSR &= ~(1<<0);//disable falling
	
	NVIC_SetPriority(EXTI0_IRQn,1);//number,priority
	
	NVIC_EnableIRQ(EXTI0_IRQn);	
}
void delay(uint32_t time)
{
	while(time--);
}
unsigned int i;
int flag = 0;
int count = 0;
void EXTI0_IRQHandler(void)
{
	/*
	1. Check the Pin, which trigerred the Interrupt
	2. Clear the Interrupt Pending Bit
	*/
	if (EXTI->PR & (EXTI_PR_PR0)) //if the interrupt is triggered by PA0
	{
		GPIOD->ODR ^= (1<<12);
		//delay(3000000);
		count++;		
		for(i=0;i<1000000;i++);
		while(!(GPIOA->IDR & (1<<0)) & (i<1000000))
		{
			i++;
		}
		for (i=0;i<1000000;i++);
		EXTI->PR = EXTI_PR_PR0;
	}
}

int main()
{
	SysClockConfig();
	GPIO_Config();
	Interrupt_Config();
	while(1)
	{
		if(flag)
		{
			GPIOD->ODR ^= (1<<12);
			//delay(3000000);
			count++;
			flag=0;
		}
	}
}