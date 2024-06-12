#include "libshield/sht4x.h"
#include "lib/i2c.h"
#include "lib/timer.h"

#define delay_us(us)        timer_wait_us(_TIM3, us)

/* all measurement commands return T (CRC) RH (CRC) */
#define SHT4X_CMD_MEASURE_HPM 0xFD
#define SHT4X_CMD_MEASURE_LPM 0xE0
#define SHT4X_CMD_READ_SERIAL 0x89
#define SHT4X_CMD_DURATION_USEC 1000

#define SHT4X_ADDRESS 0x44

static uint8_t sht4x_cmd_measure = SHT4X_CMD_MEASURE_HPM;
static uint16_t sht4x_cmd_measure_delay_us = SHT4X_MEASUREMENT_DURATION_USEC;

int16_t write(uint8_t reg, uint8_t value) {
    uint8_t data[2];
    data[0] = reg;
    data[1] = value & 0xFF;
    return i2c_write(I2C1, SHT4X_ADDRESS, data, sizeof(data));
}

uint8_t read(uint8_t reg) {
    uint8_t value;
    uint8_t reg_data[1] = {reg};
    if (i2c_write(I2C1, SHT4X_ADDRESS, reg_data, 1) != I2C_OK) {
        uart_puts(_USART2,"\n\rErreur lors de l'écriture dans le registre\n");
        return 0; 
    }

    delay_us(sht4x_cmd_measure_delay_us);

    if (i2c_read(I2C1, SHT4X_ADDRESS, &value, 1) != I2C_OK) {
        uart_puts(_USART2,"\n\rErreur lors de la lecture d'octets\n");
        return 0; 
    }

    return value;
}

int16_t write_n(uint8_t reg, uint8_t* values, uint8_t length) {
    uint8_t data[length + 1];
    data[0] = reg;
    for (size_t i = 0; i < length; i++) {
        data[i + 1] = values[i];
    }
    return i2c_write(I2C1, SHT4X_ADDRESS, data, sizeof(data));
}

int16_t read_n(uint8_t* values, uint8_t length) {
    return i2c_read(I2C1, SHT4X_ADDRESS, values, length);
}

int16_t write_cmd(uint8_t* cmd){
     return i2c_write(I2C1, SHT4X_ADDRESS, cmd, 1);
}

int16_t sht4x_measure_blocking_read(float* temperature, float* humidity) {
    int16_t ret;
    ret = write_cmd(&sht4x_cmd_measure);
    if (ret)
        return ret;
    delay_us(sht4x_cmd_measure_delay_us);
    return sht4x_read_measurement(temperature, humidity);
}

int16_t sht4x_read_measurement(float* temperature, float* humidity) {
    uint16_t rawTemp, rawHumd;
    uint16_t words[2];
    int16_t ret = read_n(words,SENSIRION_NUM_WORDS(words));
    if (ret)
        return ret;
    /**
     * formulas for conversion of the sensor signals, optimized for fixed point
     * algebra:
     * Temperature = 175 * S_T / 65535 - 45
     * Relative Humidity = 125 * (S_RH / 65535) - 6
     */

    rawTemp = ((uint16_t) words[0] << 8) | words[1];
    rawHumd = ((uint16_t) words[2] << 8) | words[3];

    *temperature = (-45.0f + 175.0f * (rawTemp / 65535.0f));
    *humidity = (-6.0f + 125.0f * (rawHumd / 65535.0f)); 

    return ret;
}

int16_t sht4x_probe(void) {
    uint32_t serial;
    return sht4x_read_serial(&serial);
}

void sht4x_enable_low_power_mode(uint8_t enable_low_power_mode) {
    if (enable_low_power_mode) {
        sht4x_cmd_measure = SHT4X_CMD_MEASURE_LPM;
        sht4x_cmd_measure_delay_us = SHT4X_MEASUREMENT_DURATION_LPM_USEC;
    } else {
        sht4x_cmd_measure = SHT4X_CMD_MEASURE_HPM;
        sht4x_cmd_measure_delay_us = SHT4X_MEASUREMENT_DURATION_USEC;
    }
}

int16_t sht4x_read_serial(uint32_t* serial) {
    const uint8_t cmd = SHT4X_CMD_READ_SERIAL;
    int16_t ret;
    uint16_t serial_words[SENSIRION_NUM_WORDS(*serial)];
    ret = write_cmd(&sht4x_cmd_measure);
    if (ret)
        return ret;
    read_n((uint8_t*)serial_words,sizeof(serial_words));
    *serial = ((uint32_t)serial_words[0] << 16) | serial_words[1];
    return I2C_OK;
}

uint8_t sht4x_get_configured_address(void) {
    return SHT4X_ADDRESS;
}