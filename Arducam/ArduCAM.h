#ifndef __ARDUCAM_H
#define __ARDUCAM_H
#include "stdbool.h"
#include "Includes/doktarApi.h"
#include "spi.h"

#define OV5642_MINI_5MP 1

#define MAX_FIFO_SIZE		0x7FFFF			//512KByte
#define ARDUCHIP_TEST1       	0x00  //TEST register
#define ARDUCHIP_FRAMES			  0x01
#define ARDUCHIP_MODE      		0x02  //Mode register
#define VSYNC_LEVEL_MASK   		0x02  //0 = High active , 		1 = Low active
#define ARDUCHIP_TIM       		0x03  //Timming control
#define ARDUCHIP_FIFO      		0x04  //FIFO and I2C control
#define FIFO_CLEAR_MASK    		0x01
#define FIFO_START_MASK    		0x02
#define BURST_FIFO_READ			0x3C  //Burst FIFO read operation
#define SINGLE_FIFO_READ		0x3D  //Single FIFO read operation
#define ARDUCHIP_TRIG      		0x41  //Trigger source
#define VSYNC_MASK         		0x01
#define CAP_DONE_MASK      		0x08
#define FIFO_SIZE1				0x42  //Camera write FIFO size[7:0] for burst to read
#define FIFO_SIZE2				0x43  //Camera write FIFO size[15:8]
#define FIFO_SIZE3				0x44  //Camera write FIFO size[18:16]

#define ARDUCAM_DELAY_MS(delay) HAL_Delay(delay)

typedef struct{
	uint32_t arducam_fifo_size;
	uint32_t width;
	uint32_t height;
}ts_arducam_info_t;
extern ts_arducam_info_t arducam_info;
#define ARDUCAM_CS_LOW HAL_GPIO_WritePin(ARDUCAM_CS_PORT, ARDUCAM_CS_PIN, RESET); HAL_Delay(5)
#define ARDUCAM_CS_HIGH HAL_GPIO_WritePin(ARDUCAM_CS_PORT, ARDUCAM_CS_PIN, SET); HAL_Delay(5)
#if !defined(ARDUCAM_CS_PORT) || !defined(ARDUCAM_CS_PIN) || !defined(ARDUCAM_CS_LOW) || !defined(ARDUCAM_CS_HIGH)
#error please define Arducam SPI settings
#endif

/**
 * This function will read how many bytes in Arducam FIFO
 * @return total FIFO size on Arduchip
 */
uint32_t Arducam_Read_FIFO_Length(void);
/**
 * This function will clear FIFO flag on Arducam
 */
void Arducam_Clear_FIFO_Flag(void);
/**
 * This function will return existing FIFO size on Arduchip
 * @return How many bytes are in FIFO
 */
uint32_t Arducam_Get_FIFO_Size(void);

bool Arducam_Read_FIFO(uint8_t buffer[], uint32_t * size);
/**
 * This function will reset Arduchip
 */
void Arducam_Reset_Arduchip(void);
/**
 * This function will trigger capture JPEG data on Arduchip
 */
bool Arducam_Start_Capture(void);
/**
 * This function will send dummy data to Arduchip for flush
 * spi bus communication
 */
void Arducam_Send_Dummy(void);
/**
 * This function will test SPI communication between Arduchip
 * @return is operation successfully?
 */
bool Arducam_SPI_Bus_Test(void);
/**
 * This function will init Arducam 5MP module with default settings
 * @return is operation successfully?
 */
bool Arducam_Mini_5MP_Init(void);
/**
 * This function will set bits of register on Arduchip
 * @param addr target register address
 * @param bit which bit?
 */
void Arducam_Set_Bit(uint8_t addr, uint8_t bit);
/**
 * This function will read specific register on Arduchip
 * @param addr target register address
 * @param bit which bit?
 * @return register value currently readed on Arduchip
 */
uint8_t Arducam_Get_Bit(uint8_t addr, uint8_t bit);
/**
 * This high level function will transfer register value to Arduchio
 * @param addr Target Arduchip register address
 * @param data New register value
 */
void Arducam_Write_Register(uint8_t addr, uint8_t data);
/**
 * This high level function will return register value on Arduchip
 * @param addr target Arduchip register address
 * @return register value transferred from SPI bus
 */
uint8_t Arducam_Read_Register(uint8_t addr);
/**
 * This function will help you to burst read
 * @warning SPI communication will not close
 */
void Arducam_Setup_FIFO(void);
#endif
