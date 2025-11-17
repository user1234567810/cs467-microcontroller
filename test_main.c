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

// Constants
// Checks every 2 seconds, can be adjusted as needed.
#define SLEEP_MS 5000

// Helper macro for test result output
#define TEST_ASSERT(cond, msg) \
    do { \
        if (cond) { \
            printf("[PASS] %s\n", msg); \
        } else { \
            printf("[FAIL] %s\n", msg); \
        } \
    } while (0)

// Test 1: Initialization of all modules (called once at startup)
void test_initialization() {
    printf("\nTest: Initialization\n");
    bool sensor_ok = dht_init();
    TEST_ASSERT(sensor_ok, "Sensor initialization");
    bool display_ok = display_init();
    TEST_ASSERT(display_ok, "Display initialization");
    bool led_ok = led_array_init();
    TEST_ASSERT(led_ok, "LED array initialization");
}

// Test: Check if hardware is still working (called in loop)
void test_hardware_status() {
    printf("\nTest: Hardware Status Check\n");

    // Test sensor by attempting to read
    dht_reading reading;
    read_from_dht(&reading);
    bool sensor_ok = (reading.humidity >= 0.0f && reading.humidity <= 100.0f);
    TEST_ASSERT(sensor_ok, "Sensor is responding");
    if (!sensor_ok) {
        printf("  -> Sensor may be disconnected or faulty\n");
    }

    // Test display by writing to it
    display_clear();
    display_set_cursor(0, 0);
    display_print("Status: OK");
    printf("[INFO] Display write attempted\n");

    // Test LED array by setting a pattern
    humidity_to_leds(50.0f);
    printf("[INFO] LED array write attempted\n");
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
        printf("Humidity: %.1f%% -> visualize on strip\n", test_values[i]);
        humidity_to_leds(test_values[i]);
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
    sleep_ms(SLEEP_MS);

    printf("\n========================================\n");
    printf("Humidity Sensor Test Suite\n");
    printf("========================================\n");

    // Initialize hardware ONCE at startup
    test_initialization();

    // Run tests in an infinite loop to detect runtime failures
    while (true) {
        printf("\n========================================\n");
        printf("Runtime Test Cycle\n");
        printf("========================================\n");

        test_hardware_status();   // Check if hardware is still working
        test_sensor_read();        // Read from sensor
        test_led_array_mapping();  // Test LED patterns
        test_display_update();     // Test display updates

        printf("\nAll tests complete.\n");
        printf("Waiting 5 seconds before next test cycle...\n\n");
        sleep_ms(SLEEP_MS);
    }

    return 0;
}
