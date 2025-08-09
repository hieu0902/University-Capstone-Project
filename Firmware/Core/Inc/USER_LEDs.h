#ifndef __USER_LED_H
#define __USER_LED_H


#include "stdint.h"
#include "gpio.h"

#define LED_GREEN_PIN      GPIO_PIN_15
#define LED_BLUE_PIN       GPIO_PIN_6
#define LED_GPIO_PORT_B    GPIOB

void toggle_led(GPIO_TypeDef* port, uint16_t pin);

void turn_off_led(GPIO_TypeDef* port, uint16_t pin);
#endif
