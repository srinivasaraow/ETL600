/*----------------------------------------------------------------------------*
 * $Workfile: AD9777_utils.h $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : 
 * Remarks      :  
 * Purpose      : Global type and function declarations
 * Copyright    : ABB Switzerland Ltd., 2003
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/UC_DSP/H/AD9777_utils.h $
 * 
 * 1     20.01.05 8:32 Chchlee
 * 
 * 2     26.05.04 16:40 Maag01
 * 
 * 
 *----------------------------------------------------------------------------*/

#ifndef AD9777_UTILS_H
#define AD9777_UTILS_H


/* PUBLIC TYPE DECLARATIONS ***************************************************/

int   AD9777_Write(int reg_address, int reg_data);
int   AD9777_Read(int reg_address);

#endif
		


