/*
 * moter.c
 *
 *  Created on: 2017/03/16
 *      Author: tattaka
 */

#define QUEUE_MAX 512

#include "machine_state.h"
#include "myPWM.h"
#include "tim.h"
#include "macro.h"
#include "encoder.h"
#include "math.h"
#include "moter.h"
#include "parameter.h"

float sigma_speed_L = 0;
float sigma_speed_R = 0;
float sigma_omega = 0;

int outduty_L = 0;
int outduty_R = 0;

float err_buff_L = 0;
float err_buff_R = 0;
float err_buff_gyro = 0;

float volatile target_speed = 0.0;
float volatile target_omega = 0.0;
float volatile target_accel = 0.0;
float volatile offset = 0.0;

float volatile run_time = 0.0;
bool rampud_flag = false;
float start_speed = 0.0;
volatile float acc_range = 0.0;
volatile float dec_range = 0.0;
int ramp_count = 0;
bool turn_flag = false;

typedef struct {
	float first_range;
	float end_range;
	float accel;
	float decel;
	float max_speed;
	float e_speed;
	float t_omega;
	float turn_rad;

} motion_parameter;

motion_parameter motion_queue[QUEUE_MAX];
int head = 0;
int tail = 0;

float accel2torque(float a){
	float t = 0;
	t = ((TIRE_DIAMETER / 2) * WEIGHT * a) / (2 * REDUCATION);
	return t;
}

float accel2valtage(float speed_now, float a){
	float v;
	float omega; //モータの回転数
	omega = SPEED2MOTERROTATE(speed_now);
	v = (R * accel2torque(a) / KM + KE * omega);
	return v;
}



void control_machine(){
	//speed[m/s], omega[rad/s]
	float target_speed_temp = target_speed;
	float speed = (speed_L[0] + speed_R[0])/2;
	if(target_accel > 0){
		if(target_speed > speed){
			target_speed = speed + target_accel * DELTA_T;
		}
		else{
			target_accel = 0;
		}
	}
	else if(target_accel < 0){
		if(target_speed < speed){
			target_speed = speed + target_accel * DELTA_T;
		}
		else{
			target_accel = 0;
		}
	}
	volatile float err_L = 0;
	volatile float err_R = 0;
	//volatile float err_gyro = 0;

	err_L = target_speed - speed_L[0];
	err_R = target_speed - speed_R[0];
	//err_gyro = target_omega - gyro_z[0];

	sigma_speed_L = sigma_speed_L + err_L * DELTA_T;
	sigma_speed_R = sigma_speed_R + err_R * DELTA_T;
	//sigma_omega = sigma_omega + err_gyro * DELTA_T;

	volatile float outL = 0;
	volatile float outR = 0;
	outL = target_accel + SPEED_L_KP * err_L + SPEED_L_KI * sigma_speed_L + SPEED_L_KD * (err_L-err_buff_L);
	outR = target_accel + SPEED_R_KP * err_R + SPEED_R_KI * sigma_speed_R + SPEED_R_KD * (err_R-err_buff_R);

	//outL -= (GYRO_KP * err_gyro + GYRO_KI * sigma_omega + GYRO_KD * (err_gyro-err_buff_gyro));
	//outR += (GYRO_KP * err_gyro + GYRO_KI * sigma_omega + GYRO_KD * (err_gyro-err_buff_gyro));

	outL = accel2valtage(speed_L[0], outL) ;
	outR = accel2valtage(speed_R[0], outR) ;

	err_buff_L = err_L;
	err_buff_R = err_R;
	//err_buff_gyro = err_gyro;

	if(outL > adc_vat){
		outL = adc_vat * 0.8;
	}
	else if(outL < -adc_vat){
		outL = -adc_vat * 0.8;
	}
	if(outR > adc_vat){
		outR = adc_vat * 0.8;
	}
	else if(outR < -adc_vat){
		outR = -adc_vat * 0.8;
	}
	outduty_L = DUTY_MAX * outL / adc_vat;
	outduty_R = DUTY_MAX * outR / adc_vat;

	target_speed = target_speed_temp;
	STBY_GPIO_SET();

	if(outduty_L < 0){
		outduty_L = -outduty_L;
		LMOTER1_GPIO_RESET();
		LMOTER2_GPIO_SET();
	}
	else{
		LMOTER1_GPIO_SET();
		LMOTER2_GPIO_RESET();
	}
	if(outduty_R < 0){
		outduty_R = -outduty_R;
		RMOTER1_GPIO_RESET();
		RMOTER2_GPIO_SET();
	}
	else{
		RMOTER1_GPIO_SET();
		RMOTER2_GPIO_RESET();
	}
	myPWM_ConfigChannel(&MOTER_TIM, outduty_L, MOTER_L_CHAN);
	myPWM_ConfigChannel(&MOTER_TIM, outduty_R, MOTER_R_CHAN);
	HAL_TIM_PWM_Start(&MOTER_TIM, MOTER_L_CHAN);
	HAL_TIM_PWM_Start(&MOTER_TIM, MOTER_R_CHAN);
}

