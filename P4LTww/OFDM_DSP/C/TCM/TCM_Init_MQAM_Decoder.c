/******************************************************************************
 *
 * PROJECT     : ETL600  
 * FILE        : $Workfile: TCM_Init_MQAM_Decoder.c $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : Initialize TCM Codec using carrier's modulation vector
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source HSD600/P4LT/OFDM_DSP/C/TCM/TCM_Init_MQAM_Decoder.c $
 * 
 * 3     5.04.06 15:28 Chhamaa
 * 
 * 3     17.03.06 14:41 Chhamaa
 * 
 * 1     19.01.05 15:42 Chchlee
 * 
 * 3     28.09.04 17:45 Maag01
 *
 * Initial version
 *       22.04.2004 vgp
 *****************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>

#include "../../h/tcm/TCM_Codec.h"
#include "../../h/tcm/TCM_Codec_externs.h"
#include "../../h/util/speeddsp.h"
#include "../../h/ofdm_defs.h"
#include "../../h/ofdm_externs.h"



//========================================

int segment ( "seg_ext_pmco" ) TCM_Init_MQAM_Decoder(int *Tmp_vec)
{

    int TCM_bitcalc(int *d2alloc, int Shaping_Flag, int *Bit_vec, float *C_Unsorted, int *Index, int len2d, int calc_Energy_Flag);
    int TCM_Init_Decoder(int *d2alloc_rx, int len2d_rx);
    int   Descrambler_Init();
    void  TCM_BER_Tester_Init(unsigned int *, int );


    // get codec main  parameters
    if ( TCM_bitcalc(Tmp_vec, Shaping_Flag_rx, Bits_in_SC_RX, C_RX, QAM_Index_RX, M_RX,1) == -1) return -1;

	// init TCM decoder
    if ( TCM_Init_Decoder(Tmp_vec, M_RX) == -1 ) return -1;
            
    // Descrambler initialization
    if ( Scrambling == ON ) {
       Descrambler_Init();
    }

    // Init BER tester
    if ( BER_Meter == ON ) {
       PCM_Generator = ON;
       TCM_BER_Tester_Init(BER_Buffer,OFDM_Num_32b_Words_RX);
    }
        
    return 0;
}

