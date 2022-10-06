#include "stm32f407xx.h"

void USART2_Config(void);
void USART2_SendChar(uint8_t c);
void USART2_SendString(char *string);
uint8_t USART2_GetChar(void);