
/******************************************************************************
 *
 * PROJECT     : DRS7 COFDM modem  
 * FILE        : $Workfile: vcopy_s_ED.asm $
 * COPYRIGHT   : 2003 TL Consulting GmbH.
 * DESCRIPTION : Copies a vector to another vector
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/vcopy_s_ED.asm $
 * 
 * 1     19.01.05 15:38 Chchlee
 * 
 * 2     30.09.04 8:27 Leeb02
 *  
 * 1     10/07/03 vgp
 * Initial version
 * 2     09/05/04 vgp
 * r1 instead of f1
 *****************************************************************************/

#include <asm_sprt.h>


.global _vcopy_s_ED;
.global _vcopy_s_EE;
.global _vcopy_s_DE;

		.segment/pm   seg_pmco;
/*****************************************************
*
* void vcopy_s_EE( float R[], int i, float X[], int j, int L );
* void vcopy_s_ED( float R[], int i, float X[], int j, int L );
* void vcopy_s_DE( float R[], int i, float X[], int j, int L );
*
* Operation : R = X;
*
*****************************************************/
_vcopy_s_ED:
_vcopy_s_EE:
_vcopy_s_DE:

      leaf_entry;

      r0 = i2; puts = r0;    /* Save registers */
	r0 = m2; puts = r0;

      i2 = r4;               /* Address of R[] */
      m2 = r8;               /* Stride of R[] */
      i4 = r12;              /* Address of X[] */
      m4 = reads( 1 );       /* Stride of X[] */
      r0 = reads( 2 );       /* Number of elements to copy */

      lcntr = r0, do cp_lp until lce;
         r1 = dm( i4, m4 );
cp_lp:   dm( i2, m2 ) = r1;

      i2 = gets( 2 );        /* Restore registers */
      m2 = gets( 1 );

      leaf_exit;

_vcopy_s_ED.end:
_vcopy_s_EE.end:
_vcopy_s_DE.end:

.endseg;
