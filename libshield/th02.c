#include "lib/i2c.h"
#include "libshield/th02.h"

#define delay_ms(ms)        timer_wait_ms(_TIM3, ms)
//#define TH02_REG_NONE		(0x04)
//static uint8_t last_reg = TH02_REG_NONE;

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
    // Start sequence
    uint8_t buffer [2];
    int st;
    uint8_t config_data[2] = {REG_CONFIG,CMD_MEASURE_TEMP}; // Write CONFIG register (0x03) with 0x11
    st = i2c_write(I2C1, TH02_ADDRESS, config_data, 2);
    if(st==I2C_ERROR) return -1;
      
    delay_ms(40);

    // Read 2 bytes of data
    st = i2c_read(I2C1, TH02_ADDRESS,buffer, 2);
    if(st==I2C_ERROR) return -1;
    uint16_t value = ((buffer[1] << 8) | buffer[0]) >> 2;
        *temp = (value / 32) - 50;
	return st;
}