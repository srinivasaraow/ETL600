
/******************************************************************************
 *
 * PROJECT     : DRS7 COFDM modem  
 * FILE        : $Workfile: vsum_i_E.asm $
 * COPYRIGHT   : TL consulting GmbH
 * DESCRIPTION : 
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/vsum_i_E.asm $
 * 
 * 1     19.01.05 15:39 Chchlee
 * 
 * 2     30.09.04 8:27 Leeb02
 *  
 * 1     10/07/03 vgp
 * Initial version
 *****************************************************************************/


#include <asm_sprt.h>


.global _vsum_i_E;

		.segment/pm   seg_pmco;

/*****************************************************
*
* int vsum_i_E( int X[], int L );
*
* Operation : R = x[0]+x[1]+...+x[L-1];
*
*****************************************************/
_vsum_i_E:
        leaf_entry;


        r0 = r0 - r0, i4 = r4;           // R=0, i4 = address of X[] 

        r4 = r4 - r4;
        lcntr = r8, do sum_lp until lce;
sum_lp:     r0 = r0 + r4, r4 = dm(i4,m6); 
        r0 = r0 + r4;

        leaf_exit;

_vsum_i_E.end:

	.endseg;
