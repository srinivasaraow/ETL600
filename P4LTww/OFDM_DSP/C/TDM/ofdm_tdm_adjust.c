/*-------------------------------------------------------------------------*
 * $Workfile: ofdm_tdm_adjust.c $
 * Part of      : ETL600
 * Language     : C
 * Created by   : Vladimir Potapov
 * Remarks      :  
 * Purpose      : 
 * Copyright    : TL Consulting GmbH, 2004
 *-------------------------------------------------------------------------*/
/* $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/TDM/ofdm_tdm_adjust.c $
 * 
 * 1     19.01.05 15:42 Chchlee
 * 
 * 1     28.09.04 18:02 Maag01
 * Initial version
*/
#include <stdlib.h>

#include "../../h/p4lt_tdm/tdm_bus_io.h"
#include "../../h/p4lt_tdm/p4lt_tdm_defs.h"
#include "../../h/util/speeddsp.h"
#include "../../h/util/util.h"

#include "../../h/tdm/ofdm_tdm_externs.h"
#include "../../h/ofdm_externs.h"

#define OFF 0
#define ON  1

void segment ( "seg_ext_pmco" ) OFDM_TDM_Adjust()
{
 
// initilize TDM pointers 
   out_first_bptr = &ofdm_sig_tx_buffer[0];
   out_last_bptr  = &ofdm_sig_tx_buffer[2*NL2Q+2*OFDM_IO_SIGNAL_CSAMPLES-1];
   vset_f_E((float *)&ofdm_sig_tx_buffer[0],0.0f,4*NL2Q+4*OFDM_IO_SIGNAL_CSAMPLES);
   out_bptr = out_first_bptr;
   out_cur_bptr = out_first_bptr;
   
   in_first_bptr = &ofdm_sig_rx_buffer[0];
   in_last_bptr  = &ofdm_sig_rx_buffer[2*NL2Q+2*OFDM_IO_SIGNAL_CSAMPLES-1];
   vset_f_E((float *)&ofdm_sig_rx_buffer[0],0.0f,4*NL2Q+4*OFDM_IO_SIGNAL_CSAMPLES);
   in_bptr = in_first_bptr;
   in_cur_bptr = in_first_bptr;

}

