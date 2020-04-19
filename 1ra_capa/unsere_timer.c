
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
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>

//#include <libopencm3/stm32/adc.h>
//#include <libopencm3/stm32/gpio.h>
/********************************************************************/

#include "unsere_timer.h"
#include "unsere_uart.h"
#include "unsere_adc.h"
#include "../2da_capa/led.h"

/********************************************************************/

int k = 0;
char maxTmp[]="150"; ///< The maximum limit of the system
char minTmp[]="050"; ///< The minimum limit of the system
char tmp[]=("Temp: ---C| Max: ---C| Min: ---C| TMPmax: ---| TMPmin: --- \r\n"); ///< Message display on terminal

/*********************************************************************
* A timer is a very important for systems, it has a lot of           *
* applications, one is delay routine that robs other tasks of CPU    *
* time that have been used to be more productive.                    *
*                                                                    *
*********************************************************************/

void timer_setup(void) {
    timer_disable_counter(TIM2);
    timer_disable_irq(TIM2, TIM_DIER_UIE);

    ///< Configuration to establish an specific operating mode
    timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE,
                   TIM_CR1_DIR_UP);
    timer_set_prescaler(TIM2, 7200);

    timer_disable_preload(TIM2);
    timer_continuous_mode(TIM2);
    timer_set_period(TIM2, 1000);  ///<  Equals to 10 miliseconds

    nvic_enable_irq(NVIC_TIM2_IRQ);
    timer_enable_irq(TIM2, TIM_DIER_UIE);
    timer_enable_counter(TIM2);
}

/********************************************************************/

/** The interruption of the used timer. Inside we found the 
* convertion from the reading of the adc and it is compared 
* the minimum with that value and also the maximum with the 
* read value. Depending the conditions will turn on or turn 
* off the leds.
*/

void tim2_isr(void) {
    timer_clear_flag(TIM2, TIM_SR_UIF);  ///< Turn off flag

    uint16_t adc_read;
    adc_read = (read_adc(1) * 330 / 4095);

    /** Comparing the value received from the adc with the maximum
    * to turn on the LED on pin A5.
    */
    if (adc_read > max) {
        LED_ON_PTA5();
        LED_OFF_PTA7();
        tmp[42]='O',tmp[43]='N',tmp[44]=' ';
        tmp[55]='O',tmp[56]='F',tmp[57]='F';

    /** Comparing the value received from the adc with the minimum
    * to turn on the LED on pin A7.
    */
    } else if (adc_read < min) {
        LED_OFF_PTA5();
        LED_ON_PTA7();
        tmp[42]='O',tmp[43]='F',tmp[44]='F';
        tmp[55]='O',tmp[56]='N',tmp[57]=' ';
    }

    /** Comparing the value received from the adc with the minimum
    * and maximum to turn off both LEDs.
    */
    /*
    else {
        LED_OFF_PTA5();
        LED_OFF_PTA7();
        tmp[42]='-', tmp[43]='-', tmp[44]='-';
        tmp[55]='-', tmp[56]='-', tmp[57]='-';
    }
    */

    /** In order to evaluate the temperature every .5 seconds the 
    * following structure is implemented.
    */
    if (k == 4) {  ///< Send Temp every 0.5 seg
        k = 0;
        char t = 2;
        char index = 8;
        do {
            tmp[index--] = (adc_read%10) + 48;
            adc_read/=10;
        }while(t--);

        ///< Send variable tmp and limits to display on terminal
        uart_send(tmp);
        tmp[17] = maxTmp[0],tmp[18] = maxTmp[1],tmp[19] = maxTmp[2];
        tmp[28] = minTmp[0],tmp[29] = minTmp[1],tmp[30] = minTmp[2];
    } else {
        k++;
    }
}
