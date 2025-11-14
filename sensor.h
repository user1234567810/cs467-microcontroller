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
#define SLEEP_TIME 100

// Structures
// Create a structure for the DHT20 sensor data (Adapted from DHT example code)
typedef struct {
    float humidity;
    float temp_celsius;
} dht_reading;

// Function prototypes
bool dht_init(void);
void read_from_dht(dht_reading *result);
float get_humidity(dht_reading *result);

// Definitions (from the DHT20 Datasheet: https://aqicn.org/air/sensor/spec/asair-dht20.pdf)
#define DHT20_I2C_ADDR 0x38
#define DHT20_CMD_TRIGGER 0xAC
#define DHT20_CMD_BYTE_1 0x33
#define DHT20_CMD_BYTE_2 0x00

// Configure I2C for DHT20 sensor
#define I2C_PORT i2c0           // We should make sure this doesn't conflict with the LCD settings
#define I2C_SDA_PIN 4
#define I2C_SCL_PIN 5
#define I2C_FREQ 100000         // Baud rate

// Define humidity conversion macros
#define HUMIDITY_BIN_TO_DEC 1048576.0f   // 2^20 = 1048576

#endif
