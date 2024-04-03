#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#define BUT_PIN 16

int main() {
    stdio_init_all();
    
    // LED
    #ifndef PICO_DEFAULT_LED_PIN
    #warning blink example requires a board with a regular LED
    #else
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 1);

    // Button
    gpio_init(BUT_PIN);
    gpio_set_dir(BUT_PIN, GPIO_IN);

    // ADC
    adc_init(); // init the adc module
    adc_gpio_init(26); // set ADC0 pin to be adc input instead of GPIO
    adc_select_input(0); // select to read from ADC0

    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
    printf("Start!\n");
 
    while (1) {

        while (true) {
            
        }

        while (true) {
            gpio_put(LED_PIN, 0);
            int samples = 0;
            scanf("Enter a number of analog samples to take: %d\r\n", samples);
            for(int i = 0; i < samples; i++) {
                uint16_t result = adc_read();
                printf("Sample %d: %d\r\n", i+1, result);
                sleep_ms(1);

            }
        }
    }
}