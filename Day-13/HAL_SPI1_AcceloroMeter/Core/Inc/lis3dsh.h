/*
 * lis3dsh.h
 *
 * Created on: Dec 7, 2025
 * Author: kiran_z6dopa8
 *
 * Purpose: Header file for LIS3DSH accelerometer driver
 */

#ifndef INC_LIS3DSH_H_
#define INC_LIS3DSH_H_

#include <stdio.h>      // Standard I/O definitions
#include <stdbool.h>    // Boolean data type support
#include "main.h"       // STM32 HAL and SPI definitions

/* ===================== LIS3DSH REGISTER ADDRESSES ===================== */

/* Control and status registers used in this program */
#define LIS_CTRL_REG4_ADDR               0x20   // Control register 4 (ODR & axis enable)
#define LIS_STATUS_ADDR                  0x27   // Status register (data ready flags)

/* Output data registers (low & high byte for each axis) */
#define LIS_OUT_X_L_ADDR                 0x28   // X-axis low byte
#define LIS_OUT_X_H_ADDR                 0x29   // X-axis high byte
#define LIS_OUT_Y_L_ADDR                 0x2A   // Y-axis low byte
#define LIS_OUT_Y_H_ADDR                 0x2B   // Y-axis high byte
#define LIS_OUT_Z_L_ADDR                 0x2C   // Z-axis low byte
#define LIS_OUT_Z_H_ADDR                 0x2D   // Z-axis high byte

/* Registers not used but defined for reference */
#define LIS_WHO_AM_I_ADDR                0x0F   // Device identification register
#define LIS_OFF_X_ADDR                   0x10   // X-axis offset register
#define LIS_OFF_Y_ADDR                   0x11   // Y-axis offset register
#define LIS_OFF_Z_ADDR                   0x12   // Z-axis offset register
#define LIS_CTRL_REG1_ADDR               0x21   // Control register 1
#define LIS_CTRL_REG2_ADDR               0x22   // Control register 2
#define LIS_CTRL_REG3_ADDR               0x23   // Control register 3
#define LIS_CTRL_REG5_ADDR               0x24   // Control register 5
#define LIS_CTRL_REG6_ADDR               0x25   // Control register 6

/* ===================== DATA RATE CONFIGURATION ===================== */

/* Output data rate used in this program */
#define LIS_DATARATE_25                  ((uint8_t)0x40)  // 25 Hz normal mode

/* Other supported output data rates (not used here) */
#define LIS_DATARATE_POWERDOWN           ((uint8_t)0x00)  // Power-down mode
#define LIS_DATARATE_3_125               ((uint8_t)0x10)  // 3.125 Hz
#define LIS_DATARATE_6_25                ((uint8_t)0x20)  // 6.25 Hz
#define LIS_DATARATE_12_5                ((uint8_t)0x30)  // 12.5 Hz
#define LIS_DATARATE_50                  ((uint8_t)0x50)  // 50 Hz
#define LIS_DATARATE_100                 ((uint8_t)0x60)  // 100 Hz
#define LIS_DATARATE_400                 ((uint8_t)0x70)  // 400 Hz
#define LIS_DATARATE_800                 ((uint8_t)0x80)  // 800 Hz
#define LIS_DATARATE_1600                ((uint8_t)0x90)  // 1600 Hz

/* ===================== AXIS ENABLE MACROS ===================== */

/* Individual axis enable bits */
#define LIS_X_ENABLE                     ((uint8_t)(1 << 0)) // Enable X-axis
#define LIS_Y_ENABLE                     ((uint8_t)(1 << 1)) // Enable Y-axis
#define LIS_Z_ENABLE                     ((uint8_t)(1 << 2)) // Enable Z-axis

/* Enable X, Y and Z axes together */
#define LIS_XYZ_ENABLE                   (LIS_X_ENABLE | LIS_Y_ENABLE | LIS_Z_ENABLE)

/* ===================== STATUS REGISTER MASK ===================== */

/* Mask to check if new X, Y, Z data is available */
#define LIS_STATUS_XYZ_Msk               0x07

/* ===================== ACCELEROMETER CONFIG STRUCT ===================== */

/* Structure to hold LIS3DSH initialization parameters */
typedef struct {
	uint8_t dataRate;          // Output data rate selection
	uint8_t enableAxes;        // Axes enable configuration
	bool interruptEnable;      // Enable/disable data ready interrupt
} LIS3DSH_InitTypedef;

/* ===================== RAW DATA STRUCT ===================== */

/* Structure to store raw accelerometer readings */
typedef struct {
	int16_t x;                 // Raw X-axis acceleration
	int16_t y;                 // Raw Y-axis acceleration
	int16_t z;                 // Raw Z-axis acceleration
} LIS3DSH_RawData;

/* ===================== FUNCTION PROTOTYPES ===================== */

/* Initialize LIS3DSH accelerometer */
void LIS3DSH_Init(SPI_HandleTypeDef *accSPI, LIS3DSH_InitTypedef *accInitDef);

/* Read raw X, Y, Z axis acceleration data */
LIS3DSH_RawData LIS3DSH_Get_RawData(void);

/* Check if new accelerometer data is ready */
bool LIS3DSH_Data_Ready(void);

/* Low-level SPI write function */
void LIS3DSH_WriteIO(uint8_t reg, uint8_t *dataW, uint8_t size);

/* Low-level SPI read function */
void LIS3DSH_ReadIO(uint8_t reg, uint8_t *dataR, uint8_t size);

#endif /* INC_LIS3DSH_H_ */
