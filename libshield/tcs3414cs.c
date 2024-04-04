#include "tcs3414cs.h"
#include "lib/i2c.h"
#include "lib/uart.h"

// Initialisation du capteur de couleur I2C TCS3414CS
void tcs3414cs_init(){
    // initialisation de l'interface i2c
    if (i2c_master_init(I2C1) != I2C_OK) {
        // erreurs d'initialisation
        uart_puts(_USART2,"Erreur: Communication I2C non initialisé\n");
        return;
    } 
    // Ecriture du byte de configuration pour régler le temps d'intégration
    uint8_t control_byte = TCS3414CS_REG_CONTROL; 

    if (i2c_write(I2C1, TCS3414CS_ADDR,control_byte, 1) != I2C_OK) {
        // Erreur d'écriture i2c
        uart_puts(_USART2,"Erreur: Impossible d'écrire au capteur\n");
        return;
    }

    // Communication réussie
    uart_puts(_USART2,"I2C color sensor bien initialisé et connecté\n");
}


// Lecture des valeurs de couleur du capteur TCS3414CS
void tcs3414cs_read_color(uint16_t *red, uint16_t *green, uint16_t *blue, uint16_t *clear){

    uint8_t data[8];  // Allocation d'espace pour les valeurs rouge, verte, bleue 

    // Démarrage de la communication I2C
    i2c_master_init(I2C1);
    // Envoi de l'adresse du capteur en écriture
    i2c_write(I2C1,TCS3414CS_ADDR << 1, NULL, 0);

    // Lecture des données couleur (rouge, vert, bleu, clair)
    i2c_write(I2C1,TCS3414CS_REG_RED, NULL, 0);
    
    i2c_write(I2C1,(TCS3414CS_ADDR << 1) | 0x01, NULL, 0); // Envoi de l'adresse du capteur en lecture
    i2c_read(I2C1, (TCS3414CS_ADDR << 1) | 0x01,data, 1); // Lecture du registre de données bas
    i2c_read(I2C1, (TCS3414CS_ADDR << 1) | 0x01,data + 1, 1); // Lecture du registre de données haut
    *red = (data[1] << 8) | data[0]; // Combinaison des données lues

    i2c_write(I2C1,TCS3414CS_REG_GREEN, NULL, 0);
    
    i2c_write(I2C1,(TCS3414CS_ADDR << 1) | 0x01, NULL, 0);
    i2c_read(I2C1, (TCS3414CS_ADDR << 1) | 0x01,data + 2, 1);
    i2c_read(I2C1, (TCS3414CS_ADDR << 1) | 0x01,data + 3, 1);
    *green = (data[3] << 8) | data[2];

    i2c_write(I2C1,TCS3414CS_REG_BLUE, NULL, 0);
    
    i2c_write(I2C1,(TCS3414CS_ADDR << 1) | 0x01, NULL, 0);
    i2c_read(I2C1, (TCS3414CS_ADDR << 1) | 0x01,data + 4, 1);
    i2c_read(I2C1, (TCS3414CS_ADDR << 1) | 0x01,data + 5, 1);
    *blue = (data[5] << 8) | data[4];

    i2c_write(I2C1,TCS3414CS_REG_CLEAR, NULL, 0);
    
    i2c_write(I2C1,(TCS3414CS_ADDR << 1) | 0x01, NULL, 0);
    i2c_read(I2C1, (TCS3414CS_ADDR << 1) | 0x01, data + 6, 1); 
    i2c_read(I2C1, (TCS3414CS_ADDR <<1) | 0x01, data + 7, 1);
    *clear = (data[7] << 8) | data[6];
}