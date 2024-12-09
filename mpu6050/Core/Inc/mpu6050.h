/*
 * mpu6050.h
 *
 *  Created on: Dec 5, 2024
 *      Author: babeler
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#define DEVICE_ADDRESS 0x68

#define FS_GYRO_250 0
#define FS_GYRO_500 8
#define FS_GYRO_1000 9
#define FS_GYRO_2000 10

#define FS_ACC_2G 0
#define FS_ACC_4G 8
#define FS_ACC_8G 9
#define FS_ACC_16G 10

#define REG_CONFIG_GYRO 27
#define REG_CONFIG_ACC 28
#define REG_USR_CTRL 107
#define REG_DATA_ACCEL_X 59
#define REG_DATA_ACCEL_Y 61
#define REG_DATA_ACCEL_Z 63
#define REG_DATA_GYR_X 67
#define REG_DATA_GYR_Y 69
#define REG_DATA_GYR_Z 71


void mpu6050_init();
void mpu6050_read();

#endif /* INC_MPU6050_H_ */
