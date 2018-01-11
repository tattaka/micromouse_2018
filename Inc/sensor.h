/*
 * sensor.h
 *
 *  Created on: 2017/06/16
 *      Author: tattaka
 */

#ifndef USERINC_SENSOR_H_
#define USERINC_SENSOR_H_

#include "stm32f4xx_hal.h"
#include "robot.h"
#include "machine_state.h"
//#include "dma.h"
#include "adc.h"

void sensing_start(void);
void sensing_stop(void);
int sensor_digital_read(int num);
int sensor_analog_read(int num);

#endif /* USERINC_SENSOR_H_ */
