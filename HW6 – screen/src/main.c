// based on adafruit and sparkfun libraries

#include <stdio.h>
#include <string.h> // for memset
#include "pico/stdlib.h"

#include "hardware/adc.h"
#include "hardware/i2c.h"

#include "pico/binary_info.h"
#include "ssd1306.h"
#include "font.h"

bool SCREEN_ON = 0;

// Function prototypes
void drawChar(int x, int y, unsigned char c);
void drawString(int x, int y, char * m);

void drawChar(int x, int y, unsigned char letter) {
    for (int i = 0; i < 5; i++) {
        unsigned char c = ASCII[letter - 32][i];
        for (int j = 0; j < 8; j++) {
            // Checking each of 8 bits in letter
            unsigned char bit = c>>j & 0b1; 
            if(bit == 0b1) {
                ssd1306_drawPixel(x + i, y + j, 1);
            }
            else {
                ssd1306_drawPixel(x + i, y + j, 0);
            }
        }
    }
}

// String – 128x32 pixels -> 25x4 letters
void drawString(int x, int y, char * m) {
    for(int i = 0; m[i] != '\0'; i++) {
        drawChar(x + ((i%25) * 5), y + ((int)(i/25) * 8), m[i]);
    }
    ssd1306_update();
}

int main() {
    // Initialization
    stdio_init_all();

    // Print in SCREEN for debugging
    if(SCREEN_ON) {
        while (!stdio_usb_connected()) {
            sleep_ms(100);
        }
    }
    printf("Start!\n");

    // I2C SDA and SCL Pins
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C); // Be SDA
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C); // Be SDL   

    // Screen Initialization
    ssd1306_setup();    
    sleep_ms(250); // sleep so that data polling and register update don't collide

    // ADC Initialization
    adc_init(); // init the adc module
    adc_gpio_init(26); // set ADC0 pin to be adc input instead of GPIO
    adc_select_input(0); // select to read from ADC0

    // Heartbeat
    const uint HEART_BEAT = 15;
    gpio_init(HEART_BEAT);
    gpio_set_dir(HEART_BEAT, GPIO_OUT);

    char message[200];

    while (1) {
        // Blink gp25 heartbeat
        gpio_put(HEART_BEAT, 1);

        // ADC Voltage Read
        uint16_t result = adc_read();
        sprintf(message, "ADC Voltage: %d", result); // Don't include \r or \n

        // Drawing with time tracked
        unsigned int start = to_us_since_boot(get_absolute_time());
        drawString(1, 1, message);
        unsigned int stop = to_us_since_boot(get_absolute_time());

        unsigned int t = stop - start;

        // Frames per second
        sprintf(message, "FPS: %f", 1000000.0/t); 
        drawString(1, 24, message);

        gpio_put(HEART_BEAT, 0);
        sleep_ms(250);
    }
}