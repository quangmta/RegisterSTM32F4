#include "SPI.h"
void SPI_Config(void)
{
	/*
	1. Enable SPI Clock
	2. Configure the Control Register
	3. Configure the CR2
	*/
	
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	
	SPI1->CR1 |= SPI_CR1_CPHA | SPI_CR1_CPOL;
	
	SPI1->CR1 |= SPI_CR1_MSTR;//master slave
	
	SPI1->CR1 |= (3<<3); //baud rate /16
	
	SPI1->CR1 &= ~SPI_CR1_MSTR;
	
	SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
	
	SPI1->CR1 &= ~(SPI_CR1_RXONLY | SPI_CR1_DFF);
	
	SPI2->CR2 = 0;
}

void GPIO_Config(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= (2<<10)|(2<<12)|(2<<14)|(1<<18);
	GPIOA->OSPEEDR |= (3<<10)|(3<<12)|(3<<14)|(3<<18);
	
	GPIOA->AFR[0] |= (5<<20)|(5<<24)|(5<<28);//AF5 for low register
}

void SPI_Enable(void)
{
	SPI1->CR1 |= SPI_CR1_SPE;
}
void SPI_Disabel(void)
{
	SPI1->CR1 &= ~SPI_CR1_SPE;
}

void CS_Enable(void)
{
	GPIOA->ODR |= (1<<9);
}

void CS_Disable(void)
{
	GPIOA->ODR &= ~(1<<9);
}

void SPI_Transmit(uint8_t *data, int size)
{
	/*
	1. Wait for the TXE bit to set in the Status Register
	2. Write the data to the Data Register
	3. After the data has been transmitted, wait for the BSY bit to reset in Status Register
	4. Clear the Overrun flag by reading DR and SR
	*/
	int i;
	while(i<size)
	{
		while(!(SPI1->SR)&(SPI_SR_TXE)){};//waot fpr TXE bit to set->This will indicate that the buffer is empty
		SPI1->DR = data[i];//load the data into the Data register
		i++;
	}
			/*During discontinuous communications, there is a 2 APB clock period delay between the
		write operation to SPI_DR and the BSY bit setting. As a consequence, in transmit-only
		mode, it is mandatory to wait first until TXE is set and then until BSY is cleared after writing
		the last data.*/
	while(!(SPI1->SR)&(SPI_SR_TXE));
	while(!(SPI1->SR)&(SPI_SR_BSY));//this will indicate that SPI is not busy in communication
	
	uint8_t temp = SPI1->DR;
	temp = SPI1->SR;	
}

void SPI_Receive(uint8_t *data, int size)
{
	/*
	1. Wait for the BSY bit to reset in Status Register
	2. Send some Dummy data befor reading the DATA
	3. Wait for the RXNE bit to Set in the status Register
	4. Read data from Data Register
	*/
	while(size)
	{
		while((SPI1->SR)&(SPI_SR_BSY));
		SPI1->DR =0;//send dummy data
		while((SPI1->SR)&(SPI_SR_RXNE));
		*data++=(SPI1->DR);
		size--;
	}	
}