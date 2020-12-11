
/******************************************************************************
 *
 * PROJECT     : 
 * FILE        : $Workfile: vdivz_f_DDP.asm $
 * COPYRIGHT   : 2004 TL Consulting GmbH
 * DESCRIPTION : Divide two vectors
 * OPERATION   : R = X ./ Y
 * PROTOTYPE   : void vdivz_f_DDP( float    R[],
 *                                 float    X[],
 *                                 float pm Y[],
 *                                 int      N );
 * ASSUMPTIONS : none.
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/vdivz_f_DDP.asm $
 * 
 * 1     19.01.05 15:38 Chchlee
 * 
 * 2     30.09.04 8:27 Leeb02
 * 1     26.04.04 vgp
 * Initial version
 * 2     08.05.04 vgp
 *   r0 = r0 - r0 instead f0 = f0 - f0 which can produce NAN
 * 
 *****************************************************************************/

#include <asm_sprt.h>

.global _vdivz_f_DDP;

.segment /pm seg_pmco;

_vdivz_f_DDP:
        leaf_entry;

        puts = r2;
        r0 = i2; puts = r0;

        i2  = r4;               // Address of R[]
        i4  = r8;               // Address of X[]
        i12 = r12;              // Address of Y[]
        r2  = reads( 1 );       // N
        f8  = 2.0;

        r0 = r0 - r0, f12 = pm( i12, m14 );
        lcntr = r2, do div_lp until lce;
		   f12 = pass f12, f4 = dm( i4, m6 );
		   if ne f0 = recips f12;
           f12 = f0 * f12;
           f4 = f0 * f4, f0 = f8 - f12;
           f12 = f0 * f12;
           f4 = f0 * f4, f0 = f8 - f12;
           f12 = f0 * f12;
           f4 = f0 * f4, f0 = f8 - f12, f12 = pm( i12, m14 );
           f0 = f0 * f4;
div_lp:    r0 = r0 - r0, dm(i2,m6) = f0;

        r2 = gets( 2 );
		i2 = gets( 1 );        

        leaf_exit;
_vdivz_f_DDP.end:

.endseg;
