/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

 /* 
 This file uses the blink example as starter code. It initializes the
 DHT20 sensor and then blinks if successful.
 */

 // Import modules
#include <stdio.h>
#include "pico/stdlib.h"
#include <stdbool.h>
#include "hardware/i2c.h"
#include <math.h>
#include "hardware/gpio.h"

/* 
Connections on Raspberry Pi Pico board.
GPIO 6 (pin 9) -> SDA on DHT20 (humidity sensor)
GPIO 7 (pin 10) -> SCL on DHT20
3.3v (pin 36) -> VCC on DHT20
GND (pin 38) -> GND on DHT20
*/

// Commands
// #ifndef LED_DELAY_MS
#define LED_DELAY_MS 250
// #endif
// By default the DHT20 sensor I2C address is 0x38
#define DHT20_I2C_ADDR 0x38
#define DHT20_CMD_INIT 0xBE
#define DHT20_CMD_TRIGGER 0xAC
#define DHT20_CMD_SOFT_RESET 0xBA
const uint DHT_PIN = 15;
const uint MAX_TIMINGS = 85;

// Configure I2C
#define I2C_PORT i2c0
#define I2C_SDA_PIN 9
#define I2C_SCL_PIN 10
#define I2C_FREQ 400000

// Initialize LED
int pico_led_init(void) {
#if defined(PICO_DEFAULT_LED_PIN)
    // A device like Pico that uses a GPIO for the LED will define PICO_DEFAULT_LED_PIN
    // so we can use normal GPIO functionality to turn the led on and off
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    return PICO_OK;
#endif
}

// Initialize DHT20 sensor
bool dht_init(void) {
    stdio_init_all();
    gpio_init(DHT_PIN);
    return true;
}

// Create a structure for the DHT20 sensor data
typedef struct {
    float humidity;
    float temp_celsius;
} dht_reading;

// Get a reading from the DHT20 sensor
void read_from_dht(dht_reading *result) {
    int data[5] = {0, 0, 0, 0, 0};
    uint last = 1;
    uint j = 0;

    gpio_set_dir(DHT_PIN, GPIO_OUT);
    gpio_put(DHT_PIN, 0);
    sleep_ms(20);
    gpio_set_dir(DHT_PIN, GPIO_IN);

    gpio_put(PICO_DEFAULT_LED_PIN, 1);
    for (uint i = 0; i < MAX_TIMINGS; i++) {
        uint count = 0;
        while (gpio_get(DHT_PIN) == last) {
            count++;
            sleep_us(1);
            if (count == 255) break;
        }
        last = gpio_get(DHT_PIN);
        if (count == 255) break;
        if ((i >= 4) && (i % 2 == 0)) {
            data [j/8] <<= 1;
            if (count > 16) data[j / 8] |= 1;
            j++;
        }
    }
    gpio_put(PICO_DEFAULT_LED_PIN, 0);
    if ((j >= 40) && (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF))) {
        result->humidity = (float) ((data[0] << 8) + data[1]) / 10;
        if (result->humidity > 100) {
            result->humidity = data[0];
        }
        result->temp_celsius = (float) (((data[2] & 0x7F) << 8) + data[3]) / 10;
        if (result->temp_celsius > 125) {
            result->temp_celsius = data[2];
        }
        if (data[2] & 0x80) {
            result->temp_celsius = -result->temp_celsius;
        }
    } else {
        printf("Bad data\n");
    }
}

// Turn the led on or off
void pico_set_led(bool led_on) {
#if defined(PICO_DEFAULT_LED_PIN)
    // Just set the GPIO on or off
    gpio_put(PICO_DEFAULT_LED_PIN, led_on);
#endif
}

int main() {
    int dht_init_status = dht_init();
    hard_assert(dht_init_status == 1);
    int rc = pico_led_init();
    // printf("Printing something now\n");
    hard_assert(rc == PICO_OK);
    while (true) {
        pico_set_led(true);
        sleep_ms(LED_DELAY_MS);
        pico_set_led(false);
        sleep_ms(LED_DELAY_MS);
    }
}
