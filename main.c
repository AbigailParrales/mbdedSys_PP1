/******************************************************************
* Copyright 2020 ITESM                                            *
*                                                                 *
*                                                                 *
* ADC_UART                                                        *
*                                                                 *
* By                                                              *
* Jesús Enrique Luna Medina A01632334                             *
* Daniela abigail Parrales Mejía A01228629                        *
* Luis Cortés Leal A01631163                                      *
*                                                                 *
* Abril 2020                                                      *
* The project titled ADC_UART is carried out with the purpose of  *
* implementing a temperature sensing system which acquires the    *
* values from the environment and delivers a response through     *
* hardware using LEDs and software displaying on a Terminal.      *
*                                                                 *
******************************************************************/

///FUNCTIONS
/**
* BRIEF description ,parameters and return values
*/

///STRUCTS
/**
* structs, enums, constants, global variables
*/

///HIGH LEVEL 

#include "3ra_capa/temp_sense.h"

/********************************************************************/

/********************************************************************/

int main(void) {
    temp_sense_init();

    for (;;) {}

    return 0;
}
