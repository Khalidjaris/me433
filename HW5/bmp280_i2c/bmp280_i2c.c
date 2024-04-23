#include <stdio.h>
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"

// Address (needs to be at least 7 bits)
#define ADDR _u(0b0100000) 
// hardware registers
#define REG_IODIR _u(0x00) // Setting as input or output (TRIS)
#define REG_GPIO _u(0x09) // Input on/off (PORT)
#define REG_OLAT _u(0x0A) // Output high/low (LAT)

void chip_init();
void set(uint8_t s);
int read();

// Changed the name of this function
void chip_init() {
    uint8_t buf[2];
    // send register number followed by its corresponding value
    buf[0] = REG_IODIR;
    buf[1] = 0b01111111;
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
}

void set(uint8_t s) {
    uint8_t buf[2];
    // send register number followed by its corresponding value
    buf[0] = REG_OLAT;
    buf[1] = s<<7;
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
}

int read() {
    uint8_t buf[1];
    uint8_t reg = REG_GPIO;
    i2c_write_blocking(i2c_default, ADDR, &reg, 1, true);
    i2c_read_blocking(i2c_default, ADDR, buf, 6, false); 
    if (buf[0] & 0b1 == 0b1) {
        return 1;
    }
    else {
        return 0;
    }
}

int main() {
    stdio_init_all();

    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C); // Be SDA
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C); // Be SDL
    
    // configure BMP280
    chip_init();

    // Heartbeat
    const uint HEART_BEAT = 25;
    gpio_init(HEART_BEAT);
    gpio_set_dir(HEART_BEAT, GPIO_OUT);

    while (1) {
        // Blink gp25 heartbeat (debugging – tell you code is running)
        gpio_put(HEART_BEAT, 1);
        char g = read(); // read gp0
        if(g == 1) {
            set(0); // turn on gp7 (function set(0))
        }
        else {
            set(1); // turn off (function set(1))
        }
        gpio_put(HEART_BEAT, 0);
        sleep_ms(100);
    }
}
