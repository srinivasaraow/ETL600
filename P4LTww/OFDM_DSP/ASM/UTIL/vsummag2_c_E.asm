
/******************************************************************************
 *
 * PROJECT     : DRS7  
 * FILE        : $Workfile: vsummag2_c_E.asm $
 * COPYRIGHT   : 2004 TL Consulting GmbH
 * DESCRIPTION : Square vector
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/vsummag2_c_E.asm $
 * 
 * 1     19.01.05 15:39 Chchlee
 * 
 * 2     30.09.04 8:27 Leeb02
 * 1     19.11.03 vgp
 * Initial version
 *****************************************************************************/


#include <asm_sprt.h>

.global _vsummag2_c_E;

		.segment/pm   seg_pmco;

/*****************************************************
*
* float vsummag2_c_E( complex X[], int L );
*
* Operation : R = SUM( Xr*Xr + Xi*Xi )
*
*****************************************************/
_vsummag2_c_E:

    leaf_entry;

    i4   = r4;       /* i4 points to X[] */
				 
    r0 = r0 - r0,  f4 = dm( i4, m6 );    // Xr 
    lcntr = r8, do mlp until lce;
      f4 = f4 * f4, f8 = dm(i4,m6);      // f4 = Xr*Xr, f8 = Xi
      f0 = f0 + f4;                      // R += Xr*Xr
      f8 = f8 * f8;                      // f8 = Xi*Xi
mlp:  f0 = f0 + f8, f4 = dm(i4,m6);      // R += Xi*Xi, 

   leaf_exit;
_vsummag2_c_E.end:

.endseg;
