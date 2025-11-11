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
GPIO 6 (pin 9) -> SDA on DHT20
GPIO 7 (pin 10) -> SCL on DHT20
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

// Initialize structure to hold data
dht_reading sensor_measurement;
dht_reading *sensor_measurement_ptr = &sensor_measurement;

// Initialize DHT20 sensor
bool dht_init(void) {
    printf("Initializing the DHT20 sensor.\n");
    i2c_init(I2C_PORT, I2C_FREQ);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    // Read status register to verify connection: To be implemented next
    uint8_t status;
    printf("Checking status register: ");
    // int status_register = i2c_read_blocking(I2C_PORT, DHT20_I2C_ADDR, &status, 1, false);
    int status_register = i2c_read_blocking_until(I2C_PORT, DHT20_I2C_ADDR, &status, 1, false, 5000);
    if (status_register < 0) {
        printf("DHT20 not found at address 0x%02X\n", DHT20_I2C_ADDR);
        return false;
    } else {
        printf("Status ok.\n");
    }

    return true;
}

// Get a reading from the DHT20 sensor (Adapted from DHT example code)
void read_from_dht(dht_reading *result) {
    // Send command trigger to sensor
    printf("Sending the command trigger.\n");
    uint8_t i2c_init_signal[3] = {DHT20_CMD_TRIGGER, DHT20_CMD_BYTE_1, DHT20_CMD_BYTE_2};
    int send_command = i2c_write_blocking(I2C_PORT, DHT20_I2C_ADDR, i2c_init_signal, 3, false);
    // int send_command = i2c_write_blocking_until(I2C_PORT, DHT20_I2C_ADDR, i2c_init_signal, 3, false, 5000);
    if (send_command < 0) {
        printf("Failed: send_command = %d\n", send_command);
    } else {
        printf("Success: send_command = %d\n", send_command);
    }
    sleep_ms(SLEEP_TIME);

    // Read data back from the sensor
    printf("Receiving data from the sensor.\n");
    uint8_t received_data[7];
    int receive_data = i2c_read_blocking(I2C_PORT, DHT20_I2C_ADDR, received_data, 7, false);
    // int receive_data = i2c_read_blocking_until(I2C_PORT, DHT20_I2C_ADDR, received_data, 7, false, 5000);
    if (receive_data < 0) {
        printf("Failed: receive_data = %d\n", receive_data);
    } else {
        printf("Success: receive_data = %d\n", receive_data);
    }

    // Check if sensor was done measuring: Status byte (0) bit 7 == 0 when ready
    if (received_data[0] & 0x80) {
        printf("Sensor is busy.\n");
    }

    // Collect raw humidity data from received_data bytes: 20 bits total
    // From Byte 1: bits [19:12]
    // From Byte 2: bits [11:4]
    // From Byte 3: bits [3:0]
    uint32_t raw_humidity = ((uint32_t)received_data[1] << 12 | (uint32_t)received_data[2] << 4 | (uint32_t)received_data[3] >> 4);

    // Convert humidity from binary to decimal percentage
    result->humidity = (raw_humidity / 1048576.0f) * 100.0f;
    result->humidity = 0.2 * 100.0f;
}

int main() {
    // Initialize stdio, then wait for sensor to connect to USB
    stdio_init_all();
    sleep_ms(5000);

    // Initialize DHT20 sensor
    int dht_init_status = dht_init();
    if (dht_init_status == 0) {
        printf("The sensor did not initialize successfully. Please restart.\n");
        return 1;
    }
    hard_assert(dht_init_status == 1);
    printf("DHT20 sensor successfully initialized.\n");

    // Start data read loop
    printf("Starting measurements.\n");
    while (true) {
        // Read after successful DHT initialization, print status while reading
        // & processing data. Adapted from the DHT example code.
        printf("\n------------------------------------------------------\n");
        read_from_dht(sensor_measurement_ptr);
        printf("Humidity: %.0f%%\n", sensor_measurement.humidity);
        sleep_ms(2000);
    }

    return 0;
}
