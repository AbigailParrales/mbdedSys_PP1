
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
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/usart.h>

/********************************************************************/

#include "unsere_uart.h"

/********************************************************************/

/********************************************************************
* The creation of a message, will an specificed limit, the message  *
* lenght is configurated too.                                       *
*                                                                   *
* The configuration for UART in order to write and read the serial  *
* data.                                                             *
*                                                                   *
********************************************************************/

void uart_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_ANALOG,
		GPIO0);
    rcc_periph_clock_enable(RCC_USART1);

    ///<  Enable UART TX (PA9)
    ///< In order to use the pin A9 to transmit the information.
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, 
		  GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);

    ///<  Enable UART RX (PA10)
    ///< In order to use the pin A10 to read the information.
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN,
                  GPIO_USART1_RX);

    usart_set_baudrate(USART1, 9600); ///<  Baudrate configurated at 9600
    usart_set_databits(USART1, 8); ///<  Data will be 8 bit long 
    usart_set_stopbits(USART1, USART_STOPBITS_1);
    usart_set_mode(USART1, USART_MODE_TX_RX);
    usart_set_parity(USART1, USART_PARITY_NONE);
    usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

    nvic_enable_irq(NVIC_USART1_IRQ); ///<  Global enable of interruption
    usart_enable_rx_interrupt(USART1); ///<  Local enable of interruption

    usart_enable(USART1);
}

void uart_send(char * txt) {
    while (*txt != '\0') {
        usart_send_blocking(USART1, *txt++);
    }
}
