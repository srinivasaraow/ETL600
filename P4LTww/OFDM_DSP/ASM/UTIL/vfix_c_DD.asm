
/******************************************************************************
 *
 * PROJECT     : COFDM Modem
 * FILE        : $Workfile: vfix_c_DD.asm $
 * COPYRIGHT   : 2003 by TL-consulting GmbH.
 * DESCRIPTION : Vector float to fixed (with scaling)
 * OPERATION   : R = fix( pow(2,exp_scale)*X )
 * PROTOTYPE   : void vfix_c_DD( int      R[],
 *                               complex_float    X[],
 *                               int      exp_scale,
 *                               int      L );
 * ASSUMPTIONS : none.
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/vfix_c_DD.asm $
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


.global _vfix_c_DD;
.global _vfix_c_DD.end;
.global _vfix_cj_DD;
.global _vfix_cj_DD.end;

		.segment/pm   seg_pmco;

_vfix_c_DD:

   leaf_entry;

   puts = r2;
   r0 = i2; puts = r0;    /* Save registers */
        
   i4 = r4;               /* Address of result vector R[2L] */
   i2 = r8;               /* Address of input complex float vector X[L] */
   r2 = reads( 1 );       /* length of input vector in comlex samples */
   s12 = r12;             /* Copy scale from PEx to PEy */

   bit set mode1 PEYEN;              /* Enter SIMD mode */
   m4 = 2;                           /* SIMD starts here */

   r2 = r2 - 1, f4 = dm( i2, m4 );
   lcntr = r2, do fix_lp until lce;
        r0 = fix f4 by r12, f4 = dm( i2, m4 );
fix_lp: dm(i4,m4 ) = r0;
   r0 = fix f4 by r12;
   dm(i4,m5) = r0;
   bit clr mode1 PEYEN;              /* Leave SIMD mode */
   nop;

   r2 = gets( 2 );
   i2 = gets( 1 );

   leaf_exit;
_vfix_c_DD.end:

//=================================================

_vfix_cj_DD:

   leaf_entry;
   
   puts = r2;
   r0 = i2; puts = r0;    /* Save registers */
        
   i4 = r4;               /* Address of result vector R[2L] */
   i2 = r8;               /* Address of input complex float vector X[L] */
   r2 = reads( 1 );       /* length of input vector in comlex samples */
   s12 = r12;             /* Copy scale from PEx to PEy */

   f8 = 1.0;
   s8 = -1.0;
   bit set mode1 PEYEN;              /* Enter SIMD mode */
   m4 = 2;                           /* SIMD starts here */

   r2 = r2 - 1, f4 = dm( i2, m4 );
   lcntr = r2, do fix_cj_lp until lce;
        f4 = f8*f4;
        r0 = fix f4 by r12, f4 = dm( i2, m4 );
fix_cj_lp: dm(i4,m4 ) = r0;
   f4 = f8*f4;
   r0 = fix f4 by r12;
   dm(i4,m5) = r0;
   bit clr mode1 PEYEN;              /* Leave SIMD mode */
   nop;

   r2 = gets( 2 );
   i2 = gets( 1 );

   leaf_exit;
_vfix_cj_DD.end:

.endseg;
