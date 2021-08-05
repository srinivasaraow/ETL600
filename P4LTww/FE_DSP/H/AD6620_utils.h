/*-------------------------------------------------------------------------*
 * $Workfile: AD6620_utils.h $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : 
 * Remarks      :  
 * Purpose      : Declaration of function prototypes
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT Prototype 2/FE_DSP/H/AD6620_utils.h $ 
 * 
 * 1     19.01.05 15:32 Chchlee
 * 
 * 1     28.05.04 13:30 Maag01
 * Initial version
 * 
 *-------------------------------------------------------------------------*/

#ifndef AD6620_UTILS_H
#define AD6620_UTILS_H

void AD6620_Reset();

int AD6620_8Bit_Reg_Write(int reg_address, int reg_data);
int AD6620_16Bit_Reg_Write(int reg_address, int reg_data);
int AD6620_32Bit_Reg_Write(int reg_address, int reg_data);

int AD6620_8Bit_Reg_Read(int reg_address);
int AD6620_16Bit_Reg_Read(int reg_address);
int AD6620_32Bit_Reg_Read(int reg_address);

int AD6620_20Bit_Reg_Write(int reg_offset, int *reg_data, int len);
int AD6620_20Bit_Reg_Read(int reg_offset, int *reg_data, int len);

#endif
		




















