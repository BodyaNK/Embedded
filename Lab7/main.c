#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_hal.h"

I2C_HandleTypeDef  I2cHandle;

void init_GPIO_Config() 
{
	GPIO_InitTypeDef GPIO;
	GPIO.Mode = GPIO_MODE_AF_OD;
	GPIO.Speed = GPIO_SPEED_FAST;
	GPIO.Pin = GPIO_PIN_6 | GPIO_PIN_7; // SCL, SDA
	GPIO.Pull = GPIO_NOPULL;
	GPIO.Alternate = GPIO_AF4_I2C1;
	
	HAL_GPIO_Init(GPIOB, &GPIO);
}

void init_I2C_Config()
{
	I2cHandle.Instance = I2C1;	
	I2cHandle.Init.ClockSpeed = 100000; //100kHz CLK
	I2cHandle.Init.DutyCycle = I2C_DUTYCYCLE_2; //50% duty cycle --> standard
	I2cHandle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT; // Set address length to 7 bit addresses
	I2cHandle.Init.OwnAddress1 = 0x00; // own address, not relevant in master mode
	I2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE; // Disable seconde Address
	I2cHandle.Init.OwnAddress2 = 0xff;
	I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE; // Disable general call
	I2cHandle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;// Disable no stretch
	
	if(HAL_I2C_Init(&I2cHandle)!=HAL_OK){
		Error_Handler();
	}
}

void I2C_Write(uint16_t device, uint16_t addr, uint16_t no_bytes, uint8_t *pData) {
	while(HAL_I2C_Mem_Write(&I2cHandle, device, addr, no_bytes,pData,  no_bytes, HAL_MAX_DELAY) != HAL_OK);
}

void I2C_WriteByte(uint16_t device, uint16_t addr, uint8_t data) {
	while(HAL_I2C_Mem_Write(&I2cHandle, device, addr, 1, (uint8_t*)&data, sizeof(data), HAL_MAX_DELAY) != HAL_OK);
}

void I2C_Read(uint16_t device, uint16_t addr, uint16_t no_bytes, uint8_t *result) {
	while(HAL_I2C_Mem_Read(&I2cHandle, device, addr, no_bytes,result,  no_bytes, HAL_MAX_DELAY) != HAL_OK);
}

uint8_t I2C_ReadByte(uint16_t device, uint16_t addr) {
	uint8_t result = 0;
	while(HAL_I2C_Mem_Read(&I2cHandle, device, addr, 1, (uint8_t*)&result, sizeof(result), HAL_MAX_DELAY) != HAL_OK);
	return result;
}

int main(void)
{
	HAL_Init();
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_I2C2_CLK_ENABLE();
	
	init_GPIO_Config();
	init_I2C_Config();
	
	uint8_t data = 0xA6;
	
	I2C_WriteByte(0xff, 0x10, data);//I2C_MEMADD_SIZE_8BIT
	
	data = I2C_ReadByte(0xff, 0x10);
	
	while (1);
}
