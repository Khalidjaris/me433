#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"
#include <math.h>

/*

   GPIO 16 (pin 21) MISO/spi0_rx-> SDO/SDO on bme280 board
   GPIO 17 (pin 22) Chip select -> CSB/!CS on bme280 board
   GPIO 18 (pin 24) SCK/spi0_sclk -> SCL/SCK on bme280 board
   GPIO 19 (pin 25) MOSI/spi0_tx -> SDA/SDI on bme280 board
   3.3v (pin 36) -> VCC on bme280 board
   GND (pin 38)  -> GND on bme280 board

*/

#define READ_BIT 0x80
#define SIN_FREQUENCY 2
#define TRI_FREQUENCY 1
#define NUMAMPS 1000

float sine_array[NUMAMPS];
float tri_array[NUMAMPS];
int32_t t_fine;

static inline void cs_select() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 0);  // Active low
    asm volatile("nop \n nop \n nop");
}

static inline void cs_deselect() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);
    asm volatile("nop \n nop \n nop");
}

// Reg = Voltage
// uint16
static void write_register(uint16_t v, uint8_t channel) {
    uint8_t buf[2];
    buf[0] = (channel<<7) | 0b0111<<4 | v>>6;
    buf[1] = v<<2;

    cs_select();
    spi_write_blocking(spi_default, buf, 2);
    cs_deselect();
    sleep_us(500);
}

void sine(float v) {
    int amp = (v/3.3) * 1024 / 2;
    for (int i = 0; i < NUMAMPS; i++) {
        float increment = (2 * M_PI) / NUMAMPS * SIN_FREQUENCY;
        sine_array[i]  = amp * sin(increment * i) + amp;
    }
}

void triangle(float v) {
    float step = 1.0/NUMAMPS;
    int amp = (v/3.3) * 1024 / 2;

    for (int i = 0; i < NUMAMPS; i++) {
        float j = i;
        // Segment 0 -> 1
        if (i < NUMAMPS/4) {
            tri_array[i] = amp + (j * step)*amp*4;
        } 
        // Segment 1 -> -1
        else if (i <= NUMAMPS*3/4 && i >= NUMAMPS/4) {
            tri_array[i] = amp*3 - (i * step)*amp*4;
        } 
        // Segment -1 -> 0
        else if (i >= NUMAMPS*3/4) {
            tri_array[i] = - (amp*3) + (i * step)*amp*4;
        }
        
        // Saturate values
        if (tri_array[i] >= 1024) {
            tri_array[i] = 1023;
        }
        else if (tri_array[i] < 0) {
            tri_array[i] = 0;
        }
    }
}

int main() {
    stdio_init_all();
    // This example will use SPI0 baud rate at 0.5MHz.
    spi_init(spi_default, 500 * 1000);
    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(PICO_DEFAULT_SPI_CSN_PIN);
    gpio_set_dir(PICO_DEFAULT_SPI_CSN_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);

    sine(3.3);
    triangle(3.3);

    while(1) {
        for (int i = 0; i < NUMAMPS; i++) {
            write_register((uint16_t) sine_array[i], 1);
            write_register((uint16_t) tri_array[i], 0);
        }
    }
}
