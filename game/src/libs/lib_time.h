/*
 * lib_time.h
 *
 * Created: 2018-09-28 08:45:17
 *  Author: Magnus Sörensen
 */ 


#ifndef LIB_TIME_H_
#define LIB_TIME_H_

#include <stdlib.h>
#include "board.h"
#include "tc.h"
#include "intc.h"
#include "gpio.h"
#include "lib_common.h"
#include "tc.h"
#include "pm.h"

typedef int pid_t;


#define LIB_TIMEREG_SUCCESSFULL 0
#define LIB_TIMEREG_FAIL -1
#define LIB_INIT_SUCCESSFULL 0
#define LIB_INIT_FAIL -1
#define FAILED_TO_CREATE_TASK -2
#define NO_TASK_REMOVED -1

// Timer specific settings 
#define TC_CHANNEL 0
/********************************
* Clock souse divider
********************************/
//#define CLK_DIVIDER 2
#define CLK_DIVIDER 32
//#define CLK_DIVIDER 32
//#define CLK_DIVIDER 128

#if (CLK_DIVIDER == 128)		
	#define  TC_SELECT		TC_CLOCK_SOURCE_TC4
#elif (CLK_DIVIDER == 32)		
	#define  TC_SELECT		TC_CLOCK_SOURCE_TC3
#elif (CLK_DIVIDER == 8)		
	#define  TC_SELECT		TC_CLOCK_SOURCE_TC2
#else
	#define  TC_SELECT		TC_CLOCK_SOURCE_TC1
#endif
// Select the right divider by setting tc select. 

#define		CLK 12000000
//#define	CLK 115200
//#define RC_VALUE CLK / CLK_DIVIDER
static tc_waveform_opt_t waveform_opt =
{
	.channel	=	TC_CHANNEL,
	.wavsel		=	TC_WAVEFORM_SEL_UP_MODE_RC_TRIGGER,
	.eevtedg	=	TC_SEL_NO_EDGE,
	.tcclks		=	TC_SELECT,
};


static const tc_interrupt_t TC_INTERRUPT_OPT = {
	.cpcs = 1,
};


//#define MAX_FUNCTIONS 20


typedef enum freqensy_t {
	khz_115 = 115200,
	mhz_12 = 12000000,
}freqensy;

typedef enum divider_t {
	tc_2=2,
	tc_8=8,	
	tc_32=32,
	tc_64=64,
	tc_128=128,
}divider;

typedef struct _lib_timer_settings 
{
	usec_t clk_freq;				//Clock cycles per seconds ie Hz.
	freqensy idle_freq;		// The base frequency when the processor don't do any thing.
	divider idle_divider;  // The divider on idle.
	freqensy shed_freq;		// The frequency when the possessor is in the scheduling loop.
	unsigned int shed_pll_divider;  // The divider inside the scheduling loop .
}lib_timer_settings;

int lib_timeInit(struct _lib_timer_settings *settings);
void lib_setClock(int speed);
pid_t lib_shedReg(on_time_t new_task, int mod);
pid_t lib_shedRem(pid_t pid);
void lib_timeOn(void);
void lib_timeOff(void);

#endif /* LIB_TIME_H_ */