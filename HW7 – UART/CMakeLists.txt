cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(my_project)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(uart_advanced
    src/ssd1306.c
    src/uart_advanced.c
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(uart_advanced pico_stdlib hardware_i2c hardware_adc hardware_uart)

# For Using Screen 
# Note: Change file name
pico_enable_stdio_usb(uart_advanced 1)
pico_enable_stdio_uart(uart_advanced 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(uart_advanced)