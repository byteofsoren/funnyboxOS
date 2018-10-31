/*
 * lib_time.c
 *
 * Created: 2018-09-28 08:45:01
 *  Author: Magnus Sörensen
 */ 

#include "lib_time.h"


/****************************************
* Global variables
****************************************/

static unsigned int pid_counter = 0;
//static on_time_t on_time_exec[MAX_FUNCTIONS];
usec_t global_time;
unsigned int exec_counter;
unsigned int tc_value;
int exec_enable;
struct _lib_task_t* lib_task_head = NULL;
struct _lib_timer_settings* lib_settings = NULL;
volatile avr32_pm_t* clock_control;


/****************************************
* Private struct defines 
****************************************/
typedef struct _lib_task_t 
{
	struct _lib_task_t *next_taask;		// Next struct in linked list
	pid_t pid;					// unique Process Identifier 
	on_time_t task;				// The pointer to the task
	int mod;					// Mod on how often it should be read.
}lib_task_t;

/****************************************
* Private function prototypes 
****************************************/
__attribute__((__interrupt__)) static void lib_update_time(void);

/****************************************
* lib_timeInit
*
* settings is an pointer to a struct that
* that contains the settings for the sheduler.
*
* Creates timers timers that happen on
* on each msec specified.
int lib_timeInit(usec_t hz)
****************************************/
int lib_timeInit(lib_timer_settings *settings)
{
	// Some global variable init.
	global_time = 0;
	exec_enable = 0;

	// reading the struct and enshure that it isent empty.
	if (settings == NULL)
	{
		return LIB_INIT_FAIL;
	}
	if (settings->clk_freq==0)
	{
		settings->clk_freq = 10; //Hz
	}
	if (settings->idle_divider == 0)
	{
		settings->idle_divider = 2;
	}
	if (settings->idle_freq == 0)
	{
		settings->idle_freq =  115200;
	}
	if (settings->shed_pll_divider == 0)
	{
		settings->shed_pll_divider = 2;
	}
	// setting up the frequency for the interrupt.
	lib_settings = settings;
	usec_t rc_value = settings->idle_freq / settings->idle_divider;
	tc_value =  rc_value/ settings->clk_freq;

	pm_switch_to_osc0(&AVR32_PM,FOSC0,OSC0_STARTUP); // Start the Osc0 clock and swith to it.
	pm_switch_to_clock(&AVR32_PM, AVR32_PM_MCSEL_SLOW);  // Then switch main clock to the slow sclock.
	
	//int temp = RC_VALUE;
	//temp = TC_SELECT;
	//tc_value = 1440;
	if (tc_value > (1<<16) - 1)
	{
		// In this case the tc_value is to big to fit inside the 16 bin register.
		while (1);
		return LIB_INIT_FAIL;
	}
 	tc_init_waveform(&AVR32_TC,&waveform_opt);
 	tc_write_rc(&AVR32_TC, TC_CHANNEL, tc_value);
 	tc_start(&AVR32_TC, TC_CHANNEL);
	/**********************************
	* Configure the interrupts
	**********************************/
	tc_configure_interrupts(&AVR32_TC, TC_CHANNEL, &TC_INTERRUPT_OPT);
	Disable_global_interrupt();
	INTC_register_interrupt(lib_update_time, AVR32_TC_IRQ0, AVR32_INTC_INT0);
	Enable_global_interrupt();
	/**********************************
	* Configure the real-time 
	**********************************/
 
	/**********************************
	* Configure the task manager 
	**********************************/
	//lib_task_head = NULL;

	return LIB_INIT_SUCCESSFULL;
}


/****************************************
* lib_setClock
*
* channel	- Sets the clock channel
* return	- None
*
* Sets the internal clock channel for
* for the chip. 
****************************************/
void lib_setClock(int channel)
{
	volatile avr32_pm_t *pm = &AVR32_PM;
	pm_switch_to_osc0(pm, FOSC0, OSC0_STARTUP);
	// Switch clock to external 12 MHz crystal
	// 1) Configure OSC0 in crystal mode, external crystal with a FOSC0 Hz frequency.
	//pm_enable_osc0_crystal(pm, FOSC0);

	// 2) Enable the OSC0
	//pm_enable_clk0(pm, OSC0_STARTUP);

	// 3) Set the main clock source as being OSC0.
	//pm_switch_to_clock(pm, AVR32_PM_MCSEL_OSC0);	
}

