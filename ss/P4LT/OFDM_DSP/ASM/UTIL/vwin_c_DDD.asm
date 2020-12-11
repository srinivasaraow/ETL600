/******************************************************************************
 *
 * PROJECT     : COFDM Modem
 * FILE        : $Workfile: vwin_c_DDD.asm $
 * COPYRIGHT   : 2004 by TL-consulting GmbH.
 * DESCRIPTION : windowing operation of complex vector
 * OPERATION   : R[i].re = X[i].re*W[i]; R[i].im = X[i].im*W[i]
 * PROTOTYPE   : void vwin_c_DDD(  complex_float          R[],
 *                               complex_float    X[],
 *                               float                      W[],
 *                               int                       L );
 * ASSUMPTIONS : none.
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/vwin_c_DDD.asm $
 * 
 * 1     19.01.05 15:39 Chchlee
 * 
 * 2     30.09.04 8:27 Leeb02
 * 
 * 1     11.02.04 vgp
 * Initial version
 * 
 *****************************************************************************/

#ifdef __ADSP21160__
#include <def21160.h>
#endif
#ifdef __ADSP21161__
#include <def21161.h>
#endif

#include <asm_sprt.h>


.global _vwin_c_DDD;
.global _vmul_ccf_DDD;

		.segment/pm   seg_pmco;

/*****************************************************
*
* void vwin_c_DDD( complex_float R[], complex_float X[], float W[], int L );
* void vmul_cf_DDD( complex_float R[], complex_float X[], float W[], int L );
*
* Operation : R[i].re = X[i].re*W[i]; R[i].im = X[i].im*W[i], i=0..L-1
*
*
*****************************************************/
_vwin_c_DDD:
_vmul_ccf_DDD:

        leaf_entry;
        r0 = i0; puts = r0;
        r0 = i1; puts = r0;
        r0 = i2; puts = r0;
		r0 = m4; puts = r0;

        i0 = r4;              // address of output complex vector R[]
        i1 = r12;             // address of input float vector W[]
        i2 = r8;              // address of input complex vector X[]
        r4 = reads(1);        // LEN
		r4 = r4 - 1;

        bit set mode1 PEYEN | BDCST1;  // Enter SIMD mode & enable broadcast mode of i1
        m4 = 2;                        

        // SIMD starts here 
        f8 = dm(i1,m6);
        f12 = dm(i2,m4);
        lcntr = r4, do mul_lp until lce;
		  f12 = f12 * f8, f8 = dm(i1,m6);
		  dm(i0,m4) = f12;
mul_lp:   f12 = dm(i2,m4);
        f12 = f12 * f8;
        dm(i0,m4) = f12;

        bit clr mode1 PEYEN | BDCST1;  // Enter SISD mode & disable broadcast mode of i1
        nop;

        i0 = gets(4);
        i1 = gets(3);
        i2 = gets(2);
        m4 = gets(1);
        leaf_exit;

_vwin_c_DDD.end:
_vmul_ccf_DDD.end:

.endseg;
