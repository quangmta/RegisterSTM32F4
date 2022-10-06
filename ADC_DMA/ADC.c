#include "ADC.h"
uint32_t delay;
/*
	Reading the temperature
	To use the sensor:
		3. Select ADC1_IN16 or ADC1_IN18 input channel.
		4. Select a sampling time greater than the minimum sampling time specified in the
		datasheet.
		5. Set the TSVREFE bit in the ADC_CCR register to wake up the temperature sensor
		from power down mode
		6. Start the ADC conversion by setting the SWSTART bit (or by external trigger)
		7. Read the resulting VSENSE data in the ADC data register
		8. Calculate the temperature using the following formula:
		Temperature (in °C) = {(VSENSE – V25) / Avg_Slope} + 25
	Where:
	– V25 = VSENSE value for 25° C
	– Avg_Slope = average slope of the temperature vs. VSENSE curve (given in mV/°C
	or µV/°C)
	Refer to the datasheet’s electrical characteristics section for the actual values of V25
	and Avg_Slope.
*/
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
	ADC1->SQR1 |= (2<<20);//3 conversions
	//7.
	GPIOA->MODER |= (3<<2)|(3<<4);	//pin PA1 and PA2
	
	//samling time Frea for Temp Sensor
	//Tconv = (Sample time+12.5)/(Freq)
	//Tconv = (480+12.5)/(84*10^6/4)=23.45 us >10 us
	ADC1->SMPR1 |= (7<<24); //sampling time for sensor
	
	//set the TSVREFE Bit
	ADC->CCR |= ADC_CCR_TSVREFE;
	
	//enable DMA
	ADC1->CR2 |= ADC_CR2_DMA;
	//continuous request
	ADC1->CR2 |= ADC_CR2_DDS;
	//Channel Sequence
	ADC1->SQR3 = 0;
	ADC1->SQR3 |= 1<<0; //SEQ1 for channel 1
	ADC1->SQR3 |= 2<<5; //SEQ2 for channel 2
	ADC1->SQR3 |= (16<<10);//SEQ3 for channel 18	
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

void ADC_Start(void)
{
	/*
	1. set the channel sequence in the SQR register
	2. clear the status register
	3. start the conversion by setting the SWSTART bit in CR2
	*/
	// since we will be polling for each channel, here we will keep one channel in the sequence at a time ADC1->SQR3 |= (channel<<0);
	//will just keep the respective channel in the sequence for the conversion
	//1.
	//ADC1->SQR3 = 0;
//	ADC1->SQR3 |= (channel <<0);
	ADC1->SR = 0;
	ADC1->CR2 |= ADC_CR2_SWSTART;
	
}
void DMA_Init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
	//Select the Data Direction
	DMA2_Stream0->CR=0;
	DMA2_Stream0->CR &= ~(3<<6); //peripheral to memory
	
	//circle mode
	DMA2_Stream0->CR |= DMA_SxCR_CIRC;
	
	//enable memory address increment
	DMA2_Stream0->CR |= DMA_SxCR_MINC;
	
	//set the size for data
	DMA2_Stream0->CR |= (1<<11)|(1<<13);//PSIZE=01,MSIZE=01 : 16bit data
	
	//select channel for the stream
	DMA2_Stream0->CR &= ~(7<<25);//chanel 0 selected	
}

void DMA_Config(uint32_t srcAdd, uint32_t destAdd, uint8_t size)
{
	DMA2_Stream0->NDTR = size; //set the size of the transfer
	
	DMA2_Stream0->PAR = srcAdd;// source address is peripheral address
	
	DMA2_Stream0->M0AR = destAdd; //destination address is memory address
	
	//enable the DMA stream
	DMA2_Stream0->CR |= DMA_SxCR_EN;
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
