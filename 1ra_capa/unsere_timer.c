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
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>
#include "unsere_timer.h"
#include "unsere_uart.h"
#include "unsere_adc.h"
#include "../2da_capa/led.h"
#include "../2da_capa/unsere_log.h"

/*********************************************************************
* A timer is a very important for systems, it has a lot of           *
* applications, one is delay routine that robs other tasks of CPU    *
* time that have been used to be more productive.                    *
*                                                                    *
*********************************************************************/

void timer_setup(void) {
    timer_disable_counter(TIM2);
    timer_disable_irq(TIM2, TIM_DIER_UIE);

    /*!< Configuration to establish an specific operating mode */
    timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE,
                   TIM_CR1_DIR_UP);
    timer_set_prescaler(TIM2, 7200);

    timer_disable_preload(TIM2);
    timer_continuous_mode(TIM2);
    /*!< Equals to 50 miliseconds */
    timer_set_period(TIM2, 5000);  

    nvic_enable_irq(NVIC_TIM2_IRQ);
    timer_enable_irq(TIM2, TIM_DIER_UIE);
    timer_enable_counter(TIM2);
}

/********************************************************************/

/*********************************************************************
* The interruption of the used timer. Inside we found the            * 
* convertion from the reading of the adc and it is compared          * 
* the minimum with that value and also the maximum with the          *
* read value. Depending the conditions will turn on or turn          *
* off the leds.                                                      *
*                                                                    *
*********************************************************************/

void tim2_isr(void) {
    timer_clear_flag(TIM2, TIM_SR_UIF);  /*!< Turn off flag */

    uint16_t adc_read;
    char* umbral_bajo;
    char* umbral_alto;

    
    adc_read = (read_adc(1) * 330 / 4095); 

    /** Comparing the value received from the adc with the maximum
    * to turn on the LED on pin A5.
    */
    if (adc_read > obtain_max()) {  /*!< upper region */
        LED_ON_PTA5();
        LED_OFF_PTA7();

        umbral_alto = "ON ";
        umbral_bajo = "OFF";

    /** Comparing the value received from the adc with the minimum
    * to turn on the LED on pin A7.
    */
    }
    if (adc_read < obtain_min()) { /*!< lower region */
        LED_OFF_PTA5();
        LED_ON_PTA7();

        umbral_alto = "OFF";
        umbral_bajo = "ON ";
    }
    if ((adc_read > obtain_min()) && (adc_read < obtain_max())) {
        LED_OFF_PTA5();
        LED_OFF_PTA7();

        umbral_alto = "OFF";
        umbral_bajo = "OFF";
    }

    /*!< Login temperature */
    log_temperature(adc_read, obtain_max(), obtain_min(), umbral_bajo, umbral_alto);

}

/********************************************************************/
