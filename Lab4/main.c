#include <stm32f4xx.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include <misc.h>
#include <stm32f4xx_syscfg.h>
#include "stm32f4xx_spi.h"
#include <stm32f4xx_exti.h>

uint8_t whoAmI;
uint16_t dataX;
uint16_t dataY;
uint16_t dataZ;

void CS_ON(void) {
  GPIO_ResetBits(GPIOE, GPIO_Pin_3);
}
void CS_OFF(void) {
  GPIO_SetBits(GPIOE, GPIO_Pin_3);
}

void gpio_init (void)
{
	GPIO_InitTypeDef GPIO_Init_Pins;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_Init_Pins.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_Init_Pins.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init_Pins.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init_Pins.GPIO_PuPd = GPIO_PuPd_NOPULL;

  GPIO_Init(GPIOA, &GPIO_Init_Pins);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

  GPIO_Init_Pins.GPIO_Pin = GPIO_Pin_3;
  GPIO_Init_Pins.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init_Pins.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init_Pins.GPIO_OType = GPIO_OType_PP;
  GPIO_Init_Pins.GPIO_PuPd = GPIO_PuPd_NOPULL;

  GPIO_Init(GPIOE, &GPIO_Init_Pins);
	
	CS_OFF();
}

void spi_init_pin (void)
{
  SPI_InitTypeDef SPI_Init_user;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

  SPI_Init_user.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_Init_user.SPI_Mode = SPI_Mode_Master;
  SPI_Init_user.SPI_DataSize = SPI_DataSize_8b;
  SPI_Init_user.SPI_CPOL = SPI_CPOL_High;
  SPI_Init_user.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_Init_user.SPI_NSS = SPI_NSS_Soft;
  SPI_Init_user.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
  SPI_Init_user.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_Init_user.SPI_CRCPolynomial = 7;

  SPI_Init(SPI1, &SPI_Init_user);

  SPI_Cmd(SPI1, ENABLE);
}

uint8_t Write_Data(uint8_t data)
{
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET){}

    SPI_I2S_SendData(SPI1, data);
    
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET){}

    return SPI_I2S_ReceiveData(SPI1);
}

void Write_Val_Reg (uint8_t address, uint8_t value)
{
  CS_ON();
  Write_Data(address);
  Write_Data(value);
  CS_OFF();
}

uint8_t Read_Val_Reg (uint8_t address)
{
  uint8_t data=0;
  address = address|0x80;
  CS_ON();
  Write_Data (address);
  data = Write_Data ((uint8_t)0x00);
  CS_OFF();
  return data;
}

int main(void) {
	gpio_init();
  spi_init_pin();

  whoAmI = Read_Val_Reg(0x0F);
  Write_Val_Reg(0x20,0x0f);

  while (1) {
      int8_t buffer[6];
			buffer[0] = Read_Val_Reg(0x28);
			buffer[1] = Read_Val_Reg(0x29);
			buffer[2] = Read_Val_Reg(0x2A);
			buffer[3] = Read_Val_Reg(0x2B);
			buffer[4] = Read_Val_Reg(0x2C);
			buffer[5] = Read_Val_Reg(0x2D);
			dataX = (int16_t)((buffer[1] << 8) + buffer[0]) * 0.06;
			dataY = (int16_t)((buffer[3] << 8) + buffer[2]) * 0.06;
			dataZ = (int16_t)((buffer[5] << 8) + buffer[4]) * 0.06;
  }
}