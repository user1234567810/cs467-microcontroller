/*
File: sensor.h
Language: C
Author: Johanna Varness
Date: 10/29/25
Description: Provides interface for structures, functions, and constants for 
    the sensor.c and main.c files.

    This file uses the Blink example as starter code. In initialization mode,
    the default Pico LED will use the example Blink pattern to indicate the
    successful initialization of the default LED and the DHT20 sensor. 
    
    This file also adapts some DHT example code. In reading mode, the default
    Pico LED will display a slower and smaller LED flash to indicate 
    successful reading status.

    The Raspberry Pi Blink and DHT example code is licensed as follows:
    Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
    SPDX-License-Identifier: BSD-3-Clause

    Blink & DHT example code is identified in the comments preceding code 
    blocks.
*/

// Check if SENSOR_H already exists; if not, define.
#ifndef SENSOR_H
#define SENSOR_H

// Structures
// Create a structure for the DHT20 sensor data (Adapted from DHT example code)
typedef struct {
    float humidity;
    float temp_celsius;
} dht_reading;

// Function prototypes
int pico_led_init(void);
bool dht_init(void);
void read_from_dht(dht_reading *result);
void pico_set_led(bool led_on);

// Definitions (adapted from Blink example code)
#define LED_DELAY_MS 250
#define DHT20_I2C_ADDR 0x38     // Default DHT20 sensor I2C addr
#define DHT20_CMD_INIT 0xBE
#define DHT20_CMD_TRIGGER 0xAC
#define DHT20_CMD_SOFT_RESET 0xBA

// Mode: Change the boolean to switch the blinking DHT status indicator
bool BLINK_AFTER_INIT = false;

// Constants (Adapted from DHT example code)
const uint DHT_PIN = 15;
const uint MAX_TIMINGS = 85;

// Configure I2C
#define I2C_PORT i2c0
#define I2C_SDA_PIN 9
#define I2C_SCL_PIN 10
#define I2C_FREQ 400000

#endif
