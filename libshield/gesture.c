#include "gesture.h"

#include <unistd.h>

void paj7620Init() {
    // Initialize I2C interface
    i2c_master_init(I2C1); // Assuming you're using I2C1

    // Select bank 0
    uint8_t buf[2] = {PAJ7620_REGITER_BANK_SEL, PAJ7620_BANK0};
    i2c_write(I2C1, PAJ7620_ID, buf, 2);

    // Check device ID
    uint8_t data;
    i2c_read(I2C1, PAJ7620_ID, &data, 1);
    if (data != PAJ7620_ID) {
        // Error with sensor
        uart_puts(_USART2,"\n\r Error with sensor");
        // return;
    }

    // Initialize registers
    for (int i = 0; i < sizeof(initRegisterArray) / sizeof(initRegisterArray[0]); i++) {
        buf[0] = initRegisterArray[i][0];
        buf[1] = initRegisterArray[i][1];
        i2c_write(I2C1, PAJ7620_ID, buf, 2);
    }

    // Select bank 0
    buf[0] = PAJ7620_REGITER_BANK_SEL;
    buf[1] = PAJ7620_BANK0;
    i2c_write(I2C1, PAJ7620_ID, buf, 2);
}

void paj7620WriteReg(uint8_t addr, uint8_t cmd) {
    uint8_t buf[2] = {addr, cmd};
    i2c_write(I2C1, PAJ7620_ID, buf, 2);
    return;
}

uint8_t* paj7620ReadReg(uint8_t addr, uint8_t qty) {
    uint8_t buf[2] = {addr, qty};
    int result;
    result = i2c_read(I2C1, PAJ7620_I2C_ADDR, buf, 2);
    return data;
}

// Function definitions
void paj7620SelectBank(uint8_t bank) {
    // 
}