/*
 * lis3dsh.c
 *
 * Created on: Dec 7, 2025
 * Author: kiran_z6dopa8
 *
 * Purpose: SPI driver source file for LIS3DSH accelerometer
 */

#include "main.h"        // STM32 HAL and board-specific definitions
#include "lis3dsh.h"     // LIS3DSH register definitions and data structures

// Macro to ENABLE chip select (CS = LOW) for LIS3DSH
#define _LIS3DSH_CS_ENABLE   HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET)

// Macro to DISABLE chip select (CS = HIGH) for LIS3DSH
#define _LIS3DSH_CS_DISABLE  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET)

// Global SPI handle used by LIS3DSH driver
static SPI_HandleTypeDef accSPI_Handle;

/*
 * Function: LIS3DSH_WriteIO
 * Purpose : Write data to LIS3DSH register via SPI
 */
void LIS3DSH_WriteIO(uint8_t reg, uint8_t *dataW, uint8_t size)
{
	uint8_t spiReg = reg;            // Store register address

	_LIS3DSH_CS_ENABLE;              // Select LIS3DSH device

	HAL_SPI_Transmit(&accSPI_Handle, &spiReg, 1, HAL_MAX_DELAY); // Send register address

	HAL_SPI_Transmit(&accSPI_Handle, dataW, 1, HAL_MAX_DELAY);   // Send data to register

	_LIS3DSH_CS_DISABLE;             // Deselect LIS3DSH device
}

/*
 * Function: LIS3DSH_ReadIO
 * Purpose : Read data from LIS3DSH register via SPI
 */
void LIS3DSH_ReadIO(uint8_t reg, uint8_t *dataR, uint8_t size)
{
	uint8_t spiBuf[4];               // SPI buffer for received data

	spiBuf[0] = reg | 0x80;          // Set MSB = 1 to indicate READ operation

	_LIS3DSH_CS_ENABLE;              // Select LIS3DSH device

	HAL_SPI_Transmit(&accSPI_Handle, spiBuf, 1, HAL_MAX_DELAY);  // Send register address

	HAL_SPI_Receive(&accSPI_Handle, spiBuf, size, HAL_MAX_DELAY); // Receive data bytes

	_LIS3DSH_CS_DISABLE;             // Deselect LIS3DSH device

	for (uint8_t i = 0; i < size; i++)
		dataR[i] = spiBuf[i];         // Copy received data to output buffer
}

/*
 * Function: LIS3DSH_Init
 * Purpose : Initialize LIS3DSH accelerometer configuration
 */
void LIS3DSH_Init(SPI_HandleTypeDef *accSPI, LIS3DSH_InitTypedef *accInitDef)
{
	uint8_t spiData;

	accSPI_Handle = *accSPI;         // Store SPI handle locally for driver use

	spiData = accInitDef->dataRate | accInitDef->enableAxes; // Configure data rate & axes

	LIS3DSH_WriteIO(LIS_CTRL_REG4_ADDR, &spiData, 1); // Write config to CTRL_REG4
}

/*
 * Function: LIS3DSH_Get_RawData
 * Purpose : Read raw X, Y, Z axis acceleration data
 */
LIS3DSH_RawData LIS3DSH_Get_RawData(void)
{
	uint8_t spiBuff[2];              // Buffer for low & high byte
	LIS3DSH_RawData rawData;         // Structure to store raw data

	LIS3DSH_ReadIO(LIS_OUT_X_L_ADDR, spiBuff, 2);     // Read X-axis data
	rawData.x = (((uint16_t)spiBuff[1]) << 8) + spiBuff[0]; // Combine bytes

	LIS3DSH_ReadIO(LIS_OUT_Y_L_ADDR, spiBuff, 2);     // Read Y-axis data
	rawData.y = (((uint16_t)spiBuff[1]) << 8) + spiBuff[0];

	LIS3DSH_ReadIO(LIS_OUT_Z_L_ADDR, spiBuff, 2);     // Read Z-axis data
	rawData.z = (((uint16_t)spiBuff[1]) << 8) + spiBuff[0];

	return rawData;                  // Return raw accelerometer readings
}

/*
 * Function: LIS3DSH_Data_Ready
 * Purpose : Wait until new accelerometer data is available
 */
bool LIS3DSH_Data_Ready(void)
{
	uint8_t accStatus;

	do
	{
		LIS3DSH_ReadIO(LIS_STATUS_ADDR, &accStatus, 1); // Read status register
	}
	while ((accStatus & LIS_STATUS_XYZ_Msk) == 0);    // Wait until XYZ data ready

	return true;                     // Data is ready to read
}
