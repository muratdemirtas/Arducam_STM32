/**
 * @brief
 * @author murat.demirtas@doktar.com
 * @file ov5642.h
 *
 */

#ifndef HL_DRIVERS_CAMERA_SENSORS_OV5642_OV5642_H_
#define HL_DRIVERS_CAMERA_SENSORS_OV5642_OV5642_H_

#include "Includes/doktarApi.h"



#define OV5642_320x240 		0	//320x240
#define OV5642_640x480		1	//640x480
#define OV5642_1024x768		2	//1024x768
#define OV5642_1280x960 	3	//1280x960
#define OV5642_1600x1200	4	//1600x1200
#define OV5642_2048x1536	5	//2048x1536
#define OV5642_2592x1944	6	//2592x1944


/**
 * This function will test camera communication over sscb or I2C bus. Some
 * cameras supporting I2C protocol.
 * @param chip_id_high_addr Camera sensor chip id high(vid) addr
 * @param chip_id_low_addr Camera sensor chip id low(pid) addr
 * @return is test passed?
 */
bool Camera_Test_SSCB_Bus(uint16_t chip_id_high_addr, uint16_t chip_id_low_addr);

/**
 * This function will set JPEG resolution in camera sensor
 * @param size new resolution(max 5MP)
 */
void OV5642_Set_JPEG_Size(uint8_t size);

#endif /* HL_DRIVERS_CAMERA_SENSORS_OV5642_OV5642_H_ */
