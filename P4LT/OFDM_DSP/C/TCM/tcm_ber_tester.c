/******************************************************************************
 *
 * PROJECT     : ETL600
 * FILE        : $Workfile: tcm_ber_tester.c $
 * COPYRIGHT   : 2004 TL Consulting GmbH
 * DESCRIPTION : TCM decoder
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/TCM/tcm_ber_tester.c $
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

unsigned int segment ( "seg_ext_pmco" ) TCM_BER_Tester(unsigned int *InPCM32, int Len, int Mask, unsigned int *Tmp_Vec_ptr)
{
	unsigned int        Saved_Seed;
	register int        i;
	unsigned int        Trellis_bit_errors, Bits_in_block;
	float               Block_error_rate;
	register unsigned int tmp, rest;
	

	// save seed 
	Saved_Seed = DRS7_getseed();

	// load seed of previous block at the end of symbol
    DRS7_setseed(BER_Seed);

    // generate vector
    DRS7_vrand_E((int *)Tmp_Vec_ptr, Len);       // Len = OFDM_Num_32b_Words_RX
    
    // save BER seed
    BER_Seed =  DRS7_getseed();

    // mask out unused bits
    Tmp_Vec_ptr[Len-1] &= Mask;           // Mask = PCM_32b_Mask_RX;
    InPCM32[Len-1] &= Mask;

    // calculate Hamming distance of two bit vectors
    vxor_i_EEE((int *)Tmp_Vec_ptr, (int *)Tmp_Vec_ptr, (int *)InPCM32, Len);
    Trellis_bit_errors = vones_i_E((int *)Tmp_Vec_ptr, Len );
    
    // find out number of bits per block
    // for testing purpose don't use variable "OFDM_Num_Bits_RX"
    Bits_in_block = 0;
    tmp = 0x80000000;
    while ( tmp ) { 
        if ( (Mask & tmp) != 0 ) Bits_in_block++;
        tmp >>= 1;
    }
    Bits_in_block += 32*(Len-1);
    
    BER_Block_Counter++;
    Bit_Errors_Sum += Trellis_bit_errors;
    if (Trellis_bit_errors != 0) Block_Errors_Sum++;
    Block_error_rate = (float)Trellis_bit_errors/(float)Bits_in_block;
    BLER = (float)Block_Errors_Sum/(float)BER_Block_Counter;       
    if ( BER_Block_Counter != 1) {
        BER += (Block_error_rate - BER)/(float)BER_Block_Counter;
    } else {
        BER = Block_error_rate;
    }
    
    // restore seed
    DRS7_setseed(Saved_Seed);    

    if (Trellis_bit_errors != 0) return 1;
    else return 0;
}

