/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: TXRX_FILT_globals.h $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : Global variables for OFDM TX and RX Filters 
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source HSD600/P4LT/OFDM_DSP/H/rx_tx new/TXRX_FILT_globals.h $
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

#ifndef TXRX_FILT_GLOBALS_H
#define TXRX_FILT_GLOBALS_H

/*****************************************************************
**					TX and RX Filters		 					**
*****************************************************************/



//********************************* BW 20 kHz *****************************************
// ****** 512 **
complex_float segment ( "seg_extn_init" ) H_Filter_RX_20_512[OFDM_N_MAX/2-OFDM_L_MAX/2+1]=
{
#include "h20_512.dat"
};
complex_float segment ( "seg_extn_init" ) H_Filter_TX_20_512[OFDM_N_MAX/2-OFDM_L_MAX/2+1]=
{
#include "h20_512.dat"
};

// ****** 256 **
complex_float segment ( "seg_extn_init" ) H_Filter_RX_20_256[OFDM_N_MAX/2-OFDM_L_MAX/2+1]=
{
#include "h20_256.dat"
};
complex_float segment ( "seg_extn_init" ) H_Filter_TX_20_256[OFDM_N_MAX/2-OFDM_L_MAX/2+1]=
{
#include "h20_256.dat"
};

// ****** 128 **
complex_float segment ( "seg_extn_init" ) H_Filter_RX_20_128[OFDM_N_MAX/4-OFDM_L_MAX/4+1]=
{
#include "h20_128.dat"
};
complex_float segment ( "seg_extn_init" ) H_Filter_TX_20_128[OFDM_N_MAX/4-OFDM_L_MAX/4+1]=
{
#include "h20_128.dat"
};

// ****** 64  (TX) ** (here only TX filter is used in Project)
complex_float segment ( "seg_extn_init" ) H_Filter_RX_20_64[OFDM_N_MAX/8-OFDM_L_MAX/8+1]=
{
#include "h20_64.dat"
};
complex_float segment ( "seg_extn_init" ) H_Filter_TX_20_64[OFDM_N_MAX/8-OFDM_L_MAX/8+1]=
{
#include "h20_64.dat"
};
// *** 64 RX double length (128-L+1)
complex_float segment ( "seg_extn_init" ) H_Filter_RX_20_128_8[128-8+1]=
{
#include "h20_128_8.dat"
};
complex_float segment ( "seg_extn_init" ) H_Filter_RX_20_128_16[128-16+1]=
{
#include "h20_128_16.dat"
};


// ********************************* BW 28 kHz *****************************************
//  ****** 512 **
complex_float segment ( "seg_extn_init" ) H_Filter_RX_28_512[OFDM_N_MAX/2-OFDM_L_MAX/2+1]=
{
#include "h28_512.dat"
};
complex_float segment ( "seg_extn_init" ) H_Filter_TX_28_512[OFDM_N_MAX/2-OFDM_L_MAX/2+1]=
{
#include "h28_512.dat"
};

// ****** 256 **
complex_float segment ( "seg_extn_init" ) H_Filter_RX_28_256[OFDM_N_MAX/2-OFDM_L_MAX/2+1]=
{
#include "h28_256.dat"
};
complex_float segment ( "seg_extn_init" ) H_Filter_TX_28_256[OFDM_N_MAX/2-OFDM_L_MAX/2+1]=
{
#include "h28_256.dat"
};

// ****** 128 **
complex_float segment ( "seg_extn_init" ) H_Filter_RX_28_128[OFDM_N_MAX/4-OFDM_L_MAX/4+1]=
{
#include "h28_128.dat"
};
complex_float segment ( "seg_extn_init" ) H_Filter_TX_28_128[OFDM_N_MAX/4-OFDM_L_MAX/4+1]=
{
#include "h28_128.dat"
};

