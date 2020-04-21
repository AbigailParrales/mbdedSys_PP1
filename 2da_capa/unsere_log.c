#include <libopencm3/stm32/usart.h>

#include "unsere_log.h"
#include "../1ra_capa/unsere_uart.h"

void log_char(char ch) {
    usart_send_blocking(USART1, ch);
}

void log_ASCII(char ascii) {
    log_char(ascii + 48);
}

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

void log_message(char* message) {
    while (*message != '\0') {
        usart_send_blocking(USART1, *message++);
    }
}

void log_enter(void) {
    log_message("\r\n");
}

void log_temperature(int temp, int max_umbral, int min_unbral) {
    log_message("\r\nTemp: ");
    log_3digs_number(temp);
    
    log_message("    Max: ");
    log_3digs_number(max_umbral);

    log_message("    Min: ");
    log_3digs_number(min_unbral);

    log_message("\r\n");
}

