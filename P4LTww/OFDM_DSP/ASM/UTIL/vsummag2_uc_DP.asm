
/******************************************************************************
 *
 * PROJECT     : DRS7  
 * FILE        : $Workfile: vsummag2_uc_DP.asm $
 * COPYRIGHT   : 2004 TL Consulting GmbH
 * DESCRIPTION : Square vector
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/vsummag2_uc_DP.asm $
 * 
 * 1     19.01.05 15:39 Chchlee
 * 
 * 2     30.09.04 8:27 Leeb02
 * 1     12.02.04 vgp
 * Initial version
 * 2     08.05.04 vgp
 *   r12 = r12 - r12 instead f12 = f12 - f12 which can produce NAN
 *****************************************************************************/


#include <asm_sprt.h>

.global _vsummag2_uc_DP;

		.segment/pm   seg_pmco;

/*****************************************************
*
* float vsummag2_uc_DP( float X[], float pm Y[], int L );
*
* Operation : R = SUM( X[i]*X[i] + Y[i]*Y[i] ), i = 0,..L
*
*****************************************************/
_vsummag2_uc_DP:

        leaf_entry;
        r0 = i4; puts = r0;
        r0 = i8; puts = r0;
        puts = r3;
        puts = r7;

        i4 = r4;             // address of input, float vector X[], real part
		i8 = r8;             // address of input, float vector Y[], imaginary part
        r4 = r12;            // LEN
		r4 = r4 - 1;          // LEN-1
		r12 = r12 - r12;

		f3 = dm(i4,m6), f8 = pm(i8,m14);
		f8 = f8 * f8, f7 = f3;
		f7 = f3 * f7, f3 = dm(i4,m6);
		f8 = f7 + f8, f7 = f3;
        lcntr = r4, do mul_lp until lce;
		  f7 = f3 * f7, f12 = f8 + f12, f8 = pm(i8,m14);
		  f8 = f8 * f8, f3 = dm(i4,m6);
mul_lp:   f8 = f7 + f8, f7 = f3;
        f0 = f8 + f12;


        i4 = gets(4);
        i8 = gets(3);
        r3 = gets(2);
        r7 = gets(1);
        leaf_exit;
_vsummag2_uc_DP.end:

.endseg;
