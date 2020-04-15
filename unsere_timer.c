#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>

#include "unsere_timer.h"

void timer_setup(void) {
    rcc_periph_clock_enable(RCC_TIM2);
    rcc_periph_clock_enable(RCC_AFIO);

    timer_disable_counter(TIM2);
    timer_disable_irq(TIM2, TIM_DIER_UIE);

    timer_set_mode(TIM2,
                   TIM_CR1_CKD_CK_INT,
                   TIM_CR1_CMS_EDGE,
                   TIM_CR1_DIR_UP);
    timer_set_prescaler(TIM2, 7200);

    timer_disable_preload(TIM2);
    timer_continuous_mode(TIM2);
    timer_set_period(TIM2, 1000);  //  Equals 0.01 seg

    nvic_enable_irq(NVIC_TIM2_IRQ);
    timer_enable_irq(TIM2, TIM_DIER_UIE);
    timer_enable_counter(TIM2);
}