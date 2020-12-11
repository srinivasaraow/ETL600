/******************************************************************************
 *
 * PROJECT     : DRS7 COFDM modem  
 * FILE        : $Workfile: vmul_c_EEE.asm $
 * COPYRIGHT   : 2003 TL Consulting GmbH.
 * DESCRIPTION : Multiplies two vectors
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/vmul_c_EEE.asm $
 * 
 * 1     19.01.05 15:38 Chchlee
 * 
 * 3     30.09.04 8:27 Leeb02
 * 
 * 1     10/07/03 vgp
 * Initial version
 * 2     20/09/04 vgp
 * save r2
 *****************************************************************************/

#include <asm_sprt.h>


.global _vmul_c_EEE;
.global _vmul_c_EED;
.global _vmul_c_EDE;
.global _vmul_c_DEE;
.global _vmul_c_DED;
.global _vmul_c_DDE;
.global _vmul_c_EDD;

.global _vmul_c_EEE.end;
.global _vmul_c_EED.end;
.global _vmul_c_EDE.end;
.global _vmul_c_DEE.end;
.global _vmul_c_DED.end;
.global _vmul_c_DDE.end;
.global _vmul_c_EDD.end;


		.segment/pm   seg_pmco;

/*****************************************************
*
* void vmul_c_EEE( complex R[], complex X[], complex Y[], int L );
*
* Operation : R = X * Y;
*
*
*****************************************************/
_vmul_c_EEE:
_vmul_c_EED:
_vmul_c_EDE:
_vmul_c_DEE:
_vmul_c_DED:
_vmul_c_DDE:
_vmul_c_EDD:

      leaf_entry;

      r0 = i1; puts = r0;  /* Save registers */
      r0 = i2; puts = r0;
      r0 = i3; puts = r0;
      puts = r2;
      puts = r13;

      i2 = r4;             /* Address of R[] */
      i3 = r8;             /* Address of X[] */
      i1 = r12;            /* Address of Y[] */
      r2 = reads( 1 );     /* Number of elements */

      f0  = dm( i3, m6 );  /* x.re */
      f1  = dm( i3, m6 );  /* x.im */
      f4  = dm( i1, m6 );  /* y.re */
      lcntr = r2, do mul_lp until lce;
         f13 = f0 * f4, f8 = dm( i1, m6 );         /* x.re * y.re */
         f12 = f1 * f8;                            /* x.im * y.im */
         f12 = f13 - f12;                          /* x.re*y.re - x.im*y.im */
         dm( i2, m6 ) = f12;
         f8 = f0 * f8, f0 = dm( i3, m6 );          /* x.re*y.im */
         f4 = f4 * f1, f1 = dm( i3, m6 );          /* x.im*y.re */
         f12 = f8 + f4, f4 = dm( i1, m6 );         /* x.re*y.im + x.im*y.re */
mul_lp:  dm( i2, m6 ) = f12;

      i1  = gets( 5 );       /* Restore registers */
      i2  = gets( 4 );
      i3  = gets( 3 );
      r2  = gets( 2 );
      r13 = gets( 1 );

      leaf_exit;

_vmul_c_EEE.end:
_vmul_c_EED.end:
_vmul_c_EDE.end:
_vmul_c_DEE.end:
_vmul_c_DED.end:
_vmul_c_DDE.end:
_vmul_c_EDD.end:

.endseg;
