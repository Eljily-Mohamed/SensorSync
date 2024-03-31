#include "lib/i2c.h"

#include "libshield/th02.h"

int TH02_begin() {
    // Initialize I2C interface
    if (i2c_master_init(I2C1) != I2C_OK) {
        // Handle initialization error
        return -1;
    } 
    // Write initialization sequence to configure TH02 sensor
    uint8_t config_data[2] = {REG_CONFIG, TH02_ADDRESS}; // Assuming u8Reg contains the configuration data
    if (i2c_write(I2C1, TH02_ADDRESS, config_data, 2) != I2C_OK) {
        // Handle write error
        return -1;
    }
    // Configuration successful
    return 1;
}
