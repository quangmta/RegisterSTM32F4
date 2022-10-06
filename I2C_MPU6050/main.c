
#include "RccConfig.h"
#include "TimerConfig.h"
#include "I2C.h"
//#include "MPU_6050.h"

void GPIO_Config(void)
 {
	 RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	 GPIOD->MODER |= (1<<24); //PD12
	 GPIOD->OSPEEDR |= (1<<25); //high speed
	 GPIOD->OTYPER &= ~(1<<12); //output push-pull
	 GPIOD->PUPDR &= ~((1<<24)|(1<<25)); //no pull	 
 }
 void MPU_Write(uint8_t Address, uint8_t Reg, uint8_t Data)
{
	I2C_Start();
	I2C_Address(Address);
	I2C_Write(Reg);
	I2C_Write(Data);
	I2C_Stop();
}
void MPU_Read(uint8_t Address, uint8_t Reg, uint8_t *buffer, uint8_t size)
{
	I2C_Start();
	I2C_Address(Address);
	I2C_Write(Reg);
	I2C_Start();
	I2C_Read(Address+0x01, buffer, size);
	I2C_Stop();
}
 
#define MPU6050_ADDR 0xD0

#define WHO_AM_I_REG 0x75
#define PWR_MGMT_1_REG 0x6B
#define SMPLRT_DIV_REG 0x19
#define ACCEL_CONFIG_REG 0x1C
#define ACCEL_XOUT_H_REG 0x3B
#define TEMP_OUT_H_REG 0x41
#define GYRO_CONFIG_REG 0x1B
#define GYRO_XOUT_H_REG 0x43


int16_t Accel_X_RAW = 0;
int16_t Accel_Y_RAW = 0;
int16_t Accel_Z_RAW = 0;

int16_t Gyro_X_RAW = 0;
int16_t Gyro_Y_RAW = 0;
int16_t Gyro_Z_RAW = 0;

float Ax, Ay, Az, Gx, Gy, Gz;

void MPU6050_Init (void)
{
	uint8_t check=0;
	uint8_t Data=0;

	// check device ID WHO_AM_I

	MPU_Read (MPU6050_ADDR,WHO_AM_I_REG, &check, 1);

	if (check == 104||check == 152)  // 0x68 will be returned by the sensor if everything goes well
	{
		// power management register 0X6B we should write all 0's to wake the sensor up
		Data = 0;
		MPU_Write (MPU6050_ADDR, PWR_MGMT_1_REG, Data);

		// Set DATA RATE of 1KHz by writing SMPLRT_DIV register
		Data = 0x07;
		MPU_Write(MPU6050_ADDR, SMPLRT_DIV_REG, Data);

		// Set accelerometer configuration in ACCEL_CONFIG Register
		// XA_ST=0,YA_ST=0,ZA_ST=0, FS_SEL=0 -> ? 2g
		Data = 0x00;
		MPU_Write(MPU6050_ADDR, ACCEL_CONFIG_REG, Data);

		// Set Gyroscopic configuration in GYRO_CONFIG Register
		// XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0 -> ? 250 ?/s
		Data = 0x00;
		MPU_Write(MPU6050_ADDR, GYRO_CONFIG_REG, Data);
	}

}

void MPU6050_Read_Accel (void)
{
	
	uint8_t Rx_data[6];
	
	// Read 6 BYTES of data starting from ACCEL_XOUT_H register

	MPU_Read (MPU6050_ADDR, ACCEL_XOUT_H_REG, Rx_data, 6);

	Accel_X_RAW = (int16_t)(Rx_data[0] << 8 | Rx_data [1]);
	Accel_Y_RAW = (int16_t)(Rx_data[2] << 8 | Rx_data [3]);
	Accel_Z_RAW = (int16_t)(Rx_data[4] << 8 | Rx_data [5]);

	/*** convert the RAW values into acceleration in 'g'
	     we have to divide according to the Full scale value set in FS_SEL
	     I have configured FS_SEL = 0. So I am dividing by 16384.0
	     for more details check ACCEL_CONFIG Register              ****/

	Ax = Accel_X_RAW/16384.0;
	Ay = Accel_Y_RAW/16384.0;
	Az = Accel_Z_RAW/16384.0;
}
int main(void)
{
	SysClockConfig();
	//	Delay_ms(1000);	
	TIM6Config();
	GPIO_Config();
	I2C_Config();
	MPU6050_Init();
	while(1)
	{
		MPU6050_Read_Accel();
		if(!(GPIOD->IDR & (1<<12))) GPIOD->ODR |= (1<<12);
		else GPIOD->ODR &= ~(1<<12);
		Delay_ms(100);	
	}
}
