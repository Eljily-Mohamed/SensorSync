#include "lib/i2c.h"
#include "libshield/th02.h"

#define delay_us(us)        timer_wait_us(_TIM3, us)


void th02_begin() {
    // Initialize I2C interface
    if (i2c_master_init(I2C1) != I2C_OK) {
        // Handle initialization error
        return;
    } 
    // Write initialization sequence to configure TH02 sensor
    uint8_t config_data[2] = {REG_CONFIG, TH02_ADDRESS}; // Assuming u8Reg contains the configuration data
    if (i2c_write(I2C1, TH02_ADDRESS, config_data, 2) != I2C_OK) {
        // Handle write error
        return;
    }
    // Configuration successful
    return;
}


int th02_read_temp(int *temp) {
    // I2C configuration
    if (i2c_master_init(I2C1) != I2C_OK) {
        // Handle I2C initialization error
        return -1;
    }

    // Start sequence
    uint8_t config_data[2] = {REG_CONFIG, 0x11}; // Write CONFIG register (0x03) with 0x11
    if (i2c_write(I2C1, TH02_ADDRESS, config_data, 2) != I2C_OK) {
        // Handle I2C write error
        return -1;
    }

    // Delay for temperature conversion
    delay_us(35); 

    // Read sequence
    uint8_t data_buffer[2];
    if (i2c_read(I2C1, TH02_ADDRESS, data_buffer, 2) != I2C_OK) {
        // Handle I2C read error
        return -1;
    }

    // Extract temperature value and convert to °C
    uint16_t value = ((data_buffer[1] << 8) | data_buffer[0]) >> 4;
    *temp = (value / 32) - 50;

    return 0; // Success
}