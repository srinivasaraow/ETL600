
/******************************************************************************
 *
 * PROJECT     : DRS7 COFDM Modem
 * FILE        : $Workfile: vset_sf_E.asm $
 * COPYRIGHT   : 2004 TL Consulting GmbH
 * DESCRIPTION : Set all elements in vector within external SDRAM to a scalar constant value
 * OPERATION   : R[i*stride] = scalar, i=0..L
 * PROTOTYPE   : void vset_sf_E( float R[],
 *                               int   stride
 *                               float scalar,
 *                               int   L );
 * ASSUMPTIONS : none.
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/vset_sf_E.asm $
 * 
 * 1     19.01.05 15:38 Chchlee
 * 
 * 2     30.09.04 8:27 Leeb02
 * 
 * 1     11/02/04 vgp
 * Initial value
 * 2     09/05/04 vgp
 * r12 instead of f12
 * 
 *****************************************************************************/

#include <asm_sprt.h>

.global _vset_sf_E;
.global _vset_si_E;

		.segment/pm   seg_pmco;
	
_vset_sf_E:
_vset_si_E:
        leaf_entry;
		r0 = m4; puts = r0;
		r0 = i4; puts = r0;

        i4 = r4;               /* Address of first */
		m4 = r8;
		r4 = reads(1);

        lcntr = r4, do set_lp until lce;
set_lp:      dm( i4, m4 ) = r12;

        m4 = gets(2);
        i4 = gets(1);
        exit;
_vset_sf_E.end:
_vset_si_E.end:

.endseg;
