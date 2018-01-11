/*
 * maze_solve.h
 *
 *  Created on: 2017/08/13
 *      Author: tattaka
 */

#ifndef USERINC_MAZE_SOLVE_H_
#define USERINC_MAZE_SOLVE_H_

#include "stm32f4xx_hal.h"
#include "stdbool.h"
#include "robot.h"
#include "parameter.h"

void map_init(int x, int y, int mode, int goal_num, ...);
int search(int x, int y, int goal_num, ...);
int solve(int x, int y, int goal_num, ...);

#endif /* USERINC_MAZE_SOLVE_H_ */
