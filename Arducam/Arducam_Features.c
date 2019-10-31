/**
 * @file Arducam_Features.c
 * @author murat.demirtas@doktar.com
 */
#include "ArduCAM.h"
ts_arducam_info_t arducam_info;
/**
 * This function will trigger software capture on Arduchip
 */
static void Arducam_Start_Capture_CMD(void);
/**
 * This function will help you to read bytes from Arducam with buffer mode
 * @return is spi communcation success?
 */
static bool Arducam_Set_FIFO_Burst(void);
/**
 * This function will reset Arduchip
 */
void Arducam_Reset_Arduchip(){
	/* send command for ready for reset*/
	Arducam_Write_Register(0x07, 0x80);
	/* wait 100 ms*/
	ARDUCAM_DELAY_MS(100);
	/* reset arduchip*/
	Arducam_Write_Register(0x07, 0x00);
	/* wait 100 ms*/
	ARDUCAM_DELAY_MS(100);
	return;
}
/**
 * This function will trigger capture JPEG data on Arduchip
 */
bool Arducam_Start_Capture()
{
	uint32_t timeout = 0;
	/* send start capture command*/
	Arducam_Start_Capture_CMD();

	/* wait for Arduchip capture status*/
	while(!Arducam_Get_Bit(ARDUCHIP_TRIG , CAP_DONE_MASK)){
		/* if timeout occurred return false*/
		if(timeout++ > 1000)
			return false;
		/* wait 1 ms*/
		ARDUCAM_DELAY_MS(1);
	}
	/* if we are here save fifo_size*/
	arducam_info.arducam_fifo_size = Arducam_Read_FIFO_Length();

	/* check fifo size*/
	if(arducam_info.arducam_fifo_size > 8)
		return true;

	return false;
}
/**
 * This function will help you to burst read
 * @warning SPI communication will not close
 */
void Arducam_Setup_FIFO(){
	/* start SPI communication*/
	ARDUCAM_CS_LOW;
	/* Set Arduchip to Burst mode*/
	Arducam_Set_FIFO_Burst();
}
/**
 * This function will help you to read bytes from Arducam with buffer mode
 * @return is spi communcation success?
 */
static bool Arducam_Set_FIFO_Burst()
{
	uint8_t tx_buffer[1];
	uint8_t rx_buffer[1];

	/* set command to send*/
	tx_buffer[0] = BURST_FIFO_READ;

	/*transfer one byte and receive one byte*/
	if(HAL_SPI_TransmitReceive(ARDUCAM_SPI_PORT, (uint8_t*)tx_buffer,(uint8_t*)rx_buffer,
												sizeof(tx_buffer), ARDUCAM_SPI_TIMEOUT) != HAL_OK)
		return false;

	/* is everyting is ok */
	return true;
}
/**
 * This function will trigger software capture on Arduchip
 */
static void Arducam_Start_Capture_CMD()
{
	/* send Arducam capture command*/
	Arducam_Write_Register(ARDUCHIP_FIFO, FIFO_START_MASK);
}
/**
 * This function will clear FIFO flag on Arducam
 */
void Arducam_Clear_FIFO_Flag()
{
	/* send Arducam capture command*/
	Arducam_Write_Register(ARDUCHIP_FIFO, FIFO_CLEAR_MASK);
}
/**
 * This function will read how many bytes in Arducam FIFO
 * @return total FIFO size on Arduchip
 */
uint32_t Arducam_Read_FIFO_Length(void)
{
	/* declare variables*/
	uint32_t len1,len2,len3,len=0;
	/* read FIFO 1*/
	len1 = Arducam_Read_Register(FIFO_SIZE1);
	/* read FIFO 2*/
	len2 = Arducam_Read_Register(FIFO_SIZE2);
	/* read FIFO 3*/
	len3 = Arducam_Read_Register(FIFO_SIZE3) & 0x7f;
	/* read total length*/
	len = ((len3 << 16) | (len2 << 8) | len1) & 0x07fffff;
	/* return total value*/
	return len;
}
/**
 * This function will return existing FIFO size on Arduchip
 * @return How many bytes are in FIFO
 */
uint32_t Arducam_Get_FIFO_Size(){
	return  arducam_info.arducam_fifo_size;
}

bool Arducam_Read_FIFO(uint8_t buffer[], uint32_t * size){
	uint8_t tx_buffer[1];
	uint8_t rx_buffer[1];
	uint32_t rec_count = 0;

	while(arducam_info.arducam_fifo_size > 0){
		rec_count = 0;
		if(arducam_info.arducam_fifo_size > 1000){
			for(uint32_t iterator = 0 ; iterator < 1000 ; iterator++){
				if(HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)tx_buffer,
											(uint8_t*)rx_buffer, 1, 1000) != HAL_OK)
					return false;
				buffer[iterator] = rx_buffer[0];
				rec_count++;
				for(int i = 0 ; i < 100; i++){};
			}
			 arducam_info.arducam_fifo_size =  arducam_info.arducam_fifo_size - 1000;
			*size = rec_count;
			return true;
		}else if( arducam_info.arducam_fifo_size < 1000){
			for(uint32_t iterator = 0 ; iterator < arducam_info.arducam_fifo_size ; iterator++){
				if(HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)tx_buffer,
											(uint8_t*)rx_buffer, 1, 1000) != HAL_OK)
					return false;
				buffer[iterator] = rx_buffer[0];
				rec_count++;
				for(int i = 0 ; i < 100; i++){};
			}
			 arducam_info.arducam_fifo_size = 0;
			*size = rec_count;
			return true;
		}
	}
	*size = 0;
	return true;
}
