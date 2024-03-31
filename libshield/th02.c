#include "lib/i2c.h"

#include "libshield/th02.h"
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

void IIC_WriteReg(uint8_t u8Reg, uint8_t u8Data) {
    uint8_t data[2];
    data[0] = u8Reg;
    data[1] = u8Data;
    int result = i2c_write(I2C1, TH02_ADDRESS, data, 2);
    if (result != I2C_OK) {
        // Handle write error
        switch (result) {
            case I2C_ERROR:
                //printf("I2C error: general error occurred.\n");
                // Log or handle the error appropriately
                break;
            case I2C_BUSY:
                //printf("I2C error: bus is busy.\n");
                // Log or handle the error appropriately
                break;
            // Add more cases for other possible error codes if needed
            default:
                //printf("I2C error: unknown error occurred.\n");
                // Log or handle the error appropriately
                break;
        }
    }
}

uint8_t isAvailable() {
    uint8_t status = IIC_ReadReg(REG_STATUS);
    if (status & STATUS_RDY_MASK) {
        return 0; // ready
    } else {
        return 1; // not ready yet
    }
}

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

float TH02_ReadTemperature(void) {
    uint8_t config_data[1] = {REG_CONFIG};
    uint8_t temperature_data[3]; // Buffer to store temperature data

    // Send the address pointer write sequence (slave address + write)
    if (i2c_write(_I2C1, TH02_ADDRESS, config_data, 1) != I2C_OK) {
        // Handle write error
        uart_printf(_USART2, "Error writing address pointer to TH02 sensor.\n");
        return 0.0f; // Return an error value or handle the error appropriately
    }

    // Read the temperature data from the TH02 sensor
    if (i2c_read(_I2C1, TH02_ADDRESS, temperature_data, 3) != I2C_OK) {
        // Handle read error
        uart_printf(_USART2, "Error reading temperature data from TH02 sensor.\n");
        return 0.0f; // Return an error value or handle the error appropriately
    }

    // Combine the two bytes to form the raw temperature value
    int16_t raw_temp = (temperature_data[1] << 8) | temperature_data[2];
    uart_printf(_USART2, "Temperature: %d\n", raw_temp);
    // Convert raw temperature data to temperature value
    float temperature = raw_temp / 32.0f - 50.0f;

    // Print the temperature value
    uart_printf(_USART2, "Temperature: %f\n", temperature);

    return temperature;
}
