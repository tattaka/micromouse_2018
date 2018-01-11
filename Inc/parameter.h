/*
 * parameter.h
 *
 *  Created on: 2017/03/07
 *      Author: tattaka
 */

#ifndef USERINC_PARAMETER_H_
#define USERINC_PARAMETER_H_

#define OFF	(0)
#define ON	(1)

//mouse parameter
#define M_PI  (3.14159265358979323846)
#define TIRE_DIAMETER	(0.025) //[m]タイヤの直径
#define TREAD_WIDTH	(0.0675) //[m]
#define ONEROTATE_TIRE (8192) //タイヤ一回転分のencoder
#define UN	(3)	//[V]
#define R	(1.07)	//[ohm]
#define KE	(0.000207)	//[V/rpm]
#define KM	(0.00198)	//[Nm/A]
#define WEIGHT (0.100)	//[kg]
#define REDUCATION	(4)	//gear
#define DELTA_T	(0.001) //[s]制御周期


#define GOAL_X	(7)
#define GOAL_Y	(7)
#define MAP_WEIGHT (0)

#define HALF_SECTION	(0.09)
#define SECTION			(0.18)

#define MAZESIZE_X		(16)
#define MAZESIZE_Y		(16)

#define SPEED_R_KP	(200)
#define SPEED_R_KI	(20)
#define SPEED_R_KD	(16)

#define SPEED_L_KP	(200)
#define SPEED_L_KI	(20)
#define SPEED_L_KD	(16)

#define GYRO_KP		(0.1)
#define GYRO_KI		(0.0)
#define GYRO_KD		(0.01)

#define DUTY_MAX	(500)

#define SENSOR1_THRESHOLD (0)
#define SENSOR2_THRESHOLD (0)
#define SENSOR3_THRESHOLD (0)
#define SENSOR4_THRESHOLD (0)
#define SENSOR5_THRESHOLD (0)
#define SENSOR6_THRESHOLD (0)

#define SENSOR3_MEDIAN (0)
#define SENSOR4_MEDIAN (0)

/*others*/
#define MUSIC_OFFSET (0)


#endif /* USERINC_PARAMETER_H_ */
