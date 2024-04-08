#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

int main() {
    stdio_init_all();

    printf("Start!\n");
 
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
    
    // LED
    const uint LED_PIN = 15;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Button
    const uint BUT_PIN = 16;
    gpio_init(BUT_PIN);
    gpio_set_dir(BUT_PIN, GPIO_IN);

    // ADC
    adc_init(); // init the adc module
    adc_gpio_init(26); // set ADC0 pin to be adc input instead of GPIO
    adc_select_input(0); // select to read from ADC0

    printf("Start!\n");

    while (1) {
        gpio_put(LED_PIN, 1);
        while (gpio_get(BUT_PIN) == 0) {}
        gpio_put(LED_PIN, 0);

        int samples;
        printf("Enter a number of analog samples to take: ");
        scanf("%d", &samples);
        printf("\r\nSamples: %d\r\n", samples);
        sleep_ms(2000);

        for(int i = 0; i < samples; i++) {
            uint16_t result = adc_read();
            printf("Sample %d: %d\r\n", i+1, result);
            sleep_ms(10);
        }
        sleep_ms(50);
    }
}