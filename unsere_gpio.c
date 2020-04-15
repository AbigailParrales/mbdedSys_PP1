#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "unsere_gpio.h"

void gpio_setup(void) {
    //  Enable GPIOA for ADC
    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_set_mode(GPIOA,
                  GPIO_MODE_INPUT,
                  GPIO_CNF_INPUT_ANALOG,
                  GPIO0|GPIO1);

    //  Set GPIOA5,A6,A7 for LEDs
    gpio_set_mode(GPIOA,
                  GPIO_MODE_OUTPUT_2_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL,
                  GPIO5|GPIO6|GPIO7);
    gpio_set(GPIOA, GPIO5|GPIO6|GPIO7);  //  LEDs off
}