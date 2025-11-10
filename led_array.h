/*
File: led_array.h
Language: C
Author: Andrew Poon
Date: 11/07/25
Description: Provides the public interface for controlling the WS2812
    RGB LED array. Declares constants and functions for initializing
    LED array, mapping humidity values to LED levels, and displaying
    loading or error animations.
*/

#ifndef LED_ARRAY_H
#define LED_ARRAY_H

#include <stdint.h>
#include <stdbool.h>


// Initialize WS2812 driver
bool led_array_init(void);

// Map humidity % to number of LEDs
uint8_t humidity_to_leds(float humidity);

// Light the first leds_on LEDs
void led_array_set(uint8_t leds_on);

// Optional visual patterns
void led_show_loading(uint32_t ms_total);
void led_show_error(uint8_t code, uint32_t ms_total);

#endif  // LED_ARRAY_H