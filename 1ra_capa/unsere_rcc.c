
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

#include <libopencm3/stm32/rcc.h>

#include "unsere_rcc.h"

/***********************************************************************/

void setting_clock(void) {
    rcc_clock_setup_in_hse_8mhz_out_72mhz(); /*!< Use this for "blue pill" */
}

void rcc_setups_for_adc(void) {
    rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_ADC1EN);
    rcc_peripheral_reset(&RCC_APB2RSTR, RCC_APB2RSTR_ADC1RST);
    rcc_peripheral_clear_reset(&RCC_APB2RSTR, RCC_APB2RSTR_ADC1RST);
    rcc_set_adcpre(RCC_CFGR_ADCPRE_PCLK2_DIV6);    /*!< Set. 12MHz, Max. 14MHz */
}

void rcc_setups_for_gpio(void) {
    
    /*!<  Enable GPIOA for ADC */
    rcc_periph_clock_enable(RCC_GPIOA);
}

void rcc_setups_for_timer(void) {
    
    /*!< To configure a timer it is enabled the number and AFIO´s clock */
    rcc_periph_clock_enable(RCC_TIM2);
    rcc_periph_clock_enable(RCC_AFIO);
}

void rcc_setups_for_uart(void) {
    
    /*!< To configure a timer it is enabled for uaart to be used */
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_USART1);
}

void rcc_setup(void) {
    setting_clock();
    rcc_setups_for_adc();
    rcc_setups_for_gpio();
    rcc_setups_for_timer();
    rcc_setups_for_uart();
}
