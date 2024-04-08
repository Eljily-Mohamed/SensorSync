#include "include/board.h"
#include "lib/io.h"
#include "lib/uart.h"
#include "lib/util.h"
#include "lib/i2c.h"
#include "lib/timer.h"

#define COLORSENSOR
#define delay_us(us)        timer_wait_us(_TIM3, us)


volatile char cmd;

#ifdef TEMP_HUMIDITYSENSOR

#include "libshield/sht4x.h"

static void on_rx_cb(char c) {
    cmd = c;
}

int main(void) {
    /* Initialize the i2c bus for the current platform */
	uart_init(_USART2, 115200, UART_8N1, on_rx_cb);
	i2c_master_init(_I2C1);
    lcd_reset();

    // /* Busy loop for initialization, because the main loop does not work without
    //  * a sensor.
    // */

    while (sht4x_probe() != 0) {
        uart_printf(_USART2,"SHT sensor probing failed\n");
		delay_us(1000000); /* sleep 1s */
    }
    uart_printf(_USART2,"SHT sensor probing successful\n");

    while (1) {
        float temperature, humidity;
        cls();
        /* Measure temperature and relative humidity and store into variables
         * temperature, humidity (each output multiplied by 1000).
         */
        int8_t ret = sht4x_measure_blocking_read(&temperature, &humidity);
        if (ret == 0) {
            /////////////////////////////////////
            int partieEntiere = (int) temperature;
            float partieFractionnaire = temperature - partieEntiere;
            //convertor partie Fractionnaire en entiere
            int partieFractionnaireEntier = (int) (partieFractionnaire * 1000);
            /////////////////////////////////////////////
             int partieEntiereH = (int) humidity;
            float partieFractionnaireH = humidity - partieEntiereH;
            //convertor partie Fractionnaire en entiere
            int partieFractionnaireEntierH = (int) (partieFractionnaireH * 1000);
            ///////////////////////////////////////////////
          
            lcd_printf("Temperature %d.%d\n",partieEntiere,partieFractionnaireEntier);
            lcd_printf("Humidity %d.%d\n",partieEntiereH,partieFractionnaireEntierH);
            uart_printf(_USART2,"\r\nmeasured temperature: %d.%d C",partieEntiere,partieFractionnaireEntier);
            uart_printf(_USART2,"\r\nmeasured Humidity: %d.%d C",partieEntiereH,partieFractionnaireEntierH);
            delay_us(10000000); /* sleep 1s */
            //uart_printf(_USART2,"\r\nmeasured humidity: %d RH \n",humidity / 1000);
        } else {
            uart_printf(_USART2,"error reading measurement\n");
        }

    }
    return 0;
}

#endif

#ifdef COLORSENSOR
#include "libshield/tcs34725.h"
#include "math.h"

static void on_rx_cb(char c)
{
    cmd = c;
}

int main(void) {
	uart_init(_USART2, 115200, UART_8N1, on_rx_cb);
	i2c_master_init(_I2C1);
	tcs34725_init();
	while (1) {

		uint16_t r, g, b, c, colorTemp ,lux;

		tcs34725_read_color(&r, &g, &b, &c);
		colorTemp=calculateColorTemperature(r,g,b);
        lux=calculateLux(r,g,b);
		uart_printf(_USART2,"\r\nTemperature en Kelvin: %d\n", colorTemp);
        uart_printf(_USART2,"\r\nIlluminance en Lux: %d\n",lux);
        delay_us(10000000);
	}

    return 0;
}
#endif