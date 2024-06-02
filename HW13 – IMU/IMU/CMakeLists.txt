cmake_minimum_required(VERSION 3.13)

set(FAMILY rp2040)
set(BOARD pico_sdk)
set(TINYUSB_FAMILY_PROJECT_NAME_PREFIX "tinyusb_dev_")

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(my_project)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(imu
    src/imu.c
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(imu pico_stdlib hardware_uart hardware_i2c hardware_adc hardware_pwm)
# target_link_libraries(imu bsp_board descriptors pico_unique_id tinyusb_device tinyusb_board tusb)

# For Using Screen 
# Note: Change file name
pico_enable_stdio_usb(imu 1)
pico_enable_stdio_uart(imu 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(imu)