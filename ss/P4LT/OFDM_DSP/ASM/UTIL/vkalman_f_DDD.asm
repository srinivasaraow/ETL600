
/******************************************************************************
 *
 * PROJECT     : 
 * FILE        : $Workfile: vkalman_f_DDD.asm $
 * COPYRIGHT   : 2004 TL Consulting GmbH
 * DESCRIPTION : First order Kalman filter
 * OPERATION   : r[i] = x[i] + a*(y[i]-x[i]), i=0..N-1
 * PROTOTYPE   : void vkalman_f_DDD( float    R[],
 *                                   float    X[],
 *                                   float    Y[],
 *                                   float    a,
 *                                   int      N );
 *
 * ASSUMPTIONS : All array starts at even address
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/vkalman_f_DDD.asm $
 * 
 * 1     19.01.05 15:38 Chchlee
 * 
 * 2     30.09.04 8:27 Leeb02
 * 1     05.05.04 vgp
 * Initial version
 * 
 *****************************************************************************/

#ifdef __ADSP21160__
#include <def21160.h>
#endif
#ifdef __ADSP21161__
#include <def21161.h>
#endif
#ifdef __ADSP21262__
#include <def21262.h>
#endif

#include <asm_sprt.h>

.global _vkalman_f_DDD;

//  .segment /pm seg_pmco;
	.segment/pm   seg_ext_pmco;

_vkalman_f_DDD:

    leaf_entry;

    r0 = m4; puts = r0;
    r0 = i2; puts = r0;
    r0 = i3; puts = r0;

    i2  = r4;              // Address of R[]
    i3  = r8;              // Address of X[] 
    i4  = r12;             // Address of Y[]
    f0 = reads(1);         // get alpha
    r4 = reads(2);         // N
	r4 = lshift r4  by -1; // N = N/2
	m4 = 2;

    bit set mode1 PEYEN;   // Enter SIMD
	s0 = f0;

    lcntr = r4, do filt_lp until lce;
	     f4 = dm(i4,m4);   // get y[i]
		 f8 = dm(i3,m4);   // get x[i]
		 f4 = f4 -  f8;    // f4 = y[i] - x[i]
		 f4 = f0*f4;       // f4 = alpha*(y[i] - x[i])
		 f8 = f8 + f4;     // f8 = x[i] + alpha*(y[i] - x[i])
filt_lp: dm(i2,m4) = f8;                      


    bit clr mode1 PEYEN;  // Enter SISD mode
    nop;

    m4  = gets( 3 );
	i2  = gets( 2 );
    i3  = gets( 1 );

    leaf_exit;
_vkalman_f_DDD.end:

.endseg;
