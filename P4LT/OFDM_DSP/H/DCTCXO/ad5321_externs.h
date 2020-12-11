#ifndef AD5321_EXTERNS_H
#define AD5321_EXTERNS_H

#ifdef __ECC__

extern int I2C_WR_Index_Tx;
extern int I2C_RD_Index_Tx;
extern int AD5321_TX_Buffer[AD5321_WR_BUF_LENGTH];
extern int I2C_TSlotCount;
extern int I2C_Bit_Mask;
extern int I2C_Busy;
extern int I2C_RD_Word;
extern int I2C_NACK;
extern unsigned int I2C_Tm_Counter;
extern int I2C_Tm_Disable;

extern unsigned int AD5321_Control_Code;
extern unsigned int AD5321_Control_Flag;

#endif

#endif
