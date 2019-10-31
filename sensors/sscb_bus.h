/**
 * @file sscb_bus.h
 * @author murat.demirtas@doktar.com
 *  Created on: Oct 29, 2019
 *      Author: fullstackcoder
 */

#ifndef HL_DRIVERS_CAMERA_SENSORS_SSCB_BUS_H_
#define HL_DRIVERS_CAMERA_SENSORS_SSCB_BUS_H_
#include "Includes/doktarApi.h"

#ifdef DOKTAR_API_USE_SSCB

#define OV5642_CHIPID_HIGH 0x300a
#define OV5642_CHIPID_LOW 0x300b
#define OV5642_CHIP_SSCB_BUS_ADDR_W 0x78

#define SSCB_DELAY_MS(delay) HAL_Delay(delay)
#if !defined(SSCB_DELAY_MS) || !defined(SSCB_I2C_BUS) || !defined(SSCB_TIMEOUT)
#error please define Arducam SPI settings
#endif
/**
 * This function will help you for setting register values on camera sensor
 * @param address Target camera sscb bus device address
 * @param regID Target camera sensor register address
 * @param regDat Target camera sensor register value
 * @return is SSCB bus communication is ok?
 */
bool SSCB_wrSensorReg16_8(uint16_t address, uint16_t regID, uint16_t regDat);
/**
 * This function will send all sensor camera settings struct's to camera sensor over
 * I2C or SSCB bus
 * @param reglist target camera settings struct
 * @return I2C or SSCB bus error size
 */
int SSCB_wrSensorRegs16_8(const struct sensor_reg reglist[]);
/**
 * This function will help you for reading register values on camera sensor
 * @param address Target camera sscb bus device address
 * @param regID Target camera sensor register address
 * @param regDat Target camera sensor register value
 * @return is SSCB bus communication is ok?
 */
bool SSCB_rdSensorReg16_8(uint16_t address, uint16_t regID, uint8_t* regDat);
#endif
#endif /* HL_DRIVERS_CAMERA_SENSORS_SSCB_BUS_H_ */
