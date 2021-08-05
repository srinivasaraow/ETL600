/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: TXRX_FILT_globals.h $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : Global variables for OFDM TX and RX Filters 
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source MAS/P4LT/OFDM_DSP/H/TX_RX_FILTERS/TXRX_FILT_globals.h $
 * 
 * 12    24.07.08 15:24 Chhamaa
 * 
 * 7     5.04.06 15:28 Chhamaa
 * 
 * 7     17.03.06 14:41 Chhamaa
 * 
 * 5     16.09.05 9:42 Chalgyg
 * OFDM_DSP_V0_10 (1160905)
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
#include "h8r_fn12_128_v4.dat"
};
complex_float segment ( "seg_extn_init" ) H_Filter_TX_8r_fn12_128[OFDM_N_MAX/4-OFDM_L_MAX/4+1]=
{
#include "h8r_fn12_128_v4.dat"
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

//================================ Dez05/Jan06 ================================================
//********************************* BW 4 kHz ** (FN=4 kHz) ***********************************
// ****** 128  (TX) ** 
complex_float segment ( "seg_extn_init" ) H_fn4_bw4_128_tx[OFDM_N_MAX/4-OFDM_L_MAX/4+1]=
{
#include "h_fn4_bw4_128_tx1.dat" //length = N-N/8+1 = 128-16+1 = 113
};
// ****** 128  (RX) ** 
complex_float segment ( "seg_extn_init" ) H_fn4_bw4_128_rx[OFDM_N_MAX/4-OFDM_L_MAX/4+1]=
{
#include "h_fn4_bw4_128_tx1.dat" //length = N-N/8+1 = 128-16+1 = 113
};

//********************************* BW 3.5 kHz ** (FN=4 kHz) ***********************************
// ****** 128  (TX) ** 
complex_float segment ( "seg_extn_init" ) H_fn4_bw35_128_tx[OFDM_N_MAX/4-OFDM_L_MAX/4+1]=
{
#include "h_fn4_bw35_128_tx1.dat" //length = N-N/8+1 = 128-16+1 = 113
};
// ****** 128  (RX) ** 
complex_float segment ( "seg_extn_init" ) H_fn4_bw35_128_rx[OFDM_N_MAX/4-OFDM_L_MAX/4+1]=
{
#include "h_fn4_bw35_128_rx4.dat" //length = N-N/8+1 = 128-16+1 = 113
};

//********************************* BW 4 kHz ** (FN=6 kHz) ***********************************
// ****** 64  (TX) ** 
complex_float segment ( "seg_extn_init" ) H_fn6_bw4_64_tx[OFDM_N_MAX/8-OFDM_L_MAX/8+1]=
{
#include "h_fn6_bw4_64_tx3.dat" //length = N-N/8+1 = 64-8+1 = 57
};
// ****** 64 (128-L)  (RX) ** 
complex_float segment ( "seg_extn_init" ) H_fn6_bw4_64_rx128[128-8+1]=
{
#include "h_fn6_bw4_64_rx128_3.dat" //length = N-N/8+1 = 128-16+1 = 113
};

//********************************* BW 4 kHz ** (FN=4.8 kHz) ***********************************
// ****** 64  (TX) ** 
complex_float segment ( "seg_extn_init" ) H_fn48_bw4_64_tx[OFDM_N_MAX/8-OFDM_L_MAX/8+1]=
{
#include "h_fn48_bw4_64_tx2.dat" //length = N-N/8+1 = 64-8+1 = 57
};
// ****** 64 (128-L)  (RX) ** 
complex_float segment ( "seg_extn_init" ) H_fn48_bw4_64_rx128[128-8+1]=
{
#include "h_fn48_bw4_64_rx128_2.dat" //length = N-N/8+1 = 128-16+1 = 113
};

//********************************* BW 3.5 kHz ** (FN=6 kHz) ***********************************
// ****** 64  (TX) ** 
complex_float segment ( "seg_extn_init" ) H_fn6_bw35_64_tx[OFDM_N_MAX/8-OFDM_L_MAX/8+1]=
{
#include "h_fn6_bw35_64_tx2.dat" //length = N-N/8+1 = 128-16+1 = 113
};
// ****** 64 (128-L)  (RX) ** 
complex_float segment ( "seg_extn_init" ) H_fn6_bw35_64_rx128[128-8+1]=
{
#include "h_fn6_bw35_64_rx128_2.dat" //length = N-N/8+1 = 128-16+1 = 113
};

//********************************* BW 3.5 kHz ** (FN=4.8 kHz) ***********************************
// ****** 64  (TX) ** 
complex_float segment ( "seg_extn_init" ) H_fn48_bw35_64_tx[OFDM_N_MAX/8-OFDM_L_MAX/8+1]=
{
#include "h_fn48_bw35_64_tx2.dat" //length = N-N/8+1 = 64-8+1 = 57
};
// ****** 64 (128-L)  (RX) ** 
complex_float segment ( "seg_extn_init" ) H_fn48_bw35_64_rx128[128-8+1]=
{
#include "h_fn48_bw35_64_rx128_1.dat" //length = 128-8+1 = 121
};

//********************************* BW 3.5 kHz ** (FN=4.0 kHz) ***********************************
// ****** 64  (TX/RX) ** 
complex_float segment ( "seg_extn_init" ) H_fn4_bw35_64_tx[OFDM_N_MAX/8-OFDM_L_MAX/8+1]=
{
#include "h_fn4_bw35_64_tx1.dat" //length = N-N/8+1 = 64-8+1 = 57
};


//********************************* BW 8.0 kHz ** (FN=9.6 kHz) ***********************************
// ****** 128  (TX) ** 
complex_float segment ( "seg_extn_init" ) H_fn96_bw8_128_tx[OFDM_N_MAX/4-OFDM_L_MAX/4+1]=
{
#include "h_fn96_bw8_128_tx1.dat" //length = N-N/8+1 = 128-16+1 = 113
};
// ****** 128  (RX) ** 
complex_float segment ( "seg_extn_init" ) H_fn96_bw8_128_rx[128-16+1]=
{
#include "h_fn96_bw8_128_rx1.dat" //length = 128-16+1 = 113
};

//********************************* BW 7.5 kHz ** (FN=9.6 kHz) ***********************************
// ****** 128  (TX) ** 
complex_float segment ( "seg_extn_init" ) H_fn96_bw75_128_tx[OFDM_N_MAX/4-OFDM_L_MAX/4+1]=
{
#include "h_fn96_bw8_128_tx1.dat" //length = N-N/8+1 = 128-16+1 = 113
};
// ****** 128  (RX) ** 
complex_float segment ( "seg_extn_init" ) H_fn96_bw75_128_rx[128-16+1]=
{
#include "h_fn96_bw8_128_rx1.dat" //length = 128-16+1 = 113
};

//********************************* BW xx with FN=BW ***********************************
// ****** 256  (TX) ** 
complex_float segment ( "seg_extn_init" ) H_fnxx_bwxx_256_tx[OFDM_N_MAX/2-OFDM_L_MAX/2+1]=
{
#include "h_fnxx_bwxx_256_tx2.dat" //length = N-N/8+1 = 256-32+1 = 225
};
// ****** 256  (RX) ** 
complex_float segment ( "seg_extn_init" ) H_fnxx_bwxx_256_rx[OFDM_N_MAX/2-OFDM_L_MAX/2+1]=
{
#include "h_fnxx_bwxx_256_tx2.dat" //length = N-N/8+1 = 256-32+1 = 225
};

//================================ Mai-Aug08 ================================================
//********************************* BW 6 kHz with FN=BW ***********************************
// ******  64  (TX) ** 
complex_float segment ( "seg_extn_init" ) H_fn6_bw6_64_tx[OFDM_N_MAX/8-OFDM_L_MAX/8+1]=
{
#include "h_fn6_bw6_64_tx1.dat" //length = N-N/8+1 = 64-8+1 = 57
};
// ******  64  (RX) ** 
complex_float segment ( "seg_extn_init" ) H_fn6_bw6_64_rx[OFDM_N_MAX/8-OFDM_L_MAX/8+1]=
{
#include "h_fn6_bw6_64_tx1.dat" //length = N-N/8+1 = 64-8+1 = 57
};

// ****** 128  (TX) ** 
complex_float segment ( "seg_extn_init" ) H_fn6_bw6_128_tx[OFDM_N_MAX/4-OFDM_L_MAX/4+1]=
{
#include "h_fn6_bw6_128_tx1.dat" //length = N-N/8+1 = 128-16+1 = 113
};
// ****** 128  (RX) ** 
complex_float segment ( "seg_extn_init" ) H_fn6_bw6_128_rx[OFDM_N_MAX/4-OFDM_L_MAX/4+1]=
{
#include "h_fn6_bw6_128_tx1.dat" //length = N-N/8+1 = 128-16+1 = 113
};

// ****** 256  (TX) ** 
complex_float segment ( "seg_extn_init" ) H_fn6_bw6_256_tx[OFDM_N_MAX/2-OFDM_L_MAX/2+1]=
{
#include "h_fn6_bw6_256_tx3.dat" //length = N-N/8+1 = 256-32+1 = 225
};
// ****** 256  (RX) ** 
complex_float segment ( "seg_extn_init" ) H_fn6_bw6_256_rx[OFDM_N_MAX/2-OFDM_L_MAX/2+1]=
{
#include "h_fn6_bw6_256_tx3.dat" //length = N-N/8+1 = 256-32+1 = 225
};

// ****** 512  (TX) ** 
complex_float segment ( "seg_extn_init" ) H_fn6_bw6_512_tx[OFDM_N_MAX/2-OFDM_L_MAX/2+1]=
{
#include "h_fn6_bw6_512_tx1.dat" //length = N-N/8+1 = 256-32+1 = 225
};
// ****** 512  (RX) ** 
complex_float segment ( "seg_extn_init" ) H_fn6_bw6_512_rx[OFDM_N_MAX/2-OFDM_L_MAX/2+1]=
{
#include "h_fn6_bw6_512_tx1.dat" //length = N-N/8+1 = 256-32+1 = 225
};
#endif

