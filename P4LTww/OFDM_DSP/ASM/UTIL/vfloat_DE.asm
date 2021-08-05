
/******************************************************************************
 *
 * PROJECT     : DRS7 COFDM modem project
 * FILE        : $Workfile: vfloat_DE.asm $
 * COPYRIGHT   : 2003 TL Consulting GmbH.
 * DESCRIPTION : Vector fixed to float (with scaling)
 * OPERATION   : R = float( X )
 * PROTOTYPE   : void vfloat_DE( float    R[],  // resides in DM memory
 *                               int      X[],  // resides in SDRAM memory
 *                               int      s,      // scaling factor
 *                               int      L );
 * ASSUMPTIONS : none.
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/vfloat_DE.asm $
 * 
 * 1     19.01.05 15:38 Chchlee
 * 
 * 3     30.09.04 8:27 Leeb02
 * 
 * 1     10/07/03 vgp
 * Initial version
 * 
 *****************************************************************************/

#include <asm_sprt.h>

.global _vfloat_DE;
.global _vfloat_ED;
.global _vfloat_DE.end;
.global _vfloat_ED.end;

		.segment/pm   seg_pmco;

_vfloat_DE:
_vfloat_ED:
        leaf_entry;

        puts = r2;
        r0 = i2; puts = r0;    /* Save registers */
        
        i4 = r4;               /* Address of R[] */
        i2 = r8;               /* Address of X[] */
        r2 = reads( 1 );       /* Number of elements */
                               /* r12 - scaling factor */

        f4 = dm( i2, m6 );
        lcntr = r2, do fix_lp until lce;
           f0 = float r4 by r12, f4 = dm( i2, m6 );
fix_lp:    dm( i4, m6 ) = r0;

        r2 = gets( 2 );
        i2 = gets( 1 );

        leaf_exit;

_vfloat_DE.end:
_vfloat_ED.end:

.endseg;
