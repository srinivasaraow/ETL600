/******************************************************************************
 *
 * PROJECT     : ETL600
 * FILE        : $Workfile: tcm_ber_tester_init.c $
 * COPYRIGHT   : 2004 TL Consulting GmbH
 * DESCRIPTION : TCM decoder
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/TCM/tcm_ber_tester_init.c $
 * 
 * 1     19.01.05 15:41 Chchlee
 * 
 * 3     28.09.04 17:45 Maag01
 * 
 * 
 * Initial version
 *       23.01.2004 vgp
*****************************************************************************/

#include "../../h/tcm/TCM_Codec.h"
#include "../../h/tcm/TCM_Codec_externs.h"
#include "../../h/util/util.h"

void segment ( "seg_ext_pmco" ) TCM_BER_Tester_Init(unsigned int *Tmp_Vec_ptr, int Len)
{
	unsigned int Saved_Seed;
	register int i;

	// save seed 
	Saved_Seed = DRS7_getseed();

	// test for startup state
	BER_Seed = INF_PCM_SEED;
	DRS7_setseed(BER_Seed);
	for (i=0; i<RX_DELAY_SYMB; i++)  DRS7_vrand_E((int *)Tmp_Vec_ptr, Len );
	BER_Seed = DRS7_getseed();

    // restore seed
    DRS7_setseed(Saved_Seed);
    
    // Init BER data
    Bit_Errors_Sum = 0;
    Block_Errors_Sum = 0;
    BER_Block_Counter = 0;
    
}

