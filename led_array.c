/*
File: led_array.c
Language: C
Author: Andrew Poon
Date: 10/30/25
Description: Provides initialization & control functionality for the
    WS2812 RGB 8 LED Strip connected to the Raspberry Pi Pico.

    Portions of low-level PIO setup and pixel packing logic were adapted from
    the official Raspberry Pi Pico SDK "ws2812" pio example and is licensed as
    follows:
    Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
    SPDX-License-Identifier: BSD-3-Clause
    Source: https://github.com/raspberrypi/pico-examples/tree/master/pio/ws2812

Responsibilities:
- Initialize and configure the LED array
- Light LEDs based on humidity levels given by the sensor
- Display loading and error animations for debugging and status indication

Requires the following modules:
- led_array.h: for interface definitions

Wiring configuration
** WS2812 RGB 8 LED Strip **
GPIO 2 (pin 4) -> DIN (data in) on LED strip
3.3V (pin 36)  -> Power rail (+)  -> VCC on LED strip
GND  (pin 38)  -> Ground rail (-) -> GND on LED strip
*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

#define LED_PIN    2
#define LED_COUNT  8

static PIO pio = pio0;
static int sm = -1;
static uint32_t led_buf[LED_COUNT];

// Pack RGB into GRB order
static inline uint32_t pack_grb(uint8_t r, uint8_t g, uint8_t b) {
    return
        ((uint32_t) (r) << 8) |
        ((uint32_t) (g) << 16) |
        (uint32_t) (b);
}

// Writes color into memory buffer
static void hw_set_pixel(uint8_t i, uint8_t r, uint8_t g, uint8_t b) {
    if (i < LED_COUNT)
        led_buf[i] = pack_grb(r, g, b);
}

// Send colors from memory buffer to LED strip
static void hw_show(void) {
    for (int i = 0; i < LED_COUNT; ++i)
        pio_sm_put_blocking(pio, sm, led_buf[i] << 8);
    sleep_us(100);
}

// Clear all LEDs
static void hw_clear(void) {
    for (int i = 0; i < LED_COUNT; ++i)
        led_buf[i] = 0;
    hw_show();
}

// Load ws2812 control program to PIO, claim unused state machine,
// set timing to 800 kHz (required for ws2812 LEDs)
static bool led_array_init(void) {
    uint offset = pio_add_program(pio, &ws2812_program);
    sm = pio_claim_unused_sm(pio, true);
    ws2812_program_init(pio, sm, offset, LED_PIN, 800000, false);
    hw_clear();
    return true;
}

// Convert humidity percentage (0–100) to LEDs (0-8)
static uint8_t humidity_to_leds(float h) {
    if (h < 0)
        h = 0;
    if (h > 100)
        h = 100;
    // How much humidity each LED represents
    float step = 100.0f / (float)LED_COUNT;
    // How many LEDs should be on
    uint8_t n = (uint8_t)((h + step / 2.0f) / step);
    if (n > LED_COUNT)
        n = LED_COUNT;
    return n;
}

// Turn on given number of LEDs and turn off rest
static void led_array_set(uint8_t leds_on) {
    if (leds_on > LED_COUNT)
        leds_on = LED_COUNT;
    for (uint8_t i = 0; i < LED_COUNT; i++) {
        if (i < leds_on) {
            hw_set_pixel(i, 0, 0, 255);
        }
        else {
            // Turn LED off
            hw_set_pixel(i, 0, 0, 0);
        }
    }
    hw_show();
}

// Loading visualization
static void show_loading(uint32_t ms_total) {

    uint32_t start_time = to_ms_since_boot(get_absolute_time());

    while (to_ms_since_boot(get_absolute_time()) - start_time < ms_total) {
        uint32_t elapsed = to_ms_since_boot(get_absolute_time()) / 60;
        int position = elapsed % (LED_COUNT * 2 - 2);
        if (position >= LED_COUNT)
            position = (LED_COUNT * 2 - 2) - position;
        for (uint8_t i = 0; i < LED_COUNT; i++)
            hw_set_pixel(i, 0, 0, 0);
        hw_set_pixel(position, 255, 255, 0);
        hw_show();
        sleep_ms(16);
    }
}

// Error visualization
static void show_error(uint8_t code, uint32_t ms_total) {

    uint32_t start_time = to_ms_since_boot(get_absolute_time());

    while (to_ms_since_boot(get_absolute_time()) - start_time < ms_total) {

        uint8_t leds_to_light = code;
        if (leds_to_light > LED_COUNT)
            leds_to_light = LED_COUNT;

        // Turn off all LEDs
        for (uint8_t i = 0; i < LED_COUNT; i++)
            hw_set_pixel(i, 0, 0, 0);

        // Light up all 'leds_to_light' LEDs in red
        for (uint8_t i = 0; i < leds_to_light; i++)
            hw_set_pixel(i, 255, 0, 0);

        // Show the pattern
        hw_show();
        sleep_ms(180);

        // Turn off pattern
        hw_clear();
        sleep_ms(180);
    }
}

// ---------------------------------------------------------------------------
// Temporary test main()
// This allows led_array.c to compile and run independently for testing
// Remove before integrating with the actual project main.c
// ---------------------------------------------------------------------------
int main(void) {
    stdio_init_all();
    if (!led_array_init()) return 1;

    show_loading(2000);

    while (true) {
        // Fake humidity values
        float tests[] = {0, 5, 12.5, 25, 37.5, 50, 62.5, 75, 87.5, 100};
        int n = sizeof(tests) / sizeof(tests[0]);
        for (int i = 0; i < 2; ++i) {
            for (int i = 0; i < n; ++i) {
                float h = tests[i];
                uint8_t leds_on = humidity_to_leds(h);
                led_array_set(leds_on);
                sleep_ms(400);
            }
        }
        // Shows visualization for error code 2, 4 and 8
        for (int i = 2; i <= 8; i*=2) {
            show_error(i, 2000);
            show_loading(2000);
        } 
    }
}