/*
File: sensor.c
Language: C
Author: Johanna Varness
Date: 10/29/25
Description: Provides initialization & reading functionality between the
    Raspberry Pi Pico and the DHT20 humidity sensor. Currently, uses 
    different LED patterns to indicate mode & status.
    
    This file utilizes some adapted DHT example code. The default Pico 
    LED will display a slower and smaller LED flash to indicate when it
    is reading and processing data.

    The Raspberry Pi DHT example code is licensed as follows:
    Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
    SPDX-License-Identifier: BSD-3-Clause

    DHT example code is identified in the comments preceding code 
    blocks.

Responsibilities:
- Initialize sensor and default Pico LED
- Periodically read humidity from the sensor

Requires the following modules:
- sensor.h: for reading humidity values

Wiring configuration
** DHT20 Sensor **
I2C0 bus
GPIO 4 (pin 6) -> SDA on DHT20
GPIO 5 (pin 7) -> SCL on DHT20
3.3v (pin 36) -> VCC on DHT20
GND (pin 38) -> GND on DHT20
*/

// Import modules
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"

// Import project files
#include "sensor.h"     // Sensor interface

// Initialize DHT20 sensor
bool dht_init(void) {
    printf("Initializing the DHT20 sensor.\n");
    i2c_init(I2C_PORT, I2C_FREQ);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    // Verify connection
    uint8_t i2c_init_signal[1] = {0x00};
    int result = i2c_read_blocking(I2C_PORT, DHT20_I2C_ADDR, i2c_init_signal, 1, false);
    if (result < 0) {
        printf("DHT20 not responding at address 0x%02X\n", DHT20_I2C_ADDR);
        return false;
    }

    return true;
}

// Get a reading from the DHT20 sensor (Adapted from DHT example code)
void read_from_dht(dht_reading *result) {
    // Send command trigger to sensor
    printf("Sending the command trigger.\n");
    uint8_t i2c_init_signal[3] = {DHT20_CMD_TRIGGER, DHT20_CMD_BYTE_1, DHT20_CMD_BYTE_2};
    int send_command = i2c_write_blocking(I2C_PORT, DHT20_I2C_ADDR, i2c_init_signal, 3, false);
    if (send_command < 0) {
        printf("Failed: send_command = %d\n", send_command);
    }
    sleep_ms(SLEEP_TIME);

    // Read data back from the sensor
    printf("Receiving data from the sensor.\n");
    uint8_t received_data[7];
    int receive_data = i2c_read_blocking(I2C_PORT, DHT20_I2C_ADDR, received_data, 7, false);
    if (receive_data < 0) {
        printf("Failed: receive_data = %d\n", receive_data);
    }

    // Check if sensor was done measuring: Status byte (0) bit 7 == 0 when ready
    if (received_data[0] & 0x80) {
        printf("Sensor is busy.\n");
        return;
    }

    // Collect raw humidity data from received_data bytes: 20 bits total
    // From Byte 1: bits [19:12]
    // From Byte 2: bits [11:4]
    // From Byte 3: bits [3:0]
    uint32_t raw_humidity = ((uint32_t)received_data[1] << 12 | (uint32_t)received_data[2] << 4 | (uint32_t)received_data[3] >> 4);

    // Convert humidity from binary to decimal percentage
    result->humidity = (raw_humidity / HUMIDITY_BIN_TO_DEC) * 100.0f;   
}

float get_humidity(dht_reading *result) {
    return result->humidity;
}
