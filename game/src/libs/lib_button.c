/*
 * lib_button.c
 *
 * Created: 2018-09-27 09:32:38
 *  Author: MDF
 */ 
 #include "lib_button.h"
/*********************************
* Macros to read information.
*********************************/
#define BUTTONX_ISON(BUTTONX,PINY) ((BUTTONX->pvr & PINY) == 0)
#define IS_REGISTERD(LIB_BT) (LIB_BT != NULL)
#define SHIFTER(BUTTONX, PINY, POS) ((BUTTONX->pvr & PINY) << POS)

/*********************************
* private function prototypes.
*********************************/
volatile avr32_gpio_port_t* button_init(unsigned port, unsigned pin);
void return_buttons(usec_t time);


/***********************************
* Storing the button adders in struct
***********************************/
typedef struct _buttons_ports_t{
	volatile avr32_gpio_port_t* button0;
	volatile avr32_gpio_port_t* button1;
	volatile avr32_gpio_port_t* button2;
	volatile avr32_gpio_port_t* joy_left;
	volatile avr32_gpio_port_t* joy_right;
	volatile avr32_gpio_port_t* joy_mid;
	volatile avr32_gpio_port_t* joy_up;
	volatile avr32_gpio_port_t* joy_down;
}buttons_ports_t;

/*********************************
* Globals.
*********************************/
volatile lib_buttons_t *lib_regButtons;
struct _buttons_ports_t *buttons;


volatile avr32_gpio_port_t* button_init(unsigned port, unsigned pin)
{
	volatile avr32_gpio_port_t* buttonStatus = &AVR32_GPIO.port[port];
	buttonStatus->gpers = pin;
	buttonStatus->oderc = pin;
	return buttonStatus;
}

on_time_t lib_buttonsInit(volatile lib_buttons_t *regButtons)
{
	// Enable buttons.
	buttons = (buttons_ports_t*) calloc(1, sizeof(buttons_ports_t));
	buttons->button0 = button_init(BUTTON0_PORT, BUTTON0_PIN);
	buttons->button1 = button_init(BUTTON1_PORT, BUTTON1_PIN);
	buttons->button2 = button_init(BUTTON2_PORT, BUTTON2_PIN);
	buttons->joy_left = button_init(BUTTON_LEFT_PORT, BUTTON_LEFT_PIN);
	buttons->joy_right = button_init(BUTTON_RIGHT_PORT, BUTTON_RIGHT_PIN);
	buttons->joy_mid = button_init(BUTTON_MID_PORT, BUTTON_MID_PIN);
	buttons->joy_up = button_init(BUTTON_UP_PORT, BUTTON_UP_PIN);
	buttons->joy_down = button_init(BUTTON_DOWN_PORT, BUTTON_DOWN_PIN);
	
	lib_regButtons = regButtons;
	
	return return_buttons;		// Returns a pointer to the function bellow
}

