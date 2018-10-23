/*
 * lib_led.h
 *
 * Created: 2018-09-27 09:22:05
 *  Author: Magnus Sörensen
 */ 


#ifndef LIB_LED_H_
#define LIB_LED_H_

#include "board.h"
#include "compiler.h"
#include <avr32/io.h>


#ifndef MAX_PIN_NUMBER 
#define MAX_PIN_NUMBER 32
#endif


unsigned int LED_PORTS[6];

#define MLED0 (1 << (LED0_GPIO & (MAX_PIN_NUMBER - 1)))
#define MLED1 (1 << (LED1_GPIO & (MAX_PIN_NUMBER - 1)))
#define MLED2 (1 << (LED2_GPIO & (MAX_PIN_NUMBER - 1)))
#define MLED3 (1 << (LED3_GPIO & (MAX_PIN_NUMBER - 1)))
#define MLED4 (1 << (LED5_GPIO & (MAX_PIN_NUMBER - 1)))
#define MLED5 (1 << (LED7_GPIO & (MAX_PIN_NUMBER - 1)))

//unsigned int LED_PINS[6] = {MLED0,MLED1,MLED2,MLED3,MLED4,MLED5};
unsigned int LED_PINS[6];

void lib_ledInit(void);
void lib_ledOn(int ledNr);
void lib_ledOff(int ledNr);
void lib_ledTogle(int ledNr);


#endif /* LIB_LED_H_ */