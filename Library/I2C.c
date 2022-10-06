#include "I2C.h"
void I2C_Config(void)
{
		/**** STEPS FOLLOWED  ************
	1. Enable the I2C CLOCK and GPIO CLOCK
	2. Configure the I2C PINs for ALternate Functions
		a) Select Alternate Function in MODER Register
		b) Select Open Drain Output 
		c) Select High SPEED for the PINs
		d) Select Pull-up for both the Pins
		e) Configure the Alternate Function in AFR Register
	3. Reset the I2C 	
	4. Program the peripheral input clock in I2C_CR2 Register in order to generate correct timings
	5. Configure the clock control registers
	6. Configure the rise time register
	7. Program the I2C_CR1 register to enable the peripheral
	*/
	//1.
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	//2. 
	GPIOB->MODER |= (2<<12) | (2<<14); //PB6 and PB7
	GPIOB->OTYPER |= (1<<6) | (1<<9);
	GPIOB->OSPEEDR |= (3<<12) | (3<<14);
	GPIOB->PUPDR |= (1<<12) | (1<<14);
	GPIOB->AFR[0] |= (4<<24) | (4<<28); //AF4 0100
	//3.
	I2C1->CR1 |= I2C_CR1_SWRST; //reset state
	I2C1->CR1 &= ~(I2C_CR1_SWRST); // not under reset
	//4.
	I2C1->CR2 |= (42<<0);//FREQ in 42 Mhz, 0:5
	//5.
	//Fclk = CCR*F clock I2C
	I2C1->CCR |= 420<<0;
	//6.
	I2C1->TRISE |= 46; //=F(PCLK1)+1
	//7. 
	I2C1->CR1 |= I2C_CR1_PE;	
}
void I2C_Start(void)
{
	/* STEP FOLLOWED
	1. Send the START condition
	2. Wait for the SB (Bit 0 in SR1) to set. This indicates tha the start condition í generated	
	*/
	I2C1->CR1 |= I2C_CR1_START; //(1<<8)
	while(!(I2C1->SR1 & I2C_SR1_SB)); //(1<<0)
}
void I2C_Write(uint8_t data)
{
	/*
	1. Wait for the TxE (bit 7 in SR1) to set. This indicates that the DR in empty
	2. send the DATA to the DR Register
	3. Wait for the BTF (bit 2 in SR1) to set. This indicates the end of LAST Data transmistions
	*/
	while(!(I2C1->SR1 & I2C_SR1_TXE)); //(1<<7)
	I2C1->DR |= data;
	while(!(I2C1->SR1 & I2C_SR1_BTF)); // (1<<2)	
}

void I2C_Address(uint8_t Address)
{
	/*
	1. Send the Slave Address to the DR Register
	2. Wait for the ADDR (bit 1 in SR1) to set. This indicates the end of address transmission.
	3. Clear the ADDR by reading the SR1 and SR2
	*/
	I2C1->DR |= Address;
	while(!(I2C1->SR1 & I2C_SR1_ADDR)); // (1<<1)	
	uint8_t temp = I2C1->SR1 | I2C1->SR2; //read SR1 and SR2 to clear the ADDR
}
void I2C_Stop(void)
{
	I2C1->CR1 |= I2C_CR1_STOP;
}
void I2C_WriteMulti(uint8_t *data, uint8_t size)
{
	/*
	1. Wait for the TxE to set.
	2. keep sending data to the DR resiter after performin the check if the TxE bit is set
	3. Once the Data transfer is complete. Wait for the BTF (bit 2 in SR1) to set. This indicates the end of Last data 
	*/
	while(!(I2C1->SR1 & I2C_SR1_TXE)); //(1<<7)
	while(size)
	{
		while(!(I2C1->SR1 & I2C_SR1_TXE)); //(1<<7)
		I2C1->DR |= (volatile uint32_t) *data++;
		size--;
	}
	while(!(I2C1->SR1 & I2C_SR1_BTF)); // (1<<2)	
}