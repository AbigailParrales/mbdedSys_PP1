#include <libopencm3/stm32/gpio.h>

#include "led.h"

void LED_ON_PTA5(void) {
    gpio_clear(GPIOA, GPIO5);
}

void LED_OFF_PTA5(void) {
    gpio_set(GPIOA, GPIO5);
}

void LED_ON_PTA7(void) {
    gpio_clear(GPIOA, GPIO7);
}

void LED_OFF_PTA7(void) {
    gpio_set(GPIOA, GPIO7);
}