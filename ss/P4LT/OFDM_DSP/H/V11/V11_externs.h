/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: V11_externs.h $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : TCM codec's global variables 
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * 
 * 
 * Initial version
 *       09.05.2004 vgp
 *****************************************************************************/

#include "v11_defs.h"
#include "../ofdm_defs.h"

#ifndef V11_EXTERNS_H
#define V11_EXTERNS_H

extern int segment ( "seg_extn SHT_NOBITS" ) V11_Bit_Buffer_TX[V11_MAX_BUFFER_LENGTH];
extern int segment ( "seg_extn SHT_NOBITS" ) V11_Bit_Buffer_RX[V11_MAX_BUFFER_LENGTH];

extern int V11_W32_Buffer_TX[2];
extern int *V11_W32_2TX_Cur;
extern int *V11_W32_2TX_Next;
extern int *V11_TX_Bit_Ptr_Write;
extern int *V11_TX_Bit_Ptr_Read;
extern int V11_TX_Bit_Buffer_Length;

extern int *V11_RX_Bit_Ptr_Write;
extern int *V11_RX_Bit_Ptr_Read;
extern int V11_RX_Bit_Buffer_Length;

extern int V11_Clock_Div_RX;
extern int V11_Clock_Div_TX;

extern int V11_IsStarted;
extern int V11_Start_IO;

extern int V11_Packing;
extern int V11_Bit_Mask;
extern int V11_TX_IsStarted;
extern int V11_RX_IsStarted;
extern int V11_TX_Buffer_IsAdjusted;
extern int V11_RX_Buffer_IsAdjusted;
extern int V11_TX_Buffer_Overrun;
extern int V11_RX_Buffer_Overrun;
extern int V11_TX_STKY;
extern int V11_RX_STKY;
extern int V11_reserved_bits_mod_packing_TX;
extern int V11_reserved_bits_mod_packing_RX;

// debug pointers
extern int *V11_TX_Ptr_Write_before_adjst;
extern int *V11_TX_Ptr_Write_after_adjst;
extern int *V11_TX_Ptr_Read_before_adjst;
extern int *V11_TX_Ptr_Read_after_adjst;
extern int *V11_RX_Ptr_Write_before_adjst;
extern int *V11_RX_Ptr_Write_after_adjst;
extern int *V11_RX_Ptr_Read_before_adjst;
extern int *V11_RX_Ptr_Read_after_adjst;

// for v11 debuging purpose
// extern int V11_xxx[32*MAX_INOUT_DATA_BUF];

#endif
