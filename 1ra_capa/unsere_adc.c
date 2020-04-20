
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

/*!< #include <libopencm3/stm32/rcc.h> */
#include <libopencm3/stm32/adc.h>

#include "unsere_adc.h"
/*!< #include "unsere_rcc.h" */

/***********************************************************************/

/************************************************************************
* To configure the ADC peripheral                                       *
* The clock from the peripheral is turned on                            *
* And the power of the peipheral must be disabled                       *
*                                                                       *
************************************************************************/

void ADC_setup(void) {
    adc_power_off(ADC1);

    adc_set_dual_mode(ADC_CR1_DUALMOD_IND);      /*!<  Independent mode*/
    adc_disable_scan_mode(ADC1);
    adc_set_right_aligned(ADC1);
    adc_set_single_conversion_mode(ADC1);
    adc_power_on(ADC1);
    adc_reset_calibration(ADC1);
    adc_calibrate_async(ADC1);
    while ( adc_is_calibrating(ADC1) ) {}
}

/***********************************************************************/

/************************************************************************
* To configure the sample time and it is sequence, specifying which     *
* channels are used.                                                    *
*                                                                       *
************************************************************************/

uint16_t read_adc(uint8_t channel) {
    adc_set_sample_time(ADC1, channel, ADC_SMPR_SMP_239DOT5CYC);
    adc_set_regular_sequence(ADC1, 1, &channel);
    adc_start_conversion_direct(ADC1);
    while ( !adc_eoc(ADC1) ) {}
    return adc_read_regular(ADC1);
}

/***********************************************************************/
