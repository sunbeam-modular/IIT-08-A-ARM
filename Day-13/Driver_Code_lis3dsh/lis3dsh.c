/*
 * lis3dsh.c
 *
 *  Created on: Dec 7, 2025
 *      Author: kiran_z6dopa8
 */
#include"main.h"
#include"lis3dsh.h"

#define _LIS3DSH_CS_ENABLE  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET)
#define _LIS3DSH_CS_DISABLE HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET)

static SPI_HandleTypeDef accSPI_Handle;

void LIS3DSH_WriteIO(uint8_t reg,uint8_t *dataW ,uint8_t size )
{
	uint8_t spiReg = reg;
	//enable CS
	_LIS3DSH_CS_ENABLE;
	//send accel regr addr
	HAL_SPI_Transmit(&accSPI_Handle,&spiReg , 1, HAL_MAX_DELAY);
	//send data bytes
	HAL_SPI_Transmit(&accSPI_Handle, dataW  , 1, HAL_MAX_DELAY);
	//disable CS
	_LIS3DSH_CS_DISABLE;
}
void LIS3DSH_ReadIO(uint8_t reg, uint8_t *dataR ,uint8_t size )
{
	uint8_t spiBuf[4];
	spiBuf[0] = reg | 0x80;
	//enable CS
	_LIS3DSH_CS_ENABLE;
	//send accel regr addr (MSB=1 --R)
	HAL_SPI_Transmit(&accSPI_Handle,spiBuf , 1, HAL_MAX_DELAY);
	//receive data bytes
	HAL_SPI_Receive(&accSPI_Handle, spiBuf, size, HAL_MAX_DELAY);
	//disable CS
	_LIS3DSH_CS_DISABLE;

	for(uint8_t i = 0 ; i < size ; i++)
		dataR[i]=spiBuf[i];

}

void LIS3DSH_Init(SPI_HandleTypeDef *accSPI, LIS3DSH_InitTypedef *accInitDef)
{
	uint8_t spiData;
	//copy accSPI into global vat
	accSPI_Handle = *accSPI;
	//enable X,Y,Z axes and set output data rate --- CTRL_REG4
	spiData = accInitDef->dataRate | accInitDef->enableAxes;
	LIS3DSH_WriteIO(LIS_CTRL_REG4_ADDR, &spiData, 1);
	//if interrupt enable , enable DRDY interrupu --LIS_CTRL_REG4_ADDR --(HW)

}


LIS3DSH_RawData LIS3DSH_Get_RawData(void)
{
	uint8_t spiBuff[2];
	LIS3DSH_RawData rawData;

	//read 16-bit x axes data
	LIS3DSH_ReadIO(LIS_OUT_X_L_ADDR, spiBuff, 2);
	rawData.x= (((uint16_t) spiBuff[1]) << 8) + spiBuff[0];

	//read 16-bit y axes data
	LIS3DSH_ReadIO(LIS_OUT_Y_L_ADDR, spiBuff, 2);
	rawData.y= (((uint16_t) spiBuff[1]) << 8) + spiBuff[0];

	//read 16-bit z axes data
	LIS3DSH_ReadIO(LIS_OUT_Z_L_ADDR, spiBuff, 2);
	rawData.z= (((uint16_t) spiBuff[1]) << 8) + spiBuff[0];

	return rawData;
}
bool LIS3DSH_Data_Ready(void)
{
	uint8_t accStatus;
	do
	{
		LIS3DSH_ReadIO(LIS_STATUS_ADDR, &accStatus, 1);
	}while((accStatus & LIS_STATUS_XYZ_Msk) == 0);
	return true;

}



