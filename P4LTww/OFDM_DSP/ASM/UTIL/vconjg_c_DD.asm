/******************************************************************************
 *
 * PROJECT     : DRS7 COFDM Modem
 * FILE        : $Workfile: vconjg_c_DD.asm $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : Calculate conjugate of a complex vector
 * OPERATION   : R = CONJ( X )
 * PROTOTYPE   : void vconjg_c_DD( complex_float    R[],
 *                                complex_float    X[],
 *                                int              L ); // number of complex elements
 * ASSUMPTIONS : L is even, R[] and X[] starts from even address
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/vconjg_c_DD.asm $
 * 
 * 1     19.01.05 15:38 Chchlee
 * 
 * 2     30.09.04 8:27 Leeb02
 * 
 * 1     13/07/03 vgp
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

.global _vconjg_c_DD;
.global _vconjg_c_DD.end;

		.segment/pm   seg_pmco;

_vconjg_c_DD:
      leaf_entry;

      r0 = i3; puts = r0;
      r0 = m4; puts = r0;

      m4 = 2;

      i4 = r4;               /* Address of R[] */
      i3 = r8;               /* Address of X[] */
      r12 = lshift r12 by -1;               /* r12 = num of elements/2 */ 

      s0 = -1.0;
      f0 = 1.0;
      bit set MODE1 PEYEN;       // SIMD Enable 
      nop;

      lcntr = r12, do lp1 until lce;
         f8 = dm(i3,m4);
         f8 = f8 * f0,  f4 = dm(i3,m4);
         f4 = f4 * f0,  dm(i4,m4) = f8;
lp1:     dm(i4,m4) = f4;


      bit clr MODE1 PEYEN;       // SIMD Disable 
      r0 = r0 - r0;

      i3 = gets( 2 );
      m4 = gets( 1 );
         
      leaf_exit;

_vconjg_c_DD.end:

.endseg;
