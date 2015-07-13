/*
 * LEDs.h
 *
 *  Created on: Apr 21, 2015
 *      Author: kieutq
 */

#ifndef LEDS_H_
#define LEDS_H_

#include "driverlib.h"

#define LED_BASE 	GPIO_PORT_P1
#define LED1 		GPIO_PIN7
//#define LED2 		GPIO_PIN1
//#define LED3 		GPIO_PIN2
//#define LED4 		GPIO_PIN3
#define LEDs	(LED1) // + LED2 + LED3 + LED4)
#define IS_LED(x)	(x == LED1) // || x == LED2 || x== LED3 || x == LED4 || x == LEDs)

typedef struct
{
	void 	(*Init)(void);
	void 	(*TurnOn)(int led);
	void 	(*TurnOff)(int led);
	void 	(*Toggle)(int led);
} LED_API_t;

extern const LED_API_t LED_API;

#endif /* LEDS_H_ */
