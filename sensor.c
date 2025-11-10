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

// Initialize DHT20 sensor (Adapted from DHT example code)
bool dht_init(void) {
    stdio_init_all();

    // I2C initialization sequence
    i2c_init(I2C_PORT, I2C_FREQ); // Quicker baud rate of 400 kHz
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    return true;
}

// Get a reading from the DHT20 sensor (Adapted from DHT example code)
void read_from_dht(dht_reading *result) {
    // Create variables
    int data[5] = {0, 0, 0, 0, 0};
    uint previous_pin_state = 1;
    uint received_data_bits = 0;

    // Send an init signal (0) to the DHT sensor, then wait
    // gpio_set_dir(DHT_PIN, GPIO_OUT);
    // gpio_put(DHT_PIN, 0);
    // uint8_t i2c_init_signal[3] = {0xAC, 0x33, 0x00};
    printf("Going to try the write blocking function now...\n");
    uint8_t i2c_init_signal[1] = {0x00};
    int bytes_written = i2c_write_blocking_until(I2C_PORT, DHT20_I2C_ADDR, i2c_init_signal, 1, false, 5000);
    if (bytes_written == PICO_ERROR_GENERIC) {
        printf("PICO ERROR GENERIC\n");
    } else {
        printf("SUCCESSFULLY WROTE %d bytes!!\n", bytes_written);
    }
    // i2c_write_blocking(I2C_PORT, DHT20_I2C_ADDR, i2c_init_signal, 3, false);
    sleep_ms(SLEEP_TIME);

    
    // Set the DHT pin to receive input
    gpio_set_dir(DHT_PIN, GPIO_IN);
    uint8_t i2c_receive_buffer[40];

    // Send an on command to the LED pin (indicates receiving/reading data)
    printf("Reading from DHT20\n");

    // Loop through timing cycles
    for (uint i = 0; i < MAX_TIMINGS; i++) {
        // Count the duration of pin status in microseconds
        uint count = 0;
        while (gpio_get(DHT_PIN) == previous_pin_state) {
            count++;
            sleep_us(1);
            if (count == 255) break;        // Times out after 255 microseconds
        }

        // Update previous pin state
        previous_pin_state = gpio_get(DHT_PIN);
        if (count == 255) break;            // Times out after 255 microseconds

        // First 4 cycles are the preamble, skip those
        // Data bits are encoded in the even cycles, process those
        if ((i >= 4) && (i % 2 == 0)) {
            // Shift bits left after processing to allow for new bit
            data [received_data_bits / 8] <<= 1;
            // Pulse > 16 microseconds represents 1 bit: set the least significant bit to 1
            if (count > 16) data[received_data_bits / 8] |= 1;
            // Increment bit counter
            received_data_bits++;
        }
    }

    // Turns off the LED when done receiving/reading
    printf("Now validating data\n");

    // Data validation: 40 bits received & checksum in byte 5 matches sum of 1st 4 bytes
    // NOTE: This section is all experimental until we can verify the readings with a proper print/output set up
    if ((received_data_bits >= 40) && (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF))) {
        // Process the data from raw bytes into decimal form
        result->humidity = (float) ((data[0] << 8) + data[1]) / 10;
        if (result->humidity > 100) {           // This block may not be necessary for the DHT20 - check when we can read the numbers
            result->humidity = data[0];
        }
        result->temp_celsius = (float) (((data[2] & 0x7F) << 8) + data[3]) / 10;
        if (result->temp_celsius > 125) {       // This block may not be necessary for the DHT20 - check when we can read the numbers
            result->temp_celsius = data[2];
        }
        // Check for negative temperatures (bit 7, byte 3 = sign bit)
        if (data[2] & 0x80) {
            result->temp_celsius = -result->temp_celsius;
        }
    } else {
        printf("Bad data\n");
    }
}

int main() {
    sleep_ms(5000);             // Give the Pico time to power up
    printf("Initializing the DHT20 sensor.\n");
    int dht_init_status = dht_init();
    hard_assert(dht_init_status == 1);
    while (true) {
        // Read after successful DHT initialization, print status while reading
        // & processing data. Adapted from the DHT example code.
        sleep_ms(5000);
        printf("Initiating the read function now...\n");
        read_from_dht(sensor_measurement_ptr);
        float fahrenheit = (sensor_measurement.temp_celsius * 9 / 5) + 32;
        printf("Humidity = %.1f%%, Temperature = %.1fC (%.1fF)\n",
        sensor_measurement.humidity, sensor_measurement.temp_celsius, fahrenheit);
        sleep_ms(2000);
    }
}
