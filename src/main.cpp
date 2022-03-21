#include "pico/stdlib.h"
#include <stdio.h>

int main(void) {
    // Initialise UART 0
    uart_init(uart1, 115200);

    // set the gpio
    gpio_set_function(4, GPIO_FUNC_UART);
    gpio_set_function(5, GPIO_FUNC_UART);

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    while (true) {
        //TODO:
        /*
            We want a few things to happen
            get the serial number stored in flash
            store it here
            when we get a char string from serial port desipher it to JSON

            depending on if it is a GET/SET request do the following

            get the serial number and return it
        */
        uart_puts(uart1, "\nBreak World\n");

        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        sleep_ms(500);
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        sleep_ms(500);
    }
}