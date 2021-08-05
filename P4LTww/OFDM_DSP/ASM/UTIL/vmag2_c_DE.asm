
/******************************************************************************
 *
 * PROJECT     : COFDM Modem
 * FILE        : $Workfile: vmag2_c_DE.asm $
 * COPYRIGHT   : 2004 by TL-consulting GmbH.
 * DESCRIPTION : Vector float to fixed (with scaling)
 * OPERATION   : R[i] = X[i].re*X[i].re + X[i].im*X[i].im, i=0..L-1
 * ASSUMPTIONS : none.
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/vmag2_c_DE.asm $
 * 
 * 1     19.01.05 15:38 Chchlee
 * 
 * 2     30.09.04 8:27 Leeb02
 * 
 * 1     14.02.04 vgp
 * Initial version
 * 
 *****************************************************************************/

#ifdef __ADSP21160__
#include <def21160.h>
#endif
#ifdef __ADSP21161__
#include <def21161.h>
#endif

#include <asm_sprt.h>


/*****************************************************
*
* void vmag2_c_DE( float R[], complex_float X[], int L );
* void vmag2_c_ED( float R[], complex_float X[], int L );
*
* Operation : R[i] = X[i].re*X[i].re + X[i].im*X[i].im, i=0..L-1
*
*****************************************************/
.global _vmag2_c_DE;
.global _vmag2_c_ED;

		.segment/pm   seg_pmco;

_vmag2_c_DE:
_vmag2_c_ED:

   leaf_entry;

   r0 = i0; puts = r0;    /* Save registers */
        
   i0 = r4;               /* Address of result float vector R[L] */
   i4 = r8;               /* Address of input complex float vector X[L] */
				          /* r12 length of input vector in comlex samples */
   lcntr = r12, do vmag2_lp until lce;
     f12 = dm(i4,m6);
     f12 = f12 * f12, f4 = dm(i4,m6); 
     f4 = f4*f4; 
     f8 = f4 + f12;
vmag2_lp: dm(i0,m6) = f8;

   i0 = gets( 1 );

   leaf_exit;
_vmag2_c_DE.end:
_vmag2_c_ED.end:

.endseg;