// ****** 64  (TX) ** (here only TX filter is used in Project)
complex_float segment ( "seg_extn_init" ) H_Filter_RX_28_64[OFDM_N_MAX/8-OFDM_L_MAX/8+1]=
{
#include "h28_64.dat"
};
complex_float segment ( "seg_extn_init" ) H_Filter_TX_28_64[OFDM_N_MAX/8-OFDM_L_MAX/8+1]=
{
#include "h28_64.dat"
};
// *** 64 RX double length (128-L+1)
complex_float segment ( "seg_extn_init" ) H_Filter_RX_28_128_8[128-8+1]=
{
#include "h28_128_8.dat"
};
complex_float segment ( "seg_extn_init" ) H_Filter_RX_28_128_16[128-16+1]=
{
#include "h28_128_16.dat"
};

// ********************************* BW 32 kHz *****************************************
//  ****** 512 **
complex_float segment ( "seg_extn_init" ) H_Filter_RX_32_512[OFDM_N_MAX/2-OFDM_L_MAX/2+1]=
{
#include "h32_512.dat"
};
complex_float segment ( "seg_extn_init" ) H_Filter_TX_32_512[OFDM_N_MAX/2-OFDM_L_MAX/2+1]=
{
#include "h32_512.dat"
};

// ****** 256 **
complex_float segment ( "seg_extn_init" ) H_Filter_RX_32_256[OFDM_N_MAX/2-OFDM_L_MAX/2+1]=
{
#include "h32_256.dat"
};
complex_float segment ( "seg_extn_init" ) H_Filter_TX_32_256[OFDM_N_MAX/2-OFDM_L_MAX/2+1]=
{
#include "h32_256.dat"
};

// ****** 128 **
complex_float segment ( "seg_extn_init" ) H_Filter_RX_32_128[OFDM_N_MAX/4-OFDM_L_MAX/4+1]=
{
#include "h32_128.dat"
};
complex_float segment ( "seg_extn_init" ) H_Filter_TX_32_128[OFDM_N_MAX/4-OFDM_L_MAX/4+1]=
{
#include "h32_128.dat"
};

// ****** 64 (TX) ** (here only TX filter is used in Project)
complex_float segment ( "seg_extn_init" ) H_Filter_RX_32_64[OFDM_N_MAX/8-OFDM_L_MAX/8+1]=
{
#include "h32_64.dat"
};
complex_float segment ( "seg_extn_init" ) H_Filter_TX_32_64[OFDM_N_MAX/8-OFDM_L_MAX/8+1]=
{ 
#include "h32_64.dat"
};
// *** 64 RX double length (128-L+1)
complex_float segment ( "seg_extn_init" ) H_Filter_RX_32_128_8[128-8+1]=
{
#include "h32_128_8.dat"
};
complex_float segment ( "seg_extn_init" ) H_Filter_RX_32_128_16[128-16+1]=
{
#include "h32_128_16.dat"
};


// ********************************* BW 4 kHz (reduced, due to ETL-Pilot)*****************

// ****** 128 **
complex_float segment ( "seg_extn_init" ) H_Filter_RX_4r_128[OFDM_N_MAX/4-OFDM_L_MAX/4+1]=
{
#include "h4r_128.dat"
};
complex_float segment ( "seg_extn_init" ) H_Filter_TX_4r_128[OFDM_N_MAX/4-OFDM_L_MAX/4+1]=
{
#include "h4r_128.dat"
};

// ****** 64 (TX)**
complex_float segment ( "seg_extn_init" ) H_Filter_RX_4r_64[OFDM_N_MAX/8-OFDM_L_MAX/8+1]=
{
#include "h4r_64.dat"
};
complex_float segment ( "seg_extn_init" ) H_Filter_TX_4r_64[OFDM_N_MAX/8-OFDM_L_MAX/8+1]=
{ 
#include "h4r_64.dat"
};
// *** 64 RX double length (128-L+1)
complex_float segment ( "seg_extn_init" ) H_Filter_RX_4r_128_8[128-8+1]=
{
#include "h4r_128_8.dat"
};
complex_float segment ( "seg_extn_init" ) H_Filter_RX_4r_128_16[128-16+1]=
{
#include "h4r_128_16.dat"
};

