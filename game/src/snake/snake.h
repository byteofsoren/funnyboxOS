/*
 * snake.h
 *
 * Created: 2018-10-01 10:56:49
 *  Author: MDF
 */ 

#define SNAKE_ALIVE 1
#define SNAKE_DEAD 0
#define SNAKE_MENU 2

#ifndef SNAKE_H_
#define SNAKE_H_

#include <stdlib.h>
#include "board.h"
#include "tc.h"
#include "intc.h"
#include "gpio.h"



#include "../libs/lib_button.h"
#include "../libs/lib_led.h"
#include "../libs/lib_time.h"
#include "../libs/display_init.h"


void snake_init(void);


#endif /* SNAKE_H_ */