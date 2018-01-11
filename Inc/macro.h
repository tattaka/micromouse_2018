/*
 * macro.h
 *
 *  Created on: 2017/03/07
 *      Author: tattaka
 */

#ifndef USERINC_MACRO_H_
#define USERINC_MACRO_H_

#include "math.h"
#include "parameter.h"

#define TREAD_CIRCUIT	(TREAD_WIDTH*M_PI)
#define TIRE_CIRCUIT	(M_PI*TIRE_DIAMETER)
#define ENCODER2LEN(s)	(TIRE_CIRCUIT * (float)(s) / ONEROTATE_TIRE)
#define LEN2ENCODER(l)	(ONEROTATE_TIRE * (l) / TIRE_CIRCUIT)
#define DEG2RAD(d)		(d*M_PI/180.0)
#define SPEED2MOTERROTATE(v)	(60*REDUCATION*(v)/TIRE_CIRCUIT)

#endif /* USERINC_MACRO_H_ */
