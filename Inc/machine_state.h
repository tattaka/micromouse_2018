/*
 * machine_state.h
 *
 *  Created on: 2017/03/09
 *      Author: tattaka
 */

#ifndef USERINC_MACHINE_STATE_H_
#define USERINC_MACHINE_STATE_H_

#include "stm32f4xx_hal.h"
#include "stdbool.h"
#include "robot.h"
#include "parameter.h"


extern float adc_vat;

extern float gyro_z[2];
extern int32_t encoder_R[2];
extern int32_t encoder_L[2];
extern float speed_R[2];
extern float speed_L[2];
extern float accel_R;
extern float accel_L;
extern volatile float omega_debug;

extern int mode;

extern float sigma_speed_L;
extern float sigma_speed_R;
extern float sigma_omega;

extern int duty;
extern int outduty_L;
extern int outduty_R;

extern float err_buff_L;
extern float err_buff_R;
extern float err_buff_gyro;

extern volatile float target_speed;
extern volatile float target_omega;
extern volatile float target_accel;

extern volatile float run_time;
extern bool rampud_flag;
extern volatile float start_time;
extern volatile float acc_range;
extern volatile float dec_range;
extern int ramp_count;
extern bool turn_flag;


extern bool left_wall_controll; //enable = 1
extern bool right_wall_controll; //enable = 1

extern uint16_t sensor_value[SENSOR_NUM];
extern uint16_t sensor_buff[3][SENSOR_NUM];
typedef enum {
	SEN_OFF,
	SEN_ON
} sensor_state;
extern sensor_state sensor_toggle;

extern unsigned int white_data[MAZESIZE_X*MAZESIZE_Y]; //maze_data
extern unsigned char dire_solve[MAZESIZE_X*MAZESIZE_Y];


#endif /* USERINC_MACHINE_STATE_H_ */
