/*
 * myADC.h
 *
 *  Created on: 2017/03/07
 *      Author: tattaka
 */

#ifndef USERINC_MYADC_H_
#define USERINC_MYADC_H_

#include "stm32f4xx_hal.h"

HAL_StatusTypeDef myADC_ConfigChannel(ADC_HandleTypeDef *hadc, uint32_t channel);


#endif /* USERINC_MYADC_H_ */
