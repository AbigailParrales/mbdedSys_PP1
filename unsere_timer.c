
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
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>

/********************************************************************/

#include "unsere_timer.h"

/********************************************************************/

/*********************************************************************
* A timer is a very important for systems, it has a lot of           *
* applications, one is delay routine that robs other tasks of CPU    *
* time that have been used to be more productive.                    *
*                                                                    *
*********************************************************************/

void timer_setup(void) {
    
    ///< To configure a timer it is enabled the number and AFIO´s clock
    rcc_periph_clock_enable(RCC_TIM2);
    rcc_periph_clock_enable(RCC_AFIO);

    timer_disable_counter(TIM2);
    timer_disable_irq(TIM2, TIM_DIER_UIE);

    ///< Configuration to establish an specific operating mode
    timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE,
                   TIM_CR1_DIR_UP);
    timer_set_prescaler(TIM2, 7200);

    timer_disable_preload(TIM2);
    timer_continuous_mode(TIM2);
    timer_set_period(TIM2, 1000);  ///<  Equals to 10 miliseconds

    nvic_enable_irq(NVIC_TIM2_IRQ);
    timer_enable_irq(TIM2, TIM_DIER_UIE);
    timer_enable_counter(TIM2);
}
