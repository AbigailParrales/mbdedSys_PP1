/**Copyright 2020 ITESM*/
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/adc.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/timer.h>

#include "unsere_gpio.h"
#include "unsere_adc.h"
#include "unsere_timer.h"
#include "unsere_uart.h"

char tmp[] = "Temp: xxxC\r\n";
char rxmsg[4] = "x";
int16_t max = 200, min = 100;
int i = 0;
int j = 0;
int k = 0;
int16_t aux_min_max = 0;

int main(void) {
    rcc_clock_setup_in_hse_8mhz_out_72mhz();        //  Use this for "blue pill"
    gpio_setup();
    ADC_setup();
    uart_setup();
    timer_setup();


    for (;;) {}

    return 0;
}

void usart1_isr(void) {
    USART1_SR = ~(1<<5);  //  borrar bandera
    char flag_set_min_max = 0;

    if (usart_recv(USART1) == 'n') {
        timer_disable_counter(TIM2);
        adc_power_off(ADC1);
        uart_send("Set Min: ");
        flag_set_min_max = 1;
    }
    if (usart_recv(USART1) == 'x') {
        timer_disable_counter(TIM2);
        adc_power_off(ADC1);
        uart_send("Set Max: ");
        flag_set_min_max = 2;
    }
    if (i < 4) {
        rxmsg[i] = usart_recv(USART1);
        i++;
        if (rxmsg[j] == '\r') {  //  User Input Enter
                while (j < (i-1)) {
                    if ((rxmsg[j] >= 48 && rxmsg[j] <= 57)) {
                        //  Valid input (numbers)
                        aux_min_max = aux_min_max*10 + (rxmsg[i]-48);
                        j++;
                    } else {
                        uart_send("\r\nInvalid, input only integers\r\n");
                        break;
                }
            }

            if (flag_set_min_max == 2) {
                    max = aux_min_max;
            } else if (flag_set_min_max == 1) {
                    min = aux_min_max;
            }

            i = 0;
            adc_power_on(ADC1);
            timer_enable_counter(TIM2);
        }
    } else {
        uart_send("\r\nError\r\n");
        i = 0;
        adc_power_on(ADC1);
        timer_enable_counter(TIM2);
    }
}

void tim2_isr(void) {
    timer_clear_flag(TIM2, TIM_SR_UIF);  //  apagar bandera

    uint16_t adc_read;
    adc_read = read_adc(1) * 330 / 4095;

    if (adc_read < min) {
        //  prender A3
        gpio_clear(GPIOA, GPIO5);
        gpio_set(GPIOA, GPIO6);
        gpio_set(GPIOA, GPIO7);

    } else if (adc_read > max) {
        //  prender A4
        gpio_clear(GPIOA, GPIO7);
        gpio_set(GPIOA, GPIO5);
        gpio_set(GPIOA, GPIO6);

    } else {
        //  prender A5
        gpio_clear(GPIOA, GPIO6);
        gpio_set(GPIOA, GPIO5);
        gpio_set(GPIOA, GPIO7);
    }

    if (k == 4) {  //  Send Temp every 0.5 seg
        k = 0;
        char t = 2;
        char index = 8;
        do {
            tmp[index--] = (adc_read%10) + 48;
            adc_read/=10;
        }while(t--);

        //  send Temp
        uart_send(tmp);
    } else {
        k++;
    }
}
