/*
 * encoder.c
 *
 *  Created on: 2017/03/08
 *      Author: tattaka
 */


#include "encoder.h"
#include "tim.h"


int32_t read_encoder_R(void)
{
	uint16_t enc_cnt = ENCODER_R_CNT;
	int32_t enc_R_read = 0;
	ENCODER_R_CNT = 0;
	if (enc_cnt > 32767){
		enc_R_read = -(int16_t)enc_cnt;
	}
	else {
		enc_R_read = -enc_cnt;
	}
	return enc_R_read;
}

int32_t read_encoder_L(void)
{
	uint16_t enc_cnt = ENCODER_L_CNT;
	int32_t enc_L_read = 0;
	ENCODER_L_CNT = 0;
	if (enc_cnt > 32767){
		enc_L_read = (int16_t)enc_cnt;
	}
	else{
		enc_L_read = enc_cnt;
	}
	return enc_L_read;
}