int rampud_control(float range, float accel, float decel, float m_speed, float e_speed){
	float max_speed;
	range = range - offset;
	start_speed = (speed_L[0] + speed_R[0])/2;
	acc_range = (m_speed*m_speed-start_speed*start_speed)/2*accel;
	dec_range = (m_speed*m_speed-e_speed*e_speed)/2*(-decel);
	if(range < (acc_range + dec_range)){
		//距離が足りなかった時の処理
		max_speed = sqrt(((-decel)*start_speed*start_speed+(accel)*e_speed*e_speed)/(accel + (-decel)));
		acc_range = (max_speed*max_speed-start_speed*start_speed)/2*accel;
		dec_range = (max_speed*max_speed-e_speed*e_speed)/2*(-decel);
	}
	else{
		max_speed = m_speed;
	}
	encoder_R[0] = 0;
	encoder_L[0] = 0;
	while(acc_range > ENCODER2LEN((encoder_R[0]+encoder_L[0])/2)){
		target_speed = max_speed;
		target_accel = accel;
		target_omega = 0;
	}
	while((range - dec_range) > ENCODER2LEN((encoder_R[0]+encoder_L[0])/2)){
		target_speed = max_speed;
		target_accel = 0;
		target_omega = 0;
	}

	while(range > ENCODER2LEN((encoder_R[0]+encoder_L[0])/2)){
		target_speed = e_speed;
		target_accel = decel;
		target_omega = 0;
	}
	return 1;
/*
	float max_speed = 0;
	if(rampud_flag == false){ //初期化
		printf("init\r\n");
		start_speed = (speed_L[0] + speed_R[0])/2;
		acc_range = ENCODER2LEN((m_speed*m_speed-start_speed*start_speed)/2*accel);
		dec_range = ENCODER2LEN((m_speed*m_speed-e_speed*e_speed)/2*(-decel));
		if(range < (acc_range + dec_range)){
			//距離が足りなかった時の処理
			max_speed = start_speed + accel * (start_speed + e_speed)/(accel + decel);
			acc_range = ENCODER2LEN((max_speed*max_speed-start_speed*start_speed)/2*accel);
			dec_range = ENCODER2LEN((max_speed*max_speed-e_speed*e_speed)/2*(-decel));
		}
		else{
			max_speed = m_speed;
		}
		encoder_R[0] = 0;
		encoder_L[0] = 0;
		ramp_count = 0;
		rampud_flag = true;
	}
	printf("exit\r\n");
	if(ramp_count == 0){
		printf("accel\r\n");
		if(acc_range > ENCODER2LEN((encoder_R[0]+encoder_L[0])/2)){
			ramp_count = 1;
		}
		else{
			target_speed = max_speed;
			target_accel = accel;
			target_omega = 0;
		}
		return 0;
	}
	else if(ramp_count == 1){
		printf("run\r\n");
		if((range - dec_range) > ENCODER2LEN((encoder_R[0]+encoder_L[0])/2)){
			ramp_count = 2;
		}
		else{
			target_speed = max_speed;
			target_accel = 0;
			target_omega = 0;
		}
		return 0;
	}
	else if(ramp_count == 2){
		printf("decel\r\n");
		if(range > ENCODER2LEN((encoder_R[0]+encoder_L[0])/2)){
			ramp_count = 3;
		}
		else{
			if(e_speed > 0.2){
				target_speed = e_speed;
			}
			else{
				target_speed = 0.2;
			}
			target_accel = decel;
			target_omega = 0;
		}
		return 0;
	}
	else if(ramp_count == 3){
		printf("end\r\n");
		target_speed = e_speed;
		target_accel = 0;
		target_omega = 0;
		rampud_flag = false;
		return 1;
	}*/
}

