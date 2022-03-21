// pico SDK stuff
#include "hardware/flash.h"
#include "pico/stdlib.h"

// c libs
#include <stdio.h>
#include <stdlib.h>
#include <string>

// library I found for JSON parsing and other goodies
#include <mjson.h>

// stuff for flash
#define FLASH_TARGET_OFFSET (128 * 1024)
const uint8_t *flash_target_contents = (const uint8_t *)(XIP_BASE + FLASH_TARGET_OFFSET);
char *id;

// NOTE: defined for RPC engine (provided by mjson)
static int sender(const char *frame, int frame_len, void *privdata) {

    for (size_t i = 0; i < frame_len; i++) {
        uart_putc(uart1, frame[i]);
    }

    return 0;
}

// NOTE: should take a param and set our serial number and the flash
static void setChassisSerialNumber(struct jsonrpc_request *r) {
    bool success = false;

    // erase our old data here
    flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);

    // get our params
    char temp[64];
    mjson_get_string(r->params, r->params_len, "$[0]", temp, sizeof(temp));

    uart_puts(uart1, temp);

    flash_range_program(FLASH_TARGET_OFFSET, (const uint8_t *)temp, FLASH_PAGE_SIZE);

    id = (char *)flash_target_contents;

    success = true;

    // do something
    jsonrpc_return_success(r, "%s", success ? "true" : "false");
}

// NOTE: should return our stored serial number
static void getChassisSerialNumber(struct jsonrpc_request *r) {
    std::string temp = std::string(id);
    // just return or string
    jsonrpc_return_success(r, "%s", ("\"" + temp + "\""));
}

// NOTE: the main loop
int main(void) {
    // init RPC stuff
    jsonrpc_init(NULL, NULL);
    jsonrpc_export("setChassisSerialNumber", setChassisSerialNumber);
    jsonrpc_export("getChassisSerialNumber", getChassisSerialNumber);

    // init UART 1 (not the USB port)
    uart_init(uart1, 115200);
    gpio_set_function(4, GPIO_FUNC_UART);
    gpio_set_function(5, GPIO_FUNC_UART);

    // basic LED stuff
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    while (true) {
        char buf[512];
        size_t len;

        if (uart_is_readable(uart1) > 0) {
            gpio_put(PICO_DEFAULT_LED_PIN, 1);

            len = 0;

            // get the string
            while (uart_is_readable_within_us(uart1, 1000) > 0) {
                char temp = uart_getc(uart1);

                if (temp == '\n') {
                    break;
                }

                buf[len] = temp;
                len++;
            }

            jsonrpc_process(buf, len, sender, NULL, NULL);
        }

        gpio_put(PICO_DEFAULT_LED_PIN, 0);
    }
}