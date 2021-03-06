
/************************************************************************
* Copyright 2020 ITESM                                                  *
*                                                                       *
*                                                                       *
* ADC_UART                                                              *
*                                                                       *
* Autorhs:                                                              *
* Jesús Enrique Luna Medina          A01632334                          *
* Daniela abigail Parrales Mejía     A01228629                          *
* Luis Cortés Leal                   A01631163                          *
*                                                                       *
* Abril 2020                                                            *
* The project titled ADC_UART is carried out with the purpose of        *
* implementing a temperature sensing system which acquires the          *
* values from the environment and delivers a response through           *
* hardware using LEDs and software displaying on a Terminal.            *
*                                                                       *
************************************************************************/

/*!< LIBRARIES */
#include <libopencm3/stm32/usart.h>
#include "unsere_log.h"
#include "../1ra_capa/unsere_uart.h"

/***********************************************************************/

/*!< Log the char received */
void log_char(char ch) {
    usart_send_blocking(USART1, ch);
}

/*!< Log the ASCII character of the char received */
void log_ASCII(char ascii) {
    log_char(ascii + 48);
}

/*!< Log the ASCII form of a three digit number */
void log_3digs_number(int number) {
    char centenas = 0;
    char decenas = 0;
    char unidades = 0;

    int tmp_num = number;

    unidades = tmp_num%10;
    decenas = ((tmp_num%100) - unidades) / 10;
    centenas = ((tmp_num%1000)-((decenas*10)+unidades))/100;

    log_ASCII(centenas);
    log_ASCII(decenas);
    log_ASCII(unidades);
}

/*!< Logs a message */
void log_message(char* message) {
    while (*message != '\0') {
        usart_send_blocking(USART1, *message++);
    }
}

/*!< Logs the line jump and return to begining of the line */
void log_enter(void) {
    log_message("\r\n");
}

/*!< Display on terminal temperature message */
void log_temperature(int temp, int max_umbral, int min_unbral, char* LED_low, char* LED_high) {
    log_message("\r\nTemp: ");
    log_3digs_number(temp);
    
    log_message("    Max: ");
    log_3digs_number(max_umbral);

    log_message("    Min: ");
    log_3digs_number(min_unbral);

    log_message("    LED-Low: ");
    log_message(LED_low);

    log_message("    LED-High: ");
    log_message(LED_high);

    log_message("\r\n");
}

/***********************************************************************/
