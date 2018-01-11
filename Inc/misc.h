/*
 * misc.h
 *
 *  Created on: 2017/03/07
 *      Author: tattaka
 */

#ifndef USERINC_MISC_H_
#define USERINC_MISC_H_

#include "stm32f4xx_hal.h"
#include "robot.h"
#include "tim.h"

uint32_t DWT_Delay_Init(void);
uint32_t DWT_Get_Cycle(void);
float get_process_time(uint32_t, uint32_t);
__STATIC_INLINE void DWT_Delay_us(volatile uint32_t microseconds){
  uint32_t clk_cycle_start = DWT->CYCCNT;
  /* Go to number of cycles for system */
  microseconds *= (HAL_RCC_GetHCLKFreq() / 1000000);
  /* Delay till end */
  while ((DWT->CYCCNT - clk_cycle_start) < microseconds);
}

void music(uint32_t, uint32_t, uint32_t, int);
void mode_select();
#endif /* USERINC_MISC_H_ */
