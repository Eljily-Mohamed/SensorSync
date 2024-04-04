#include "TH02_dev.h"
#include "../lib/i2c.h"

// Define your I2C address for TH02
#define TH02_I2C_ADDRESS 0x40
#define LM75_REG_NONE	0x04
#define delay_ms(ms)        timer_wait_ms(_TIM3,us)

static uint8_t last_reg = LM75_REG_NONE;
// Function implementations
void TH02_dev_begin(void) {
    int result;
    // Initialize your I2C interface here
    if(i2c_master_init(I2C1) != I2C_OK){
        //send message via uart interface to say succces 
        uart_printf(_USART2,"Conexion Error");  
        return;   
    }
    result = i2c_write(I2C1, TH02_I2C_ADDRESS, 0x00, 1);
    if(result == I2C_OK){
          //send message via uart interface to say succces 
          uart_printf(_USART2,"\n\rStart sending\n");
    return;
    }else{
         //send message via uart interface to say succces 
         uart_printf(_USART2,"Conexion to Device failed");  
         return;  
    }
    return; 
}

int TH02_dev_ReadTemperature(void) {
    // Start a new temperature conversion
     uint16_t value; 
    if (last_reg!=CMD_MEASURE_TEMP) {
		last_reg = CMD_MEASURE_TEMP;
		TH02_dev_IIC_WriteCmd(CMD_MEASURE_TEMP);
	}else{
         // Read temperature data from sensor
         value = TH02_dev_IIC_ReadData(REG_DATA_H, 3);
    }
    uint8_t configure;
    configure = TH02_dev_IIC_ReadReg(REG_CONFIG);
    //send message via uart interface to say succces 
    uart_printf(_USART2,"value in configration = %d\n",configure); 
    // Wait until conversion is done
    //while (!TH02_dev_isAvailable());
    value = value >> 2;
    // Calculate temperature using formula
    int temperature = (value / 32) - 50;
    return temperature;
}

int TH02_dev_ReadHumidity(void) {
    // Start a new humidity conversion
    TH02_dev_IIC_WriteCmd(CMD_MEASURE_HUMI);

    // Wait until conversion is done
    while (!TH02_dev_isAvailable());

    // Read humidity data from sensor
    uint16_t value = TH02_dev_IIC_ReadData(REG_DATA_H, 3);
    value = value >> 4;

    // Calculate humidity using formula
    int humidity = (value / 16) - 24;

    return humidity;
}

uint8_t TH02_dev_isAvailable(void) {
    // Read status register to check if conversion is done
    //TH02_dev_IIC_WriteReg(REG_CONFIG, u8Cmd);
    uint8_t status = TH02_dev_IIC_ReadReg(REG_STATUS);
    uart_printf(_USART2,"value in status = %d\n",status); 
    if (status & STATUS_RDY_MASK) {
        return 1; // Ready
    } else {
        return 0; // Not ready yet
    }
}

// Helper functions for I2C communication
void TH02_dev_IIC_WriteCmd(uint8_t u8Cmd) {
    TH02_dev_IIC_WriteReg(REG_CONFIG, u8Cmd);
}


uint8_t TH02_dev_IIC_ReadReg(uint8_t u8Reg) {
    uint8_t data;
    // Send a register reading command
    TH02_dev_IIC_WriteCmd(u8Reg);
    // Read data from I2C
    i2c_read(I2C1,(TH02_I2C_ADDRESS | 1), &data, 1);
    return data;
}

void TH02_dev_IIC_WriteReg(uint8_t u8Reg, uint8_t u8Data) {
    uint8_t buf[2] = {u8Reg, u8Data};
    int result = i2c_write(I2C1, TH02_I2C_ADDRESS, buf, 2);
    if (result != I2C_OK) {
        // //send message via uart interface to say succces 
        // uart_printf(_USART2,"\n\rWrite Error\n");
    }else{
         //send message via uart interface to say succces 
        // uart_printf(_USART2,"\n\rWrite Success\n");
    }
}


uint16_t TH02_dev_IIC_ReadData(uint8_t u8Reg, uint8_t num) {
    uint16_t data = TH02_dev_IIC_ReadData2byte(u8Reg, num);
    return data;
}

uint16_t TH02_dev_IIC_ReadData2byte(uint8_t u8Reg, uint8_t num) {
    uint16_t TempData = 0;
    uint8_t tmpArray[num];

    TH02_dev_IIC_WriteCmd(u8Reg);

    i2c_read(I2C1,(TH02_I2C_ADDRESS | 1), tmpArray, num);

    TempData = (tmpArray[num - 2] << 8) | tmpArray[num - 1];
    return TempData;
}
