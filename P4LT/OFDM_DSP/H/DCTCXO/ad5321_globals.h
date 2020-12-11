#ifndef AD5321_GLOBALS_H
#define AD5321_GLOBALS_H

int  segment ( "seg_extn SHT_NOBITS" ) I2C_WR_Index_Tx;
int  segment ( "seg_extn SHT_NOBITS" ) I2C_RD_Index_Tx;
int  segment ( "seg_extn SHT_NOBITS" ) AD5321_TX_Buffer[AD5321_WR_BUF_LENGTH]; // 8 commands by 8 stages
int  segment ( "seg_extn SHT_NOBITS" ) I2C_TSlotCount;
int  segment ( "seg_extn SHT_NOBITS" ) I2C_Bit_Mask;
int  segment ( "seg_extn SHT_NOBITS" ) I2C_Busy;
int  segment ( "seg_extn SHT_NOBITS" ) I2C_RD_Word;
int  segment ( "seg_extn SHT_NOBITS" ) I2C_NACK;
unsigned int  segment ( "seg_extn SHT_NOBITS" ) I2C_Tm_Counter;
int  segment ( "seg_extn SHT_NOBITS" ) I2C_Tm_Disable;

unsigned int segment ( "seg_extn SHT_NOBITS" ) AD5321_Control_Code;
unsigned int segment ( "seg_extn SHT_NOBITS" ) AD5321_Control_Flag;

#endif
