/*
 * Arducam_Features.c
 *
 *  Created on: Oct 29, 2019
 *      Author: fullstackcoder
 */


#include "ArduCAM.h"



uint32_t arducam_fifo_size = 0;


void Arducam_Reset_Arduchip(){
	Arducam_Write_Register(0x07, 0x80);
	HAL_Delay(100);
	Arducam_Write_Register(0x07, 0x00);
	HAL_Delay(100);
}


void Arducam_Start_Capture()
{
	Arducam_Start_Capture_CMD();
	while(!Arducam_Get_Bit(ARDUCHIP_TRIG , CAP_DONE_MASK)){;}
	arducam_fifo_size = Arducam_Read_FIFO_Length();
}

void Arducam_Setup_FIFO(){
	ARDUCAM_CS_LOW;
	Arducam_Set_FIFO_Burst();
}

bool Arducam_Set_FIFO_Burst()
{
	uint8_t tx_buffer[1];
	uint8_t rx_buffer[1];
	tx_buffer[0] = BURST_FIFO_READ;

	if(HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)tx_buffer,(uint8_t*)rx_buffer, 1, 100) != HAL_OK)
		return false;

	return true;
}

void Arducam_Start_Capture_CMD(void)
{
	Arducam_Write_Register(ARDUCHIP_FIFO, FIFO_START_MASK);
}

void Arducam_Clear_FIFO_Flag(void )
{
	Arducam_Write_Register(ARDUCHIP_FIFO, FIFO_CLEAR_MASK);
}

uint32_t Arducam_Read_FIFO_Length(void)
{
	uint32_t len1,len2,len3,len=0;
	len1 = Arducam_Read_Register(FIFO_SIZE1);
	len2 = Arducam_Read_Register(FIFO_SIZE2);
	len3 = Arducam_Read_Register(FIFO_SIZE3) & 0x7f;
	len = ((len3 << 16) | (len2 << 8) | len1) & 0x07fffff;
	return len;
}


uint32_t Arducam_Get_FIFO_Size(){
	return arducam_fifo_size;
}

bool Arducam_Read_FIFO(uint8_t buffer[], uint32_t * size){
	uint8_t tx_buffer[1];
	uint8_t rx_buffer[1];
	uint32_t rec_count = 0;

	while(arducam_fifo_size > 0){
		rec_count = 0;
		if(arducam_fifo_size > 1000){
			for(uint32_t iterator = 0 ; iterator < 1000 ; iterator++){
				if(HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)tx_buffer,
											(uint8_t*)rx_buffer, 1, 1000) != HAL_OK)
					return false;
				buffer[iterator] = rx_buffer[0];
				rec_count++;
				for(int i = 0 ; i < 100; i++){};
			}
			arducam_fifo_size = arducam_fifo_size - 1000;
			*size = rec_count;
			return true;
		}else if(arducam_fifo_size < 1000){
			for(uint32_t iterator = 0 ; iterator < arducam_fifo_size ; iterator++){
				if(HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)tx_buffer,
											(uint8_t*)rx_buffer, 1, 1000) != HAL_OK)
					return false;
				buffer[iterator] = rx_buffer[0];
				rec_count++;
				for(int i = 0 ; i < 100; i++){};
			}
			arducam_fifo_size = 0;
			*size = rec_count;
			return true;
		}
	}
	*size = 0;
	return true;
}
