# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

# Export compile commands json file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include($ENV{PICO_SDK_PATH}/tools/CMakeLists.txt)

# Set name of project (as PROJECT_NAME) and C/C++ standards
project(ina C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    inc/
    regs/
)

# add_subdirectory(dral)
# add_subdirectory(pio)

# Link to pico_stdlib (gpio, time, etc. functions)
target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    hardware_i2c
)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# Enable usb output
pico_enable_stdio_usb(${PROJECT_NAME} 1)

# Disable uart output, as it uses pins 0 and 1
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})
