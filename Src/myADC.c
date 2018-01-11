/*
 * myADC.c
 *
 *  Created on: 2017/03/07
 *      Author: tattaka
 */

#include "adc.h"
#include "myADC.h"

HAL_StatusTypeDef myADC_ConfigChannel(ADC_HandleTypeDef *hadc, uint32_t channel)
{
    ADC_ChannelConfTypeDef sConfig;

    sConfig.Channel = channel;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    return HAL_ADC_ConfigChannel(hadc, &sConfig);
}
