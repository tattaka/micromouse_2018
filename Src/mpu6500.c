/*
 * mpu6500.c
 *
 *  Created on: 2017/03/07
 *      Author: tattaka
 */

#include "mpu6500.h"
#include "string.h"
#include "math.h"
#include "machine_state.h"

static float gyro_z_offset = 0.0;
static bool offset_calc = false;
static float offset_sum = 0.0;
static uint16_t offset_cnt = 0;

void spi_write_reg(SPI_HandleTypeDef * 	hspi, uint8_t reg_addr, uint8_t data)
{
	MPU6500_CS_RESET();

	uint8_t send_data[2];
	send_data[0] = reg_addr & 0x7F;  // アドレスはwriteに指定
	send_data[1] = data;

	uint8_t dummy[2];
	HAL_SPI_TransmitReceive(hspi, send_data, dummy, 2, 100);

	MPU6500_CS_SET();
}

void spi_read_reg(SPI_HandleTypeDef * hspi, uint8_t reg_addr, uint8_t *buffer, size_t size)
{
	MPU6500_CS_RESET();

	// 先頭バイトで読み取るレジスタのアドレスを指定するので
	// サイズはsize+1とする
	// Spi_start_xchg, memcpyにおいても同様
	uint8_t send_data[size+1];
	uint8_t rcv_data[size+1];
	send_data[0] = reg_addr | 0x80; // アドレスはreadに指定

	HAL_SPI_TransmitReceive(hspi, send_data, rcv_data, size+1, 100);

	memcpy(buffer, rcv_data+1, size);

	MPU6500_CS_SET();
}

void MPU6500_init()
{
	// リセット
	MPU6500_SPI.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
	HAL_SPI_Init(&MPU6500_SPI);
	spi_write_reg(&MPU6500_SPI, MPU6500_RA_PWR_MGMT_1, 0x80);
	for (volatile int i=0;i<100000;i++);

	// MPU6500のデバイスIDをチェック
	uint8_t device_id;
	spi_read_reg(&MPU6500_SPI, MPU6500_RA_WHO_AM_I, &device_id, 1);
	if (device_id != MPU6500_DEVICE_ID) {
		//error
		while (1);
	}

	// スリープ解除
	spi_write_reg(&MPU6500_SPI, MPU6500_RA_PWR_MGMT_1, 0x00);

	// DLPF_CFG = 0 : GyroのLPFを無効化
	// FIFOは使わない
	spi_write_reg(&MPU6500_SPI, MPU6500_RA_CONFIG, 0x00);

	// Gyroのフルスケールを+-2000dpsに設定
	// FCHOICE_B = b00 : Band=8600Hz, Delay=0.064ms, Fs=32kHz
	spi_write_reg(&MPU6500_SPI, MPU6500_RA_GYRO_CONFIG, 0x18);

	MPU6500_SPI.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	HAL_SPI_Init(&MPU6500_SPI);
}

float MPU6500_read_gyro_z()
{
	uint8_t buf[2];
	spi_read_reg(&MPU6500_SPI, MPU6500_RA_GYRO_ZOUT_H, buf, 2);
	const int16_t gyro_z = (int16_t)(((uint16_t)buf[0] << 8) | ((uint16_t)buf[1]));

	// /GYRO_FACTORで[deg/s]
	// /180*PIで[rad/s]
	const float omega = (float)gyro_z / GYRO_FACTOR / 180.0f * M_PI;
	//const float omega = (float)gyro_z / 32768 * 2000;
	if (offset_calc) {
		offset_cnt++;
		offset_sum += omega;
		if (offset_cnt > GYRO_OFFSET_CNT) {
			gyro_z_offset = offset_sum / GYRO_OFFSET_CNT;
			offset_cnt = 0;
			offset_sum = 0;
			offset_calc = false;
		}
	}
	//gyro_z_offset = -0.0835;
	return omega - gyro_z_offset;
}

void MPU6500_calib_offset()
{
	offset_calc = true;
}
