#include <libopencm3/stm32/rcc.h>

#include "unsere_rcc.h"

void setting_clock(void) {
    rcc_clock_setup_in_hse_8mhz_out_72mhz(); ///< Use this for "blue pill"
}

void rcc_setups_for_adc(void) {
    rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_ADC1EN);
    rcc_peripheral_reset(&RCC_APB2RSTR, RCC_APB2RSTR_ADC1RST);
    rcc_peripheral_clear_reset(&RCC_APB2RSTR, RCC_APB2RSTR_ADC1RST);
    rcc_set_adcpre(RCC_CFGR_ADCPRE_PCLK2_DIV6);    ///< Set. 12MHz, Max. 14MHz
}

void rcc_setups_for_gpio(void) {
    ///<  Enable GPIOA for ADC
    rcc_periph_clock_enable(RCC_GPIOA);
}

void rcc_setups_for_timer(void) {
    ///< To configure a timer it is enabled the number and AFIO´s clock
    rcc_periph_clock_enable(RCC_TIM2);
    rcc_periph_clock_enable(RCC_AFIO);
}

void rcc_setups_for_uart(void) {
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
