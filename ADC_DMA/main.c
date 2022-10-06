#include "RccConfig.h"
#include "TimerConfig.h"
#include "ADC.h"

uint16_t RxData[3];
float Temperature;

int main(void)
{
	SysClockConfig();
	TIM6Config();
	
	ADC_Init();
	ADC_Enable();
	DMA_Init();
	
	DMA_Config((uint32_t) &ADC1->DR,(uint32_t) &RxData, 3);
	ADC_Start();
	
	while(1)
	{
		Temperature = (((float)(3.3*RxData[2]/(float)4095) -0.76) / 0.0025) + 25;
	}
}
