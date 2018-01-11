/*
 * encoder.h
 *
 *  Created on: 2017/03/08
 *      Author: tattaka
 */

#ifndef USERINC_ENCODER_H_
#define USERINC_ENCODER_H_

#include "stm32f4xx_hal.h"
#include "robot.h"


int32_t read_encoder_R (void);
int32_t read_encoder_L (void);

#endif /* USERINC_ENCODER_H_ */
