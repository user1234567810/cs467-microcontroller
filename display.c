/*
File: display.c
Language: C
Author: Andrew Poon
Date: 11/08/25
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

// flags for display entry mode
const int LCD_ENTRYSHIFTINCREMENT = 0x01;
const int LCD_ENTRYLEFT = 0x02;

// flags for display and cursor control
const int LCD_BLINKON = 0x01;
const int LCD_CURSORON = 0x02;
const int LCD_DISPLAYON = 0x04;

// flags for display and cursor shift
const int LCD_MOVERIGHT = 0x04;
const int LCD_DISPLAYMOVE = 0x08;

// flags for function set
const int LCD_5x10DOTS = 0x04;
const int LCD_2LINE = 0x08;
const int LCD_8BITMODE = 0x10;

// flag for backlight control
const int LCD_BACKLIGHT = 0x08;

const int LCD_ENABLE_BIT = 0x04;

// By default these LCD display drivers are on bus address 0x27
static int addr = 0x27;

// Modes for lcd_send_byte
#define LCD_CHARACTER  1
#define LCD_COMMAND    0

#define MAX_LINES      2
#define MAX_CHARS      16

void display_init(i2c_inst_t *i2c, uint sda, uint scl, uint8_t addr);

void display_clear(void);

void display_set_cursor(uint8_t col, uint8_t row);

void display_print(const char *text);