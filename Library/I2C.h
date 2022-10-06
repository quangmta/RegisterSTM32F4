#include "stm32f407xx.h"
void I2C_Config(void);
void I2C_Start(void);
void I2C_Write(uint8_t data);
void I2C_Address(uint8_t Address);
void I2C_Stop(void);
void I2C_WriteMulti(uint8_t *data, uint8_t size);
