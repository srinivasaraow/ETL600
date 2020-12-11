/******************************************************************************
 *
 * PROJECT     : ETL600
 * FILE        : $Workfile: TCM_Shaping_Decoder.c $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : TCM shaping decoder
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/TCM/TCM_Shaping_Decoder.c $
 * 
 * 1     19.01.05 15:42 Chchlee
 * 
 * 3     28.09.04 17:45 Maag01
 * 
 * 
 * Initial version
 *       08.09.2003 vgp
 * Independent TX/RX directions
 *       13.01.2004 vgp
 * Input/output parameter revision 
 *       17.01.2004 vgp
 * Memory usage reduction
 *       17.01.2004 vgp
 *****************************************************************************/
#include <complex.h>

#include "../../h/tcm/TCM_Codec.h"

void TCM_Shaping_Decoder(int *d2alloc, int len2d) 
{

    extern int           OFDM_Shaping_Flag_RX;     // shaping flag: 1 - on, 0 - off
	extern int           *RX_Shaping_Bits_Ptr;  // points information bits to be decoded by shaping code
    
    void TCM_Decoder_Vcopy_SPbits_DD( int R[],  int *, int L );
    void TCM_Decoder_Vget_SPbits_DD(  int R[],  int *, int L );

	if ( OFDM_Shaping_Flag_RX == 0 ) {
// shaping is off
        TCM_Decoder_Vcopy_SPbits_DD(RX_Shaping_Bits_Ptr, d2alloc, len2d);
   	} else {
// shaping is on
        TCM_Decoder_Vget_SPbits_DD(RX_Shaping_Bits_Ptr, d2alloc, len2d);
   	}
}

