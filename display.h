/*
File: display.h
Language: C
Author: Andrew Poon
Date: 11/08/25
Description: Provides the public interface for controlling the LCD1602
    display. Declares constants and functions for initializing the
    LCD display, displaying humidity values to LCD, and displaying
    system status / error visualizations.
*/

#ifndef DISPLAY_H
#define DISPLAY_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"

/**
 * @brief Initialize the LCD1602 display using provided I2C instance
 */
void display_init(i2c_inst_t *i2c, uint sda, uint scl, uint8_t addr);

/**
 * @brief Clear the LCD display and reset the cursor to home position
 */
void display_clear(void);

/**
 * @brief Move the cursor to a specific column and row.
 */
void display_set_cursor(uint8_t col, uint8_t row);

/**
 * @brief Print a null-terminated string starting at the current cursor position.
 */
void display_print(const char *text);

#endif  // DISPLAY_H
