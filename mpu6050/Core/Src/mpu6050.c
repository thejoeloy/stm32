/*
 * mpu6050.c
 *
 *  Created on: Dec 5, 2024
 *      Author: babeler
 */

#include <main.h>
#include <stdio.h>
#include <string.h>
#include "mpu6050.h"

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;

void mpu6050_init() {
	char sensor_ready_buffer[] = "Device is ready\n";
	char sensor_not_ready_buffer[] = "Device is not ready\n";
	char gyro_ready_buffer[] = "Gyroscope configured\n";
	char gyro_not_ready_buffer[] = "Gyroscope failed\n";
	char accel_ready_buffer[] = "Accelerometer ready\n";
	char accel_not_ready_buffer[] = "Accelerometer failed\n";
	char sleep_ready_buffer[] = "Sleep exited\n";
	char sleep_not_ready_buffer[] = "Failed to exit sleep\n";

	if (HAL_I2C_IsDeviceReady(&hi2c1, (DEVICE_ADDRESS << 1) + 0, 1, 100) == HAL_OK) {
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		HAL_UART_Transmit(&huart2, (uint8_t*)sensor_ready_buffer, strlen(sensor_ready_buffer), 10);
	} else {
		HAL_UART_Transmit(&huart2, (uint8_t*)sensor_not_ready_buffer, strlen(sensor_not_ready_buffer), 10);
	}

	uint8_t temp_data = FS_GYRO_500;
	if (HAL_I2C_Mem_Write(&hi2c1, (DEVICE_ADDRESS << 1) + 0, REG_CONFIG_GYRO, 1, &temp_data, 1, 100) == HAL_OK) {
		HAL_UART_Transmit(&huart2, (uint8_t*)gyro_ready_buffer, strlen(gyro_ready_buffer), 10);
	} else {
		HAL_UART_Transmit(&huart2, (uint8_t*)gyro_not_ready_buffer, strlen(gyro_not_ready_buffer), 10);
	}

	temp_data = FS_ACC_4G;
	if (HAL_I2C_Mem_Write(&hi2c1, (DEVICE_ADDRESS << 1) + 0, REG_CONFIG_ACC, 1, &temp_data, 1, 100) == HAL_OK) {
		HAL_UART_Transmit(&huart2, (uint8_t*)accel_ready_buffer, strlen(accel_ready_buffer), 10);
	} else {
		HAL_UART_Transmit(&huart2, (uint8_t*)accel_not_ready_buffer, strlen(accel_not_ready_buffer), 10);
	}

	temp_data = 0;
	if (HAL_I2C_Mem_Write(&hi2c1, (DEVICE_ADDRESS << 1) + 0, REG_USR_CTRL, 1, &temp_data, 1, 100) == HAL_OK) {
		HAL_UART_Transmit(&huart2, (uint8_t*)sleep_ready_buffer, strlen(sleep_ready_buffer), 10);
	} else {
		HAL_UART_Transmit(&huart2, (uint8_t*)sleep_not_ready_buffer, strlen(sleep_not_ready_buffer), 10);
	}
}

void mpu6050_read() {
	uint8_t data_x[2];
	uint8_t data_y[2];
	uint8_t data_z[2];
	uint8_t data_g_x[2];
	uint8_t data_g_y[2];
	uint8_t data_g_z[2];

	int16_t x_acc;
	int16_t y_acc;
	int16_t z_acc;
	int16_t x_gyr;
	int16_t y_gyr;
	int16_t z_gyr;

	HAL_I2C_Mem_Read(&hi2c1, (DEVICE_ADDRESS << 1) + 1, REG_DATA_ACCEL_X, 1, data_x, 2, 100);
	HAL_I2C_Mem_Read(&hi2c1, (DEVICE_ADDRESS << 1) + 1, REG_DATA_ACCEL_Y, 1, data_y, 2, 100);
	HAL_I2C_Mem_Read(&hi2c1, (DEVICE_ADDRESS << 1) + 1, REG_DATA_ACCEL_Z, 1, data_z, 2, 100);
	HAL_I2C_Mem_Read(&hi2c1, (DEVICE_ADDRESS << 1) + 1, REG_DATA_GYR_X, 1, data_g_x, 2, 100);
	HAL_I2C_Mem_Read(&hi2c1, (DEVICE_ADDRESS << 1) + 1, REG_DATA_GYR_Y, 1, data_g_y, 2, 100);
	HAL_I2C_Mem_Read(&hi2c1, (DEVICE_ADDRESS << 1) + 1, REG_DATA_GYR_Z, 1, data_g_z, 2, 100);

	x_acc = ((int16_t)data_x[0] << 8) + data_x[1];
	y_acc = ((int16_t)data_y[0] << 8) + data_y[1];
	z_acc = ((int16_t)data_z[0] << 8) + data_z[1];
	x_gyr = ((int16_t)data_x[0] << 8) + data_g_x[1];
	y_gyr = ((int16_t)data_y[0] << 8) + data_g_y[1];
	z_gyr = ((int16_t)data_g_z[0] << 8) + data_g_z[1];

	char uart_buffer[100];
	snprintf(uart_buffer, sizeof(uart_buffer), "Accel: X=%d Y=%d Z=%d | Gyro: X=%d Y=%d Z=%d\n", x_acc, y_acc, z_acc, x_gyr, y_gyr, z_gyr);
	HAL_UART_Transmit(&huart2, (uint8_t*)uart_buffer, strlen(uart_buffer), 10);
}


