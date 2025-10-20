# Set the minimum required version of CMake
cmake_minimum_required(VERSION 4.2.0)
# Include the Pico SDK
include(pico_sdk_import.cmake)
# Define the project and supported languages
project(humidity_sensor C)
# Initialize the Pico SDK
pico_sdk_init()
# Add the main executable and source files
add_executable(humidity_sensor
    main.c
    sensor.c
    led_array.c
    display.c
)
# Link the executable with Pico standard library and hardware libraries
target_link_libraries(humidity_sensor pico_stdlib hardware_gpio hardware_i2c)
# Generate extra output files (e.g., .uf2 for flashing the Pico)
pico_add_extra_outputs(humidity_sensor)
