/*
File: sensor.c
Language: C
Author: Johanna Varness
Date: 10/29/25
Description: Provides initialization & reading functionality between the
    Raspberry Pi Pico and the DHT20 humidity sensor. Currently, uses 
    different LED patterns to indicate mode & status.

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

// Initialize LED (Adapted from Blink example code)
int pico_led_init(void) {
#if defined(PICO_DEFAULT_LED_PIN)
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    return PICO_OK;
#endif
}

// Initialize DHT20 sensor (Adapted from DHT example code)
bool dht_init(void) {
    stdio_init_all();

    // I2C initialization sequence
    i2c_init(i2c_default, 100 * 1000); // Standard baud rate of 100 kHz
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

    // Set the DHT pin to send output, send an init signal (0), then wait
    gpio_set_dir(DHT_PIN, GPIO_OUT);
    gpio_put(DHT_PIN, 0);
    sleep_ms(SLEEP_TIME);

    // Set the DHT pin to receive input
    gpio_set_dir(DHT_PIN, GPIO_IN);

    // Send an on command to the LED pin (indicates receiving/reading data)
    gpio_put(PICO_DEFAULT_LED_PIN, 1);

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
    gpio_put(PICO_DEFAULT_LED_PIN, 0);

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

// Turn the led on or off (Adapted from the Blink example code)
void pico_set_led(bool led_on) {
#if defined(PICO_DEFAULT_LED_PIN)
    gpio_put(PICO_DEFAULT_LED_PIN, led_on);
#endif
}

int main() {
    int dht_init_status = dht_init();
    hard_assert(dht_init_status == 1);
    // This logic adapted from the Blink example code
    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);
    while (true) {
        // This block adapted from the Blink example code
        // Blink after successful DHT & LED initialization
        if (BLINK_AFTER_INIT) {
            pico_set_led(true);
            sleep_ms(LED_DELAY_MS);
            pico_set_led(false);
            sleep_ms(LED_DELAY_MS);
        }

        // Read after successful DHT & LED initialization, slower blink pattern
        if (!BLINK_AFTER_INIT) {
            // Adapted from the DHT example code
            dht_reading reading;
            read_from_dht(&reading);
            float fahrenheit = (reading.temp_celsius * 9 / 5) + 32;         // placeholder
            printf("Humidity = %.1f%%, Temperature = %.1fC (%.1fF)\n",      // placeholder
               reading.humidity, reading.temp_celsius, fahrenheit);         // placeholder
            sleep_ms(2000);
        }
    }
}
