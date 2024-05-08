#include "include/board.h"
#include "lib/io.h"
#include "lib/uart.h"
#include "lib/util.h"
#include "lib/i2c.h"
#include "lib/timer.h"
#include "libshield/sht4x.h"
#include "libshield/tcs34725.h"
#include "constants.h"

volatile char command;

static void on_command_received(char c) {
    command = c;
}

static void on_zegbee_command_received(char c) {
    command = c;
}

int main(void) {
    uart_init(_USART2, 115200, UART_8N1, on_command_received);
    uart_init(_USART1, 115200, UART_8N1, on_zegbee_command_received);
    i2c_master_init(_I2C1);
    lcd_reset();
    cls();
    
    tcs34725_init();

    while (sht4x_probe() != 0) {
        uart_printf(UART_TO_USE, "SHT sensor probing failed\n");
        delay_us(DELAY_1_SECOND); // Sleep 1s
    }

    while (1) {
        #ifndef TEST_IHM
            uart_printf(UART_TO_USE, "\r\nEntrez une commande : ");
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

                    uart_printf(UART_TO_USE, "\r\ntemperature: %d.%d C", temp_int, temp_int_frac);
                    uart_printf(UART_TO_USE, "\r\nhumidity: %d.%d", hum_int, hum_int_frac);

                    delay_us(DELAY_1_SECOND); // Sleep 1s

                } else {
                    uart_printf(UART_TO_USE, "Error reading measurement\n");
                }
                break;
            }
            case 'c': {// Get current color
                uint16_t red, green, blue, clear, color_temp, lux;
                float x, y;

                tcs34725_read_color(&red, &green, &blue, &clear);
                color_temp = calculateColorTemperature(red, green, blue, &x, &y);
                lux = calculateLux(red, green, blue);

                
                // Convertir les coordonnées chromatiques en parties entières et décimales
                int x_int =(int)x;
                float x_frac = x-x_int;
                int x_frac_int = (int)(x_frac * 1000);
                int y_int = (int)y;
                float y_frac = y-y_int;
                int y_frac_int= (int)(y_frac*1000);

                #ifndef TEST_IHM
                // Afficher (x,y) avec illuminance (color + brightness  )
                uart_printf(UART_TO_USE, "\r\nx = %d.%d , y=%d.%d, illuminance=%d\r\n", x_int, x_frac_int, y_int, y_frac_int,lux);
                uart_printf(UART_TO_USE, "\r\nTemperature en Kelvin: %d\n", color_temp);
                #else
                // Display x, y without illuminance
                uart_printf(UART_TO_USE, "\r\n%d.%d,%d.%d,%d", x_int, x_frac_int, y_int, y_frac_int, lux);
                #endif

                delay_us(DELAY_1_SECOND); // Sleep 1s

                break;
            }
        }
    }
    return 0;
}
