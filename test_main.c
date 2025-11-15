/*
File: test_main.c
Language: C
Author: Trevor Carlyle
Date: 11/09/25
Description: Test suite for the Microcontroller Home Humidity Sensor project.
Responsibilities:
- Test initialization of all hardware modules (sensor, display, LED array)
- Test reading humidity values from the sensor
- Test updating the display and LED array with various humidity values

Usage:
This file is intended to be compiled and run on the Raspberry Pi Pico
to verify correct operation of the main project modules. 
Each test prints its result to the console.
*/

#include <stdio.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "sensor.h"
#include "display.h"
#include "led_array.h"

// Helper macro for test result output
#define TEST_ASSERT(cond, msg) \
    do { \
        if (cond) { \
            printf("[PASS] %s\n", msg); \
        } else { \
            printf("[FAIL] %s\n", msg); \
        } \
    } while (0)

// Test 1: Initialization of all modules
void test_initialization() {
    printf("\nTest: Initialization\n");
    bool sensor_ok = dht_init();
    TEST_ASSERT(sensor_ok, "Sensor initialization");
    // Can't test display_init because it returns void
    display_init(i2c0, 6, 7, 0x27);
    printf("Display initialized)\n");
    bool led_ok = led_array_init();
    TEST_ASSERT(led_ok, "LED array initialization");
}

// Test 2: Read humidity from sensor
void test_sensor_read() {
    printf("\nTest: Sensor Read\n");
    dht_reading reading;
    read_from_dht(&reading);
    TEST_ASSERT(reading.humidity >= 0.0f && reading.humidity <= 100.0f,
                "Humidity reading in valid range (0-100%)");
    printf("Humidity: %.1f%%\n", reading.humidity);
}

// Test 3: LED array mapping for various humidity values
void test_led_array_mapping() {
    printf("\nTest: LED Array Mapping\n");
    float test_values[] = {0.0f, 20.0f, 40.0f, 60.0f, 80.0f, 100.0f};
    for (int i = 0; i < 6; ++i) {
        uint8_t leds = humidity_to_leds(test_values[i]);
        printf("Humidity: %.1f%% -> LEDs on: %u\n", test_values[i], leds);
        led_array_set(leds); // Set the number of LEDs to light up
        sleep_ms(500); // Wait 500ms so you can see the result
    }
    printf("Check that the correct number of LEDs light up for each value.\n");
}

// Test 4: Display update for humidity
void test_display_update() {
    printf("\nTest: Display Update\n");
    float test_humidities[] = {12.3f, 45.6f, 78.9f};
    for (int i = 0; i < 3; ++i) {
        display_clear();
        display_set_cursor(0, 0);
        // Prepare a string buffer to hold the formatted humidity text
        char buf[17];
         // Format the humidity value as a string, e.g., "Humidity: 12.3%"
        snprintf(buf, sizeof(buf), "Humidity: %.1f%%", test_humidities[i]);
        // Print the string to the display
        display_print(buf);
        printf("Display should show: %s\n", buf);
        sleep_ms(1000); // Wait 1000ms so you can see the result
    }
}

int main() {
    stdio_init_all();
    printf("\nHumidity Sensor Test Suite\n");
    test_initialization();
    test_sensor_read();
    test_led_array_mapping();
    test_display_update();
    printf("\nAll tests complete.\n");
    return 0;
}
