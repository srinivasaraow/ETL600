/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: Adjust_V11_TX_Buffer.c $
 * COPYRIGHT   : 2004 TL Consulting GmbH
 * DESCRIPTION : Initialize OFDM modem basic settings
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT/OFDM_DSP/C/V11/Adjust_V11_TX_Buffer.c $
 * 
 * 5     7.09.05 11:23 Ats
 * Exchange from TLC
 * 
 * 4     7.03.05 8:44 Ats
 * Exchange with TLC
 * 
 * 3     15.02.05 10:05 Chalgyg
 * OFDM Modem Version 0.09G
 * 
 * Initial version
 *       31.01.2005 vgp
 * Required buffers length correction for slave mode of operating
 *       04.02.05 vgp
*****************************************************************************/

#include <stdlib.h>
#include <math.h>
#include "../../h/v11/v11_externs.h"
#include "../../h/ofdm_externs.h"

extern int V11_Debug;
 
void segment ( "seg_ext_pmco" ) Adjust_V11_TX_Buffer()
{

	if  ( !V11_TX_Buffer_IsAdjusted ) {

		// for debug purpose only
		V11_TX_Ptr_Write_before_adjst = V11_TX_Bit_Ptr_Write;
		V11_TX_Ptr_Read_before_adjst  = V11_TX_Bit_Ptr_Read;
/*
      if ( V11_TX_Bit_Ptr_Write != V11_Bit_Buffer_TX && V11_TX_Bit_Ptr_Read > V11_TX_Bit_Ptr_Write ) {

        V11_TX_Buffer_Overrun = 1; 
        V11_Debug = V11_RX_Bit_Ptr_Write - V11_Bit_Buffer_RX;

      } else {

		// debugging: verify write pointer
		if ( V11_TX_Bit_Ptr_Write != V11_Bit_Buffer_TX ) {
			asm("nop;");			
			asm("nop;");			
			asm("nop;");			
		}
*/
#if 1
	
		// Reinitialise V11 write pointer. (rd pointer is following wr pointer, so wr pointer should
 		// be at least User_inf_bits away from rd pointer)
		V11_TX_Bit_Ptr_Write = V11_TX_Bit_Ptr_Read + V11_reserved_bits_mod_packing_TX - V11_Packing;
		// if pointer outside of buffer then wrap around
		if (V11_TX_Bit_Ptr_Write > V11_Bit_Buffer_TX + V11_TX_Bit_Buffer_Length)
			V11_TX_Bit_Ptr_Write = V11_TX_Bit_Ptr_Write - V11_TX_Bit_Buffer_Length;

		// for debug purpose only
		V11_TX_Ptr_Write_after_adjst = V11_TX_Bit_Ptr_Write;
		V11_TX_Ptr_Read_after_adjst  = V11_TX_Bit_Ptr_Read;

        
#endif      
   	  
      
   	  V11_TX_Buffer_IsAdjusted  = 1;
  }
}
