/*
 * interrupt.c
 *
 *  Created on: 2017/03/09
 *      Author: tattaka
 */

#include "interrupt.h"
#include "mpu6500.h"
#include "encoder.h"
#include "macro.h"
#include "moter.h"
#include "myPWM.h"
#include "misc.h"
#include "sensor.h"
#include <string.h>

float adc_vat = 0;

float gyro_z[2] = {0};
int32_t encoder_R[2] = {0};
int32_t encoder_L[2] = {0};
float speed_L[2] = {0};
float speed_R[2] = {0};
float accel_L = 0;
float accel_R = 0;

uint16_t sensor_value[SENSOR_NUM] = {0};
uint16_t sensor_buff[3][SENSOR_NUM] = {{0}};
sensor_state sensor_toggle = SEN_OFF;

volatile float omega_debug = 0;
volatile int gyro_calib_count = 0;



void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef * htim){
	if (htim->Instance == htim5.Instance) {
		/*100us周期でジャイロ・エンコーダ・速度・加速度を取得*/
		HAL_ADC_Start_IT(&VAT_ADC);
		encoder_R[1] = encoder_R[0];
		encoder_L[1] = encoder_L[0];
		encoder_R[0] += read_encoder_R();
		encoder_L[0] += read_encoder_L();
		speed_L[1] = speed_L[0];
		speed_R[1] = speed_R[0];
		speed_L[0] = ENCODER2LEN(encoder_L[0]-encoder_L[1])/(DELTA_T/10);
		speed_R[0] = ENCODER2LEN(encoder_R[0]-encoder_R[1])/(DELTA_T/10);
		accel_L = (speed_L[0] - speed_L[1])/DELTA_T/10;
		accel_R = (speed_R[0] - speed_R[1])/DELTA_T/10;
	}
	if (htim->Instance == htim6.Instance) {
		/*1ms周期で制御をかける*/
		gyro_z[1] = gyro_z[0];

		gyro_z[0] = MPU6500_read_gyro_z();
		if(gyro_calib_count <= 1000){
			gyro_z[0] = 0;
			gyro_calib_count++;
		}
		else if(gyro_calib_count > 1000){
			omega_debug = omega_debug + gyro_z[0] * DELTA_T;
			control_machine();
		}
	}
	if(htim->Instance == htim7.Instance){
		DEBUG_LED1_RESET();
		DEBUG_LED2_RESET();
		DEBUG_LED3_RESET();
		DEBUG_LED4_RESET();
		DEBUG_LED5_RESET();
		DEBUG_LED6_RESET();
		if(adc_vat >= 7.4 && adc_vat < 7.56){
			DEBUG_LED1_SET();
		}
		else if(adc_vat >= 7.56 && adc_vat < 7.72){
			DEBUG_LED1_SET();
			DEBUG_LED2_SET();
		}
		else if(adc_vat >= 7.72 && adc_vat < 7.88){
			DEBUG_LED1_SET();
			DEBUG_LED2_SET();
			DEBUG_LED3_SET();
		}
		else if(adc_vat >= 7.88 && adc_vat < 8.02){
			DEBUG_LED1_SET();
			DEBUG_LED2_SET();
			DEBUG_LED3_SET();
			DEBUG_LED4_SET();
		}
		else if(adc_vat >= 8.02 && adc_vat < 8.2){
			DEBUG_LED1_SET();
			DEBUG_LED2_SET();
			DEBUG_LED3_SET();
			DEBUG_LED4_SET();
			DEBUG_LED5_SET();
		}
		else if(adc_vat >= 8.2){
			DEBUG_LED1_SET();
			DEBUG_LED2_SET();
			DEBUG_LED3_SET();
			DEBUG_LED4_SET();
			DEBUG_LED5_SET();
			DEBUG_LED6_SET();
		}
	}
	if(HAL_ADC_Start_DMA(&hadc1, (uint32_t *)sensor_buff[2], SENSOR_NUM)!=HAL_OK){
		Error_Handler();
	}
}

void HAL_ADC_ConvCpltCallback (ADC_HandleTypeDef * hadc){
	if(hadc->Instance == SENSOR_ADC.Instance){
		if(sensor_toggle == SEN_OFF){
			memcpy(sensor_buff[0], sensor_buff[2], sizeof(uint16_t)*SENSOR_NUM);
			SENSOR_LED1_SET();
			SENSOR_LED2_SET();
			SENSOR_LED3_SET();
			DWT_Delay_us(10);
			sensor_toggle = SEN_ON;
		}
		else{
			memcpy(sensor_buff[1], sensor_buff[2], sizeof(uint16_t)*SENSOR_NUM);
			SENSOR_LED1_RESET();
			SENSOR_LED2_RESET();
			SENSOR_LED3_RESET();
			DWT_Delay_us(10);
			sensor_toggle = SEN_OFF;
		}
		for(int i = 0; i < SENSOR_NUM; i++){
			sensor_value[i] = sensor_buff[1][i] - sensor_buff[0][i];
			if(sensor_value[i] > 32768){
				sensor_value[i] = 0;
			}
		}
	}
	if(hadc->Instance == VAT_ADC.Instance){
			adc_vat = ((float)HAL_ADC_GetValue(&VAT_ADC)*3.3f / 4096.0f)*3;
	}
}
