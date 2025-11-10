/*
File: main.c
Language: C
Author: Trevor Carlyle
Date: 10/29/25
Description: Entry point for the Humidity Sensor project using Raspberry Pi Pico.

Responsibilities:
- Initialize hardware and subsystems (sensor, display, LED array)
- Periodically read humidity from the sensor
- Update the LED array and display with the current humidity
- Implements error handling

Assumes the following modules exist:
- sensor.c / sensor.h: for reading humidity values
- display.c / display.h: for updating the screen display
- led_array.c / led_array.h: for controlling the 6-stage LED array
*/

// Include standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "pico/stdlib.h"
// Include project files
#include "sensor.h"     // Sensor interface (placeholder)
#include "display.h"    // Display interface (placeholder)
#include "led_array.h"  // LED array interface (placeholder)
// Constants
// Checks every 2 seconds
#define HUMIDITY_CHECK_INTERVAL_MS 2000

int main() {

    printf("Raspberry Pi Humidity Sensor: Initializing hardware...\n");

    // Initialize hardware (sensor, dislay, led array)
    if (!sensor_init()) { // sensor_init is a placeholder function and comes from sensor.c/.h
        printf("ERROR: Failed to initialize humidity sensor!\n");
        // Optionally, blink an error pattern on the LEDs here
        return 1;
    }
    if (!display_init()) { // display_init is a placeholder function and comes from sensor.c/.h
        printf("ERROR: Failed to initialize display!\n");
        return 1;
    }
    if (!led_array_init()) { // led_array_init is a placeholder function and comes from sensor.c/.h
        printf("ERROR: Failed to initialize LED array!\n");
        return 1;
    }

    printf("Initialization complete. Entering main loop.\n");

    // Main loop
    while (true) {
        // Declare variable to hold humidity value
        float humidity = 0.0f;
        // Get the humidity and store it in humidity variable.
        // sensor_read_humidity is a placeholder function from sesnor.c/.h
        bool sensor_ok = sensor_read_humidity(&humidity);
        // If sensor_ok is true, the read was successful
        if (sensor_ok) {
            // Print humidity level
            printf("Humidity: %.1f%%\n", humidity);

            // Update numeric display
            // display_show_humidity is a placeholder function from display.c/.h
            display_show_humidity(humidity);

            // Use LED array module to determine how many LEDs to light up
            // humidity_to_leds and led_array_set are placeholder functions from led_array.c/.h
            uint8_t leds_on = humidity_to_leds(humidity);
            // light up leds_on number of leds
            led_array_set(leds_on);
        //If sensor_ok is false, the read was unsucessful
        } else {
            printf("WARNING: Failed to read humidity sensor!\n");
        }

        // Wait before next check
        sleep_ms(HUMIDITY_CHECK_INTERVAL_MS);
    }

    // Should never reach here
    return 0;
}

/*
Notes:
- sensor_init(), display_init(), led_array_init(), sensor_read_humidity(), display_show_humidity(), humidity_to_leds(), led_array_set()
  are all assumed to be implemented in their respective modules (sensor.c/.h, display.c/.h, led_array.c/.h)
- The HUMIDITY_CHECK_INTERVAL_MS constant can be adjusted as needed.
*/
