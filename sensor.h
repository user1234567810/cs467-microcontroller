/*
File: sensor.h
Language: C
Author: Johanna Varness
Date: 10/29/25
Description: Provides interface for structures, functions, and constants for 
    the sensor.c and main.c files.
    
    This file utilizes some adapted DHT example code. The default Pico 
    LED will display a slower and smaller LED flash to indicate when it
    is reading and processing data.

    The Raspberry Pi DHT example code is licensed as follows:
    Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
    SPDX-License-Identifier: BSD-3-Clause

    DHT example code is identified in the comments preceding code 
    blocks.
*/

// Check if SENSOR_H already exists; if not, define.
#ifndef SENSOR_H
#define SENSOR_H

// User can update the sleep value if adjustments are needed.
const uint SLEEP_TIME = 20;

// Structures
// Create a structure for the DHT20 sensor data (Adapted from DHT example code)
typedef struct {
    float humidity;
    float temp_celsius;
} dht_reading;

// Function prototypes
bool dht_init(void);
void read_from_dht(dht_reading *result);

// Definitions (adapted from Blink example code)
#define DHT20_I2C_ADDR 0x38     // Default DHT20 sensor I2C addr
#define DHT20_CMD_INIT 0xBE
#define DHT20_CMD_TRIGGER 0xAC
#define DHT20_CMD_SOFT_RESET 0xBA

// Configure I2C
#define I2C_PORT i2c0
#define I2C_SDA_PIN 9
#define I2C_SCL_PIN 10
#define I2C_FREQ 400000

// Constants (Adapted from DHT example code)
const uint DHT_PIN = 15;
const uint MAX_TIMINGS = 85;

#endif
