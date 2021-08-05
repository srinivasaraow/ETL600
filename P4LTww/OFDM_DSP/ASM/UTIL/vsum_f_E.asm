
/******************************************************************************
 *
 * PROJECT     : ETL600 modem  
 * FILE        : $Workfile: vsum_f_E.asm $
 * COPYRIGHT   : 2004, TL consulting GmbH
 * DESCRIPTION : 
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/vsum_f_E.asm $
 * 
 * 1     19.01.05 15:39 Chchlee
 * 
 * 2     30.09.04 8:27 Leeb02
 *  
 * 1     02/03/04 vgp
 * Initial version
 * 2     08.05.04 vgp
 *   r0 = r0 - r0 instead f0 = f0 - f0 which can produce NAN
 *   r4 = r4 - r4 instead f4 = f4 - f4 which can produce NAN
 *****************************************************************************/


#include <asm_sprt.h>


.global _vsum_f_E;

//		.segment/pm   seg_pmco;
		.segment/pm   seg_ext_pmco;

/*****************************************************
*
* float vsum_f_E( float X[], int L );
*
* Operation : R = x[0]+x[1]+...+x[L-1];
*
*****************************************************/
_vsum_f_E:
        leaf_entry;


        r0 = r0 - r0, i4 = r4;           // R=0.0, i4 = address of X[] 

        r4 = r4 - r4;
        lcntr = r8, do sum_lp until lce;
sum_lp:     f0 = f0 + f4, f4 = dm(i4,m6); 
        f0 = f0 + f4;

        leaf_exit;

_vsum_f_E.end:

	.endseg;
