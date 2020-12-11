/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: TXRX_FILT_externs.h $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : Extern variables for OFDM TX and RX Filters 
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source HSD600/P4LT/OFDM_DSP/H/rx_tx new/TXRX_FILT_externs.h $
 * 
 * 1     5.04.06 15:37 Chhamaa
 * 
 * 1     7.02.06 11:28 Chhamaa
 * 
 * 2     24.02.05 7:35 Chchlee
 * 
 * 1     19.01.05 15:45 Chchlee
 * 
 * 2     1.10.04 12:07 Leeb02
 * 
 * 
 * Initial version
 *       07.07.2004 PL
 *****************************************************************************/


#include "../ofdm_defs.h"
#include <complex.h>

#ifndef TXRX_FILT_EXTERNS_H
#define TXRX_FILT_EXTERNS_H

/*****************************************************************
**					TX and RX Filters		 					**
*****************************************************************/



extern complex_float H_Filter_TX_32_64[OFDM_N_MAX/8-OFDM_L_MAX/8+1];
extern complex_float H_Filter_TX_32_128[OFDM_N_MAX/4-OFDM_L_MAX/4+1];
extern complex_float H_Filter_TX_32_256[OFDM_N_MAX/2-OFDM_L_MAX/2+1];
extern complex_float H_Filter_TX_32_512[OFDM_N_MAX/2-OFDM_L_MAX/2+1];

extern complex_float H_Filter_RX_32_64[OFDM_N_MAX/8-OFDM_L_MAX/8+1]; // replaced by next 2
extern complex_float H_Filter_RX_32_128_8[128-8+1];
extern complex_float H_Filter_RX_32_128_16[128-16+1];
extern complex_float H_Filter_RX_32_128[OFDM_N_MAX/4-OFDM_L_MAX/4+1];
extern complex_float H_Filter_RX_32_256[OFDM_N_MAX/2-OFDM_L_MAX/2+1];
extern complex_float H_Filter_RX_32_512[OFDM_N_MAX/2-OFDM_L_MAX/2+1];

extern complex_float H_Filter_TX_28_64[OFDM_N_MAX/8-OFDM_L_MAX/8+1];
extern complex_float H_Filter_TX_28_128[OFDM_N_MAX/4-OFDM_L_MAX/4+1];
extern complex_float H_Filter_TX_28_256[OFDM_N_MAX/2-OFDM_L_MAX/2+1];
extern complex_float H_Filter_TX_28_512[OFDM_N_MAX/2-OFDM_L_MAX/2+1];

extern complex_float H_Filter_RX_28_64[OFDM_N_MAX/8-OFDM_L_MAX/8+1]; // replaced by next 2
extern complex_float H_Filter_RX_28_128_8[128-8+1];
extern complex_float H_Filter_RX_28_128_16[128-16+1];
extern complex_float H_Filter_RX_28_128[OFDM_N_MAX/4-OFDM_L_MAX/4+1];
extern complex_float H_Filter_RX_28_256[OFDM_N_MAX/2-OFDM_L_MAX/2+1];
extern complex_float H_Filter_RX_28_512[OFDM_N_MAX/2-OFDM_L_MAX/2+1];

extern complex_float H_Filter_TX_20_64[OFDM_N_MAX/8-OFDM_L_MAX/8+1];
extern complex_float H_Filter_TX_20_128[OFDM_N_MAX/4-OFDM_L_MAX/4+1];
extern complex_float H_Filter_TX_20_256[OFDM_N_MAX/2-OFDM_L_MAX/2+1];
extern complex_float H_Filter_TX_20_512[OFDM_N_MAX/2-OFDM_L_MAX/2+1];

extern complex_float H_Filter_RX_20_64[OFDM_N_MAX/8-OFDM_L_MAX/8+1]; // replaced by next 2
extern complex_float H_Filter_RX_20_128_8[128-8+1];
extern complex_float H_Filter_RX_20_128_16[128-16+1];
extern complex_float H_Filter_RX_20_128[OFDM_N_MAX/4-OFDM_L_MAX/4+1];
extern complex_float H_Filter_RX_20_256[OFDM_N_MAX/2-OFDM_L_MAX/2+1];
extern complex_float H_Filter_RX_20_512[OFDM_N_MAX/2-OFDM_L_MAX/2+1];

extern complex_float H_Filter_TX_4r_64[OFDM_N_MAX/8-OFDM_L_MAX/8+1];
extern complex_float H_Filter_TX_4r_128[OFDM_N_MAX/4-OFDM_L_MAX/4+1];

extern complex_float H_Filter_RX_4r_64[OFDM_N_MAX/8-OFDM_L_MAX/8+1]; //replaced by next 2
extern complex_float H_Filter_RX_4r_128_8[128-8+1];
extern complex_float H_Filter_RX_4r_128_16[128-16+1];
extern complex_float H_Filter_RX_4r_128[OFDM_N_MAX/4-OFDM_L_MAX/4+1];

/*
extern complex_float H_Filter_TX_8r_64[OFDM_N_MAX/8-OFDM_L_MAX/8+1];
extern complex_float H_Filter_RX_8r_64[OFDM_N_MAX/8-OFDM_L_MAX/8+1];
extern complex_float H_Filter_TX_8r_128[OFDM_N_MAX/4-OFDM_L_MAX/4+1];
extern complex_float H_Filter_RX_8r_128[OFDM_N_MAX/4-OFDM_L_MAX/4+1];
*/

//extern complex_float H_Filter_TX_32_128[OFDM_N_MAX/4-OFDM_L_MAX/4+1];

extern complex_float H_Filter_TX_8_64[OFDM_N_MAX/8-OFDM_L_MAX/8+1];
extern complex_float H_Filter_TX_8_128[OFDM_N_MAX/4-OFDM_L_MAX/4+1];
extern complex_float H_Filter_TX_8_256[OFDM_N_MAX/2-OFDM_L_MAX/2+1];
extern complex_float H_Filter_TX_8_512[OFDM_N_MAX/2-OFDM_L_MAX/2+1];

extern complex_float H_Filter_RX_8_64[OFDM_N_MAX/8-OFDM_L_MAX/8+1]; // replaced by next 1
extern complex_float H_Filter_RX_8_128_8[128-8+1];
extern complex_float H_Filter_RX_8_128[OFDM_N_MAX/4-OFDM_L_MAX/4+1];
extern complex_float H_Filter_RX_8_256[OFDM_N_MAX/2-OFDM_L_MAX/2+1];
extern complex_float H_Filter_RX_8_512[OFDM_N_MAX/2-OFDM_L_MAX/2+1];

extern complex_float H_Filter_RX_8r_fn12_128[OFDM_N_MAX/4-OFDM_L_MAX/4+1];
extern complex_float H_Filter_TX_8r_fn12_128[OFDM_N_MAX/4-OFDM_L_MAX/4+1];

extern complex_float H_Filter_TX_4r_fn6_64[OFDM_N_MAX/8-OFDM_L_MAX/8+1];
extern complex_float H_Filter_RX_4r_fn6_128_8[128-8+1];

#endif
