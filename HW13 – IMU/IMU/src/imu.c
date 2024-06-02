/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h> // for memset
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"

#include <stdlib.h>
// #include "bsp/board.h"
// #include "tusb.h"
// #include "usb_descriptors.h"

// config registers
#define ADDR 0x68
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C
#define PWR_MGMT_1 0x6B
#define PWR_MGMT_2 0x6C
// sensor data registers:
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40

#define TEMP_OUT_H   0x41
#define TEMP_OUT_L   0x42

#define GYRO_XOUT_H  0x43
#define GYRO_XOUT_L  0x44
#define GYRO_YOUT_H  0x45
#define GYRO_YOUT_L  0x46
#define GYRO_ZOUT_H  0x47
#define GYRO_ZOUT_L  0x48

#define WHO_AM_I     0x75

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+

/* Blink pattern
 * - 250 ms  : device not mounted
 * - 1000 ms : device mounted
 * - 2500 ms : device is suspended
 */
enum  {
  BLINK_NOT_MOUNTED = 250,
  BLINK_MOUNTED = 1000,
  BLINK_SUSPENDED = 2500,
};

static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;

void led_blinking_task(void);
void hid_task(void);

int who_am_i() {
    uint8_t buf[1];
    uint8_t reg = WHO_AM_I;
    i2c_write_blocking(i2c_default, ADDR, &reg, 1, true);
    i2c_read_blocking(i2c_default, ADDR, buf, 1, false); 
    printf(buf); // Prints "h" = 104 = 0x68
    printf("\n");
}

void chip_init() {
    uint8_t buf[2];
    // Turning chip on
    buf[0] = PWR_MGMT_1;
    buf[1] = 0x00;
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
    // Enable accelerometer
    buf[0] = ACCEL_CONFIG;
    buf[1] = 0b00; // ±2g
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
    // Enable gyroscope
    buf[0] = GYRO_CONFIG;
    // buf[1] = 0b00; // 2000 º/s
    buf[1] = 0b00011000;
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
}

// Gets raw data from IMU
void read_raw(uint8_t *buf) {
    uint8_t reg = ACCEL_XOUT_H;
    i2c_write_blocking(i2c_default, ADDR, &reg, 1, true); 
    i2c_read_blocking(i2c_default, ADDR, buf, 14, false);
}

void process_data(uint8_t *buf, float *dataf) {
    // Processes it
    signed short data[7];
    for (int i = 0; i < 7; i = i + 1) {
        data[i] = buf[2*i]<<8 | buf[2*i + 1];
    }
    // Acceleration (g)
    dataf[0] = data[0] * 0.000061;
    dataf[1] = data[1] * 0.000061;
    dataf[2] = data[2] * 0.000061;
    // Temperature (ºC)
    dataf[3] = (data[3]/340.00) + 36.53;
    // Gyroscope (º/s)
    dataf[4] = data[4] * 0.007630;
    dataf[5] = data[5] * 0.007630;
    dataf[6] = data[6] * 0.007630;
}

int main() {

    // Pico initialization
    stdio_init_all();
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
    
    // I2C SDA and SCL Pins
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C); // Be SDA
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C); // Be SDL

    // Chip Initialization
    chip_init();

    // Heartbeat
    const uint HEART_BEAT = 25;
    gpio_init(HEART_BEAT);
    gpio_set_dir(HEART_BEAT, GPIO_OUT);

    printf("Hello!\n");
    who_am_i();
    int print_delay = 0;
    int print_delay_wrap = 10;

    while (1) {
        gpio_put(HEART_BEAT, 1);
        sleep_ms(5); // read at 100 Hz

        uint8_t arr[14];
        read_raw(arr);
        
        // Print raw data
        // if(print_delay > print_delay_wrap) {
        //     for (int i = 0; i < 14; i++) {
        //         if (i == 13) {
        //             printf("%d\n", arr[i]);
        //         } else {
        //             printf("%d\t", arr[i]);
        //         }
        //     }
        //     print_delay = 0;
        // }
        
        // Print processed data
        float data[7];
        process_data(arr, data);

        if(print_delay > print_delay_wrap) {
            printf("Acc.X: %.3f\t", data[0]);
            printf("Acc.Y: %.3f\t", data[1]);
            printf("Acc.Z: %.3f\n", data[2]);
            printf("Temp.: %.3f\n", data[3]);
            printf("Gyro.X: %.3f\t", data[4]);
            printf("Gyro.Y: %.3f\t", data[5]);
            printf("Gyro.Z: %.3f\n\n", data[6]);
            print_delay = 0;
        }

        gpio_put(HEART_BEAT, 0);
        sleep_ms(5); // read at 100 Hz
        print_delay = print_delay + 1;
    }
}
