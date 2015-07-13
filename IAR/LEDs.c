/*
 * LEDs.c
 *
 *  Created on: Apr 21, 2015
 *      Author: kieutq
 */

#include "LEDs.h"

static void LED_Init(void);
static void LED_TurnOn(int led);
static void LED_TurnOff(int led);
static void LED_Toggle(int led);

const LED_API_t LED_API =
{
		.Init = LED_Init,
		.TurnOn = LED_TurnOn,
		.TurnOff = LED_TurnOff,
		.Toggle = LED_Toggle
};


int led1;//, led2, led3, led4, leds;

void LED_Init(void)
{
	GPIO_setAsOutputPin(LED_BASE, LEDs);
	GPIO_setOutputHighOnPin(LED_BASE, LEDs);
	led1 = 0;
//	led2 = 0;
//	led3 = 0;
//	led4 = 0;
//	leds = 0;
}
void LED_TurnOn(int led)
{
	if (!IS_LED(led))
		return;
	GPIO_setOutputLowOnPin(LED_BASE, led);
	switch(led)
	{
	case LED1:
		led1 = 1;
		break;
//	case LED2:
//		led2 = 1;
//		break;
//	case LED3:
//		led3 = 1;
//		break;
//	case LED4:
//		led4 = 1;
//		break;
//	case LEDs:
//		leds = 1;
//		led1 = 1;
//		led2 = 1;
//		led3 = 1;
//		led4 = 1;
//		break;
	default:
		break;
	}
}
void LED_TurnOff(int led)
{
	if (!IS_LED(led))
		return;
	GPIO_setOutputHighOnPin(LED_BASE, led);
	switch(led)
	{
	case LED1:
		led1 = 0;
		break;
//	case LED2:
//		led2 = 0;
//		break;
//	case LED3:
//		led3 = 0;
//		break;
//	case LED4:
//		led4 = 0;
//		break;
//	case LEDs:
//		leds = 0;
//		led1 = 0;
//		led2 = 0;
//		led3 = 0;
//		led4 = 0;
//		break;
	default:
		break;
	}
}
void LED_Toggle(int led)
{
	if (!IS_LED(led))
		return;
	switch (led)
	{
	case LED1:
		if (led1)
		{
			GPIO_setOutputHighOnPin(LED_BASE, led);
			led1 = 0;
		}
		else
		{
			GPIO_setOutputLowOnPin(LED_BASE, led);
			led1 = 1;
		}
		break;
//	case LED2:
//		if (led2)
//		{
//			GPIO_setOutputHighOnPin(LED_BASE, led);
//			led2 = 0;
//		}
//		else
//		{
//			GPIO_setOutputLowOnPin(LED_BASE, led);
//			led2 = 1;
//		}
//		break;
//	case LED3:
//		if (led3)
//		{
//			GPIO_setOutputHighOnPin(LED_BASE, led);
//			led3 = 0;
//		}
//		else
//		{
//			GPIO_setOutputLowOnPin(LED_BASE, led);
//			led3 = 1;
//		}
//		break;
//	case LED4:
//		if (led4)
//		{
//			GPIO_setOutputHighOnPin(LED_BASE, led);
//			led4 = 0;
//		}
//		else
//		{
//			GPIO_setOutputLowOnPin(LED_BASE, led);
//			led4 = 1;
//		}
//		break;
//	case LEDs:
//		if (leds)
//		{
//			GPIO_setOutputHighOnPin(LED_BASE, led);
//			leds = 0;
//			led1 = 0;
//			led2 = 0;
//			led3 = 0;
//			led4 = 0;
//		}
//		else
//		{
//			GPIO_setOutputLowOnPin(LED_BASE, led);
//			leds = 1;
//			led1 = 1;
//			led2 = 1;
//			led3 = 1;
//			led4 = 1;
//		}
//		break;
	default:
		break;
	}

}
