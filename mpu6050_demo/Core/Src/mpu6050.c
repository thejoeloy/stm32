/*
 * mpu6050.c
 *
 *  Created on: Feb 17, 2024
 *      Author: thejoey
 */
#include <main.h>
#include <stdio.h>
#include "mpu6050.h"

extern I2C_HandleTypeDef hi2c1;

void mpu6050_init() {
	if (HAL_I2C_IsDeviceReady(&hi2c1, (DEVICE_ADDRESS << 1) + 0, 1, 100) == HAL_OK) {
		  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		  printf("Device is ready \n");
	} else {
		printf("Device is not ready, please adjust cables\n");
	}

	uint8_t temp_data = FS_GYRO_500;
	if (HAL_I2C_Mem_Write(&hi2c1, (DEVICE_ADDRESS << 1) + 0, REG_CONFIG_GYRO, 1, &temp_data, 1, 100) == HAL_OK) {
		printf("Configuring Gyroscope\n");
	} else {
		printf("Failed to write to configure gyroscope\n ");
	}

	temp_data = FS_ACC_4G;
	if (HAL_I2C_Mem_Write(&hi2c1, (DEVICE_ADDRESS << 1) + 0, REG_CONFIG_ACC, 1, &temp_data, 1, 100) == HAL_OK) {
		printf("Configuring accelerometer\n");
	} else {
		printf("Failed to configure accelerometer\n ");
	}

	temp_data = 0;
	if (HAL_I2C_Mem_Write(&hi2c1, (DEVICE_ADDRESS << 1) + 0, REG_USR_CTRL, 1, &temp_data, 1, 100) == HAL_OK) {
		printf("Exiting sleep mode\n");
	} else {
		printf("Failed to exit from sleep mode\n ");
	}
}

void mpu6050_read() {
	uint8_t data_x[2];
	int16_t x_acc;
	HAL_I2C_Mem_Read(&hi2c1, (DEVICE_ADDRESS << 1) + 1, REG_DATA_ACCEL_X, 1, data_x, 2, 100);
	x_acc = ((int16_t)data_x[0] << 8) + data_x[1];
	printf("x axis acceleration: %d\n", x_acc);

	uint8_t data_y[2];
	int16_t y_acc;
	HAL_I2C_Mem_Read(&hi2c1, (DEVICE_ADDRESS << 1) + 1, REG_DATA_ACCEL_Y, 1, data_y, 2, 100);
	y_acc = ((int16_t)data_y[0] << 8) + data_y[1];
	printf("y axis acceleration: %d\n", y_acc);

	uint8_t data_z[2];
	int16_t z_acc;
	HAL_I2C_Mem_Read(&hi2c1, (DEVICE_ADDRESS << 1) + 1, REG_DATA_ACCEL_Z, 1, data_z, 2, 100);
	z_acc = ((int16_t)data_z[0] << 8) + data_z[1];
	printf("z axis acceleration: %d\n", z_acc);

	uint8_t data_g_x[2];
	int16_t x_gyr;
	HAL_I2C_Mem_Read(&hi2c1, (DEVICE_ADDRESS << 1) + 1, REG_DATA_GYR_X, 1, data_g_x, 2, 100);
	x_gyr = ((int16_t)data_x[0] << 8) + data_x[1];
	printf("x axis gyroscope: %d\n", x_gyr);

	uint8_t data_g_y[2];
	int16_t y_gyr;
	HAL_I2C_Mem_Read(&hi2c1, (DEVICE_ADDRESS << 1) + 1, REG_DATA_GYR_Y, 1, data_g_y, 2, 100);
	y_gyr = ((int16_t)data_y[0] << 8) + data_g_y[1];
	printf("y axis gyroscope: %d\n", y_gyr);

	uint8_t data_g_z[2];
	int16_t z_gyr;
	HAL_I2C_Mem_Read(&hi2c1, (DEVICE_ADDRESS << 1) + 1, REG_DATA_GYR_Z, 1, data_g_z, 2, 100);
	z_gyr = ((int16_t)data_g_z[0] << 8) + data_g_z[1];
	printf("z axis gyroscope: %d\n", z_gyr);
}

