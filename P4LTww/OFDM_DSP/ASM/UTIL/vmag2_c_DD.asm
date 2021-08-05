
/******************************************************************************
 *
 * PROJECT     : COFDM Modem
 * FILE        : $Workfile: vmag2_c_DD.asm $
 * COPYRIGHT   : 2004 by TL-consulting GmbH.
 * DESCRIPTION : Vector float to fixed (with scaling)
 * OPERATION   : R[i] = X[i].re*X[i].re + X[i].im*X[i].im, i=0..L-1, L >=2
 * ASSUMPTIONS : X[] starts at even address, L >= 2
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/vmag2_c_DD.asm $
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
* void vmag2_c_DD( float R[], complex_float X[], int L );
*
* Operation : R[i] = X[i].re*X[i].re + X[i].im*X[i].im, i=0..L-1, L >=2
*
*****************************************************/
.global _vmag2_c_DD;

		.segment/pm   seg_pmco;

_vmag2_c_DD:

   leaf_entry;

   r0 = i1; puts = r0;    /* Save registers */
        
   i1 = r4;               /* Address of result float vector R[L] */
   i4 = r8;               /* Address of input complex float vector X[L] */
				          /* r12 length of input vector in comlex samples */
   r4 = r12 - 1;
   bit set mode1 PEYEN;   // Enter SIMD mode & enable broadcast mode of i1
   m4 = 2;                /* SIMD starts here */

   f12 = dm( i4, m4 );
   lcntr = r4, do vmag2_lp until lce;
        f4 = f12 * f12, f12 = dm( i4, m4 ); ;
        f8 = s4; 
		f8 = f4 + f8;
vmag2_lp: dm(i1,m6) = f8;
   f4 = f12 * f12;
   bit clr mode1 PEYEN;   // Enter SISD mode & disable broadcast mode of i1
   f8 = s4;
   f8 = f4 + f8;
   dm(i1,m5) = f8;

   i2 = gets( 1 );

   leaf_exit;
_vmag2_c_DD.end:

.endseg;
