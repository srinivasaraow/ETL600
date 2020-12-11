/******************************************************************************
 *
 * PROJECT     : COFDM Modem
 * FILE        : $Workfile: vmag2_uc_DDP.asm $
 * COPYRIGHT   : 2004 by TL-consulting GmbH.
 * DESCRIPTION : absolute value of unpacked complex vector
 * OPERATION   : R[i] = X[i]*X[i] + Y[i]*Y[i]
 * PROTOTYPE   : void vabs_uc_DDP( float          R[],
 *                                 float          X[],
 *                                 float pm       Y[],
 *                                 int            L );
 * ASSUMPTIONS : none.
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/vmag2_uc_DDP.asm $
 * 
 * 1     19.01.05 15:38 Chchlee
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


.global _vmag2_uc_DDP;

		.segment/pm   seg_pmco;

/*****************************************************
*
* void vmag2_uc_DDP( float R[], float X[], float pm Y[], int L );
*
* Operation : R[i] = X[i]*X[i] + Y[i]*Y[i], i=0..L-1
*
*
*****************************************************/
_vmag2_uc_DDP:

        leaf_entry;
        r0 = i0; puts = r0;
        r0 = i4; puts = r0;
        r0 = i8; puts = r0;

        i0 = r4;              // address of output,float vector R[]
        i4 = r8;              // address of input, float vector X[], real part
		i8 = r12;             // address of input, float vector Y[], imaginary part
        r12 = reads(1);       // LEN
		r12 = r12 - 1;        // LEN-1

		f4 = dm(i4,m6), f8 = pm(i8,m14);
		f4 = f4 * f4;
		f8 = f8 * f8;
		f8 = f4 + f8, f4 = dm(i4,m6);
        lcntr = r12, do mag2_lp1 until lce;
		  f4 = f4 * f4, dm(i0,m6) = f8, f8 = pm(i8,m14);
		  f8 = f8 * f8;
mag2_lp1: f8 = f4 + f8, f4 = dm(i4,m6);
        dm(i0,m6) = f8;


        i0 = gets(3);
        i4 = gets(2);
        i8 = gets(1);
        leaf_exit;

_vmag2_uc_DDP.end:

.endseg;
