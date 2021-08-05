
/******************************************************************************
 *
 * PROJECT     : DRS7 COFDM modem  
 * FILE        : $Workfile: vcopy_EE.asm $
 * COPYRIGHT   : TL consulting GmbH
 * DESCRIPTION : Copies a vector to another vector, both lie in external memory
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/vcopy_EE.asm $
 * 
 * 1     19.01.05 15:38 Chchlee
 * 
 * 2     30.09.04 8:27 Leeb02
 *  
 * 1     10/07/03 vgp
 * Initial version
 *****************************************************************************/


#include <asm_sprt.h>


.global _vcopy_EE;
.global _vcopy_ED;
.global _vcopy_DE;

		.segment/pm   seg_pmco;

/*****************************************************
*
* void vcopy_EE( float R[], float X[], int L );
*
* Operation : R = X;
*
*****************************************************/
_vcopy_EE:
_vcopy_ED:
_vcopy_DE:
        leaf_entry;

        r0 = i2;               /* Save registers */
        puts = r0;

        i2 = r4;               /* Address of R[] */
        i4 = r8;               /* Address of X[] */

        lcntr = r12, do cp_lp until lce;
           r0 = dm( i4, m6 );
cp_lp:     dm( i2, m6 ) = r0;

        i2 = gets( 1 );        /* Restore registers */

        leaf_exit;

_vcopy_EE.end:
_vcopy_ED.end:
_vcopy_DE.end:

	.endseg;
