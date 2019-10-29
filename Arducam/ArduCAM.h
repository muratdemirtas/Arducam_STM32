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



bool Arducam_Read_FIFO(uint8_t buffer[], uint32_t * size);

void Arducam_Reset_Arduchip(void);

#define ARDUCAM_CS_PORT GPIOA
#define ARDUCAM_CS_PIN GPIO_PIN_4
#define ARDUCAM_CS_LOW HAL_GPIO_WritePin(ARDUCAM_CS_PORT, ARDUCAM_CS_PIN, RESET); HAL_Delay(5)

#define ARDUCAM_CS_HIGH HAL_GPIO_WritePin(ARDUCAM_CS_PORT, ARDUCAM_CS_PIN, SET); HAL_Delay(5)
bool Arducam_Set_FIFO_Burst();
uint32_t Arducam_Read_FIFO_Length(void);
uint8_t read_reg(uint8_t addr);
uint32_t read_fifo_length(void);
bool set_fifo_burst();
uint8_t Arducam_Read_Register(uint8_t addr);
void Arducam_Write_Register(uint8_t addr, uint8_t data);
#endif
