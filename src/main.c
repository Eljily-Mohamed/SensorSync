#include "include/board.h"
#include "lib/io.h"
#include "lib/uart.h"
#include "lib/util.h"
#include "lib/i2c.h"
#include "lib/timer.h"

#define TEMP_HUMIDITYSENSOR
#define delay_us(us)     timer_wait_ms(_TIM3, us)


volatile char cmd;

#ifdef TEMP_HUMIDITYSENSOR

// #define delay_us(us)        timer_wait_us(_TIM3,us,NULL)

static void on_rx_cb(char c) {
    cmd = c;
}

int main(void) {
    /* Initialize the i2c bus for the current platform */
	uart_init(_USART2, 115200, UART_8N1, on_rx_cb);
	i2c_master_init(_I2C1);

    /* Busy loop for initialization, because the main loop does not work without
     * a sensor.
    */

    while (sht4x_probe() != STATUS_OK) {
        printf("SHT sensor probing failed\n");
        sensirion_sleep_usec(1000000); /* sleep 1s */
    }
    printf("SHT sensor probing successful\n");

    while (1) {
        int32_t temperature, humidity;
        /* Measure temperature and relative humidity and store into variables
         * temperature, humidity (each output multiplied by 1000).
         */
        int8_t ret = sht4x_measure_blocking_read(&temperature, &humidity);
        if (ret == STATUS_OK) {
            printf("measured temperature: %d degreeCelsius, "
                   "measured humidity: %d percentRH\n",
                   temperature / 1000, humidity / 1000);
        } else {
            printf("error reading measurement\n");
        }

        delay_us(1000000); /* sleep 1s */
    }
    return 0;
}

#endif

#ifdef COLORSENSOR
#include "libshield/tcs3414cs.h"
static void on_rx_cb(char c)
{
    cmd = c;
}

int main(void)
{
	// Variables pour stocker les valeurs de couleur
    uint16_t red, green, blue, clear;
	uart_init(_USART2, 115200, UART_8N1, on_rx_cb);
	i2c_master_init(_I2C1);
	tcs3414cs_init();

	
	// Lecture des valeurs de couleur
	tcs3414cs_read_color(&red, &green, &blue, &clear);
	uart_printf(_USART2,"\n\rValeur rouge: %u\n", red);
	uart_printf(_USART2,"\n\rValeur verte: %u\n", green);
	uart_printf(_USART2,"\n\rValeur bleue: %u\n", blue);
	uart_printf(_USART2,"\n\rValeur claire: %u\n", clear);
	while(1) {}
	return 0;
}
#endif