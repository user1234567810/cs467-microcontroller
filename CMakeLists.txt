# Generated Cmake Pico project file
cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Include the Pico SDK
include(pico_sdk_import.cmake)

# Define the project and supported languages
project(humidity_sensor C CXX ASM)

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
