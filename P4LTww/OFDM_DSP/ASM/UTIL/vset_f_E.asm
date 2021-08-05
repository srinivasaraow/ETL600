
/******************************************************************************
 *
 * PROJECT     : DRS7 COFDM Modem
 * FILE        : $Workfile: vset_f_E.asm $
 * COPYRIGHT   : 2003 TL Consulting GmbH
 * DESCRIPTION : Set all elements in vector within external SDRAM to a scalar constant value
 * OPERATION   : R = s
 * PROTOTYPE   : void vset_f_E( float R[],
 *                              float s,
 *                              int   L );
 * ASSUMPTIONS : none.
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/vset_f_E.asm $
 * 
 * 1     19.01.05 15:38 Chchlee
 * 
 * 2     30.09.04 8:27 Leeb02
 * 
 * 1     10/07/03 vgp
 * Initial value
 * 2     09/05/04 vgp
 * r8 instead of f8, vgp
 * 
 *****************************************************************************/

#include <asm_sprt.h>

.global _vset_f_E;
.global _vset_i_E;

		.segment/pm   seg_pmco;
	
_vset_f_E:
_vset_i_E:
        leaf_entry;

        i4 = r4;               /* Address of first */

        lcntr = r12, do set_lp until lce;
set_lp:      dm( i4, m6 ) = r8;

        exit;
_vset_f_E.end:
_vset_i_E.end:

.endseg;
