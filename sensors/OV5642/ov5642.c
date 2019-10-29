/*
 * ov5642.c
 *
 *  Created on: Oct 29, 2019
 *      Author: fullstackcoder
 */

#include <HL_Drivers/Camera/sensors/OV5642/ov5642_regs.c>
#include "ov5642.h"
#include "HL_Drivers/Camera/sensors/sscb_bus.h"

/**
 * This function will test camera communication over sscb or I2C bus. Some
 * cameras supporting I2C protocol.
 * @param chip_id_high_addr Camera sensor chip id high(vid) addr
 * @param chip_id_low_addr Camera sensor chip id low(pid) addr
 * @return is test passed?
 */
bool Camera_Test_SSCB_Bus(uint16_t chip_id_high_addr, uint16_t chip_id_low_addr){
	/* declaring test variables*/
	uint8_t vid_val = 0, pid_val =0, test_pass = 0, test_fail = 0;
	/* read test over sscb bus*/
	for(uint8_t i = 0; i < 5; i++)
	{
		SSCB_rdSensorReg16_8(chip_id_high_addr, &vid_val);
		SSCB_rdSensorReg16_8(chip_id_low_addr, &pid_val);
		if ((vid_val != 0x56) || (pid_val != 0x42))
			test_fail++;
		else
			test_pass++;
	}
	/*check counter for test*/
	if(test_pass == 5){
		return true;
	}else
		return false;
}

/**
 * This function will set JPEG resolution in camera sensor
 * @param size new resolution(max 5MP)
 */
void OV5642_Set_JPEG_Size(uint8_t size)
{
  switch (size)
  {
    case OV5642_320x240:
    	SSCB_wrSensorRegs16_8(ov5642_320x240);
      break;
    case OV5642_640x480:
    	SSCB_wrSensorRegs16_8(ov5642_640x480);
      break;
    case OV5642_1024x768:
    	SSCB_wrSensorRegs16_8(ov5642_1024x768);
      break;
    case OV5642_1280x960:
    	SSCB_wrSensorRegs16_8(ov5642_1280x960);
      break;
    case OV5642_1600x1200:
    	SSCB_wrSensorRegs16_8(ov5642_1600x1200);
      break;
    case OV5642_2048x1536:
    	SSCB_wrSensorRegs16_8(ov5642_2048x1536);
      break;
    case OV5642_2592x1944:
    	SSCB_wrSensorRegs16_8(ov5642_2592x1944);
      break;
    default:
    	SSCB_wrSensorRegs16_8(ov5642_320x240);
      break;
  }
}
