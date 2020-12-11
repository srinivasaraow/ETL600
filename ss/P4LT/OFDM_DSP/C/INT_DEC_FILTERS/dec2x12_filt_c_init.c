/******************************************************************************
 *
 * PROJECT     : DRS7 COFDM Modem for 2116x
 * FILE        : $Workfile: dec2x12_filt_c_init.c $
 * COPYRIGHT   : 2003 by TL Consulting GmbH.
 * DESCRIPTION : Initialize 12 taps decimation filter by 2 array of delays
 * OPERATION   : 
 * PROTOTYPE   : void Dec2x12_filt_c_init(complex_float *Delay_Line)
 *
 * ASSUMPTIONS : 
 *
 * REVISION HISTORY:
 * COMPLEXITY : 
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/INT_DEC_FILTERS/dec2x12_filt_c_init.c $
 * 
 * 1     19.01.05 15:39 Chchlee
 * 
 * 5     2.12.04 17:54 Chhamaa
 *
 * 3     12-10-04 vgp Polyphase filter unitialization
 * 2 	 23-06-04  PL  implemented in ETL600
 * 1     27-10-03  VGP
 * Initial version
 * 
 *****************************************************************************/

#include <complex.h>
#include "../../h/util/speeddsp.h"

#include "../../h/int_dec_filters/INTDEC_defs.h"

//==========================================
//void Dec2x12_filt_c_init(complex_float *Delay_Line)
void segment ( "seg_ext_pmco" ) Dec2x12_filt_c_init(complex_float *Delay_Line)
{
   register complex_float **fc_ptr;
   
    vset_i_D((int *)Delay_Line,0,2*(DECFILT_2_LENGTH+1));
    fc_ptr = (complex_float **)(((int *)Delay_Line)+2*(DECFILT_2_LENGTH+1)-1);
    *fc_ptr = Delay_Line+(DECFILT_2_LENGTH-1);   

}
//========================================
