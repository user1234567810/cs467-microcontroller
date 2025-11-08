/*
File: display.c
Language: C
Author: Andrew Poon
Date: 11/08/25
Description: Provides initialization & control functionality for the
    LCD1602 display connected to the Raspberry Pi Pico.

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
3.3v (pin 36)   -> Power rail (+)  -> VCC on LCD1602
GND (pin 38)    -> Ground rail (-) -> GND on LCD1602
*/
