#include "tcs34725.h"
#include "lib/i2c.h"
#include "lib/uart.h"
#include "lib/io.h"
#define SAMPLING_PERIOD     100     //us                            
#define BLINK_SPEED_BASE    1000    //ms
                            
#define delay_us(us)        timer_wait_us(_TIM3, us)

tcs34725IntegrationTime_t _tcs34725IntegrationTime = TCS34725_INTEGRATIONTIME_700MS;
tcs34725Gain_t _tcs34725Gain = TCS34725_GAIN_1X;
void write8(uint8_t reg, uint8_t value) {
    uint8_t data[2];
    data[0] = TCS34725_COMMAND_BIT | reg;
    data[1] = value & 0xFF;

    if (i2c_write(I2C1, TCS34725_ADDRESS, data, sizeof(data)) != I2C_OK) {
        uart_puts(_USART2,"\n\r Erreur lors de l'écriture dans I2C\n");
        return;
    }
}

uint8_t read8(uint8_t reg) {
    uint8_t value;

    uint8_t reg_data[1] = { TCS34725_COMMAND_BIT | reg };

    if (i2c_write(I2C1, TCS34725_ADDRESS, reg_data, 1) != I2C_OK) {
        uart_puts(_USART2,"\n\rErreur lors de l'écriture dans le registre\n");
        return 0; 
    }

    if (i2c_read(I2C1, TCS34725_ADDRESS, &value, 1) != I2C_OK) {
        uart_puts(_USART2,"\n\rErreur lors de la lecture d'octets\n");
        return 0; 
    }

    return value;
}

uint16_t read16(uint8_t reg) {
    uint16_t x;

    uint8_t reg_data[1] = { TCS34725_COMMAND_BIT | reg };
    if (i2c_write(I2C1, TCS34725_ADDRESS, reg_data, 1) != I2C_OK) {
        uart_puts(_USART2,"\n\rError during register write\n");
        return 0; 
    }

    uint8_t data[2];
    if (i2c_read(I2C1, TCS34725_ADDRESS, data, 2) != I2C_OK) {
        uart_puts(_USART2,"\n\rErreur lors de la lecture d'octets\n");
        return 0; 
    }

    x = data[1];
    x <<= 8;
    x |= data[0];

    return x;
}

void enable(void) {
    write8(TCS34725_ENABLE, TCS34725_ENABLE_PON);

    delay_us(SAMPLING_PERIOD); 

    write8(TCS34725_ENABLE, TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN);
}

void setIntegrationTime(tcs34725IntegrationTime_t it) {
    write8(TCS34725_ATIME, it);
}

void setGain(tcs34725Gain_t gain) {
    write8(TCS34725_CONTROL, gain);
    _tcs34725Gain = gain;
}

void tcs34725_init(void) {
    if (i2c_master_init(I2C1) != I2C_OK) {
        uart_puts(_USART2, "\n\rErreur: Communication I2C non initialisée\n");
        return;
    }

    uint8_t id = read8(TCS34725_ID);
    if ((id != 0x44) && (id != 0x10)) {
        uart_puts(_USART2, "\n\rErreur: ID Incorrect\n");
        return;
    }

    tcs34725_config_t config = {
        .enable = TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN,
        .integration_time = TCS34725_INTEGRATIONTIME_700MS,
        .gain = TCS34725_GAIN_1X
    };

    write8(TCS34725_ENABLE, config.enable);
    write8(TCS34725_ATIME, config.integration_time);
    write8(TCS34725_CONTROL, config.gain);

   // delay_us(SAMPLING_PERIOD);

    uart_puts(_USART2, "\n\rTCS34725 I2C Color Sensor bien initialisé\n");
    return;
}

void tcs34725_read_color(uint16_t *red, uint16_t *green, uint16_t *blue, uint16_t *clear) {
    *clear = read16(TCS34725_CDATAL);
    *red = read16(TCS34725_RDATAL);
    *green = read16(TCS34725_GDATAL);
    *blue = read16(TCS34725_BDATAL);

    uint8_t atime_reg = read8(TCS34725_ATIME);
    uint16_t integration_time = (256 - atime_reg) * 2.4;

    switch (integration_time) {
        case 2: // 2.4 ms
            delay_us(3000);
            break;
        case 24: // 24 ms
            delay_us(24000);
            break;
        case 50: // 50 ms
            delay_us(50000);
            break;
        case 101: // 101 ms
            delay_us(101000);
            break;
        case 154: // 154 ms
            delay_us(154000);
            break;
        case 700: // 700 ms
            delay_us(700000);
            break;
        default:
            break;
    }
}

uint16_t calculateColorTemperature(uint16_t r, uint16_t g, uint16_t b) {
	int X, Y, Z;
	int x, y;
	int XYZ_sum;
	
	X = (-14282 * (int)r + 154924 * (int)g - 95641 * (int)b) / 100000;
	Y = (-32466 * (int)r + 157837 * (int)g - 73191 * (int)b) / 100000;
	Z = (-68202 * (int)r + 77073 * (int)g + 56332 * (int)b) / 100000;

	
	XYZ_sum = X + Y + Z;
	if (XYZ_sum != 0) {
			x = (X * 10000) / XYZ_sum; 
			y = (Y * 10000) / XYZ_sum; 
	} else {
			x = 5000; // valeur par défaut
			y = 5000; // valeur par défaut
	}

	int x_int = x / 10000;
	int x_frac = x % 10000;
	int y_int = y / 10000;
	int y_frac = y % 10000;

    // Afficher les valeurs x et y
	uart_printf(_USART2, "\r\nx = %d.%d , y= %d.%d\r\n",x_int,x_frac,y_int,y_frac);

    // Calculer n
    int n = ((x * 10000) - (3320 * 10000)) / ((1858 * 10000) - (y * 10000));
    uart_printf(_USART2,"\r\n%d",n);

    // Calculer cct
    int cct = ((437 * n * n * n) + (3601 * n * n) + (6861 * n) + 5517);
    return (uint16_t)cct;
}
/*uint16_t calculateLux(uint16_t r, uint16_t g, uint16_t b) {
    float illuminance;

    illuminance = (-0.32466F * r) + (1.57837F * g) + (-0.73191F * b);

    return (uint16_t)illuminance;
}*/