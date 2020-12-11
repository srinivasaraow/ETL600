/******************************************************************************
 *
 * PROJECT     : ETL600  
 * FILE        : $Workfile: TCM_Init_MQAM_Encoder.c $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : Initialize TCM Codec using carrier's modulation vector
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source HSD600/P4LT/OFDM_DSP/C/TCM/TCM_Init_MQAM_Encoder.c $
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

int segment ( "seg_ext_pmco" ) TCM_Init_MQAM_Encoder(int *Tmp_vec)
{

    int TCM_bitcalc(int *d2alloc, int Shaping_Flag, int *Bit_vec, float *C_Unsorted, int *Index, int len2d, int calc_Energy_Flag);
    int TCM_Init_Encoder(int *d2alloc_tx, int len2d_tx);
    int   Scrambler_Init();
    void  TCM_PCM_Gen_Init();


    // get codec main  parameters
    if ( TCM_bitcalc(Tmp_vec, Shaping_Flag_tx, Bits_in_SC_TX, C_TX, QAM_Index_TX, M_TX, 1) == -1) return -1;

	// init TCM encoder
    if ( TCM_Init_Encoder(Tmp_vec, M_TX) == -1 ) return -1;
            
    // Scrambler initialization
    if ( Scrambling == ON ) {
       Scrambler_Init();
    }

    // Init BER tester
    if ( BER_Meter == ON ) {
       PCM_Generator = ON;
    }
    
    // Init PCM generator
    if (PCM_Generator == ON ) {
       TCM_PCM_Gen_Init();
    }
    
    return 0;
}

