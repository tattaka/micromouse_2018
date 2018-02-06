/*
 * mpu6500.h
 *
 *  Created on: 2017/03/07
 *      Author: tattaka
 */

#ifndef USERINC_MPU6500_H_
#define USERINC_MPU6500_H_

#include "stm32f4xx_hal.h"
#include "robot.h"
#include "spi.h"
#include "stdbool.h"

/*MPU6500のレジスタマップ*/
#define MPU6500_RA_SELF_TEST_X_GYRO	0x00
#define MPU6500_RA_SELF_TEST_Y_GYRO	0x01
#define MPU6500_RA_SELF_TEST_Z_GYRO	0x02
#define MPU6500_RA_SELF_TEST_X_ACCEL	0x0D
#define MPU6500_RA_SELF_TEST_Y_ACCEL	0x0E
#define MPU6500_RA_SELF_TEST_Z_ACCEL	0x0F
#define MPU6500_RA_XG_OFFSET_H		0x13
#define MPU6500_RA_XG_OFFSET_L		0x14
#define MPU6500_RA_YG_OFFSET_H		0x15
#define MPU6500_RA_YG_OFFSET_L		0x16
#define MPU6500_RA_ZG_OFFSET_H		0x17
#define MPU6500_RA_ZG_OFFSET_L		0x18
#define MPU6500_RA_SMPLRT_DIV		0x19
#define MPU6500_RA_CONFIG			0x1A
#define MPU6500_RA_GYRO_CONFIG		0x1B
#define MPU6500_RA_ACCEL_CONFIG		0x1C
#define MPU6500_RA_ACCEL_CONFIG2	0x1D
#define MPU6500_RA_LP_ACCEL_ODR		0x1E
#define MPU6500_RA_WOM_THR			0x1F
#define MPU6500_RA_FIFO_EN			0x23
// ...
// 外部I2C周りは省略
// ...
#define MPU6500_RA_INT_PIN_CFG		0x37
#define MPU6500_RA_INT_ENABLE		0x38
#define MPU6500_RA_INT_STATUS		0x3A
#define MPU6500_RA_ACCEL_XOUT_H		0x3B
#define MPU6500_RA_ACCEL_XOUT_L		0x3C
#define MPU6500_RA_ACCEL_YOUT_H		0x3D
#define MPU6500_RA_ACCEL_YOUT_L		0x3E
#define MPU6500_RA_ACCEL_ZOUT_H		0x3F
#define MPU6500_RA_ACCEL_ZOUT_L		0x40
#define MPU6500_RA_TEMP_OUT_H		0x41
#define MPU6500_RA_TEMP_OUT_L		0x42
#define MPU6500_RA_GYRO_XOUT_H		0x43
#define MPU6500_RA_GYRO_XOUT_L		0x44
#define MPU6500_RA_GYRO_YOUT_H		0x45
#define MPU6500_RA_GYRO_YOUT_L		0x46
#define MPU6500_RA_GYRO_ZOUT_H		0x47
#define MPU6500_RA_GYRO_ZOUT_L		0x48
// ...
// 外部I2C周りは省略
// ...
#define MPU6500_RA_SIGNAL_PATH_RESET	0x68
#define MPU6500_RA_MOT_DETECT_CTRL		0x69
#define MPU6500_RA_USER_CTRL		0x6A
#define MPU6500_RA_PWR_MGMT_1		0x6B
#define MPU6500_RA_PWR_MGMT_2		0x6C
#define MPU6500_RA_FIFO_COUNTH		0x72
#define MPU6500_RA_FIFO_COUNTL			0x73
#define MPU6500_RA_FIFO_R_W			0x74
#define MPU6500_RA_WHO_AM_I			0x75
#define MPU6500_RA_XA_OFFSET_H		0x77
#define MPU6500_RA_XA_OFFSET_L		0x78
#define MPU6500_RA_YA_OFFSET_H		0x7A
#define MPU6500_RA_YA_OFFSET_L		0x7B
#define MPU6500_RA_ZA_OFFSET_H		0x7D
#define MPU6500_RA_ZA_OFFSET_L		0x7E

#define MPU6500_DEVICE_ID			0x70
/* ジャイロのゲインがフルスケールで2000dpsの時の値*/
#define GYRO_FACTOR  16.4f

#define GYRO_OFFSET_CNT 1000


void spi_write_reg(SPI_HandleTypeDef * 	hspi, uint8_t reg_addr, uint8_t data);
void spi_read_reg(SPI_HandleTypeDef * 	hspi, uint8_t reg_addr, uint8_t *buffer, size_t size);
void MPU6500_init();
float MPU6500_read_gyro_z();
void MPU6500_calib_offset();
#endif /* USERINC_MPU6500_H_ */
