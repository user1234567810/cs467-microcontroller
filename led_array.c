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
- Support placeholder testing

Requires the following modules:
- led_array.h: for interface definitions

Wiring configuration
** WS2812 RGB 8 LED Strip **
GPIO 2 (pin 4) -> DIN (data in) on LED strip
3.3V (pin 36)  -> Power rail (+)  -> VCC on LED strip
GND  (pin 38)  -> Ground rail (-) -> GND on LED strip
*/


/*
--Outline--

Functions:

led_array_init()

humidity_to_leds(humidity):
    - Convert humidity percentage (0-100) to LED count (0-8)
    - Return number of LEDs to light

led_array_set(num_leds):
    - Humidity visualization (lights num of LEDs from humidity_to_leds)

led_array_show_loading():
    - Loading visualization (cycle through LEDs)

led_array_show_error(code):
    - Error visualization (blinking red)

Testing:
    - Use mock humidity values
    - Will make temp main.c for local testing
*/

#include <stdio.h>
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
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return
        ((uint32_t) (r) << 8) |
        ((uint32_t) (g) << 16) |
        (uint32_t) (b);
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

// Writes color into memory buffer
static void hw_set_pixel(uint8_t i, uint8_t r, uint8_t g, uint8_t b) {
    if (i < LED_COUNT)
        led_buf[i] = pack_grb(r, g, b);
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

// Sets lit LEDs to blue
static void set_color(uint8_t idx, uint8_t lit, uint8_t *r, uint8_t *g, uint8_t *b) {
    *r = 0;
    *g = 0;
    *b = 255;
}

// Turn on given number of LEDs and turn off rest
static void led_array_set(uint8_t leds_on) {
    if (leds_on > LED_COUNT)
        leds_on = LED_COUNT;
    for (uint8_t i = 0; i < LED_COUNT; i++) {
        if (i < leds_on) {
            // Pick and set color if LED supposed to be on
            uint8_t r, g, b;
            set_color(i, leds_on, &r, &g, &b);
            hw_set_pixel(i, r, g, b);
        }
        else {
            // Turn LED off
            hw_set_pixel(i, 0, 0, 0);
        }
    }
    hw_show();
}

int main(void) {
    stdio_init_all();
    if (!led_array_init()) return 1;

    // Fake humidity values
    float tests[] = {0, 5, 12.5, 25, 37.5, 50, 62.5, 75, 87.5, 100};
    int n = sizeof(tests) / sizeof(tests[0]);

    while (true) {
        for (int i = 0; i < n; ++i) {
            float h = tests[i];
            uint8_t leds_on = humidity_to_leds(h);
            led_array_set(leds_on);
            sleep_ms(400);
        }
    }
}