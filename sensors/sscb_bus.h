/*
 * sscb_bus.h
 *
 *  Created on: Oct 29, 2019
 *      Author: fullstackcoder
 */

#ifndef HL_DRIVERS_CAMERA_SENSORS_SSCB_BUS_H_
#define HL_DRIVERS_CAMERA_SENSORS_SSCB_BUS_H_


#include "Includes/doktarApi.h"


#define OV5642_CHIPID_HIGH 0x300a

#define OV5642_CHIPID_LOW 0x300b

struct sensor_reg {
	uint16_t reg;
	uint16_t val;
};

bool rdSensorReg16_8(uint16_t regID, uint8_t* regDat);
bool wrSensorReg16_8(uint16_t regID, uint16_t regDat);
int wrSensorRegs16_8(const struct sensor_reg reglist[]);

#endif /* HL_DRIVERS_CAMERA_SENSORS_SSCB_BUS_H_ */
