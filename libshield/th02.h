#ifndef _TH02_H
#define _TH02_H

#include "include/board.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#define TH02_ADDRESS		(0x40)
#define REG_STATUS           0x00
#define REG_DATA_H           0x01
#define REG_DATA_L           0x02
#define REG_CONFIG           0x03
#define REG_ID               0x11

#define STATUS_RDY_MASK      0x01    //poll RDY,0 indicate the conversion is done

#define CMD_MEASURE_HUMI     0x01    //perform a humility measurement
#define CMD_MEASURE_TEMP     0x11    //perform a temperature measurement

#define TH02_WR_REG_MODE      0xC0
#define TH02_RD_REG_MODE      0x80

/****************************************************************************/
/***        fonctions Definition                                              ***/
/****************************************************************************/

    void TH02_begin();
    uint8_t isAvailable();
    float TH02_ReadTemperature(void);
    // float ReadHumidity(void);
	
    void IIC_WriteCmd(uint8_t u8Cmd);
    uint8_t IIC_ReadReg(uint8_t u8Reg);
    void IIC_WriteReg(uint8_t u8Reg, uint8_t u8Data);
    
    // uint16_t IIC_ReadData(uint8_t u8Reg,uint8_t num);
    // uint16_t IIC_ReadData2byte(uint8_t u8Reg,uint8_t num);	


#endif