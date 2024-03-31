#include "th02.h"
#include "lib/i2c.h"


void IIC_WriteCmd(uint8_t u8Cmd) {
    uint8_t data[2];
    data[0] = u8Cmd;
    i2c_write(I2C1, TH02_ADDRESS, data, 1);
}

uint8_t IIC_ReadReg(uint8_t u8Reg) {
    // Send a register reading command
    IIC_WriteCmd(u8Reg);
    // Read data from the TH02 sensor
    uint8_t data;
    i2c_read(I2C1, TH02_ADDRESS, &data, 1); // Assuming reading one byte

    return data;
}

void IIC_WriteReg(uint8_t u8Reg, uint8_t u8Data){
    uint8_t data[2];
    data[0] = u8Reg;
    data[1] = u8Data;
    i2c_write(I2C1, TH02_ADDRESS, data, 2);
}

uint8_t isAvailable() {
    uint8_t status = IIC_ReadReg(REG_STATUS);
    if (status & STATUS_RDY_MASK) {
        return 0; // ready
    } else {
        return 1; // not ready yet
    }
}

void TH02_begin() {
    // Initialize I2C interface
    if (i2c_master_init(I2C1) != I2C_OK) {
        // Handle initialization error
        return;
    }
    // Now, you need to initialize the TH02 sensor
    // Write initialization sequence to configure TH02 sensor
    uint8_t config_data[2] = {REG_CONFIG, TH02_ADDRESS}; // Assuming u8Reg contains the configuration data
    if (i2c_write(I2C1, TH02_ADDRESS, config_data, 2) != I2C_OK) {
        // Handle write error
        return;
    }
}


float TH02_ReadTemperature(void) {
    uint8_t cmd = CMD_MEASURE_TEMP;
    uint8_t data[3];

    // Start a new temperature conversion
    i2c_write(_I2C1, TH02_ADDRESS, &cmd, 1);

    // Wait until conversion is done
    while (!isAvailable());

    // Read temperature data from TH02 sensor
    i2c_read(_I2C1, TH02_ADDRESS, data, 3);

    // Convert raw temperature data to temperature value
    uint16_t value = (data[0] << 8) | data[1];
    value = value >> 2;

    /*
        Formula:
        Temperature(C) = (Value/32) - 50
    */

    float temperature = (value / 32.0) - 50.0;

    return temperature;
}
