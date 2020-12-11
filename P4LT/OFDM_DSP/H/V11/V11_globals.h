/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: V11_globals.h $
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

#ifndef V11_GLOBALS_H
#define V11_GLOBALS_H

int segment ( "seg_extn SHT_NOBITS" ) V11_Bit_Buffer_TX[V11_MAX_BUFFER_LENGTH];
int segment ( "seg_extn SHT_NOBITS" ) V11_Bit_Buffer_RX[V11_MAX_BUFFER_LENGTH];

asm(".align 2;");
int segment ( "seg_extn SHT_NOBITS" ) V11_W32_Buffer_TX[2];

int segment ( "seg_extn SHT_NOBITS" ) *V11_W32_2TX_Cur;
int segment ( "seg_extn SHT_NOBITS" ) *V11_W32_2TX_Next;

int segment ( "seg_extn SHT_NOBITS" ) *V11_TX_Bit_Ptr_Write;
int segment ( "seg_extn SHT_NOBITS" ) *V11_TX_Bit_Ptr_Read;
int segment ( "seg_extn SHT_NOBITS" ) V11_TX_Bit_Buffer_Length;

int segment ( "seg_extn SHT_NOBITS" ) *V11_RX_Bit_Ptr_Write;
int segment ( "seg_extn SHT_NOBITS" ) *V11_RX_Bit_Ptr_Read;
int segment ( "seg_extn SHT_NOBITS" ) V11_RX_Bit_Buffer_Length;

int segment ( "seg_extn SHT_NOBITS" ) V11_Clock_Div_RX;
int segment ( "seg_extn SHT_NOBITS" ) V11_Clock_Div_TX;

int segment ( "seg_extn SHT_NOBITS" ) V11_IsStarted;
int segment ( "seg_extn SHT_NOBITS" ) V11_Start_IO;

int segment ( "seg_extn SHT_NOBITS" ) V11_Packing;
int segment ( "seg_extn SHT_NOBITS" ) V11_Bit_Mask;
int segment ( "seg_extn SHT_NOBITS" ) V11_TX_IsStarted;
int segment ( "seg_extn SHT_NOBITS" ) V11_RX_IsStarted;
int segment ( "seg_extn SHT_NOBITS" ) V11_TX_Buffer_IsAdjusted;
int segment ( "seg_extn SHT_NOBITS" ) V11_RX_Buffer_IsAdjusted;
int segment ( "seg_extn SHT_NOBITS" ) V11_TX_Buffer_Overrun;
int segment ( "seg_extn SHT_NOBITS" ) V11_RX_Buffer_Overrun;
int segment ( "seg_extn SHT_NOBITS" ) V11_TX_STKY;
int segment ( "seg_extn SHT_NOBITS" ) V11_RX_STKY;
int segment ( "seg_extn SHT_NOBITS" ) V11_reserved_bits_mod_packing_TX;
int segment ( "seg_extn SHT_NOBITS" ) V11_reserved_bits_mod_packing_RX;

// debug pointers
int segment ( "seg_extn SHT_NOBITS" ) *V11_TX_Ptr_Write_before_adjst;
int segment ( "seg_extn SHT_NOBITS" ) *V11_TX_Ptr_Write_after_adjst;
int segment ( "seg_extn SHT_NOBITS" ) *V11_TX_Ptr_Read_before_adjst;
int segment ( "seg_extn SHT_NOBITS" ) *V11_TX_Ptr_Read_after_adjst;
int segment ( "seg_extn SHT_NOBITS" ) *V11_RX_Ptr_Write_before_adjst;
int segment ( "seg_extn SHT_NOBITS" ) *V11_RX_Ptr_Write_after_adjst;
int segment ( "seg_extn SHT_NOBITS" ) *V11_RX_Ptr_Read_before_adjst;
int segment ( "seg_extn SHT_NOBITS" ) *V11_RX_Ptr_Read_after_adjst;


// for v11 debuging purpose
//int segment ( "seg_extn SHT_NOBITS" ) V11_xxx[32*MAX_INOUT_DATA_BUF];

#endif
