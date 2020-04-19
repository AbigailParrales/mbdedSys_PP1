
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

#ifndef UNSERE_RCC_H  
#define UNSERE_RCC_H 

/********************************************************************/

void setting_clock(void);
void rcc_setups_for_adc(void);
void rcc_setups_for_gpio(void);
void rcc_setups_for_timer(void);
void rcc_setups_for_uart(void);
void rcc_setup(void);

/********************************************************************/

#endif
