
/*********************************************************************
* Copyright 2020 ITESM                                               *
*                                                                    *
*                                                                    *
* ADC_UART                                                           *
*                                                                    *
* By                                                                 *
* Jesús Enrique Luna Medina A01632334                                *
* Daniela abigail Parrales Mejía A01228629                           *
* Luis Cortés Leal A01631163                                         *
*                                                                    *
* Abril 2020                                                         *
* The project titled ADC_UART is carried out with the purpose of     *
* implementing a temperature sensing system which acquires the       *
* values from the environment and delivers a response through        *
* hardware using LEDs and software displaying on a Terminal.         *
*                                                                    *
*********************************************************************/

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "unsere_gpio.h"

/********************************************************************/

void gpio_setup(void) {
    ///<  Enable GPIOA for ADC
    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_set_mode(GPIOA,
                  GPIO_MODE_INPUT,
                  GPIO_CNF_INPUT_ANALOG,
                  GPIO0|GPIO1);

    ///< Set GPIOA5,A7 for LEDs
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ, 
                  GPIO_CNF_OUTPUT_PUSHPULL, GPIO5|GPIO7);
    
    ///< LEDs are turned off
    gpio_set(GPIOA, GPIO5|GPIO7);
}
    
