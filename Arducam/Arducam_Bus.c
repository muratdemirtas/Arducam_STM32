/**
 * @file Arducam_Bus.c
 * @author murat.demirtas@doktar.com
 * @brief This file only includes Arduchip SPI communication bus functions
 */
#include "ArduCAM.h"
/**
 * This function will read specific register value on Arduchip
 * @param address Target Arduchip register address
 * @return
 */
static uint8_t Arducam_Bus_Read(uint8_t address);

/**
 * This function will transfer register value to target register Address on Arduchip
 * @param address Target Arduchip register Address
 * @param value Target Arduchip register value
 * @return is spi transfer successfully?
 */
static bool Arducam_Bus_Write(uint8_t address, uint8_t value);
/**
 * This function will set bits of register on Arduchip
 * @param addr target register address
 * @param bit which bit?
 */
void Arducam_Set_Bit(uint8_t addr, uint8_t bit)
{
	uint8_t temp;
	/* read old register status*/
	temp = Arducam_Read_Register(addr);
	/* Write new register value with new value*/
	Arducam_Write_Register(addr, temp | bit);
}
/**
 * This function will read specific register on Arduchip
 * @param addr target register address
 * @param bit which bit?
 * @return register value currently readed on Arduchip
 */
uint8_t Arducam_Get_Bit(uint8_t addr, uint8_t bit)
{
	/* declare variable for register*/
	uint8_t temp;
	/* read register on Arduchip with address*/
	temp = Arducam_Read_Register(addr);
	/* check bit is set or not?*/
	temp = temp & bit;
	/* return register value*/
	return temp;
}
/**
 * This function will read specific register value on Arduchip
 * @param address Target Arduchip register address
 * @return
 */
static uint8_t Arducam_Bus_Read(uint8_t address)
{
	uint8_t tx_buffer[1] = {0};
	uint8_t rx_buffer[1] = {0};

	/* set target address on this byte*/
	tx_buffer[0] = address;

	/* start spi communication*/
	ARDUCAM_CS_LOW;

	/*transfer one byte to Arduchip and wait response*/
	if(HAL_SPI_Transmit(ARDUCAM_SPI_PORT, (uint8_t*)tx_buffer, sizeof(tx_buffer),ARDUCAM_SPI_TIMEOUT) != HAL_OK)
		goto BUS_ERR;

	/* is everythings looks like good then read register value*/
	tx_buffer[0] = 0x00;

	/*receive on byte from Arduchip*/
	if(HAL_SPI_TransmitReceive(ARDUCAM_SPI_PORT, (uint8_t*)tx_buffer,
								(uint8_t*)rx_buffer, sizeof(tx_buffer), ARDUCAM_SPI_TIMEOUT) != HAL_OK)
		goto BUS_ERR;

	/* stop spi communication*/
	ARDUCAM_CS_HIGH;
/* if error occurred while transfer stop communication*/
BUS_ERR:
	ARDUCAM_CS_HIGH;
	/* return existing read value*/
	return rx_buffer[0];
}
/**
 * This function will transfer register value to target register Address on Arduchip
 * @param address Target Arduchip register Address
 * @param value Target Arduchip register value
 * @return is spi transfer successfully?
 */
static bool Arducam_Bus_Write(uint8_t address, uint8_t value)
{
	uint8_t tx_buffer[1];

	/* send tx byte zero to target register address*/
	tx_buffer[0] = address;

	/* start SPI communication*/
	ARDUCAM_CS_LOW;

	/*transfer one byte to Arduchip*/
	if(HAL_SPI_Transmit(ARDUCAM_SPI_PORT, (uint8_t*)tx_buffer, sizeof(tx_buffer), ARDUCAM_SPI_TIMEOUT) != HAL_OK)
		goto BUS_ERR;

	/*set register value to tx byte*/
	tx_buffer[0] = value;

	/*transfer one byte to Arduchip*/
	if(HAL_SPI_Transmit(ARDUCAM_SPI_PORT, (uint8_t*)tx_buffer, sizeof(tx_buffer), ARDUCAM_SPI_TIMEOUT) != HAL_OK)
		goto BUS_ERR;

	/* stop spi communication*/
	ARDUCAM_CS_HIGH;

/* if error occurred while transfer stop communication*/
BUS_ERR:
	ARDUCAM_CS_HIGH;
	/* if we are here, everythings is ok*/
	return true;
}
/**
 * This high level function will return register value on Arduchip
 * @param addr target Arduchip register address
 * @return register value transferred from SPI bus
 */
uint8_t Arducam_Read_Register(uint8_t addr)
{
	/* declare variable for register data*/
	uint8_t data;
	/* read one byte from address on Arduchip*/
	data = Arducam_Bus_Read(addr & 0x7F);
	/*return register value*/
	return data;
}
/**
 * This high level function will transfer register value to Arduchio
 * @param addr Target Arduchip register address
 * @param data New register value
 */
void Arducam_Write_Register(uint8_t addr, uint8_t data)
{
	/* transfer one byte to specific address on Arduchip*/
	Arducam_Bus_Write(addr | 0x80, data);
}
