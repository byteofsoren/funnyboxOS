/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include <asf.h>
#include "compiler.h"
#include "board.h"
#include "tc.h"
#include "intc.h"
#include "gpio.h"

#include "libs/lib_led.h"
#include "libs/lib_button.h"
#include "libs/lib_time.h"

// include the games.
#include "snake/snake.h"

/*********************************
* Function prototypes
*********************************/
void mdelay(int milliseconds);
void test_button_interrupt(usec_t time);
void test_led0_counter(usec_t time);
void test_button0(int button);


/*********************************
* Globals 
*********************************/
int led0_pid = 0;

//Sleep function from slides
#define CYCLES_PER_MS (115/11)
void mdelay(int milliseconds) {
	long volatile cycles = (milliseconds * CYCLES_PER_MS); //Calculate the numbers of loops needed to pass the right amount of time
	//Loop until the number of milliseconds has passed
	while(cycles != 0) {
		cycles--;
	}
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	lib_ledInit();
	/*
	lib_buttons_t my_buttons = {
		.onButton0 = test_button0,
	};
	*/
	lib_timer_settings my_settings = {
		.clk_freq = 10,
		.idle_divider = tc_2,
		.idle_freq = khz_115,
		.shed_freq = mhz_12,
	};
	lib_timeInit(&my_settings);	
	/*lib_shedReg(test_button_interrupt, 0);
	led0_pid = lib_shedReg(test_led0_counter, 0);
	on_time_t test = lib_buttonsInit(&my_buttons);
	lib_shedReg(test, 0);
	*/
	//snake_init();
	
	
	lib_timeOn();
	while (1);
	/* Insert application code here, after the board has been initialized. */
}

void test_button0(int button)
{
	lib_ledTogle(3);	
	lib_shedRem(led0_pid);
}

void test_button_interrupt(usec_t time)
{
	lib_ledTogle(1);	
}
void test_led0_counter(usec_t time)
{
	static usec_t old_time = 0;
	if ((time - old_time) > 30)
	{
		lib_ledTogle(0);
		old_time = time;
	}
}
