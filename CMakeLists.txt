cmake_minimum_required(VERSION 3.12)
include(pico_sdk_import.cmake)

set(projname "project1")

project(${projname} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

option(ENABLE_WIFI "Enable WiFi (Pico 2 W only)" OFF)

#include(example_auto_set_url.cmake)

# add_subdirectory()

# include_directories()

add_executable(${projname}
        main.c
        display.c
        led_array.c
        sensor.c
        )

target_include_directories(${projname} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

if(ENABLE_WIFI)
    target_sources(${projname} PRIVATE
        network.c
    )
endif()


# Generate PIO header from .pio file
pico_generate_pio_header(${projname} ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio)

# adjust to enable stdio via usb, or uart
pico_enable_stdio_usb(${projname} 1)
pico_enable_stdio_uart(${projname} 0)

# Link the executable with Pico standard library and hardware libraries
target_link_libraries(${projname}
    pico_stdlib
    hardware_gpio
    hardware_i2c
    hardware_pio
    hardware_clocks )


if(ENABLE_WIFI)
    target_compile_definitions(${projname} PRIVATE ENABLE_WIFI=1)

    target_link_libraries(${projname}
        pico_cyw43_arch_lwip_threadsafe_background
    )
endif()

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${projname})

# Build the test executable
add_executable(${projname}_test
        test_main.c
        display.c
        led_array.c
        sensor.c
        )

# Generate PIO header for test executable
pico_generate_pio_header(${projname}_test ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio)

# Enable stdio via USB for test executable
pico_enable_stdio_usb(${projname}_test 1)
pico_enable_stdio_uart(${projname}_test 0)

# Link libraries for test executable
target_link_libraries(${projname}_test
    pico_stdlib
    hardware_gpio
    hardware_i2c
    hardware_pio
    hardware_clocks )

# Create output files for test executable
pico_add_extra_outputs(${projname}_test)