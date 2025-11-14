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
#include <stdbool.h>

#define LCD_I2C_PORT      i2c0
#define LCD_I2C_SDA_PIN   2
#define LCD_I2C_SCL_PIN   3
#define LCD_I2C_ADDR      0x27
#define LCD_I2C_FREQ      100000

/**
 * @brief Initialize the LCD1602 display using provided I2C instance
 * 
 * @return True if initialization succeeds, false otherwise
 */
bool display_init(void);

/**
 * @brief Clear the LCD display and reset the cursor to home position
 */
void display_clear(void);

/**
 * @brief Move the cursor to a specific column and row.
 * 
 * @param col  Column position (0–15)
 * @param row  Row position (0–1)
 */
void display_set_cursor(uint8_t col, uint8_t row);

/**
 * @brief Print a null-terminated string starting at the current cursor position.
 * 
 * @param text  String to display (max 16 characters per line)
 */
void display_print(const char *text);

#endif  // DISPLAY_H
