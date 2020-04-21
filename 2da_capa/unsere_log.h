#ifndef UNSERE_LOG_H
#define UNSERE_LOG_H

void log_temperature(int temp, int max_umbral, int min_unbral, char* LED_low, char* LED_high);
void log_message(char* message);
void log_3digs_number(int number);
void log_ASCII(char ascii);
void log_enter(void);
void log_char(char ch);

#endif