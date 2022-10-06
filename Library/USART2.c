#include "USART.h"
void USART2_Config(void)
{
	/*** STEP FLOWWED ***********
	1. Enable the UART clock anh GPIO_CLOCK
	2. Configurate the UART PINS for Alternate Functions
	3. Enable the USART by writing the UE bit in USART_CR1  register to 1
	4. Program the M bit in USART_CR1 to define the word length.
	5. Select the sesired baud rate using the USART_SRR register
	6. Enable the Transmitter/Receiver by Setting the TE and RE bits in USRAT_CR1 Register.	
	*/
	//1.
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN; //(1<<17)
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; //(1<<0)
	
	//2.
	GPIOA->MODER |= (2<<4)|(2<<6); //PA2 and PA3

	GPIOA->OSPEEDR |= (2<<4)|(2<<6); //High speed
	
	GPIOA->AFR[0] |= (7<<8) | (7<<12); //(0111 -->AF7 ALternate function for USART2 at pin A2 and A3)
	//	0 for low 0->7; 1 for hight 8-15
	
	//3.
	//USART2->CR1 = 0x00;
	USART2->CR1 |= USART_CR1_UE; //1<<13; enable
	
	//4. 
	USART2->CR1 &= ~(1<<12); // 8 bit length of word
	
	//5. 
	//25.6.3
	// Fck/(8*2*115200)=22.7864583 => DIV_Mantissa[15:11] = 22
	//16*0.7864583 = 12.58333 => 13 => DIV_Fraction[3:0] = 13
	USART2->BRR |= (13<<0) | (22<<4);
	
	//6.
	USART2->CR1 |= (1<<2) | (1<<3); //( RE and TE)	
}

void USART2_SendChar(uint8_t c)
{
	/*
	1. Write the datea to send in the USART_DR register (this clears the TXE bit). Repeat this
		gor each data to be transmitted in case of single buffer
	2. After writing the  last data in to the USART_DR register, wait iuntil TC=1. This indicates 
		that the transmission of the last frame if complete. This is required for instance when
		the USART is disabled or enters the Halt mode to avoid corrupting the last transmission.
	*/
	USART2->DR |= c;
	while (!(USART2->SR & USART_SR_TC)); //transmission is complete (1<<6)
}
void USART2_SendString(char*string)
{
	while(*string) USART2_SendChar(*string++);
}
uint8_t USART2_GetChar(void)
{
	/* steps
	1. Wait for the RXNE bit to set. It indicates tha the data has been received and can be read.
	2. Read the data from USART_DR Register. This also clears the RXNE bit.
	*/
	uint8_t temp;
	while(!(USART2->SR & (USART_SR_RXNE))); //1<<5
	temp = USART2->DR;
	return temp;
}