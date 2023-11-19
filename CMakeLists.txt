# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

# Export compile commands json file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include($ENV{PICO_SDK_PATH}/tools/CMakeLists.txt)

# Set name of project (as PROJECT_NAME) and C/C++ standards
project(firmware C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME}
    firmware/main/src/main.cpp
    firmware/drivers/pico/i2c/src/pico_i2c_driver.cpp
    firmware/drivers/pico/spi/src/pico_spi_driver.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    firmware
    regs
    firmware/lwip
)

# add_subdirectory(dral)
# add_subdirectory(pio)

# Link to pico_stdlib (gpio, time, etc. functions)
target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    hardware_i2c
    hardware_spi
    pico_lwip
    pico_lwip_nosys
)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# target_include_directories(pico_lwip INTERFACE
#     ${CMAKE_CURRENT_LIST_DIR}/lwip
# )

# Enable usb output
pico_enable_stdio_usb(${PROJECT_NAME} 1)

# Disable uart output, as it uses pins 0 and 1
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})
