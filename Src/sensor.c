/*
 * sensor.c
 *
 *  Created on: 2017/06/16
 *      Author: tattaka
 */

#include "sensor.h"

void sensing_start(void){
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)sensor_value, SENSOR_NUM);
}

void sensing_stop(void){
	HAL_ADC_Stop_DMA(&hadc1);
	for(int i = 0; i < 6; i++){
		sensor_value[i] = 0;
	}
}

int sensor_digital_read(int num){
	if(num == 1){
		if(sensor_value[0] > SENSOR1_THRESHOLD){
			return 0;
		}
		else{
			return 1;
		}
	}
	else if(num == 2){
		if(sensor_value[1] > SENSOR2_THRESHOLD){
			return 0;
		}
		else{
			return 1;
		}
	}
	else if(num == 3){
		if(sensor_value[2] > SENSOR3_THRESHOLD){
			return 0;
		}
		else{
			return 1;
		}
	}
	else if(num == 4){
		if(sensor_value[3] > SENSOR4_THRESHOLD){
			return 0;
		}
		else{
			return 1;
		}
	}
	else if(num == 5){
		if(sensor_value[4] > SENSOR5_THRESHOLD){
			return 0;
		}
		else{
			return 1;
		}
	}
	else if(num == 6){
		if(sensor_value[5] > SENSOR6_THRESHOLD){
			return 0;
		}
		else{
			return 1;
		}
	}
	else{
		return 0;
	}
}
int sensor_analog_read(int num){
	if(sensor_digital_read(num) == 0){
		return 0;
	}
	else if((num < 1)&&(num > 6)){
		return 0;
	}
	else{
		return sensor_value[num-1];
	}
}
