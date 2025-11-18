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

/**
 * @brief Create a structure for the DHT20 sensor data (Adapted from DHT example code)
 */
typedef struct {
    float humidity;
    float temp_celsius;
    float temp_fahrenheit;
} dht_reading;


// Function prototypes
/**
 * @brief Initialize the DHT20 sensor using the provided I2C instance
 * 
 * Must be called once at startup before any measurements will be read.
 * @return True if initialization succeeds, false otherwise
 */
bool dht_init(void);

/**
 * @brief Initiate, read, and process DHT20 sensor measurement data
 * 
 * @param *result A pointer to the dht_reading structure storing measurement values
 */
void read_from_dht(dht_reading *result);

/**
 * @brief Convert a given float value from Celsius to Fahrenheit
 * 
 * @param float A Celsius float value
 * 
 * @return Fahrenheit float value
 */
float celsius_to_fahrenheit(float temp_celsius);

/**
 * @brief Get the humidity value stored in the dht_reading structure provided
 * 
 * @param *result A pointer to the dht_reading structure storing measurement values
 * 
 * @return Most recent humidity reading as a float percentage value
 */
float get_humidity(dht_reading *result);

/**
 * @brief Get the temperature value in Celsius stored in the dht_reading structure provided
 * 
 * @param *result A pointer to the dht_reading structure storing measurement values
 * 
 * @return Most recent temperature reading in Celsius as a float value
 */
float get_temp_celsius(dht_reading *result);

/**
 * @brief Get the temperature value in Fahrenheit stored in the dht_reading structure provided
 * 
 * @param *result A pointer to the dht_reading structure storing measurement values
 * 
 * @return Most recent temperature reading in Fahrenheit as a float value
 */
float get_temp_fahrenheit(dht_reading *result);

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
#define BIN_TO_DEC 1048576.0f   // 2^20 = 1048576

#endif
