/**********************************************************************************
* Copyright 2020 ITESM                                                            *
*                                                                                 *                                                                                     *
* ADC_UART                                                                        *
*                                                                                 *
* By                                                                              *
* Jesús Enrique Luna Medina A01632334                                             *
* Daniela abigail Parrales Mejía A01228629                                        *
* Luis Cortés Leal A01631163                                                      *
*                                                                                 *
* Abril 2020                                                                      *
* The project titled ADC_UART is carried out with the purpose of                  *
* implementing a temperature sensing system which acquires the                    *
* values from the environment and delivers a response through                     *
* hardware using LEDs and software displaying on a Terminal.                      *
*                                                                                 *
**********************************************************************************/

///FUNCTIONS
/**
* BRIEF description ,parameters and return values
*/

///STRUCTS
/**
* structs, enums, constants, global variables
*/

///HIGH LEVEL 

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

/*********************************************************************************/

char tmp[]=("Temp: XXXC| Max: 200C| Min: 100C| TMPmax: Off| TMPmin: Off \r\n");
char maxTmp[]="200";
char minTmp[]="100";
char rxmsg[4] = "x";

int16_t max = 200, min = 100;
int16_t aux_min_max = 0;

int i = 0, j = 0, k = 0;

/*********************************************************************************/

int main(void) {
    rcc_clock_setup_in_hse_8mhz_out_72mhz(); ///  Use this for "blue pill"
    gpio_setup();
    ADC_setup();
    uart_setup();
    timer_setup();


    for (;;) {}

    return 0;
}

/*********************************************************************************/

/**********************************************************************************
* Interruption of uart, the x and n keys are configurate                          *
* in order to set the maximum temperature and the minimum.                        *
* Also with the restriction of entering letters instead of numbers                *
* and displaying a message to inform the user of that warning.                    *
**********************************************************************************/                    

void usart1_isr(void) {
    USART1_SR = ~(1<<5);  //  borrar bandera
    char flag_set_min_max = 0;

/**
* When the 'n' is pressed, the configuration to set the miminum
* temperature is displayed.
*/

    if (usart_recv(USART1) == 'n') {
        timer_disable_counter(TIM2);
        adc_power_off(ADC1);
        uart_send("Set Min: ");
        flag_set_min_max = 1;
    }

/** When the 'x' is pressed, the configuration to set the maximum
* temperature is displayed.
*/

    if (usart_recv(USART1) == 'x') {
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

**********************************************************************************/

/********************************************************************************** 
* The interruption of the used timer. Inside we found the                         *
* convertion from the reading of the adc and it is compared                       *
* the minimum with that value and also the maximum with the                       *
* read value. Depending the conditions will turn on or turn                       *
* off the leds.                                                                   * 
**********************************************************************************/

void tim2_isr(void) {
    timer_clear_flag(TIM2, TIM_SR_UIF);  ///  Turn off flag

    uint16_t adc_read;
    adc_read = (read_adc(1) * 330 / 4095);

/** Comparing the value received from the adc with the maximum
* and maximum temperature assigned to turn on the respective led,
* also  displays which is on or off.
*/
    if (adc_read > max) {
        gpio_clear(GPIOA, GPIO5);
        gpio_set(GPIOA, GPIO7);
        tmp[42]='O';
        tmp[43]='N';
        tmp[44]=' ';
        tmp[55]='O';
        tmp[56]='F';
        tmp[57]='F';

    } else if (adc_read < min) {
        gpio_clear(GPIOA, GPIO7);
        gpio_set(GPIOA, GPIO5);
        tmp[42]='O';
        tmp[43]='F';
        tmp[44]='F';
        tmp[55]='O';
        tmp[56]='N';
        tmp[57]=' ';
    }else {
        gpio_set(GPIOA, GPIO7);
        gpio_set(GPIOA, GPIO5);
        tmp[42]='-';
        tmp[43]='-';
        tmp[44]='-';
        tmp[55]='-';
        tmp[56]='-';
        tmp[57]='-';
    }

/** In order to evaluate the temperature every .5 seconds the 
* following structure is implemented.
*/
    if (k == 4) {  ///  Send Temp every 0.5 seg
        k = 0;
        char t = 2;
        char index = 8;
        do {
            tmp[index--] = (adc_read%10) + 48;
            adc_read/=10;
        }while(t--);

        /// Send Temp
        uart_send(tmp);
    } else {
        k++;
    }
}
