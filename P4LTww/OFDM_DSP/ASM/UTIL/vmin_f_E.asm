
/******************************************************************************
 *
 * PROJECT     : ETL600
 * FILE        : $Workfile: vmin_f_E.asm $
 * COPYRIGHT   : 2004, TL Consulting GmbH.
 * DESCRIPTION : Vector minimum
 * OPERATION   : r = min( X )
 * PROTOTYPE   : float vmin_f_E( float    X[],
 *                               int      L );
 * ASSUMPTIONS : none.
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/vmin_f_E.asm $
 * 
 * 1     19.01.05 15:38 Chchlee
 * 
 * 2     30.09.04 8:27 Leeb02
 * 
 * 1     22.04.04 vgp
 * Initial version
 * 
 *****************************************************************************/

#include <asm_sprt.h>

.global _vmin_f_E;

//		.segment/pm   seg_pmco;
		.segment/pm   seg_ext_pmco;


_vmin_f_E:
        leaf_entry;

        i4 = r4;               /* Address of X[] */

        f0 = dm( i4, m6 );
        f4 = f0;
        lcntr = r8, do lp until lce;
lp:       f0 = min( f0, f4 ), f4  = dm( i4, m6 );


        leaf_exit;

_vmin_f_E.end:

.endseg;
