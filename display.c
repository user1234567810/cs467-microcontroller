/*
File: display.c
Language: C
Author: Andrew Poon
Date: 11/09/25
Description:
    Provides initialization & control functionality for the LCD1602 16x2 character
    display connected to the Raspberry Pi Pico.

    Portions of low-level LCD command sequencing and I2C write patterns were adapted
    from the official Raspberry Pi Pico SDK LCD1602 I2C example and is licensed as
    follows:
    Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
    SPDX-License-Identifier: BSD-3-Clause
    Source: https://github.com/raspberrypi/pico-examples/tree/master/i2c/lcd_1602_i2c

Responsibilities:
- Initialize and configure the LCD1602 over I2C
- Clear the display and position the cursor
- Print text strings to the LCD

Requires the following modules:
- display.h: for interface definitions

Wiring configuration
** LCD1602 Display **
GPIO 6 (pin 9)  -> SDA on LCD1602
GPIO 7 (pin 10) -> SCL on LCD1602
3.3v (pin 36)   -> VCC on LCD1602
GND (pin 38)    -> GND on LCD1602
*/

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "display.h"

const int LCD_CLEARDISPLAY = 0x01;
const int LCD_RETURNHOME = 0x02;
const int LCD_ENTRYMODESET = 0x04;
const int LCD_DISPLAYCONTROL = 0x08;
const int LCD_CURSORSHIFT = 0x10;
const int LCD_FUNCTIONSET = 0x20;
const int LCD_SETCGRAMADDR = 0x40;
const int LCD_SETDDRAMADDR = 0x80;

// Flags for display entry mode
const int LCD_ENTRYSHIFTINCREMENT = 0x01;
const int LCD_ENTRYLEFT = 0x02;

// Flags for display and cursor control
const int LCD_BLINKON = 0x01;
const int LCD_CURSORON = 0x02;
const int LCD_DISPLAYON = 0x04;

// Flags for display and cursor shift
const int LCD_MOVERIGHT = 0x04;
const int LCD_DISPLAYMOVE = 0x08;

// Flags for function set
const int LCD_5x10DOTS = 0x04;
const int LCD_2LINE = 0x08;
const int LCD_8BITMODE = 0x10;

// Flag for backlight control
const int LCD_BACKLIGHT = 0x08;

const int LCD_ENABLE_BIT = 0x04;

// Modes for lcd_send_byte
#define LCD_CHARACTER  1
#define LCD_COMMAND    0

#define MAX_LINES      2
#define MAX_CHARS      16
#define DELAY_US       600  // Delay in microseconds  

static i2c_inst_t *s_i2c = NULL;   // Pointer to active I2C instance
static uint8_t     s_addr = 0x27;  // I2C address of the LCD

// Send one byte (command or data) to LCD via I2C
static inline void i2c_write_byte(uint8_t v) {
    i2c_write_blocking(s_i2c, s_addr, &v, 1, false);
}

// Pulse the LCD's Enable line to latch data
static void lcd_toggle_enable(uint8_t val) {
    sleep_us(DELAY_US);
    i2c_write_byte(val | LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
    i2c_write_byte(val & ~LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
}

static void lcd_send_byte(uint8_t val, uint8_t mode) {
    // Split byte value into two halves
    uint8_t high = mode | (val & 0xF0) | LCD_BACKLIGHT;
    uint8_t low = mode | ((val << 4) & 0xF0) | LCD_BACKLIGHT;

    // Send each half and toggle Enable so LCD latches the data
    i2c_write_byte(high);
    lcd_toggle_enable(high);
    i2c_write_byte(low);
    lcd_toggle_enable(low);
}

void display_init(i2c_inst_t *i2c, uint sda, uint scl, uint8_t addr) {
    // Store I2C instance and LCD address
    s_i2c = i2c;
    s_addr = addr;

    // Configure SDA and SCL pins for I2C
    gpio_set_function(sda, GPIO_FUNC_I2C);
    gpio_set_function(scl, GPIO_FUNC_I2C);
    gpio_pull_up(sda);
    gpio_pull_up(scl);

    // Initialize LCD into 4-bit mode
    lcd_send_byte(0x03, 0);
    lcd_send_byte(0x03, 0);
    lcd_send_byte(0x03, 0);
    lcd_send_byte(0x02, 0);

    // Set text entry, display mode, and clear screen
    lcd_send_byte(LCD_ENTRYMODESET   | LCD_ENTRYLEFT, 0);
    lcd_send_byte(LCD_FUNCTIONSET    | LCD_2LINE, 0);
    lcd_send_byte(LCD_DISPLAYCONTROL | LCD_DISPLAYON, 0);
    lcd_send_byte(LCD_CLEARDISPLAY, 0);
    sleep_ms(2);  // Allow LCD time to finish clear
}

void display_clear(void){
    lcd_send_byte(LCD_CLEARDISPLAY, LCD_COMMAND);
    sleep_ms(2);  // Allow LCD time to finish clear
}

// Moves the LCD cursor to a specific line/position
void display_set_cursor(uint8_t col, uint8_t row) {
    if (row >= MAX_LINES) row = MAX_LINES - 1;
    if (col >= MAX_CHARS) col = MAX_CHARS - 1;
    uint8_t base = (row == 0) ? 0x80 : 0xC0;
    lcd_send_byte(base + col, LCD_COMMAND);
}

// Take a string and show it on the LCD by sending one char at a time
void display_print(const char *s) {
    if (!s) return;
    while (*s) {
        lcd_send_byte((uint8_t)(*s++), LCD_CHARACTER);
    }
}