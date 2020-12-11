/*----------------------------------------------------------------------------*
 * $Workfile: AD9777_dout.h $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : 
 * Remarks      :  
 * Purpose      : Global type and function declarations
 * Copyright    : ABB Switzerland Ltd., 2003
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
 * $Log: /branches/Source HSD600/P4LT/UC_DSP/H/AD9777_dout.h $
 * 
 * 4     4.04.06 18:35 Chmibuh
 * AD9777_Stop_Data_Port commented out
 * 
 * 2     8.02.05 14:45 Chbrbus
 * UC DAC interrupt syncronized with SPORT13 interrupt - reduced DAC
 * buffers from 3 to 2 ! Cleaned Interrupt functions and EP1
 * initialisation.
 * 
 * 1     20.01.05 8:32 Chchlee
 * 
 * 3     26.05.04 18:31 Maag01
 * 
 * 2     26.05.04 16:40 Maag01
 * 
 * 
 *----------------------------------------------------------------------------*/
 
#ifndef AD7777_DOUT_H
#define AD7777_DOUT_H



/* PUBLIC TYPE DECLARATIONS ***************************************************/

int AD9777_Init_Data_Port(int *IQBuf1_ptr, int *IQBuf2_ptr, int Len);
void AD9777_Start_Data_Port();
//void AD9777_Stop_Data_Port();

#endif
