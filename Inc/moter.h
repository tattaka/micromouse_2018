/*
 * moter.h
 *
 *  Created on: 2017/03/16
 *      Author: tattaka
 */

#ifndef USERINC_MOTER_H_
#define USERINC_MOTER_H_

#include "stm32f4xx_hal.h"
float accel2torque(float a); //トルクを出力する
float accel2valtage(float speed_now, float a);
void control_machine();
int rampud_control(float range, float accel, float decel, float max_speed, float end_speed);
int turn(float radius, float turn_rad);
int motion(int head);
int motion_full();
int motion_enpty();
int motion_enqueue(
float first_range,
float end_range,
float accel,
float decel,
float max_speed,
float e_speed,
float radius,
float turn_rad);
int motion_dequeue();
void motion_clear();
void fail_safe(void);
#endif /* USERINC_MOTER_H_ */
