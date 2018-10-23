/*
 * lib_led.c
 *
 * Created: 2018-09-27 09:21:50
 *  Author: Magnus Sörensen
 */ 

#include "lib_led.h"
void lib_ledInit(void)
{
		int n = 0;
		LED_PORTS[0] = LED0_GPIO/ MAX_PIN_NUMBER;
		LED_PORTS[1] = LED1_GPIO/ MAX_PIN_NUMBER;
		LED_PORTS[2] = LED2_GPIO/ MAX_PIN_NUMBER;
		LED_PORTS[3] = LED3_GPIO/ MAX_PIN_NUMBER;
		LED_PORTS[4] = LED4_GPIO/ MAX_PIN_NUMBER;
		LED_PORTS[5] = LED5_GPIO/ MAX_PIN_NUMBER;
		LED_PINS[0] =MLED0;
		LED_PINS[1] =MLED1;
		LED_PINS[2] =MLED2;
		LED_PINS[3] =MLED3;
		LED_PINS[4] =MLED4;
		LED_PINS[5] =MLED5;
		for (n = 0; n < 6; n++)
		{
			volatile avr32_gpio_port_t *ledStatus = &AVR32_GPIO.port[LED_PORTS[n]];
			ledStatus->gpers = LED_PINS[n];
			ledStatus->oders = LED_PINS[n];
			ledStatus->ovrs = LED_PINS[n];
		}
}
void lib_ledOn(int ledNr)
{
	volatile avr32_gpio_port_t *ledStatus = &AVR32_GPIO.port[LED_PORTS[ledNr]];
	ledStatus->ovrc = LED_PINS[ledNr];
}
void lib_ledOff(int ledNr)
{
	volatile avr32_gpio_port_t *ledStatus = &AVR32_GPIO.port[LED_PORTS[ledNr]];
	ledStatus->ovrs= LED_PINS[ledNr];
}
void lib_ledTogle(int ledNr)
{
	volatile avr32_gpio_port_t *ledStatus = &AVR32_GPIO.port[LED_PORTS[ledNr]];
	ledStatus->ovrt = LED_PINS[ledNr];
}
