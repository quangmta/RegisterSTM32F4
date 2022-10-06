#include "RccConfig.h"

void TIM6Config(void)
{
	/**** Steps to follow***
	1. Enable Timer Clock
	2. Set the prescalar and the ARR
	3. Enable the Timer, and wait for the update Flag to set
	*/
	//1.
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN; //(1<<4)
	//2.
	TIM6->PSC = 84-1;
	TIM6->ARR = 0xffff-1;
	//3.
	TIM6->CR1 |= TIM_CR1_CEN; //(1<<0)
	while(!(TIM6->SR & (1<<0)));		
}
void Delay_us(uint32_t us)
{
	TIM6->CNT = 0;
	while(TIM6->CNT < us);
}	
void Delay_ms(uint32_t ms)
 {
	 for( uint32_t i=0;i<ms;i++)
	 {
		 Delay_us(1000);
	 }	 
 }
 
void GPIO_Config(void)
 {
	 RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	 GPIOD->MODER |= (1<<24); //PD12
	 GPIOD->OSPEEDR |= (1<<25); //high speed
	 GPIOD->OTYPER &= ~(1<<12); //output push-pull
	 GPIOD->PUPDR &= ~((1<<24)|(1<<25)); //no pull	 
 }
int main(void)
{
	SysClockConfig();
	TIM6Config();
	GPIO_Config();	
	while(1)
	{
		GPIOD->ODR |= (1<<12);
		Delay_ms(1000);
		GPIOD->ODR &= ~(1<<12);
		Delay_ms(1000);
	}
}