void return_buttons(usec_t time)
{
	/*volatile avr32_gpio_port_t* buttonStatus = NULL;
	buttonStatus = &AVR32_GPIO.port[BUTTON0_PORT];
	unsigned long int *test0;
	unsigned long int *test1;
	test0 = buttonStatus->pvr & BUTTON0_PIN;
	test1 = lib_regButtons->onButton0;
	if (((buttonStatus->pvr & BUTTON0_PIN) == 0) && ((test1 !=NULL)))
	{
		lib_regButtons->onButton0(buttonStatus->pvr);
	}

	buttonStatus = &AVR32_GPIO.port[BUTTON1_PORT];
	test0 = buttonStatus->pvr & BUTTON1_PIN;
	test1 = lib_regButtons->onButton1;
	if (((buttonStatus->pvr & BUTTON1_PIN) == 0) && ((test1 !=NULL)))
	{
		lib_regButtons->onButton1(buttonStatus->pvr);
	}

	buttonStatus = &AVR32_GPIO.port[BUTTON2_PORT];
	test0 = buttonStatus->pvr & BUTTON2_PIN;
	test1 = lib_regButtons->onButton2;
	if (((buttonStatus->pvr & BUTTON2_PIN) == 0) && ((test1 !=NULL)))
	{
		lib_regButtons->onButton2(buttonStatus->pvr);
	}
	*/
	int button_total_status = 0;
	//buttons->button0->pvr;
	button_total_status  =	SHIFTER(buttons->button0,	BUTTON0_PIN,		0) | 
							SHIFTER(buttons->button1,	BUTTON1_PIN,		1) | 
							SHIFTER(buttons->button2,	BUTTON2_PIN,		2) |
							SHIFTER(buttons->joy_left,	BUTTON_LEFT_PIN,	3) |
							SHIFTER(buttons->joy_right, BUTTON_RIGHT_PIN,	4) |
							SHIFTER(buttons->joy_mid,	BUTTON_MID_PIN,		5) |
							SHIFTER(buttons->joy_up,	BUTTON_UP_PIN,		6) |
							SHIFTER(buttons->joy_down,	BUTTON_DOWN_PIN,	7);

	/******************************
	* on buttons 0 to 2
	******************************/
	/*
	int on0 = 0;
	int on1 = 0;
	int reg0 = 0;
	int reg1 = 0;
	on0 = BUTTONX_ISON(buttons->button0, BUTTON0_PIN) ? 1 : 0;
	reg0 = IS_REGISTERD(lib_regButtons->onButton0) ? 1 : 0;
	on1 = buttons->button0->pvr & BUTTON0_PIN;
	reg1 = (lib_regButtons->onButton0 != NULL) ? 1 : 0;
	*/
	
							
	if ( (BUTTONX_ISON(buttons->button0, BUTTON0_PIN)) && (IS_REGISTERD(lib_regButtons->onButton0)) )
	{
		lib_regButtons->onButton0(button_total_status);
	}
	
	if ( (BUTTONX_ISON(buttons->button1, BUTTON1_PIN)) && (IS_REGISTERD(lib_regButtons->onButton1)) )
	{
		lib_regButtons->onButton1(button_total_status);
	}
	if ( (BUTTONX_ISON(buttons->button2, BUTTON2_PIN)) && (IS_REGISTERD(lib_regButtons->onButton2)) )
	{
		lib_regButtons->onButton2(button_total_status);
	}

	/******************************
	* on joystick
	******************************/
	if ( (BUTTONX_ISON(buttons->joy_left, BUTTON_LEFT_PIN)) && (IS_REGISTERD(lib_regButtons->onLeft)) )
	{
		lib_regButtons->onLeft(button_total_status);
	}
	if ( (BUTTONX_ISON(buttons->joy_right, BUTTON_RIGHT_PIN)) && (IS_REGISTERD(lib_regButtons->onRight)) )
	{
		lib_regButtons->onRight(button_total_status);
	}
	if ( (BUTTONX_ISON(buttons->joy_mid, BUTTON_MID_PIN)) && (IS_REGISTERD(lib_regButtons->onMid)) )
	{
		lib_regButtons->onLeft(button_total_status);
	}
	if ( (BUTTONX_ISON(buttons->joy_up, BUTTON_UP_PIN)) && (IS_REGISTERD(lib_regButtons->onUp)) )
	{
		lib_regButtons->onUp(button_total_status);
	}
	if ( (BUTTONX_ISON(buttons->joy_down, BUTTON_DOWN_PIN)) && (IS_REGISTERD(lib_regButtons->onDown)) )
	{
		lib_regButtons->onDown(button_total_status);
	}
	/******************************
	* on any key;
	******************************/
	if ((button_total_status != 0) && (IS_REGISTERD(lib_regButtons->onAnykey)))
	{
		lib_regButtons->onAnykey(button_total_status);
	}
}

