#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/usart.h>

#include "unsere_uart.h"

void uart_setup(void) {
    rcc_periph_clock_enable(RCC_USART1);

    //  Enable UART TX (PA9)
    gpio_set_mode(GPIOA,
                  GPIO_MODE_OUTPUT_50_MHZ,
                  GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
                  GPIO_USART1_TX);

    //  Enable UART RX (PA10)
    gpio_set_mode(GPIOA,
                  GPIO_MODE_INPUT,
                  GPIO_CNF_INPUT_PULL_UPDOWN,
                  GPIO_USART1_RX);

    usart_set_baudrate(USART1, 9600);  //  Baudrate 9600
    usart_set_databits(USART1, 8);
    usart_set_stopbits(USART1, USART_STOPBITS_1);
    usart_set_mode(USART1, USART_MODE_TX_RX);
    usart_set_parity(USART1, USART_PARITY_NONE);
    usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

    nvic_enable_irq(NVIC_USART1_IRQ);   //  Habilitador global de interrupcion
    usart_enable_rx_interrupt(USART1);   //  Habilitador local de interrupcion

    usart_enable(USART1);
}

void uart_send(char * txt) {
    while (*txt != '\0') {
        usart_send_blocking(USART1, *txt++);
    }
}
