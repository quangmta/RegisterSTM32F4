#include "ADC.h"
uint32_t delay;
void ADC_Init(void)
{
	/************** STEPS TO FOLLOW *****************

1. Enable ADC and GPIO clock
2. Set the prescalar in the Common Control Register (CCR)
3. Set the Scan Mode and Resolution in the Control Register 1 (CR1)
4. Set the Continuous Conversion, EOC, and Data Alignment in Control Reg 2 (CR2)
5. Set the Sampling Time for the channels in ADC_SMPRx
6. Set the Regular channel sequence length in ADC_SQR1
7. Set the Respective GPIO PINs in the Analog Mode

************************************************/
	//1.
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	//2.
	ADC->CCR |= (1<<16);//presclalar =4;
	//3.
	ADC1->CR1 |= ADC_CR1_SCAN;
	ADC1->CR1 &= ~(3<<24);//res 12 bit
	//4.
	ADC1->CR2 |= ADC_CR2_CONT;
	ADC1->CR2 |= ADC_CR2_EOCS;
	ADC1->CR2 &= ~ADC_CR2_ALIGN;
	//5.
	ADC1->SMPR2 &= ~((7<<3)|(7<<6));//sampling time of 3 cycles for changel 1 and 2
	//6.
	ADC1->SQR1 |= (1<<20);//2 conversions
	//7.
	GPIOA->MODER |= (3<<2)|(3<<4);	
}
void ADC_Enable(void)
{
	/*
	1. enbale the adc by setting adon bit in CR2
	2. wait for adc to stabilize (approx 10us)
	*/
	ADC1->CR2 |= ADC_CR2_ADON;
	delay = 10000;
	while(delay--);
}

void ADC_Start(uint8_t channel)
{
	/*
	1. set the channel sequence in the SQR register
	2. clear the status register
	3. start the conversion by setting the SWSTART bit in CR2
	*/
	// since we will be polling for each channel, here we will keep one channel in the sequence at a time ADC1->SQR3 |= (channel<<0);
	//will just keep the respective channel in the sequence for the conversion
	//1.
	ADC1->SQR3 = 0;
	ADC1->SQR3 |= (channel <<0);
	ADC1->SR = 0;
	ADC1->CR2 |= ADC_CR2_SWSTART;
	
}
void ADC_WaitForConv(void)
{
	while(!(ADC1->SR & ADC_SR_EOC));
}
uint16_t ADC_GetVal(void)
{
	return ADC1->DR;//read the Data Register
}
void ADC_Disable(void)
{
	ADC1->CR2 &= ~(ADC_CR2_CONT);
}