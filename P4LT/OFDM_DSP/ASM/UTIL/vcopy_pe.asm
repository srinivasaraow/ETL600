
/******************************************************************************
 *
 * PROJECT     : DRS7 COFDM modem  
 * FILE        : $Workfile: vcopy_pe.asm $
 * COPYRIGHT   : 2004, TL consulting GmbH
 * DESCRIPTION : Copies a vector to another vector, both lie in external memory
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/vcopy_pe.asm $
 * 
 * 1     19.01.05 15:38 Chchlee
 * 
 * 2     30.09.04 8:27 Leeb02
 *  
 * 1     28/01/04 vgp
 * Initial version
 *****************************************************************************/


#include <asm_sprt.h>


.global _vcopy_PE;
.global _vcopy_EP;

		.segment/pm   seg_pmco;

/*****************************************************
*
* void vcopy_PE( float pm R[], float X[], int L );
*
* Operation : R = X;
*
*****************************************************/
_vcopy_PE:
        leaf_entry;

        i8 = r4;               /* Address of R[] */
        i4 = r8;               /* Address of X[] */
        r12 = r12-1, i4 = r8;

        r0 = dm(i4,m6);
        lcntr = r12, do cp_lp until lce;
cp_lp:     r0 = dm( i4, m6 ), pm( i8, m14 ) = r0;
        pm( i8, m14 ) = r0;

        leaf_exit;

_vcopy_PE.end:

/*****************************************************
*
* void vcopy_EP( float R[], float pm X[], int L );
*
* Operation : R = X;
*
*****************************************************/
_vcopy_EP:
        leaf_entry;

        i4 = r4;               /* Address of R[] */
        i8 = r8;               /* Address of X[] */
        r12 = r12-1, i4 = r8;

        r0 = pm(i8,m14);
        lcntr = r12, do cp_lp1 until lce;
cp_lp1:     dm( i4, m6 ) = r0, r0 = pm( i8, m14 );
        dm( i4, m6 ) = r0;

        leaf_exit;

_vcopy_EP.end:

	.endseg;
