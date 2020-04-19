//#include <libopencm3/stm32/rcc.h>

#include "../1ra_capa/unsere_gpio.h"
#include "../1ra_capa/unsere_adc.h"
#include "../1ra_capa/unsere_timer.h"
#include "../1ra_capa/unsere_uart.h"
#include "../1ra_capa/unsere_rcc.h"
#include "temp_sense.h"

void temp_sense_init(void) {
    rcc_setup();
    gpio_setup();
    ADC_setup();
    uart_setup();
    timer_setup();
}
