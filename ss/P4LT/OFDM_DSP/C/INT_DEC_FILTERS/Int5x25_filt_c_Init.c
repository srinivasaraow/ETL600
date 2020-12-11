/******************************************************************************
 *
 * PROJECT     : DRS7 COFDM Modem for 2116x
 * FILE        : $Workfile: Int5x25_filt_c_Init.c $
 * COPYRIGHT   : 2006 by TL Consulting GmbH.
 * DESCRIPTION : Initialize 25 taps interpolation filter by 5 array of delays
 * OPERATION   : 
 * PROTOTYPE   : void Int5x25_filt_c_Init(complex_float *Delay_Line)
 *
 * ASSUMPTIONS : 
 *
 * REVISION HISTORY:
 * COMPLEXITY : 
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source HSD600/P4LT/OFDM_DSP/C/INT_DEC_FILTERS/Int5x25_filt_c_Init.c $
 * 
 * 1     5.04.06 15:34 Chhamaa
 * 
 * 2     17.03.06 14:41 Chhamaa
 * 4   20-01-06 PL created 5x25 based on 4x24
 * 3   12-10-04 vgp Polyphase filter unitialization
 * 2	 22-06-04  PL  implemented in ETL600
 * 1     27-10-03  VGP
 * Initial version
 * 
 *****************************************************************************/
#include <complex.h>
#include "../../h/util/speeddsp.h"
#include "../../h/int_dec_filters/INTDEC_defs.h"

//==========================================
//void Int5x25_filt_c_init(complex_float *Delay_Line)
void segment ( "seg_ext_pmco" ) Int5x25_filt_c_init(complex_float *Delay_Line)
{
   register complex_float **fc_ptr;
   
    vset_i_D((int *)Delay_Line,0,2*(INTFILT_5_LENGTH/INTFILT_5_POLYNUMBER+1));
    fc_ptr = (complex_float **)(((int *)Delay_Line)+2*(INTFILT_5_LENGTH/INTFILT_5_POLYNUMBER+1)-1);
    *fc_ptr = Delay_Line +(INTFILT_5_LENGTH/INTFILT_5_POLYNUMBER-1);      
   
}
//========================================


