/*
 * myPWM.h
 *
 *  Created on: 2017/03/07
 *      Author: tattaka
 */

#ifndef USERINC_MYPWM_H_
#define USERINC_MYPWM_H_

#include "stm32f4xx_hal.h"
#include "tim.h"

HAL_StatusTypeDef myPWM_ConfigChannel(TIM_HandleTypeDef *htim, uint32_t pulse, uint32_t channel);



#endif /* USERINC_MYPWM_H_ */
