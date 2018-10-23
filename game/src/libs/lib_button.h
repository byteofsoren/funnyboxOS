/*
 * lib_button.h
 *
 * Created: 2018-09-27 09:32:21
 *  Author: Magnus Sörensen
 */ 


#ifndef LIB_BUTTON_H_
#define LIB_BUTTON_H_

#include <avr32/io.h>
#include "compiler.h"
#include "board.h"
#include "intc.h"
#include "gpio.h"
#include "lib_common.h"
#include <stdlib.h>


#ifndef MAX_PIN_NUMBER
#define MAX_PIN_NUMBER 32
#endif




/******************************
* Button defines
******************************/
#define BUTTON0_PORT (GPIO_PUSH_BUTTON_0 / MAX_PIN_NUMBER)
#define BUTTON0_PIN (1 << (GPIO_PUSH_BUTTON_0 & (MAX_PIN_NUMBER - 1)))
#define BUTTON1_PORT (GPIO_PUSH_BUTTON_1 / MAX_PIN_NUMBER)
#define BUTTON1_PIN (1 << (GPIO_PUSH_BUTTON_1 & (MAX_PIN_NUMBER - 1)))
#define BUTTON2_PORT (GPIO_PUSH_BUTTON_2 / MAX_PIN_NUMBER)
#define BUTTON2_PIN (1 << (GPIO_PUSH_BUTTON_2 & (MAX_PIN_NUMBER - 1)))
/******************************
* Joystick defines
******************************/
#define BUTTON_LEFT_PORT (GPIO_JOYSTICK_LEFT / MAX_PIN_NUMBER)
#define BUTTON_LEFT_PIN (1 << (GPIO_JOYSTICK_LEFT & (MAX_PIN_NUMBER - 1)))
#define BUTTON_RIGHT_PORT (GPIO_JOYSTICK_RIGHT / MAX_PIN_NUMBER)
#define BUTTON_RIGHT_PIN (1 << (GPIO_JOYSTICK_RIGHT & (MAX_PIN_NUMBER - 1)))

#define BUTTON_MID_PORT (GPIO_JOYSTICK_PUSH / MAX_PIN_NUMBER)
#define BUTTON_MID_PIN (1 << (GPIO_JOYSTICK_PUSH & (MAX_PIN_NUMBER - 1)))

#define BUTTON_UP_PORT (GPIO_JOYSTICK_UP / MAX_PIN_NUMBER)
#define BUTTON_UP_PIN (1 << (GPIO_JOYSTICK_UP & (MAX_PIN_NUMBER - 1)))
#define BUTTON_DOWN_PORT (GPIO_JOYSTICK_DOWN / MAX_PIN_NUMBER)
#define BUTTON_DOWN_PIN (1 << (GPIO_JOYSTICK_DOWN & (MAX_PIN_NUMBER - 1)))


/******************************
* lib_buttons_t
* 
* This struct contains function
* pointers that is accessed
* when a press of button ocures. 
******************************/
typedef struct lib_buttons_t {
	//void (*USART_onInterrupt)(char);	
	onStatePtr	onButton0;
	onStatePtr	onButton1;
	onStatePtr	onButton2;
	onStatePtr	onLeft;
	onStatePtr	onRight;
	onStatePtr	onMid;
	onStatePtr	onUp;
	onStatePtr	onDown;
	onStatePtr  onAnykey;
}lib_buttons_t;

/******************************
* Prototypes
******************************/

on_time_t lib_buttonsInit(volatile lib_buttons_t *regButtons);



#endif /* LIB_BUTTON_H_ */