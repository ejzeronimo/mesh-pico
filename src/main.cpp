// c libs
#include <stdio.h>
#include <stdlib.h>

// pico SDK stuff
#include "hardware/flash.h"
#include "pico/stdlib.h"

// library I found for JSON parsing and other goodies
#include <mjson.h>

// stuff for flash
#define FLASH_TARGET_OFFSET (256 * 1024)
const uint8_t *flash_target_contents = (const uint8_t *)(XIP_BASE + FLASH_TARGET_OFFSET);

// serial number props
#define SN_LENGTH 128

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

    // get our params
    char temp[1024];
    int result = mjson_get_string(r->params, r->params_len, "$.serialNumber", temp, sizeof(temp));

    if (result <= SN_LENGTH && result != -1) {
        // make json to store
        char jsonFlash[SN_LENGTH + 64];

        strcpy(jsonFlash, "{\"serialNumber\":\"");
        strcat(jsonFlash, temp);
        strcat(jsonFlash, "\"}");

        // erase our old data and store the new data
        flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);
        flash_range_program(FLASH_TARGET_OFFSET, (const uint8_t *)jsonFlash, FLASH_PAGE_SIZE);

        success = true;

        // do something
        jsonrpc_return_success(r, "%s", success ? "true" : "false");
    } else {
        jsonrpc_return_error(r, -32602, "Invalid Request", NULL);
    }
}

// NOTE: should return our stored serial number
static void getChassisSerialNumber(struct jsonrpc_request *r) {
    // check the flash store
    char raw[SN_LENGTH];

    if (-1 != mjson_get_string((char *)flash_target_contents, FLASH_PAGE_SIZE, "$.serialNumber", raw, sizeof(raw))) {
        // just return or string
        char formatted[SN_LENGTH + 3];

        strcpy(formatted, "\"");
        strcat(formatted, raw);
        strcat(formatted, "\"");

        jsonrpc_return_success(r, "%s", formatted);
    } else {
        // return an error
        jsonrpc_return_error(r, -32603, "Internal Error", NULL);
    }
}

// NOTE: the main loop
int main(void) {
    // init UART 1 (not the USB port)
    uart_init(uart1, 115200);
    gpio_set_function(4, GPIO_FUNC_UART);
    gpio_set_function(5, GPIO_FUNC_UART);

    // check the flash store
    const char *buf;
    int len;

    if (MJSON_TOK_STRING != mjson_find((char *)flash_target_contents, FLASH_PAGE_SIZE, "$.serialNumber", &buf, &len)) {
        // write JSON with empty string there
        flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);

        const char *s = "{ \"serialNumber\" : \"\" }"; // {"serialNumber":""}

        flash_range_program(FLASH_TARGET_OFFSET, (const uint8_t *)s, FLASH_PAGE_SIZE);
    }

    // init RPC stuff
    jsonrpc_init(NULL, NULL);
    jsonrpc_export("setChassisSerialNumber", setChassisSerialNumber);
    jsonrpc_export("getChassisSerialNumber", getChassisSerialNumber);

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