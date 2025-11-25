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
 * @brief Display humidity representation on LED strip
 *
 * Converts a humidity reading into a number of LEDs and
 * updates the LED strip to illuminate those LEDs.
 * @param humidity Relative humidity percentage (0.0–100.0)
 */
void humidity_to_leds(float humidity);

/**
 * @brief Enable or disable LED array output
 *
 * When disabled, all LEDs are turned off and humidity_to_leds()
 * will not illuminate anything until re-enabled.
 * @param enabled true to enable LED output, false to disable
 */
void led_array_set_enabled(bool enabled);

/**
 * @brief Get the current LED output state
 *
 * Shows whether the LED array is currently allowed to display
 * visualizations.
 *
 * @return true if LED output is enabled, false if disabled
 */
bool led_array_is_enabled(void);

/**
 * @brief Display a loading "ping pong" animation pattern
 *
 * Useful for initialization and booting states
 * @param ms_total Duration in milliseconds
 */
void led_array_show_loading(uint32_t ms_total);

/**
 * @brief Display error pattern based on error code
 *
 * Each code maps to unique pattern (see README.md for reference)
 * @param code Error code
 * @param ms_total Duration in milliseconds
 */
void led_array_show_error(uint8_t code, uint32_t ms_total);

#endif  // LED_ARRAY_H