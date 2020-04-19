
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

//#include <libopencm3/stm32/rcc.h>
//#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/usart.h>

#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/adc.h>
/********************************************************************/

#include "unsere_uart.h"

/********************************************************************/

int16_t max = 150, min = 50;

char rxmsg[4] = "x"; ///< Key pressed to configure superior limit
int i = 0, j = 0;
int16_t aux_min_max = 0; ///< Used for receive the letter entered from the keyboard

/********************************************************************
* The creation of a message, will an specificed limit, the message  *
* lenght is configurated too.                                       *
*                                                                   *
* The configuration for UART in order to write and read the serial  *
* data.                                                             *
*                                                                   *
********************************************************************/

void uart_setup(void) {
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

/********************************************************************/

/** Interruption from uart, the x and n keys are configurate 
* in order to set the maximum temperature and the minimum.
* Also with the restriction of entering letters instead of numbers 
* and displaying a message to inform the user of that warning.
*/

void usart1_isr(void) {
    USART1_SR = ~(1<<5);  ///< Erase flag
    char flag_set_min_max = 0;
    char c_received = usart_recv(USART1);
    usart_send_blocking(USART1, c_received);

    /** When the 'n' is pressed, the configuration to set the miminum
    * temperature is displayed.
    */
    if (c_received == 'n') {
        timer_disable_counter(TIM2);
        adc_power_off(ADC1);
        uart_send("Set Min: ");
        flag_set_min_max = 1;
    }

    /** When the 'x' is pressed, the configuration to set the maximum
    * temperature is displayed.
    */

    if (c_received == 'x') {
        timer_disable_counter(TIM2);
        adc_power_off(ADC1);
        uart_send("Set Max: ");
        flag_set_min_max = 2;
    }

    /** The value written by the user is verified if there are only 
    * numbers, if there is a letter in that space, the terminal will 
    * display a warning message. If there are only numbers the characters
    * are sent.
    */
    if (i < 4) {
        rxmsg[i] = usart_recv(USART1);
        i++;
        if (rxmsg[j] == '\r') {  ///< User Input Enter
                while (j < (i-1)) {
                    if ((rxmsg[j] >= 48 && rxmsg[j] <= 57)) {
                        //  Valid input (numbers)
                        aux_min_max = aux_min_max*10 + (rxmsg[i]-48);
                        j++;
                    } else {
                        uart_send("\r\nInvalid, input only integers\r\n"); ///< This message will be displayed
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
        uart_send("\r\nError\r\n"); ///< This message will be displayed
        i = 0;
        adc_power_on(ADC1);
        timer_enable_counter(TIM2);
    }
}