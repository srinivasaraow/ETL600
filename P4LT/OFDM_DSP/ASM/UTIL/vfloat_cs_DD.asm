
/******************************************************************************
 *
 * PROJECT     : COFDM Modem
 * FILE        : $Workfile: vfloat_cs_DD.asm $
 * COPYRIGHT   : 2003 by TL-consulting GmbH.
 * DESCRIPTION : Complex vector fixed to float (with scaling)
 * OPERATION   : R[j] = float( pow(2,exp_scale)*X[i], i=i+stride )
 * PROTOTYPE   : void vfloat_cs_DD( complex_float  R[],
 *                                  int            X[],
 *                                  int complex_stride,
 *                                  int      exp_scale,
 *                                  int             L );
 * ASSUMPTIONS : none.
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/vfloat_cs_DD.asm $
 * 
 * 1     19.01.05 15:38 Chchlee
 * 
 * 3     30.09.04 8:27 Leeb02
 * 
 * 1     05.07.03 vgp
 * Initial version
 * 2     20.09.04 vgp
 *  save r2
 * 
 *****************************************************************************/

#ifdef __ADSP21160__
#include <def21160.h>
#endif
#ifdef __ADSP21161__
#include <def21161.h>
#endif

#include <asm_sprt.h>


.global _vfloat_cs_DD;

		.segment/pm   seg_pmco;

_vfloat_cs_DD:

   leaf_entry;

   puts = r2;
   r0 = i2; puts = r0;    /* Save registers */
   r0 = m2; puts = r0;
        
   i4 = r4;               /* Address of result complex_float vector R[L] */
   i2 = r8;               /* Address of input integer complex vector X[stride*L] */
   r12 = r12 + r12;       /* real stride */
   m4 = r12;
   r12 = reads( 1 );      /* scale */
   s12 = r12;             /* Copy scale from PEx to PEy */
   m2 = 2;

   r2 = reads( 2 );       /* length of output vector in comlex samples */

   bit set mode1 PEYEN;   /* Enter SIMD mode */
   m2 = 2;

   r2 = r2 - 1, r4 = dm( i2, m4 );
   lcntr = r2, do fix_lp until lce;
        f0 = float r4 by r12, f4 = dm( i2, m4 );
fix_lp: dm(i4,m2 ) = f0;
   f0 = float r4 by r12;
   dm(i4,m5) = f0;
   bit clr mode1 PEYEN;              /* Leave SIMD mode */
   nop;

   r2 = gets( 3 );
   i2 = gets( 2 );
   m2 = gets( 1 );

   leaf_exit;
_vfloat_cs_DD.end:


.endseg;
