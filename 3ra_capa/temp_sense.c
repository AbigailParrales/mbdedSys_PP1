
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
#include "../1ra_capa/unsere_gpio.h"
#include "../1ra_capa/unsere_adc.h"
#include "../1ra_capa/unsere_timer.h"
#include "../1ra_capa/unsere_uart.h"
#include "../1ra_capa/unsere_rcc.h"
#include "temp_sense.h"

/***********************************************************************/

void temp_sense_init(void) {
    
    /*!< All the set up are included in order to make the system works */
    rcc_setup();
    gpio_setup();
    ADC_setup();
    uart_setup();
    timer_setup();
}

/***********************************************************************/
