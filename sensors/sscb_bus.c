/**
 * @file sscb_bus.c
 * @author muratdemirtas@doktar.com
 */

#include "sscb_bus.h"
#ifdef DOKTAR_API_USE_SSCB
/**
 * This function will help you for setting register values on camera sensor
 * @param address Target camera sscb bus device address
 * @param regID Target camera sensor register address
 * @param regDat Target camera sensor register value
 * @return is SSCB bus communication is ok?
 */
bool SSCB_wrSensorReg16_8(uint16_t address, uint16_t regID, uint16_t regDat)
{
	uint8_t tx_buffer[3];
	tx_buffer[0] = regID>>8;
	tx_buffer[1] = (uint8_t)regID;
	tx_buffer[2] = (uint8_t)regDat;

#if API_SSCB_AS_I2C
	if(HAL_I2C_Master_Transmit(SSCB_I2C_BUS, address,
								(uint8_t*)tx_buffer, sizeof(tx_buffer), SSCB_TIMEOUT) != HAL_OK)
		return false;
	SSCB_DELAY_MS(1);
#else
	#error not implemented bitbanging gpio module
#endif

	return true;
}
/**
 * This function will send all sensor camera settings struct's to camera sensor over
 * I2C or SSCB bus
 * @param reglist target camera settings struct
 * @return I2C or SSCB bus error size
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

    err = SSCB_wrSensorReg16_8(OV5642_CHIP_SSCB_BUS_ADDR_W, reg_addr, reg_val);
    next++;
  }
  return err;
}
/**
 * This function will help you for reading register values on camera sensor
 * @param address Target camera sscb bus device address
 * @param regID Target camera sensor register address
 * @param regDat Target camera sensor register value
 * @return is SSCB bus communication is ok?
 */
bool SSCB_rdSensorReg16_8(uint16_t address, uint16_t regID, uint8_t* regDat)
{

	uint8_t tx_buffer[2];
	tx_buffer[0] = regID>>8;
	tx_buffer[1] = (uint8_t)regID;
	uint16_t rw_address = address;

#if API_SSCB_AS_I2C

	if(HAL_I2C_Master_Transmit(SSCB_I2C_BUS, rw_address, (uint8_t*)tx_buffer, sizeof(tx_buffer), SSCB_TIMEOUT) != HAL_OK)
		return false;

	rw_address = rw_address + 1;

	if(HAL_I2C_Master_Receive(SSCB_I2C_BUS, rw_address, (uint8_t*)regDat, sizeof(tx_buffer) - 1,  SSCB_TIMEOUT) != HAL_OK)
	  return false;

	SSCB_DELAY_MS(1);
#else
	#error not implemented bitbanging gpio module
#endif

	return true;
}
#endif
