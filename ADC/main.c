#include "RccConfig.h"
#include "ADC.h"
uint16_t ADC_VAL[2]={0,0};

int main()
{
	SysClockConfig();
	ADC_Init();
	ADC_Enable();
	
	while(1)
	{
		ADC_Start(1);
		ADC_WaitForConv();
		ADC_VAL[0]=ADC_GetVal();
		
		ADC_Start(2);
		ADC_WaitForConv();
		ADC_VAL[1]=ADC_GetVal();
		
	}
}