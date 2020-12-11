/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: V11_defs.h $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : TCM codec's global variables 
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /Source/P4LT/OFDM_DSP/H/V11/V11_defs.h $
 * 
 * 7     7.09.05 11:24 Ats
 * Exchange from TLC
 * 
 * 6     7.03.05 8:43 Ats
 * Exchange with TLC
 * 
 * 5     15.02.05 10:06 Chalgyg
 * OFDM Modem Version 0.09G
 * 
 * 4     8.02.05 13:35 Chalgyg
 * Bufferlength adjusted from TLC to make it work on all baudrates.
 * 
 * 3     30.09.04 8:47 Leeb02
 * 
 * 
 * Initial version
 *       09.05.2004 vgp
 *****************************************************************************/
#include "../ofdm_defs.h"

#ifndef V11_DEFS_H
#define V11_DEFS_H

#define V11_MAX_SLEN           32  // maximum 32 V11 bits per word, min 3 bits
#define V11_PACKING_RESERVE    V11_MAX_SLEN

#define TDM_PERIOD_SEC         (6.0f/64000.0f)  
#define V11_MAX_DATA_RATE_BPS  320000.0f
#define V11_CONTROL_DELAY_SEC  0.001
#define V11_FPGA_DELAY_SEC     0.000100

#define V11_MIN_MAX_LENGTH     (32*MAX_INOUT_DATA_BUF) //Max number of bits per OFDM_Symbol
#define V11_MAX_DELAY_SEC      (2.0f*TDM_PERIOD_SEC+V11_CONTROL_DELAY_SEC+V11_FPGA_DELAY_SEC)
#define V11_RESERVED_LENGTH    (V11_MAX_DATA_RATE_BPS*V11_MAX_DELAY_SEC)

//#define V11_MAX_BUFFER_LENGTH  ((int)(3.0f*(float)V11_MIN_MAX_LENGTH+V11_RESERVED_LENGTH+0.5f) + 2*V11_PACKING_RESERVE) 
#define V11_MAX_BUFFER_LENGTH  (2*V11_MIN_MAX_LENGTH + 10*V11_PACKING_RESERVE) 

#endif
