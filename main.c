/*
File: main.c
Language: C
Author: Trevor Carlyle
Date: 10/29/25
Last Updated: 11/9/25
Description: Entry point for the Humidity Sensor project using Raspberry Pi Pico.

Responsibilities:
- Initialize hardware and subsystems (sensor, display, LED array)
- Periodically read humidity from the sensor
- Update the LED array and display with the current humidity
- Implements error handling

Assumes the following modules exist:
- sensor.c / sensor.h: for reading humidity values
- display.c / display.h: for updating the screen display
- led_array.c / led_array.h: for controlling the 8-stage LED array
*/

// Include standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "pico/stdlib.h"

// Include project files
#include "sensor.h"     // Sensor interface (sensor.c/.h)
#include "display.h"    // Display interface (display.c/.h)
#include "led_array.h"  // LED array interface (led_array.c/.h)

// Optional WiFi feature toggle (only use with Pico2W)
#ifdef ENABLE_WIFI
#include "network.h"
#endif

// Globals shared with network.c
float g_latest_humidity = 0.0f;
float g_latest_temp_f   = 0.0f;

// Constants
// Checks every 2 seconds, can be adjusted as needed.
#define HUMIDITY_CHECK_INTERVAL_MS 2000
#define SLEEP_MS 5000

int main() {
    stdio_init_all(); // Initialize stdio
    sleep_ms(SLEEP_MS);
    printf("Raspberry Pi Humidity Sensor: Initializing hardware...\n");

    // Initialize the DHT20 humidity sensor (sensor.c/.h)
    if (!dht_init()) {
        printf("ERROR: Failed to initialize humidity sensor!\n");
        // Show error pattern on LED array (led_array.c/.h)
        led_array_show_error(2, 2000); // Show error code 2 for 2 seconds
        return 1;
    }
  
    // Initialize the display (display.c/.h)
    if (!display_init()) {
        printf("ERROR: Failed to initialize LCD display!\n");
        led_array_show_error(3, 2000); // Show error code 3 for 2 seconds
        return 1;
    }
  
    // Initialize the LED array (led_array.c/.h)
    if (!led_array_init()) {
        printf("ERROR: Failed to initialize LED array!\n");
        // Show error pattern on LED array (led_array.c/.h)
        led_array_show_error(4, 2000); // Show error code 4 for 2 seconds
        return 1;
    }

// Start WiFi access point and web server
// This block only runs when compile time flag ENABLE_WIFI is used
#ifdef ENABLE_WIFI
    if (!wifi_start_ap("PICO2W-AP", "capstone467")) {
        printf("ERROR: Failed to start WiFi access point.\n");
    } else {
        if (!web_server_start(80)) {
            printf("ERROR: Failed to start web server.\n");
        } else {
            printf("WiFi AP active. Connect to SSID 'PICO2W-AP' and open http://192.168.4.1/\n");
        }
    }
#endif

    printf("Initialization complete. Entering main loop.\n");

    // Main loop
    while (true) {
        // Structure to hold sensor readings (sensor.c/.h)
        dht_reading reading;
        // Read humidity and temperature from DHT20 (sensor.c/.h)
        read_from_dht(&reading);

        // Store latest readings for the web UI
        g_latest_humidity = reading.humidity;
        g_latest_temp_f   = reading.temp_fahrenheit;

        // Print only humidity to output
        printf("Humidity: %.1f%%\n", reading.humidity);
        // Update the LCD display (display.c/.h)
        display_clear(); // Clear previous display
        display_set_cursor(0, 0); // Go to the top line of display
        char line1[17]; // Declare an array line1
        // Format a string with the current humidity value and stores it in line1
        snprintf(line1, sizeof(line1), "Humidity: %.1f%%", reading.humidity);
        // Use display_print from (display.c/.h)
        display_print(line1);

        // Display temperature in fahrenheit on LCD
        display_set_cursor(0, 1);
        char line2[17];
        snprintf(line2, sizeof(line2), "Temp: %.1fF", reading.temp_fahrenheit);
        display_print(line2);

        // Update the LED array (led_array.c/.h)
        humidity_to_leds(reading.humidity);
    
        // Wait before next check
        sleep_ms(HUMIDITY_CHECK_INTERVAL_MS);
    }

    // Should never reach here
    return 0;
}