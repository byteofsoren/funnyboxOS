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
#define PONG_START 1	// The ball is on a plate A
#define PONG_STARTA 1	// The ball is on a plate A
#define PONG_STARTB 2	// The ball is on a plate B
#define PONG_RUN 3		// The ball is flying
#define PONG_GOAL 4		// The ball is in the goal
#define PONG_MENU 5		// Ask user to if it want to replay the game.
#define PONG_YES 6
#define PONG_NO 7


#define PONG_PLAYER_B_OFFSET 11
#define PONG_PLAYER_A_OFFSET 4


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



void poing_init(void);



#endif /* PONG_H_ */