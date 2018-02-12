/*
 * misc.c
 *
 *  Created on: 2017/03/07
 *      Author: tattaka
 */

#include "misc.h"
#include "myPWM.h"
#include "machine_state.h"
#include "stdbool.h"
#include "parameter.h"
#include "sensor.h"

int mode;

uint32_t DWT_Delay_Init(void) {
  /* Disable TRC */
  CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk; // ~0x01000000;
  /* Enable TRC */
  CoreDebug->DEMCR |=  CoreDebug_DEMCR_TRCENA_Msk; // 0x01000000;

  /* Disable clock cycle counter */
  DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; //~0x00000001;
  /* Enable  clock cycle counter */
  DWT->CTRL |=  DWT_CTRL_CYCCNTENA_Msk; //0x00000001;

  /* Reset the clock cycle counter value */
  DWT->CYCCNT = 0;
	/* 3 NO OPERATION instructions */
	__ASM volatile ("NOP");
  __ASM volatile ("NOP");
  __ASM volatile ("NOP");

  /* Check if clock cycle counter has started */
	if(DWT->CYCCNT){
		return 0; /*clock cycle counter started*/
	}
	else{
		return 1; /*clock cycle counter not started*/
  }
}

uint32_t DWT_Get_Cycle(void){
	uint32_t cycle = DWT->CYCCNT;
	return cycle;
}

float get_process_time(uint32_t start,uint32_t stop){
	float time = ((float)stop-(float)start)/(float)SystemCoreClock;
	return time*1000000;
}

void music(uint32_t pulse, uint32_t prescaler, uint32_t delay, int state){
	if(state == 1){
	MUSIC_TIM.Init.Prescaler = prescaler;
	HAL_TIM_Base_Init(&MUSIC_TIM);
	myPWM_ConfigChannel(&MUSIC_TIM, pulse, MUSIC_CHAN);
	HAL_TIM_PWM_Start(&MUSIC_TIM, MUSIC_CHAN); //starts PWM on CH1N pin

	HAL_Delay(delay);
	}
	else if(state == 0){
	HAL_TIM_PWM_Stop(&MUSIC_TIM, MUSIC_CHAN);
	}
}

void mode_select(){
	bool flag = true;
	int mode_temp = 0;
	int mode_buff = 0;
	int musiccount[14] = {1532, 1362, 1213, 1145, 1020, 909, 809, 764, 681, 607, 572, 510, 454, 404};

	for(int i = 0; i < 3; i++){
		encoder_R[i] = 0;
		encoder_L[i] = 0;
	}
	HAL_TIM_Encoder_Start(&ENCODER_L_TIM,TIM_CHANNEL_1);
	HAL_TIM_Encoder_Start(&ENCODER_L_TIM,TIM_CHANNEL_2);
	HAL_TIM_Encoder_Start(&ENCODER_R_TIM,TIM_CHANNEL_1);
	HAL_TIM_Encoder_Start(&ENCODER_R_TIM,TIM_CHANNEL_2);
	while(flag){
		mode_temp = encoder_R[0] / ONEROTATE_TIRE;
		if(mode_temp > 6){
			mode_temp = 6;
			encoder_R[0] = ONEROTATE_TIRE * 6;
		}
		if(mode_temp < 0){
			mode_temp = 0;
			encoder_R[0] = 0;
		}
		if((mode_temp-mode_buff)!=0){
			music(5, musiccount[6-(mode_temp-mode_buff)], 100, 1);
			music(0, 0, 0, 0);
		}
		DEBUG_LED1_RESET();
		DEBUG_LED2_RESET();
		DEBUG_LED3_RESET();
		DEBUG_LED4_RESET();
		DEBUG_LED5_RESET();
		DEBUG_LED6_RESET();
		if(mode_temp == 1){
			DEBUG_LED1_SET();
		}
		else if(mode_temp == 2){
			DEBUG_LED1_SET();
			DEBUG_LED2_SET();
		}
		else if(mode_temp == 3){
			DEBUG_LED1_SET();
			DEBUG_LED2_SET();
			DEBUG_LED3_SET();
		}
		else if(mode_temp == 4){
			DEBUG_LED1_SET();
			DEBUG_LED2_SET();
			DEBUG_LED3_SET();
			DEBUG_LED4_SET();
		}
		else if(mode_temp == 5){
			DEBUG_LED1_SET();
			DEBUG_LED2_SET();
			DEBUG_LED3_SET();
			DEBUG_LED4_SET();
			DEBUG_LED5_SET();
		}
		else if(mode_temp == 6){
			DEBUG_LED1_SET();
			DEBUG_LED2_SET();
			DEBUG_LED3_SET();
			DEBUG_LED4_SET();
			DEBUG_LED5_SET();
			DEBUG_LED6_SET();
		}

		if(sensor_digital_read(3) == 1){
      mode = mode_temp;
      for(int i = 0; i < 2; i++){
        encoder_R[i] = 0;
        encoder_L[i] = 0;
      }
      flag = false;
		}
		mode_buff = mode_temp;
	}
}
