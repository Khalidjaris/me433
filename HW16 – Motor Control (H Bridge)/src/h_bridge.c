#include <stdio.h>
#include <string.h> // for memset
#include "pico/stdlib.h"
#include "hardware/uart.h"
//#include "hardware/irq.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"

#define IN1_pwm 15
#define IN2 14
#define IN3_pwm 13
#define IN4 12

#define CUTOFF 30
#define FLOOR 25

int left_duty[101];
int right_duty[101];
volatile char m[100];
volatile int direction = 1;

void pwm_pin_init();
void set_speed();
void generate_gain_curves();

// Set for 1250 Hz
void pwm_pin_init() {
    gpio_set_function(IN1_pwm, GPIO_FUNC_PWM); // Set the LED Pin to be PWM
    uint slice_num = pwm_gpio_to_slice_num(IN1_pwm); // Get PWM slice number
    float div = 1000; // must be between 1-255
    pwm_set_clkdiv(slice_num, div); // divider
    uint16_t wrap = 62500; // when to rollover, must be less than 65535
    pwm_set_wrap(slice_num, wrap);
    pwm_set_enabled(slice_num, true); // turn on the PWM

    gpio_set_function(IN3_pwm, GPIO_FUNC_PWM); // Set the LED Pin to be PWM
    uint slice_num_2 = pwm_gpio_to_slice_num(IN3_pwm); // Get PWM slice number
    pwm_set_clkdiv(slice_num_2, div); // divider
    pwm_set_wrap(slice_num_2, wrap);
    pwm_set_enabled(slice_num_2, true); // turn on the PWM

    gpio_put(IN2, 0);
    gpio_put(IN4, 0);
}

void set_speed(const uint IN_pin, uint percent) {
    int duty = percent * 625; // 34.7 per degree + 2.5% duty cycle
    pwm_set_gpio_level(IN_pin, duty); 
}

void generate_gain_curves() {
    for (int i = 0; i < 101; i++) {
        if (i <= CUTOFF) {
            left_duty[i] = (int) i * 100/CUTOFF;
            right_duty[i] = 100;
        } else if (i > CUTOFF && i < (100-CUTOFF)) {
            left_duty[i] = 100;
            right_duty[i] = 100;
        } else {
            left_duty[i] = 100;
            right_duty[i] = i * -100/CUTOFF + 333;
        }
    }
    // for(int i = 0; i < 101; i++) {
    //     printf("%d  ", left_duty[i]);
    // } 
    // printf("\n");
    // for(int i = 0; i < 101; i++) {
    //     printf("%d  ", right_duty[i]);
    // } 
    // printf("\n");
}

void set_from_gain(int num) {
    pwm_set_gpio_level(IN1_pwm, (625 * left_duty[num]));
    pwm_set_gpio_level(IN3_pwm, (625 * right_duty[num]));
    printf("\r\nLeft: %d\n", left_duty[num]);
    printf("Right: %d\n", right_duty[num]);
}

int main() {

    stdio_init_all();
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }

    // Initialize GPIOs
    // Motor 1
    gpio_init(IN1_pwm);
    gpio_set_dir(IN1_pwm, GPIO_OUT);
    gpio_init(IN2);
    gpio_set_dir(IN2, GPIO_OUT);
    // Motor 2
    gpio_init(IN3_pwm);
    gpio_set_dir(IN3_pwm, GPIO_OUT);
    gpio_init(IN4);
    gpio_set_dir(IN4, GPIO_OUT);

    // Heartbeat
    const uint HEART_BEAT = 25;
    gpio_init(HEART_BEAT);
    gpio_set_dir(HEART_BEAT, GPIO_OUT);

    printf("Hello!\n");
    pwm_pin_init();
    generate_gain_curves();

    while (1) {
        int position;
        gpio_put(HEART_BEAT, 1);
        printf("Enter graph position from 0 to 100: ");
        scanf("%d", &position);
        // printf("\r\nPosition: %d\r\n", position);
        // set_speed(IN1_pwm, position);
        // set_speed(IN3_pwm, position);
        set_from_gain(position);
        sleep_ms(200);
        gpio_put(HEART_BEAT, 0);
        sleep_ms(200);
    }
}
