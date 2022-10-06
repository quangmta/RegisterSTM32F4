#include "stm32f4xx.h"
void SPI_Config(void);
void GPIO_Config(void);
void SPI_Enable(void);
void SPI_Disabel(void);
void CS_Enable(void);
void CS_Disable(void);
void SPI_Transmit(uint8_t *data, int size);
void SPI_Receive(uint8_t *data, int size);