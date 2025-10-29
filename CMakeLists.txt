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

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${projname})

