/*
 * interrupt.h
 *
 *  Created on: 2017/03/09
 *      Author: tattaka
 */

#ifndef USERINC_INTERRUPT_H_
#define USERINC_INTERRUPT_H_

#include "stm32f4xx_hal.h"
#include "adc.h"
#include "tim.h"
#include "robot.h"
#include "machine_state.h"
#include "parameter.h"


void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef * htim);
void HAL_ADC_ConvCpltCallback (ADC_HandleTypeDef * hadc);

#endif /* USERINC_INTERRUPT_H_ */