// ********************************* BW 8 kHz (reduced, due to ETL-Pilot)*****************
/*
// ****** 128 **
complex_float segment ( "seg_extn_init" ) H_Filter_RX_8r_128[OFDM_N_MAX/4-OFDM_L_MAX/4+1]=
{
#include "h8r_128.dat"
};
complex_float segment ( "seg_extn_init" ) H_Filter_TX_8r_128[OFDM_N_MAX/4-OFDM_L_MAX/4+1]=
{
#include "h8r_128.dat"
};

// ****** 64 **
complex_float segment ( "seg_extn_init" ) H_Filter_RX_8r_64[OFDM_N_MAX/8-OFDM_L_MAX/8+1]=
{
#include "h8r_64.dat"
};
complex_float segment ( "seg_extn_init" ) H_Filter_TX_8r_64[OFDM_N_MAX/8-OFDM_L_MAX/8+1]=
{ 
#include "h8r_64.dat"
};	
*/

//********************************* BW 8 kHz ** (FN=12 kHz) ***********************************
// ****** 512 **
complex_float segment ( "seg_extn_init" ) H_Filter_RX_8_512[OFDM_N_MAX/2-OFDM_L_MAX/2+1]=
{
#include "h8_fn12_512.dat"
};
complex_float segment ( "seg_extn_init" ) H_Filter_TX_8_512[OFDM_N_MAX/2-OFDM_L_MAX/2+1]=
{
#include "h8_fn12_512.dat"
};

// ****** 256 **
complex_float segment ( "seg_extn_init" ) H_Filter_RX_8_256[OFDM_N_MAX/2-OFDM_L_MAX/2+1]=
{
#include "h8_fn12_256.dat"
};
complex_float segment ( "seg_extn_init" ) H_Filter_TX_8_256[OFDM_N_MAX/2-OFDM_L_MAX/2+1]=
{
#include "h8_fn12_256.dat"
};

// ****** 128 **
complex_float segment ( "seg_extn_init" ) H_Filter_RX_8_128[OFDM_N_MAX/4-OFDM_L_MAX/4+1]=
{
#include "h8_fn12_128.dat"
};
complex_float segment ( "seg_extn_init" ) H_Filter_TX_8_128[OFDM_N_MAX/4-OFDM_L_MAX/4+1]=
{
#include "h8_fn12_128.dat"
};

// ****** 64  (TX) ** (here only TX filter is used in Project)
complex_float segment ( "seg_extn_init" ) H_Filter_RX_8_64[OFDM_N_MAX/8-OFDM_L_MAX/8+1]=
{
#include "h8_fn12_64.dat"
};
complex_float segment ( "seg_extn_init" ) H_Filter_TX_8_64[OFDM_N_MAX/8-OFDM_L_MAX/8+1]=
{
#include "h8_fn12_64.dat"
};
// *** 64 RX double length (128-L+1)
complex_float segment ( "seg_extn_init" ) H_Filter_RX_8_128_8[128-8+1]=
{
#include "h8rx_128_8.dat"
};

//********************************* BW 8r kHz ** (FN=12 kHz) ***********************************
// ****** 128 **
complex_float segment ( "seg_extn_init" ) H_Filter_RX_8r_fn12_128[OFDM_N_MAX/4-OFDM_L_MAX/4+1]=
{
#include "h8r_fn12_128_v1.dat"
};
complex_float segment ( "seg_extn_init" ) H_Filter_TX_8r_fn12_128[OFDM_N_MAX/4-OFDM_L_MAX/4+1]=
{
#include "h8r_fn12_128_v1.dat"
};



//********************************* BW 4r kHz ** (FN=6 kHz) ***********************************
// ****** 64  (TX) ** (here only TX filter is used in Project)
complex_float segment ( "seg_extn_init" ) H_Filter_TX_4r_fn6_64[OFDM_N_MAX/8-OFDM_L_MAX/8+1]=
{
//#include "h4r_fn6_64_v3.dat"
#include "h4r_fn6_64_v7.dat"
};
// *** 64 RX double length (128-L+1)
complex_float segment ( "seg_extn_init" ) H_Filter_RX_4r_fn6_128_8[128-8+1]=
{
#include "h4rx_fn6_128_8.dat"
};


	
#endif
