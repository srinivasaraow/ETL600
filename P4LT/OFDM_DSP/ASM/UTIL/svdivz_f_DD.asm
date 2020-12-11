
/******************************************************************************
 *
 * PROJECT     : 
 * FILE        : $Workfile: svdivz_f_DD.asm $
 * COPYRIGHT   : 2004 TL Consulting GmbH
 * DESCRIPTION : Divide two vectors
 * OPERATION   : R = s ./ X
 * PROTOTYPE   : void svdivz_f_DD( float    R[],
 *                                float    s,
 *                                float    X[],
 *                                int      N );
 * ASSUMPTIONS : if x[i] == 0.0 then r[i] = 0.0, i=0,..,N-1
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/svdivz_f_DD.asm $
 * 
 * 1     19.01.05 15:38 Chchlee
 * 
 * 2     30.09.04 8:27 Leeb02
 * 
 * 1     26.04.04 vgp
 *   Initial version
 * 2     08.05.04 vgp
 *   r0 = r0 - r0 instead f0 = f0 - f0 which can produce NAN
 * 
 *****************************************************************************/

#include <asm_sprt.h>

.global _svdivz_f_DD;

//      .segment /pm seg_pmco;
		.segment/pm   seg_ext_pmco;

_svdivz_f_DD:
        leaf_entry;

        puts = r2;
        puts = r3;
        r0 = i2; puts = r0;

        i2 = r4;               // Address of R[]
        f3 = f8;                 // Get Scalar
        i4 = r12;              // Address of X[]
        r2 = reads( 1 );       // N
        f8 = 2.0;

        r0 = r0 - r0, f12 = dm(i4,m6);
        lcntr = r2, do div_lp until lce;
           f12 = pass f12, f4 = f3;
           if ne f0 = recips f12; 
           f12 = f0 * f12;
           f4 = f0 * f4, f0 = f8 - f12;
           f12 = f0 * f12;
           f4 = f0 * f4, f0 = f8 - f12;
           f12 = f0 * f12;
           f4 = f0 * f4, f0 = f8 - f12, f12 = dm(i4,m6);
           f0 = f0 * f4;
div_lp:    r0 = r0 - r0, dm(i2,m6) = f0;

        r2 = gets( 3 );
        r3 = gets( 2 );
        i2 = gets( 1 );       

        leaf_exit;

_svdivz_f_DD.end:

.endseg;
