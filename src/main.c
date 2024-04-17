#include "include/board.h"
#include "lib/io.h"
#include "lib/uart.h"
#include "lib/util.h"
#include "lib/i2c.h"
#include "lib/timer.h"
#include "libshield/sht4x.h"
#include "libshield/tcs34725.h"

#define MAIN_GLOBAL_INIT

#define TEST_IHM

#ifdef MAIN_GLOBAL_INIT

#define DELAY_1_SECOND      1000000
#define DELAY_10_SECONDS    10000000

#define delay_us(us)        timer_wait_us(_TIM3, us)

volatile char command;

static void on_command_received(char c) {
    command = c;
}

int main(void) {
    uart_init(_USART2, 115200, UART_8N1, on_command_received);
    i2c_master_init(_I2C1);
    lcd_reset();
    cls();
    //tcs34725_init();

    while (sht4x_probe() != 0) {
        uart_printf(_USART2, "SHT sensor probing failed\n");
        delay_us(DELAY_1_SECOND); // Sleep 1s
    }

    while (1) {
        #ifdef MAIN_GLOBAL_INIT
        //uart_printf(_USART2, "\r\nEntrez une commande : ");
        #endif

        while (!command);

        switch (command) {
            case 't': { // Get current temperature & humidity
                float temperature, humidity;
                int8_t ret = sht4x_measure_blocking_read(&temperature, &humidity);
                if (ret == 0) {
                    int temp_int = (int) temperature;
                    float temp_frac = temperature - temp_int;
                    int hum_int = (int) humidity;
                    float hum_frac = humidity - hum_int;

                    int temp_int_frac = (int)(temp_frac * 1000);
                    int hum_int_frac = (int)(hum_frac * 1000);

                    //test IHM
                    #ifndef TEST_IHM
                        uart_printf(_USART2, "\r\nMeasured temperature: %d.%d C", temp_int, temp_int_frac);
                        uart_printf(_USART2, "\r\nMeasured humidity: %d.%d C", hum_int, hum_int_frac);
                    #endif
                   

                    //test IHM
                    #ifdef TEST_IHM
                       uart_printf(_USART2, "\r\n%d.%d", temp_int, temp_int_frac);
                    #endif

                    delay_us(DELAY_1_SECOND); // Sleep 1s
                } else {
                    uart_printf(_USART2, "Error reading measurement\n");
                }
                break;
            }
            case 'c': {// Get current color
                uint16_t red, green, blue, clear, color_temp, lux;
                tcs34725_read_color(&red, &green, &blue, &clear);
                color_temp = calculateColorTemperature(red, green, blue);
                lux = calculateLux(red, green, blue);

                uart_printf(_USART2, "\r\nTemperature en Kelvin: %d\n", color_temp);
                uart_printf(_USART2, "\r\nIlluminance en Lux: %d\n", lux);
                delay_us(DELAY_10_SECONDS); // Sleep 10s
                break;
            }
        }
    }
    return 0;
}

#endif
