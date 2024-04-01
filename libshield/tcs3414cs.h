#ifndef TCS3414CS_H
#define TCS3414CS_H

#include <stdint.h>


#define TCS3414CS_ADDR          0x39 // Adresse du capteur TCS3414CS
#define TCS3414CS_REG_CONTROL   0x00 // Registre de contrôle
#define TCS3414CS_REG_TIMING     0x01  // Adresse du registre de temporisation (temps d'intégration/gain)
#define TCS3414CS_REG_RED       0x12 // Registre RED 12:low 13:high
#define TCS3414CS_REG_GREEN     0x10 // Registre GREEN 10:low 11:high
#define TCS3414CS_REG_BLUE      0x14 // Registre BLUE 14:low 15:high
#define TCS3414CS_REG_CLEAR     0x16 // Registre CLEAR 16:low 17:high

// Prototypes des fonctions pour l'initialisation et la lecture du capteur de couleur i2c
void tcs3414cs_init();
void tcs3414cs_read_color(uint16_t *red, uint16_t *green, uint16_t *blue, uint16_t *clear);

#endif /* TCS3414CS_H */
