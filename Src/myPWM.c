/*
 * myPWM.c
 *
 *  Created on: 2017/03/07
 *      Author: tattaka
 */

#include "myPWM.h"

HAL_StatusTypeDef myPWM_ConfigChannel(TIM_HandleTypeDef *htim, uint32_t pulse, uint32_t channel)
{
	TIM_OC_InitTypeDef sConfigOC;
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = pulse;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;

  return HAL_TIM_PWM_ConfigChannel(htim, &sConfigOC, channel);
}
