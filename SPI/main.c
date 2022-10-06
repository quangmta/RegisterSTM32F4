#include "RccConfig.h"
#include "TimerConfig.h"
#include "SPI.h"

uint8_t data[3]={0x02,0x32,0xf6};

int main(void)
{
	SysClockConfig();
	GPIO_Config();
	SPI_Config();
	
	SPI_Transmit(data,3);
	
	CS_Enable();
	SPI_Enable();
	CS_Disable();
	while(1)
	{
		
	}
}
