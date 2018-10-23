/*
 * pong.h
 *
 * Created: 2018-10-21 17:14:32
 *  Author: MDF
 */ 



#ifndef PONG_H_
#define PONG_H_

#define PONG_GAME_SPEED 100

#define PONG_EXIT 0
#define PONG_START 1	// The ball is on a plate
#define PONG_RUN 2		// The ball is flying
#define PONG_GOAL 3		// The ball is in the goal
#define PONG_MENU 4		// Ask user to if it want to replay the game.


#include <stdlib.h>
#include "board.h"
#include "tc.h"
#include "intc.h"
#include "gpio.h"



#include "../libs/lib_button.h"
#include "../libs/lib_led.h"
#include "../libs/lib_time.h"
#include "../libs/display_init.h"
#include "../libs/lib_math.h"

int pong_status = PONG_START;


void poing_init(void);



#endif /* PONG_H_ */