cmake_minimum_required(VERSION 3.12)
include(pico_sdk_import.cmake)

set(projname "project1")

project(${projname} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

#include(example_auto_set_url.cmake)

# add_subdirectory()

# include_directories()

add_executable(${projname}
        main.c
        display.c
        led_array.c
        sensor.c
        )

target_link_libraries(${projname} 
        pico_stdlib
        )

# adjust to enable stdio via usb, or uart
pico_enable_stdio_usb(${projname} 1)
pico_enable_stdio_uart(${projname} 0)

# Link the executable with Pico standard library and hardware libraries
target_link_libraries(humidity_sensor 
    pico_stdlib 
    hardware_gpio 
    hardware_i2c )

