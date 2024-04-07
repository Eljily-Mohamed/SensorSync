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

int16_t sht4x_measure_blocking_read(float* temperature, float* humidity) {
    int16_t ret;

    ret = sht4x_measure();
    if (ret)
        return ret;
    delay_us(sht4x_cmd_measure_delay_us);
    return sht4x_read(temperature, humidity);
}

int16_t sht4x_measure(void) {
    return i2c_write(I2C1,SHT4X_ADDRESS, &sht4x_cmd_measure, 1);
}

int16_t sht4x_read(float* temperature, float* humidity) {
    uint16_t rawTemp, rawHumd;
    uint16_t words[2];
    int16_t ret = i2c_read(I2C1,SHT4X_ADDRESS,words,SENSIRION_NUM_WORDS(words));

    /**
     * formulas for conversion of the sensor signals, optimized for fixed point
     * algebra:
     * Temperature = 175 * S_T / 65535 - 45
     * Relative Humidity = 125 * (S_RH / 65535) - 6
     */

    rawTemp = ((uint16_t) words[0] << 8) | words[1];
    rawHumd = ((uint16_t) words[2] << 8) | words[3];

    *temperature = (-45.0f + 175.0f * (rawTemp / 65535.0f));
    *humidity = (-6.0f + 125.0f * (rawHumd / 65535.0f)); // corrected formula

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

    ret = i2c_write(I2C1,SHT4X_ADDRESS, &cmd, 1);
    if (ret)
        return ret;

    delay_us(SHT4X_CMD_DURATION_USEC);
    ret = i2c_read(I2C1,SHT4X_ADDRESS, serial_words,SENSIRION_NUM_WORDS(serial_words));
    *serial = ((uint32_t)serial_words[0] << 16) | serial_words[1];

    return ret;
}


uint8_t sht4x_get_configured_address(void) {
    return SHT4X_ADDRESS;
}