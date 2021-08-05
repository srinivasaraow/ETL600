/******************************************************************************
 *
 * PROJECT     : ETL600  
 * FILE        : $Workfile: TCM_Init_Decoder.c $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : Initialize TCM Codec using carrier's modulation vector
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/TCM/TCM_Init_Decoder.c $
 * 
 * 1     19.01.05 15:41 Chchlee
 * 
 * 3     28.09.04 17:45 Maag01
 *
 * Initial version
 *       08.09.2003 vgp
 * Independent TX/RX directions
 *       13.01.2004 vgp
 * Short word storage and addressing for RX and TX bit buffers
 *       18.01.2004 vgp
 * Short word storage and addressing for d2alloc_tx[] d2alloc_rx[] buffers
 *       18.01.2004 vgp
 * Scrambler/descrambler initialization
 *       23.01.2004 vgp
 * Seed value initialization for PCM bits internal generation
 *       23.01.2004 vgp
 * Execution from SDRAM
 *       06.04.04 vgp
 * 22.04.04 vgp
 *****************************************************************************/
 
#include <stdlib.h>
#include <math.h>
#include <complex.h>

#include "../../h/tcm/TCM_Codec.h"
#include "../../h/tcm/TCM_Codec_externs.h"
#include "../../h/util/speeddsp.h"
#include "../../h/ofdm_defs.h"
#include "../../h/util/util.h"


//========================================

int segment ( "seg_ext_pmco" ) TCM_Init_Decoder(int *D2alloc_rx, int len2d_rx)
{
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	
//!!!!  d2alloc_rx[] array must reside INTERNAL SRAM memory
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	
    
    	
// check RX definitions and parameters
    if ( len2d_rx < 4 ) return -1;           // LEN_2D must be not less than 4
    if ( len2d_rx & 0x1 ) return -1;         // LEN_2D must be even
    
	if ( (TCM_Shaping_Flag & 0xfffffffe) != 0 ) return -1;
	else OFDM_Shaping_Flag_RX = TCM_Shaping_Flag;

	if ( TCM_Code_Bits != (3*len2d_rx)/2 - CRC_LEN - TERM_BITS ) return -1;
    else OFDM_Code_Bits_RX    = TCM_Code_Bits;

    if ( (TCM_Gray_Bits < 0) || (TCM_Gray_Bits > len2d_rx*(QAM_ORDER_MAX_TX-4)) ) return -1;
	else OFDM_Gray_Bits_RX    = TCM_Gray_Bits;
    
	if ( TCM_QAM_Energy <= 0.0f ) return -1;
	else OFDM_QAM_Energy_RX   = TCM_QAM_Energy; 
	
	if ( TCM_Shaping_Bits < 0 ) return -1;
	if (TCM_Shaping_Flag == 1) {
		if ( TCM_Shaping_Bits > len2d_rx  ) return -1;
	} else {
		if ( TCM_Shaping_Bits > 2*len2d_rx ) return -1;
	}
    OFDM_Shaping_Bits_RX = TCM_Shaping_Bits;

    if ( TCM_Num_Inf_Bits != TCM_Code_Bits + TCM_Gray_Bits + TCM_Shaping_Bits )  return -1;
	else OFDM_Num_Bits_RX = TCM_Num_Inf_Bits;
	
    // test that required number of bits will fit in to buffer
    if ( OFDM_Num_Bits_RX > BIT_BUFFER_LENGTH_RX-CRC_LEN ) return -1; // not enough space in bit buffers 

    OFDM_Num_32b_Words_RX = OFDM_Num_Bits_RX >> 5; // whole number of inf. 32bit words TCM scheme
    
    // get bit mask for RX bit buffer packed in 32bit words    
    PCM_32b_Mask_RX = 0xffffffff << (32 - ( OFDM_Num_Bits_RX & 0x1f ));
    if ( PCM_32b_Mask_RX == 0 ) PCM_32b_Mask_RX = 0xffffffff;
    else OFDM_Num_32b_Words_RX++;

    // init pointers for PCM buffer
    Out_PCM_Ptr = Out_PCM_Buffer;

	// store modulation allocation using 16bit addressing mode
	vcopy_EE((float *)(2*(unsigned int)d2alloc_rx), (float *)D2alloc_rx, len2d_rx);
    
    return 0;
}

