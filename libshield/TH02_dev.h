#ifndef _TH02_DEV_H
#define _TH02_DEV_H

#include <stdint.h>

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define TH02_I2C_DEV_ID      0x40
#define REG_STATUS           0x00
#define REG_DATA_H           0x01
#define REG_DATA_L           0x02
#define REG_CONFIG           0x03
#define REG_ID               0x11

#define STATUS_RDY_MASK      0x01    //poll RDY,0 indicate the conversion is done

#define CMD_MEASURE_HUMI     0x01    //perform a humility measurement
#define CMD_MEASURE_TEMP     0xb    //perform a temperature measurement

/****************************************************************************/
/***        Function Definitions                                          ***/
/****************************************************************************/

void TH02_dev_begin(void);
uint8_t TH02_dev_isAvailable(void);
int TH02_dev_ReadTemperature(void);
int TH02_dev_ReadHumidity(void);
void TH02_dev_IIC_WriteCmd(uint8_t u8Cmd);
uint8_t TH02_dev_IIC_ReadReg(uint8_t u8Reg);
void TH02_dev_IIC_WriteReg(uint8_t u8Reg, uint8_t u8Data);
uint16_t TH02_dev_IIC_ReadData(uint8_t u8Reg, uint8_t num);
uint16_t TH02_dev_IIC_ReadData2byte(uint8_t u8Reg, uint8_t num);

#endif
