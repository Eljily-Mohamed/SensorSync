#include "lib/i2c.h"
#include "libshield/th02.h"

#define delay_ms(ms)        timer_wait_ms(_TIM3, ms)
//#define TH02_REG_NONE		(0x04)
//static uint8_t last_reg = TH02_REG_NONE;

void th02_begin() {
    // Initialize I2C interface
    if (i2c_master_init(I2C1) != I2C_OK) {
        uart_puts(_USART2,"Erreur: Communication I2C non initialisé\n");
        return;
    } 
    // Write initialization sequence to configure TH02 sensor
    uint8_t config_data[2] = {REG_CONFIG, TH02_ADDRESS}; // Assuming u8Reg contains the configuration data
    if (i2c_write(I2C1, TH02_ADDRESS, config_data, 2) != I2C_OK) {
        // Erreur d'écriture i2c
        uart_puts(_USART2,"Erreur: Impossible d'écrire au capteur\n");
        return;
    }
    // Communication réussie
    uart_puts(_USART2,"I2C humidity and temperature sensor bien initialisé et connecté\n");
    return;
}



// Function to read temperature from TH02 sensor
int th02_read_temp(int *temp) {
    // Set configuration for temperature measurement
    if (i2c_write(I2C1, TH02_ADDRESS,REG_CONFIG, 1) != I2C_OK) {
        return -1; // Error writing to configuration register
    }
    if (i2c_write(I2C1, TH02_ADDRESS,CMD_MEASURE_TEMP, 1) != I2C_OK) {
        return -1; // Error writing configuration data
    }

    // Poll RDY bit in STATUS register until it becomes low
    uint8_t status;
    do {
        if (i2c_read(I2C1, TH02_ADDRESS,REG_STATUS, 1) != I2C_OK) {
            return -1; // Error reading status register
        }
        status &= 0x01; // Mask RDY bit
    } while (status != 0);

    // Read temperature data
    uint8_t temp_data[2];
    if (i2c_read(I2C1, TH02_ADDRESS,REG_DATA_H,2) != I2C_OK) {
        return -1; // Error reading temperature data
    }
    
    // Convert temperature data to Celsius
    uart_printf(_USART2,"\n\rLa température est %d",temp_data[0]);
    uart_printf(_USART2,"\n\rLa température est %d",temp_data[1]);
    int16_t raw_temp = (temp_data[0] << 8) | temp_data[1];
    *temp = (int)((raw_temp / 32.0) - 50.0);

    return 0; // Success
}