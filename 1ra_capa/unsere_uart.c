
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
#include "../2da_capa/unsere_log.h"

/********************************************************************/

int16_t number_received = 0;

int max = 150;
int min = 50;

int i = 0;  //Store the amount of meaningful symbols that have been ingresed
int p_index = 0;    //Store the index when parsing a message given
int parsing_in_progress = 0;    //Flag that indicates a parsing in progress
                                //0 = no parsing in progress
                                //1 = parsing in progress
                                //-1 = parsing successful
int changing_limits = 0;    //Flag for indicating a changing limits process
                            //0 = no changing limits
                            //-2 = tentaively changing minimum limits
                            //2 = tentaively changing maximum limits
                            //-1 = changing minimum limits
                            //1 = changing maximum limits

char rxmsg[4] = "---";  //Storing the received input from user

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

void parse_message(char *message_expected, char char_received) {
    if(char_received == message_expected[p_index]){
        if(!parsing_in_progress) {   //Inicio del mensaje
            parsing_in_progress = 1;
        } else if(char_received == '\0'){    //End of message
            parsing_in_progress = -1;    //[PARSE SUCCESSFULL]
            p_index = -1;   //with the addition it turns into 0
        }
        p_index++;
    }
    else{
        parsing_in_progress = 0;
        p_index = 0;
    }
}

int is_number(char ch) {
    if((ch >= 48) && (ch<=57)){
        return 1;
    } else {
        return 0;
    }
}

char* number_2_string(int num) {
    int tmp_num = num;

    static char tmp_s[4];

    char centenas = 0;
    char decenas = 0;
    char unidades = 0;

    unidades = tmp_num%10;
    decenas = ((tmp_num%100) - unidades) / 10;
    centenas = ((tmp_num%1000)-((decenas*10)+unidades))/100;

    tmp_s[0] = centenas + 48;
    tmp_s[1] = decenas + 48;
    tmp_s[2] = unidades + 48;

    uart_send(tmp_s);

    return tmp_s;
}

int obtain_min(void) {
    return min;
}

int obtain_max(void) {
    return max;
}

/********************************************************************/

/** Interruption from uart, the x and n keys are configurate 
* in order to set the maximum temperature and the minimum.
* Also with the restriction of entering letters instead of numbers 
* and displaying a message to inform the user of that warning.
*/

void usart1_isr(void) {
    USART1_SR = ~(1<<5);  ///< Erase flag
    char c_received = usart_recv(USART1);

    usart_send_blocking(USART1, c_received);

    //First character of a sequence received (i == 0) AND
    //NOT in process of changing limits AND 
    //NOT a number
    if ((i == 0) && ((!changing_limits) && (!is_number(c_received)))) {  
        //uart_send("\r\nFirst char received\n\r"); 
        if (c_received == 'n') { // changing minimum?
            changing_limits = -2;   //indicating change tentative2minimum

            timer_disable_counter(TIM2);
            //adc_power_off(ADC1);
            log_message("\r\nPlease confirm pressing Enter\n\r");
        }
        if (c_received == 'x') { // changing maximum?
            changing_limits = 2;    //indicating change tentative2maximum

            timer_disable_counter(TIM2);
            //adc_power_off(ADC1);
            log_message("\r\nPlease confirm pressing Enter\n\r");
        }
    }
    //In tentative2minimum or in tentative2maximum
    else if ((i == 0) && ((changing_limits == -2) || (changing_limits == 2))) {
        if ((changing_limits == -2)) {   // tentative2minimum
            //uart_send("\r\ntentative2minimum\n\r");
            if (c_received == '\r'){    // Enter pressed
                changing_limits = -1;
                number_received = 0;
                rxmsg[0] = '-';
                rxmsg[1] = '-';
                rxmsg[2] = '-';
                log_message("Set Min: [include leading zero(s)]\r\n");
            }
            else {  //incorrect sequence
                log_message("\r\nIncorrect sequence\r\n");
                changing_limits = 0;

                adc_power_on(ADC1);
                timer_enable_counter(TIM2);
            }
        }
        if ((changing_limits == 2)) {   // tentative2maximum
            //uart_send("\r\ntentative2maximum\n\r");
            if (c_received == '\r'){    // Enter pressed
                changing_limits = 1;
                number_received = 0;
                rxmsg[0] = '-';
                rxmsg[1] = '-';
                rxmsg[2] = '-';
                log_message("Set Max: [include leading zero(s)]\r\n");
            }
            else {  //incorrect sequence
                log_message("\r\nIncorrect sequence\r\n");

                changing_limits = 0;

                adc_power_on(ADC1);
                timer_enable_counter(TIM2);
            }
        }
    }
    //Not first character received and in changing_limits process
    else if (((changing_limits == -1) || (changing_limits == 1))) {
        //uart_send("\r\nEstoy en el Else If\n\r");
        if ((changing_limits == -1)) {   // 2minimum
            log_message("\r\nChanging minimum...\n\r");
            if (is_number(c_received) && (i < 4)){  //proceding with receiving
                rxmsg[i] = c_received;
                number_received = (number_received*10) + (c_received-48);

                log_message("\r\nYou've entered: ");
                log_3digs_number(number_received);
                log_enter();
                i++;
            }
            else if ((c_received == '\r') && (i < 4)) { //successfull change
                log_message("\r\nNumber Entered\r\n");
                min = number_received;
                log_message("\r\nMinimum Umbral Changed to: ");
                log_3digs_number(min);
                log_message("!!!\r\n");

                i = 0;
                changing_limits = 0;

                adc_power_on(ADC1);
                timer_enable_counter(TIM2);
            }
            else { //Incorrect char OR exceded lenght
                log_message("\r\nError\r\n");

                i = 0;
                changing_limits = 0;

                adc_power_on(ADC1);
                timer_enable_counter(TIM2);
            }
        }
        if ((changing_limits == 1)) {   // 2maximum
            log_message("\r\nChanging maximum...\n\r");
            if (is_number(c_received) && (i < 4)){  //proceding with receiving
                rxmsg[i] = c_received;
                number_received = (number_received*10) + (c_received-48);

                log_message("\r\nYou've entered: ");
                log_3digs_number(number_received);
                log_enter();
                i++;
            }
            else if ((c_received == '\r') && (i < 4)) { //successfull change
                log_message("\r\nNumber Entered\r\n");
                max = number_received;
                log_message("\r\nMaximum Umbral Changed to: ");
                log_3digs_number(max);
                log_message("!!!\r\n");

                i = 0;
                changing_limits = 0;

                adc_power_on(ADC1);
                timer_enable_counter(TIM2);
            }
            else { //Incorrect char OR exceded lenght
                log_message("\r\nError\r\n");

                i = 0;
                changing_limits = 0;

                adc_power_on(ADC1);
                timer_enable_counter(TIM2);
            }
        }
    }
}