#ifndef PAJ7620_H
#define PAJ7620_H

#include <stdint.h>

// Registers and variables for the gesture sensor
#define GES_REACTION_TIME 500000 
#define GES_ENTRY_TIME 800000 
#define GES_QUIT_TIME 1000000


#define BANK0 0
#define BANK1 1

#define PAJ7620_ADDR_BASE 0x00

// REGISTER BANK SELECT
#define PAJ7620_REGITER_BANK_SEL (PAJ7620_ADDR_BASE + 0xEF) // W

// DEVICE ID
#define PAJ7620_ID 0x73

// REGISTER BANK 0
#define PAJ7620_ADDR_SUSPEND_CMD (PAJ7620_ADDR_BASE + 0x3)      // W
#define PAJ7620_ADDR_GES_PS_DET_MASK_0 (PAJ7620_ADDR_BASE + 0x41) // RW
#define PAJ7620_ADDR_GES_PS_DET_MASK_1 (PAJ7620_ADDR_BASE + 0x42) // RW
#define PAJ7620_ADDR_GES_PS_DET_FLAG_0 (PAJ7620_ADDR_BASE + 0x43) // R
#define PAJ7620_ADDR_GES_PS_DET_FLAG_1 (PAJ7620_ADDR_BASE + 0x44) // R
#define PAJ7620_ADDR_STATE_INDICATOR (PAJ7620_ADDR_BASE + 0x45)  // R
#define PAJ7620_ADDR_PS_HIGH_THRESHOLD (PAJ7620_ADDR_BASE + 0x69)   // RW
#define PAJ7620_ADDR_PS_LOW_THRESHOLD (PAJ7620_ADDR_BASE + 0x6A)    // RW
#define PAJ7620_ADDR_PS_APPROACH_STATE (PAJ7620_ADDR_BASE + 0x6B)  // R
#define PAJ7620_ADDR_PS_RAW_DATA (PAJ7620_ADDR_BASE + 0x6C)        // R

// REGISTER BANK 1
#define PAJ7620_ADDR_PS_GAIN (PAJ7620_ADDR_BASE + 0x44)           // RW
#define PAJ7620_ADDR_IDLE_S1_STEP_0 (PAJ7620_ADDR_BASE + 0x67)    // RW
#define PAJ7620_ADDR_IDLE_S1_STEP_1 (PAJ7620_ADDR_BASE + 0x68)    // RW
#define PAJ7620_ADDR_IDLE_S2_STEP_0 (PAJ7620_ADDR_BASE + 0x69)    // RW
#define PAJ7620_ADDR_IDLE_S2_STEP_1 (PAJ7620_ADDR_BASE + 0x6A)    // RW
#define PAJ7620_ADDR_OP_TO_S1_STEP_0 (PAJ7620_ADDR_BASE + 0x6B)   // RW
#define PAJ7620_ADDR_OP_TO_S1_STEP_1 (PAJ7620_ADDR_BASE + 0x6C)   // RW
#define PAJ7620_ADDR_OP_TO_S2_STEP_0 (PAJ7620_ADDR_BASE + 0x6D)   // RW
#define PAJ7620_ADDR_OP_TO_S2_STEP_1 (PAJ7620_ADDR_BASE + 0x6E)   // RW
#define PAJ7620_ADDR_OPERATION_ENABLE (PAJ7620_ADDR_BASE + 0x72)  // RW

// PAJ7620_REGITER_BANK_SEL
#define PAJ7620_BANK0 0
#define PAJ7620_BANK1 1

// PAJ7620_ADDR_SUSPEND_CMD
#define PAJ7620_I2C_WAKEUP 1
#define PAJ7620_I2C_SUSPEND 0

// PAJ7620_ADDR_OPERATION_ENABLE
#define PAJ7620_ENABLE 1
#define PAJ7620_DISABLE 0

// ADC, delete
#define REG_ADDR_RESULT 0x00
#define REG_ADDR_ALERT 0x01
#define REG_ADDR_CONFIG 0x02
#define REG_ADDR_LIMITL 0x03
#define REG_ADDR_LIMITH 0x04
#define REG_ADDR_HYST 0x05
#define REG_ADDR_CONVL 0x06
#define REG_ADDR_CONVH 0x07

#define GES_RIGHT_FLAG (1 << 0)
#define GES_LEFT_FLAG (1 << 1)
#define GES_UP_FLAG (1 << 2)
#define GES_DOWN_FLAG (1 << 3)
#define GES_FORWARD_FLAG (1 << 4)
#define GES_BACKWARD_FLAG (1 << 5)
#define GES_CLOCKWISE_FLAG (1 << 6)
#define GES_COUNT_CLOCKWISE_FLAG (1 << 7)
#define GES_WAVE_FLAG (1 << 0)

// Gesture output
#define FORWARD 1
#define BACKWARD 2
#define RIGHT 3
#define LEFT 4
#define UP 5
#define DOWN 6
#define CLOCKWISE 7
#define ANTI_CLOCKWISE 8
#define WAVE 9

extern int paj7620initRegisterArray[][2];
// Initialise les capteurs
void init();

// Sélectionne une banque de registres sur le capteur de gestes
void paj7620SelectBank(uint8_t bank);

// Écrit un octet dans un registre du capteur de gestes
void paj7620WriteReg(uint8_t addr, uint8_t cmd);

// Lit un bloc d'octets de longueur "qty" à partir de l'adresse "addr" sur le capteur de gestes
uint8_t paj7620ReadReg(uint8_t addr, uint8_t qty);

// Renvoie une valeur du capteur de gestes qui peut être utilisée dans un programme
int return_gesture();

//Affichage de gestes
void print_gesture();

#endif //PAJ7620_H
