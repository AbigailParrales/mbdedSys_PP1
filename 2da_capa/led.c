
/************************************************************************
* Copyright 2020 ITESM                                                  *
*                                                                       *
*                                                                       *
* ADC_UART                                                              *
*                                                                       *
* Autorhs:                                                              *
* Jesús Enrique Luna Medina          A01632334                          *
* Daniela abigail Parrales Mejía     A01228629                          *
* Luis Cortés Leal                   A01631163                          *
*                                                                       *
* Abril 2020                                                            *
* The project titled ADC_UART is carried out with the purpose of        *
* implementing a temperature sensing system which acquires the          *
* values from the environment and delivers a response through           *
* hardware using LEDs and software displaying on a Terminal.            *
*                                                                       *
************************************************************************/

/*!< LIBRARIES */
#include <libopencm3/stm32/gpio.h>

#include "led.h"

/************************************************************************
* In order to get more structurated the way of turning on or off the    *
* LEDs, a succesfull way is to have organized with related names so     *
* the user find it more easy to understand                              *
*                                                                       *
************************************************************************/

void LED_ON_PTA5(void) {
    gpio_clear(GPIOA, GPIO5);
}

void LED_OFF_PTA5(void) {
    gpio_set(GPIOA, GPIO5);
}

void LED_ON_PTA7(void) {
    gpio_clear(GPIOA, GPIO7);
}

void LED_OFF_PTA7(void) {
    gpio_set(GPIOA, GPIO7);
}

/************************************************************************/
