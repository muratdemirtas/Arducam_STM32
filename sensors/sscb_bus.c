/*
 * sscb_bus.c
 *
 *  Created on: Oct 29, 2019
 *      Author: fullstackcoder
 */

#include "sscb_bus.h"

/**
 *
 * @param regID
 * @param regDat
 * @return
 */
bool SSCB_wrSensorReg16_8(uint16_t regID, uint16_t regDat)
{
	uint8_t tx_buffer[3];
	tx_buffer[0] = regID>>8;
	tx_buffer[1] = (uint8_t)regID;
	tx_buffer[2] = (uint8_t)regDat;

	if(HAL_I2C_Master_Transmit(&hi2c1, 0x78, (uint8_t*)tx_buffer, 3, 100) != HAL_OK)
		return false;
	HAL_Delay(1);
	return true;
}
/**
 *
 * @param reglist
 * @return
 */
int SSCB_wrSensorRegs16_8(const struct sensor_reg reglist[])
{
  int err = 0;

  unsigned int reg_addr;
  unsigned char reg_val;
  const struct sensor_reg *next = reglist;

  while ((reg_addr != 0xffff) | (reg_val != 0xff))
  {
    reg_addr =next->reg;
    reg_val = next->val;

    if(reg_addr == 0xffff)
    	break;

    err = SSCB_wrSensorReg16_8(reg_addr, reg_val);
    next++;
  }
  return err;
}
/**
 *
 * @param regID
 * @param regDat
 * @return
 */
bool SSCB_rdSensorReg16_8(uint16_t regID, uint8_t* regDat)
{

	uint8_t tx_buffer[2];
	tx_buffer[0] = regID>>8;
	tx_buffer[1] = (uint8_t)regID;

	if(HAL_I2C_Master_Transmit(&hi2c1, 0x78, (uint8_t*)tx_buffer, 2, 100) != HAL_OK)
		return false;

	if(HAL_I2C_Master_Receive(&hi2c1, 0x79, (uint8_t*)regDat, 1, 100) != HAL_OK)
	  return false;

	return true;
}