int turn(float t_omega, float turn_rad){
	return 0;
}

int motion(int head){
	if(motion_queue[head].t_omega == 0){
		rampud_control(motion_queue[head].first_range, motion_queue[head].accel,
			motion_queue[head].decel, motion_queue[head].max_speed, motion_queue[head].e_speed);
	}
	else{
		rampud_control(motion_queue[head].first_range, 0, 0, (speed_L[0]+speed_R[0])/2, (speed_L[0]+speed_R[0])/2);
		turn(motion_queue[head].t_omega, motion_queue[head].turn_rad);
		rampud_control(motion_queue[head].end_range, 0, 0, (speed_L[0]+speed_R[0])/2, (speed_L[0]+speed_R[0])/2);
	}
	return 1;
}

int motion_full(){
	if(head == (tail + 1) % QUEUE_MAX){
		return 1;
	}
	else{
		return 0;
	}
}
int motion_enpty(){
	if(head == tail){
		return 1;
	}
	else{
		return 0;
	}
}

int motion_enqueue(
		float first_range,
		float end_range,
		float accel,
		float decel,
		float max_speed,
		float e_speed,
		float t_omega,
		float turn_rad){
	if(motion_full()==1){
		return 0;
	}
	/*
	 * if(1つ前のモーションが現在のモーションと繋がっていれば、)
	 * モーションを統合
	 * motion_queue[tail].first_range =+ first_range;
	 * motion_queue[tail].accel =+ accel;(<max_accel)
	 * motion_queue[tail].decel =+ decel;(<max_decel)
	 * motion_queue[tail].max_speed =+ max_speed;(<max_speed)
	 * motion_queue[tail].turn_rad =+ turn_rad;
	 */
	motion_queue[tail].first_range = first_range;
	motion_queue[tail].end_range = end_range;
	motion_queue[tail].accel = accel;
	motion_queue[tail].decel = decel;
	motion_queue[tail].max_speed = max_speed;
	motion_queue[tail].e_speed = e_speed;
	motion_queue[tail].t_omega = t_omega;
	motion_queue[tail].turn_rad = turn_rad;
	if(tail + 1 == QUEUE_MAX){
		tail = 0;
	}
	else{
		tail++;
	}
	return 1;
}
int motion_dequeue(){
	if(motion_enpty()==1){
		return 1;
	}
	motion(head);
	if(head + 1 == QUEUE_MAX){
		head = 0;
	}
	else{
		head++;
	}
	return 0;
}

void motion_clear(){
	for(int i = 0; i <= tail; i++){
		motion_queue[i].first_range = 0;
		motion_queue[i].end_range = 0;
		motion_queue[i].accel = 0;
		motion_queue[i].decel = 0;
		motion_queue[i].max_speed = 0;
		motion_queue[i].e_speed = 0;
		motion_queue[i].t_omega = 0;
		motion_queue[i].turn_rad = 0;
	}
}
void fail_safe(void){
	STBY_GPIO_RESET();
	encoder_L[0] = 0;
	encoder_L[1] = 0;
	encoder_L[2] = 0;
	encoder_R[0] = 0;
	encoder_R[1] = 0;
	encoder_R[2] = 0;
}
