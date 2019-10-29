/*
 * Arducam_Bus.c
 *
 *  Created on: Oct 29, 2019
 *      Author: fullstackcoder
 */

#include "ArduCAM.h"
#include "spi.h"

void Arducam_Set_Bit(uint8_t addr, uint8_t bit)
{
	uint8_t temp;
	temp = Arducam_Read_Register(addr);
	Arducam_Write_Register(addr, temp | bit);
}

uint8_t Arducam_Get_Bit(uint8_t addr, uint8_t bit)
{
  uint8_t temp;
  temp = Arducam_Read_Register(addr);
  temp = temp & bit;
  return temp;
}

static uint8_t Arducam_Bus_Read(uint8_t address)
{
	uint8_t tx_buffer[1] = {0};
	uint8_t rx_buffer[1] = {0};

	tx_buffer[0] = address;

	ARDUCAM_CS_LOW;

	if(HAL_SPI_Transmit(&hspi1, (uint8_t*)tx_buffer, 1,1000) != HAL_OK)
		goto BUS_ERR;

	tx_buffer[0] = 0x00;

	if(HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)tx_buffer,
								(uint8_t*)rx_buffer, 1, 100) != HAL_OK)
		goto BUS_ERR;

	ARDUCAM_CS_HIGH;

BUS_ERR:
	ARDUCAM_CS_HIGH;

	return rx_buffer[0];
}

static bool Arducam_Bus_Write(uint8_t address, uint8_t value)
{
	uint8_t tx_buffer[1];
	tx_buffer[0] = address;

	ARDUCAM_CS_LOW;

	HAL_SPI_Transmit(&hspi1, (uint8_t*)tx_buffer, 1, 1000);

	tx_buffer[0] = value;
	HAL_SPI_Transmit(&hspi1, (uint8_t*)tx_buffer, 1, 1000);

	ARDUCAM_CS_HIGH;

	return true;
}


uint8_t Arducam_Read_Register(uint8_t addr)
{
	uint8_t data;
	data = Arducam_Bus_Read(addr & 0x7F);
	return data;
}
void Arducam_Write_Register(uint8_t addr, uint8_t data)
{
	Arducam_Bus_Write(addr | 0x80, data);
}
