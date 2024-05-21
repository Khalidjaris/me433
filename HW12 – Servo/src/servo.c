#include <stdio.h>
#include <string.h> // for memset
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"

#define LEDPIN 21

volatile char m[100];
volatile int current;

void servo_init();
void change_angle(uint angle);

// Set for 50 Hz
void servo_init() {
  gpio_set_function(LEDPIN, GPIO_FUNC_PWM); // Set the LED Pin to be PWM
  uint slice_num = pwm_gpio_to_slice_num(LEDPIN); // Get PWM slice number
  float div = 40; // must be between 1-255
  pwm_set_clkdiv(slice_num, div); // divider
  uint16_t wrap = 62500; // when to rollover, must be less than 65535
  pwm_set_wrap(slice_num, wrap);
  pwm_set_enabled(slice_num, true); // turn on the PWM

//   pwm_set_gpio_level(LEDPIN, wrap / 8); // set the duty cycle to 12.5% (180º)
//   pwm_set_gpio_level(LEDPIN, wrap/40); // set the duty cycle to 12.5% (180º)
//   sleep_ms(200);
  current = wrap/40;
}

void change_angle(uint angle) {
    int duty = angle * 34.7 + 1450; // 34.7 per degree + 2.5% duty cycle
    if(duty - current > 0) {
        while(current < duty) {
            pwm_set_gpio_level(LEDPIN, current + 34.7); 
            sleep_ms(10);
            current = current + 34.7;
        }
    }
    else {
        while(current > duty) {
            pwm_set_gpio_level(LEDPIN, current - 34.7); 
            sleep_ms(10);
            current = current - 34.7;
        }
    }
}


int main() {

    // SERVO Initialize
    servo_init();

    // PRINTF Initialize
    // stdio_init_all();
    // while (!stdio_usb_connected()) {
    //     sleep_ms(100);
    // }

    // Heartbeat
    const uint HEART_BEAT = 15;
    gpio_init(HEART_BEAT);
    gpio_set_dir(HEART_BEAT, GPIO_OUT);

    /////////

    printf("Hello!\n");

    while (1) {
        // Heartbeat
        // gpio_put(HEART_BEAT, 1);
        // sleep_ms(200);
        // gpio_put(HEART_BEAT, 0);
        // sleep_ms(200);
        while(1) {
            change_angle(180);
            // sleep_ms(2000);
            change_angle(0);
            // sleep_ms(2000);
        }
    }
}