#include "stm32f407xx.h"
#include "stm32f4xx.h"
void ADC_Init(void);
void ADC_Enable(void);
void ADC_Start(uint8_t channel);
void ADC_WaitForConv(void);
uint16_t ADC_GetVal(void);
void ADC_Disable(void);
