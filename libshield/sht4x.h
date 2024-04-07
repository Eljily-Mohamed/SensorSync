#ifndef SHT4X_H
#define SHT4X_H

#include <stdint.h>

#define STATUS_OK 0
#define STATUS_ERR_BAD_DATA (-1)
#define STATUS_CRC_FAIL (-2)
#define STATUS_UNKNOWN_DEVICE (-3)
#define SHT4X_MEASUREMENT_DURATION_USEC 10000 /* 10ms "high repeatability" */
#define SHT4X_MEASUREMENT_DURATION_LPM_USEC 2500 /* 2.5ms "low repeatability" */    
#define SENSIRION_COMMAND_SIZE 2
#define SENSIRION_WORD_SIZE 2
#define SENSIRION_NUM_WORDS(x) (sizeof(x) / SENSIRION_WORD_SIZE)
#define SENSIRION_MAX_BUFFER_WORDS 32

/**
 * Detects if a sensor is connected by reading out the ID register.
 * If the sensor does not answer or if the answer is not the expected value,
 * the test fails.
 *
 * @return 0 if a sensor was detected
 */
int16_t sht4x_probe(void);

/**
 * 
 * Starts a measurement and then reads out the results. This function blocks
 * while the measurement is in progress. The duration of the measurement depends
 * on the sensor in use, please consult the datasheet.
 * Temperature is returned in [degree Celsius], multiplied by 1000,
 * and relative humidity in [percent relative humidity], multiplied by 1000.
 *
 * @param temperature   the address for the result of the temperature
 * measurement
 * @param humidity      the address for the result of the relative humidity
 * measurement
 * @return              0 if the command was successful, else an error code.
 */
int16_t sht4x_measure_blocking_read(int32_t* temperature, int32_t* humidity);

/**
 * Starts a measurement in high precision mode. Use sht4x_read() to read out the
 * values, once the measurement is done. The duration of the measurement depends
 * on the sensor in use, please consult the datasheet.
 *
 * @return     0 if the command was successful, else an error code.
 */
int16_t sht4x_measure(void);

/**
 * Reads out the results of a measurement that was previously started by
 * sht4x_measure(). If the measurement is still in progress, this function
 * returns an error.
 * Temperature is returned in [degree Celsius], multiplied by 1000,
 * and relative humidity in [percent relative humidity], multiplied by 1000.
 *
 * @param temperature   the address for the result of the temperature
 * measurement
 * @param humidity      the address for the result of the relative humidity
 * measurement
 * @return              0 if the command was successful, else an error code.
 */
int16_t sht4x_read(int32_t* temperature, int32_t* humidity);

/**
 * Enable or disable the SHT's low power mode
 *
 * @param enable_low_power_mode 1 to enable low power mode, 0 to disable
 */
void sht4x_enable_low_power_mode(uint8_t enable_low_power_mode);

/**
 * Read out the serial number
 *
 * @param serial    the address for the result of the serial number
 * @return          0 if the command was successful, else an error code.
 */
int16_t sht4x_read_serial(uint32_t* serial);

/**
 * Return the driver version
 *
 * @return Driver version string
 */
const char* sht4x_get_driver_version(void);

/**
 * Returns the configured SHT4x address.
 *
 * @return SHT4x_ADDRESS
 */
uint8_t sht4x_get_configured_address(void);


#endif /* SHT4X_H */