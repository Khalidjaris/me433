#include <stdio.h>

#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"

 /* Example code to talk to a BMP280 temperature and pressure sensor

    GPIO PICO_DEFAULT_I2C_SDA_PIN (on Pico this is GP4 (pin 6)) -> SDA on BMP280
    board
    GPIO PICO_DEFAULT_I2C_SCK_PIN (on Pico this is GP5 (pin 7)) -> SCL on
    BMP280 board
    3.3v (pin 36) -> VCC on BMP280 board
    GND (pin 38)  -> GND on BMP280 board
 */

 // device has default bus address of 0x76

// Address (needs to be at least 7 bits)
#define ADDR _u(0b0100000) 
// hardware registers
#define REG_IODIR _u(0x00) // Setting as input or output (TRIS)
#define REG_GPIO _u(0x09) // Input on/off (PORT)
#define REG_OLAT _u(0x0A) // Output high/low (LAT)

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
    uint8_t buf[6];
    uint8_t reg = REG_GPIO;
    i2c_write_blocking(i2c_default, ADDR, &reg, 1, true);
    return i2c_read_blocking(i2c_default, ADDR, buf, 6, false); 
}

int main() {
    stdio_init_all();
    // I2C is "open drain", pull ups to keep signal high when no data is being sent
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C); // Be SDA
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C); // Be SDL

    // configure BMP280
    chip_init();

    while (1) {
        // Blink gp25 heartbeat (debugging – tell you code is running)
        char g = read(); // read gp0
        if(read() == 0) {
            set(1); // turn on gp7 (function set(0))
        }
        else {
            set(0); // turn off (function set(1))
            // delay
        }
    }
}