/****************************************
* lib_timeReg
*
* in	- on_time function pointer.
* in	- modulus operation.
* out	- on successful add of function
*
* Registers a function pointer to be 
* executed when a time interrupt happens.
****************************************/
pid_t lib_shedReg(on_time_t new_task_pointer, int mod)
{
	/* Deprecated 
	if ((on_time_task_counter < MAX_FUNCTIONS) && (on_time != NULL))
	{
		on_time_exec[on_time_task_counter] = on_time;
		on_time_task_counter++;
		return LIB_TIMEREG_SUCCESSFULL;
	} else {
		return LIB_TIMEREG_FAIL;
	}
	*/
	// allocate the task
	lib_task_t* new_task = (lib_task_t*) calloc(1,sizeof(lib_task_t));
	if (!new_task)
	{
		return FAILED_TO_CREATE_TASK;
	}
	new_task->mod = (mod > 0 ? mod : 1);
	new_task->pid = pid_counter;
	new_task->next_taask = NULL;
	new_task->task = new_task_pointer;
	
	
	// If it isn't the first task created then search for the last added task. 
	if ( lib_task_head != NULL)
	{
		// Search for the end of list
		lib_task_t *current = lib_task_head;
		while(current->next_taask != NULL)
		{
			current = current->next_taask;
		}
		// Add the task last
		current->next_taask = new_task;
	} else {
		lib_task_head = new_task;
	}
	pid_counter++;
	return new_task->pid;
}

pid_t lib_shedRem(pid_t pid)
{
	// No element test
	if (lib_task_head == NULL)
	{
		return NO_TASK_REMOVED;
	}
	// One element test
	if ((lib_task_head->next_taask == NULL) && (lib_task_head->pid == pid))
	{
		lib_task_t *temp = lib_task_head;
		lib_task_head = NULL;	
		free(temp);
	}	
	else
	{
		// More then one element.
		lib_task_t *current = lib_task_head;
		lib_task_t *trailing = NULL;
		// Search for pid
		while ((current->pid != pid) && (current->next_taask != NULL))
		{
			trailing = current;
			current = current->next_taask;
		} 
		if (current->pid == pid)
		{
			// Found a pid and now it is removed
			trailing->next_taask = (current->next_taask != NULL ? current->next_taask : NULL);
			int rem_pid = current->pid;
			free(current);
			return rem_pid;
		}
		else
		{
			// Did not found a pid because it did not exist.
			return NO_TASK_REMOVED;
		}
	}
	return NO_TASK_REMOVED;
}


/****************************************
* lib_update_time (private)
* 
* On each interrupt the it starts each
* of the registered functions.
****************************************/
__attribute__((__interrupt__)) static void lib_update_time(void)
{
	/* Deprecated 
	global_time++;
	if ((exec_enable == 1) && (on_time_task_counter > 0))
	{
		for (exec_counter = 0; exec_counter < on_time_task_counter; exec_counter++)
		{
			on_time_exec[exec_counter](global_time);
		}
	}
	*/
	// Go to fast clock
	
	lib_task_t *current = lib_task_head;
	if ((current != NULL) && (exec_enable == 1))
	{
		//pm_wait_for_clk0_ready(clock_control);
		//pm_switch_to_osc0(clock_control, FOSC0, OSC0_STARTUP);
		//pm_switch_to_clock(clock_control, AVR32_PM_MCSEL_OSC0);  // Then switch main clock to Osc0
		pm_switch_to_clock(&AVR32_PM, AVR32_PM_MCSEL_OSC0);  // Then switch main clock to the slow sclock.
		// Run each of the tasks in the linked list. 
		while (current != NULL)
		{
			if ((global_time % (current->mod ) == 0))
			{
				current->task(global_time);
			}
				
			current = (current->next_taask != NULL ? current->next_taask : NULL);
		}
		global_time ++;
		pm_switch_to_clock(clock_control, AVR32_PM_MCSEL_SLOW  );
	}
	// Go to slow clock
	tc_read_sr(&AVR32_TC, TC_CHANNEL);
}


/****************************************
* lib_timeOn and lib_timeOff
*
* Ether enables the timer or disables
* the timer.
****************************************/
void lib_timeOn(void)
{
	exec_enable  = 1;
}
void lib_timeOff(void)
{
	exec_enable  = 0;
}
