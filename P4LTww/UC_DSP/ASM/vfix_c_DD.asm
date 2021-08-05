
//---------------------------------------------------------------------------
// COPYRIGHT:  (c) 2004 TL-Consulting GmbH, Frankfurt am Main 
//
// MODULE       $Workfile: vfix_c_DD.asm $
//
// PROJECT:    P4LT system board of ETL600 on DRS7 board
//
// COMPILER    VisualDSP 3.0, service pack 1
//
// DATE        04-FEB-2004
//
// AUTHOR:     Dr. Vladimir Potapov
//  
// ABSTRACT    Vector float to fixed (with scaling)
// OPERATION   R = fix( pow(2,exp_scale)*X )
// PROTOTYPE   void vfix_c_DD( int      R[],
//                             complex_float    X[],
//                             int      exp_scale,
//                             int      L );
//                  
// PREMISES:   
//
// REMARKS:    
//  
// HISTORY:    04.02.04  Initial Version, vgp
//
/*----------------------------------------------------------------------------
 * 
 * $Log: /Source/P4LT Prototype 2/UC_DSP/ASM/vfix_c_DD.asm $
 * 
 * 1     20.01.05 8:32 Chchlee
 * 
 * 1     28.04.04 17:19 Leeb02
 * 
 * 1     28.04.04 17:04 Leeb02
 * 
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

		.segment/pm   seg_pmco;

_vfix_c_DD:

   leaf_entry;

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

   i2 = gets( 1 );

   leaf_exit;
_vfix_c_DD.end:

.endseg;
