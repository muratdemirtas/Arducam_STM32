
#include <HL_Drivers/Camera/sensors/OV5642/ov5642_regs.c>
#include "main.h"
#include "ArduCAM.h"
#include "HL_Drivers/Camera/sensors/sscb_bus.h"
#include "HL_Drivers/Camera/sensors/OV5642/ov5642.h"
#include "spi.h"
#include "string.h"
#include "i2c.h"
#include "usart.h"


void ArduCAM_Init()
{

	Arducam_Reset_Arduchip();

	SSCB_wrSensorReg16_8(0x3008, 0x80);
	SSCB_wrSensorRegs16_8(OV5642_QVGA_Preview);
  	HAL_Delay(100);

  	SSCB_wrSensorRegs16_8(OV5642_JPEG_Capture_QSXGA);
  	SSCB_wrSensorRegs16_8(ov5642_320x240);
	HAL_Delay(100);
	SSCB_wrSensorReg16_8(0x3818, 0xa8);
	SSCB_wrSensorReg16_8(0x3621, 0x10);
	SSCB_wrSensorReg16_8(0x3801, 0xb0);
	SSCB_wrSensorReg16_8(0x4407, 0x04);
	SSCB_wrSensorReg16_8(0x5888, 0x00);

    Arducam_Set_Bit(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);

	Arducam_Clear_FIFO_Flag();
	Arducam_Write_Register(ARDUCHIP_FRAMES, 0x00);
	OV5642_Set_JPEG_Size(OV5642_320x240 );
	HAL_Delay(100);
}

void Arducam_Send_Dummy(){
	ARDUCAM_CS_LOW;
	uint8_t tx_buf[1] = {0x0};
	HAL_SPI_Transmit(&hspi1,(uint8_t*)tx_buf, 1, 100);
	ARDUCAM_CS_HIGH;
}
/**
 *
 * @return
 */
bool Arducam_SPI_Bus_Test(){
	uint8_t reg_value = 0, received_data = 0, test_pass = 0, test_fail = 0;
	Arducam_Send_Dummy();

	for(uint8_t iterator = 0 ; iterator < 255 ; iterator++){
		Arducam_Write_Register(ARDUCHIP_TEST1, reg_value);
		received_data = Arducam_Read_Register(ARDUCHIP_TEST1);
		HAL_Delay(1);
		if (received_data == reg_value)
			test_pass++;
		else
			test_fail++;
		reg_value++;
	}
	if(test_pass == 255){
		return true;
	}else
		return false;
}
/**
 *
 * @return
 */
bool Arducam_Mini_5MP_Init(){
	bool ret_val = false;
	ret_val = Arducam_SPI_Bus_Test();
	ret_val = Camera_Test_SSCB_Bus(OV5642_CHIPID_HIGH, OV5642_CHIPID_LOW);

	ArduCAM_Init();
	HAL_Delay(1000);
	return ret_val;
}
