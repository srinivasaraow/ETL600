/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: V11_Init.c $
 * COPYRIGHT   : 2004 TL Consulting GmbH
 * DESCRIPTION : Initialize OFDM modem basic settings
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT/OFDM_DSP/C/V11/V11_Init.c $
 * 
 * 9     7.09.05 11:23 Ats
 * Exchange from TLC
 * 
 * 8     7.03.05 8:44 Ats
 * Exchange with TLC
 * 
 * 7     15.02.05 10:05 Chalgyg
 * OFDM Modem Version 0.09G
 * 
 * 3     28.09.04 17:45 Maag01
 * 
 * Initial version
 *       09.05.2004 vgp
 * Minimization of V11 buffering delay based on user information bits per
 * ofdm symbol 'User_inf_bits_TX'
 *       27.01.2005 vgp
 * Required buffers length correction for slave mode of operating
 *       04.02.05 vgp
*****************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include "../../h/util/util.h"
#include "../../h/v11/v11_externs.h"
#include "../../h/ofdm_externs.h"

void segment ( "seg_ext_pmco" ) V11_Init()
{
    int i, k;
    int user_inf_bits_mod_packing;
    int Get_V11_Divider(int User_inf_bits_TX);

  // Clear V11 Tx & Rx Startup flags and variables
  V11_TX_IsStarted = 0;
  V11_RX_IsStarted = 0;
  V11_TX_Buffer_IsAdjusted = 0;
  V11_RX_Buffer_IsAdjusted = 0;
  V11_TX_Buffer_Overrun = 0;
  V11_RX_Buffer_Overrun = 0;
  V11_TX_STKY = 0;
  V11_RX_STKY = 0;
	  
  // Initialise V11 driver's output two 32 bit word packing buffer and its pointers
  V11_W32_Buffer_TX[0] = 0;
  V11_W32_Buffer_TX[1] = 0;
  V11_W32_2TX_Cur = &V11_W32_Buffer_TX[0];
  V11_W32_2TX_Next = &V11_W32_Buffer_TX[1];

  V11_Packing = 0;
  V11_reserved_bits_mod_packing_TX = 0;
  V11_TX_Bit_Buffer_Length = 0;
  V11_reserved_bits_mod_packing_RX = 0;
  V11_RX_Bit_Buffer_Length = 0;

    if ( User_inf_bits_TX > 0 )	
    {    	
    	
	  // number V11s inf. bits per word
      V11_Packing = Get_V11_Divider(User_inf_bits_TX);  // number of V11 bits per drivers i/o word
	                                                    // in the range 3..32 bits per word
	  // fext, fdep instruction bit mask
	  V11_Bit_Mask = (1<<6) | (V11_Packing-1);

	

// Initialise V11 output bit buffer and its pointers

      // get user # of information bits modulo packing
      user_inf_bits_mod_packing = (User_inf_bits_TX % V11_Packing) == 0  ? User_inf_bits_TX :
          V11_Packing*(User_inf_bits_TX/V11_Packing + 1);  
        
      // get user # of reserved bits modulo packing
//      reserved_bits_mod_packing = user_inf_bits_mod_packing + 2*V11_Packing;
      V11_reserved_bits_mod_packing_TX = user_inf_bits_mod_packing + 2*V11_Packing;

      // get buffer length        
//      V11_TX_Bit_Buffer_Length = User_inf_bits_TX + reserved_bits_mod_packing;
      V11_TX_Bit_Buffer_Length = User_inf_bits_TX + V11_reserved_bits_mod_packing_TX;
        
      // clear buffer
      vset_i_E(V11_Bit_Buffer_TX,0,V11_TX_Bit_Buffer_Length);

      // initialize pointers
      V11_TX_Bit_Ptr_Read  = V11_Bit_Buffer_TX;
//      V11_TX_Bit_Ptr_Write = V11_Bit_Buffer_TX + V11_reserved_bits_mod_packing_TX;
      V11_TX_Bit_Ptr_Write = V11_Bit_Buffer_TX + User_inf_bits_TX;
    
 // Initialise V11 input bit buffer and its pointers
 
      // get user # of information bits modulo packing
      user_inf_bits_mod_packing = (User_inf_bits_RX % V11_Packing) == 0  ? User_inf_bits_RX :
          V11_Packing*(User_inf_bits_RX/V11_Packing + 1);  
        
      // get user # of reserved bits modulo packing
      V11_reserved_bits_mod_packing_RX = user_inf_bits_mod_packing + 2*V11_Packing;

      // get buffer length        
      V11_RX_Bit_Buffer_Length = User_inf_bits_RX + V11_reserved_bits_mod_packing_RX;

      // clear buffer
      vset_i_E(V11_Bit_Buffer_RX,0,V11_RX_Bit_Buffer_Length);
 
      // initialize pointers
      V11_RX_Bit_Ptr_Write = V11_Bit_Buffer_RX;      
      V11_RX_Bit_Ptr_Read  = V11_Bit_Buffer_RX + V11_reserved_bits_mod_packing_RX; 
      
      asm("nop;");
      asm("nop;");
      asm("nop;");
      
    }

    V11_IsStarted = 0;
    
}

