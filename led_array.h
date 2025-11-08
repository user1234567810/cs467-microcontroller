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


/**
 * @brief Initialize WS2812 LED driver and PIO state machine
 *
 * Must be called once at startup before any LED updates
 * @return true if initialization succeeds
 */
bool led_array_init(void);

/**
 * @brief Convert a humidity percentage to number of LEDs to light
 *
 * Used to map sensor readings to LED visual output
 * @param humidity Relative humidity percentage (0.0–100.0)
 * @return Number of LEDs to illuminate (0–8)
 */
uint8_t humidity_to_leds(float humidity);

/**
 * @brief Display a loading "ping pong" animation pattern
 *
 * Useful for initialization and booting states
 * @param ms_total Duration in milliseconds
 */
void led_show_loading(uint32_t ms_total);

/**
 * @brief Display error pattern based on error code
 *
 * Each code maps to unique pattern (see README.md for reference)
 * @param code Error code
 * @param ms_total Duration in milliseconds
 */
void led_show_error(uint8_t code, uint32_t ms_total);

#endif  // LED_ARRAY_H