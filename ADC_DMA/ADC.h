#include "stm32f407xx.h"
#include "stm32f4xx.h"
void ADC_Init(void);
void ADC_Enable(void);
void ADC_Start(void);
void ADC_WaitForConv(void);
uint16_t ADC_GetVal(void);
void ADC_Disable(void);
void DMA_Init(void);
void DMA_Config(uint32_t srcAdd, uint32_t destAdd, uint8_t size);
