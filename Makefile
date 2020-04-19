BINARY = main
SRCFILES = main.c 1ra_capa/unsere_gpio.c 1ra_capa/unsere_adc.c 1ra_capa/unsere_timer.c 1ra_capa/unsere_uart.c 1ra_capa/unsere_rcc.c 3ra_capa/temp_sense.c 2da_capa/led.c

all: elf bin

include ../Makefile.incl
