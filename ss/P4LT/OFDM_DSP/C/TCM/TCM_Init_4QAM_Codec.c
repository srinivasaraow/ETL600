/******************************************************************************
 *
 * PROJECT     : ETL600  
 * FILE        : $Workfile: TCM_Init_4QAM_Codec.c $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : Initialize TCM Codec using carrier's modulation vector
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /branches/Source HSD600/P4LT/OFDM_DSP/C/TCM/TCM_Init_4QAM_Codec.c $
 * 
 * 3     5.04.06 15:28 Chhamaa
 * 
 * 3     17.03.06 14:41 Chhamaa
 * 
 * 1     19.01.05 15:41 Chchlee
 * 
 * 3     28.09.04 17:45 Maag01
 *
 * Initial version
 *       22.04.2004 vgp, PL
 *****************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>

#include "../../h/tcm/TCM_Codec.h"
#include "../../h/tcm/TCM_Codec_externs.h"
#include "../../h/OFDM_externs.h"
#include "../../h/util/speeddsp.h"
#include "../../h/ofdm_defs.h"
#include "../../h/util/util.h"



//========================================

int segment ( "seg_ext_pmco" ) TCM_Init_4QAM_Codec(int len2d, int *Tmp_vec)
// Tmp_vec - points array of 2*len2d words
{

    //int TCM_bitcalc(int *d2alloc, int Shaping_Flag, int *Bit_vec, float *C_Unsorted, int *Index, int len2d);
    int TCM_bitcalc(int *d2alloc, int Shaping_Flag, int *Bit_vec, float *C_Unsorted, int *Index, int len2d, int calc_Energy_Flag);
    int TCM_Init_Encoder(int *d2alloc_tx, int len2d_tx);
    int TCM_Init_Decoder(int *d2alloc_rx, int len2d_rx);
    void  TCM_Dummy_Gen_Init();


    // init QAM allocation table
    vset_i_E(Tmp_vec, 2, len2d);

    // get codec main  parameters
    if ( TCM_bitcalc(&Tmp_vec[len2d], 0, Tmp_vec, C_TX, QAM_Index_TX, len2d, 1) == -1) return -1;

	// init TCM encoder
    if ( TCM_Init_Encoder(&Tmp_vec[len2d], len2d) == -1 ) return -1;

	// init TCM decoder
    if ( TCM_Init_Decoder(&Tmp_vec[len2d], len2d) == -1) return -1;

    // Initialize Generator for QAM4 dummy
    TCM_Dummy_Gen_Init();
                
    return 0;
}

