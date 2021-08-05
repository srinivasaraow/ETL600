/******************************************************************************
 *
 * PROJECT     : COFDM Modem
 * FILE        : $Workfile: vabs_uc_DDP.asm $
 * COPYRIGHT   : 2004 by TL-consulting GmbH.
 * DESCRIPTION : absolute value of unpacked complex vector
 * OPERATION   : R[i] = sqrt(X[i]*X[i] + Y[i]*Y[i])
 * PROTOTYPE   : void vabs_uc_DDP( float          R[],
 *                                 float          X[],
 *                                 float pm       Y[],
 *                                 int            L );
 * ASSUMPTIONS : none.
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/vabs_uc_DDP.asm $
 * 
 * 1     19.01.05 15:38 Chchlee
 * 
 * 2     30.09.04 8:27 Leeb02
 * 
 * 1     11.02.04 vgp
 * Initial version
 *   r4 = r4 - r4 instead f4 = f4 - f4 which can produce NAN
 * 
 *****************************************************************************/

#ifdef __ADSP21160__
#include <def21160.h>
#endif
#ifdef __ADSP21161__
#include <def21161.h>
#endif

#include <asm_sprt.h>


.global _vabs_uc_DDP;

		.segment/pm   seg_pmco;

/*****************************************************
*
* void vabs_uc_DDP( float R[], float X[], float pm Y[], int L );
*
* Operation : R[i] = sqrt(X[i]*X[i] + Y[i]*Y[i]), i=0..L-1
*             if c <= 0.0 , then sqrt(c) = 0.0
*
*****************************************************/
_vabs_uc_DDP:

        leaf_entry;
        r0 = i0; puts = r0;
        r0 = i4; puts = r0;
        r0 = i8; puts = r0;
		puts = r1;
		puts = r2;
		puts = r10;


        i0 = r4;              // address of output,float vector R[]
        i4 = r8;              // address of input, float vector X[], real part
		i8 = r12;             // address of input, float vector Y[], imaginary part
        r2 = reads(1);        // LEN
		r2 = r2 - 1;          // LEN-1

		f10 = 3.0;
		f1  = 0.5;
		r4 = r4 - r4, f12 = dm(i4,m6), f8 = pm(i8,m14); // load x[0], y[0]
		f12 = f12 * f12;
		f8 = f8 * f8;
		f8 = f12 + f8;
		// f8 = sqrt(f8) = sqrt(C), f4 = 0.0
        if gt f4  = rsqrts f8;          // X0 
        f12 = f4*f4;                    // f12 = X0^2
        f12 = f12*f8;                   // f12 = C*X0^2
        f4  = f1*f4,  f12 = f10-f12;    // f4 = 0.5*X0; f12 = 3.0 - C*X0^2
        f4  = f4*f12;                   // f4 = X1 = 0.5*X0*(3.0-C*X0^2)
        f12 = f4*f4;                    // f12 = X1^2
        f12 = f12*f8;                   // f12 = C*X1^2
        f4  = f1*f4,  f12 = f10-f12;    // f4 = 0.5*X1; f12 = 3.0 - C*X1^2
        f4  = f4*f12;                   // f4 = X2 = 0.5*X1(3.0-C*X1^2)
        f12 = f4*f4;                    // f12 = X2^2
        f12 = f12*f8;                   // f12 = C*X2^2
        f4  = f1*f4,  f12 = f10-f12;    // f4 = 0.5*X2, f12 = 3.0-C*X2^2
        f4  = f4*f12;                   // f4 = X3 =0.5*X2(3.0-C*X2^2)
        f8  = f4*f8, f12 = dm(i4,m6);   // f8 = sqrt(f8), f12 = x[1]
        lcntr = r2, do abs_lp until lce;
		  f12 = f12 * f12, dm(i0,m6) = f8, f8 = pm(i8,m14);  // f8 = y[i]
		  f8 = f8 * f8, f4 = m5;        // f4 = 0.0
          f8 = f12 + f8;
		  // f8 = sqrt(f8) = sqrt(C); f4 = 0.0
          if gt f4  = rsqrts f8;        // X0 
          f12 = f4*f4;                  // f12 = X0^2
          f12 = f12*f8;                 // f12 = C*X0^2
          f4  = f1*f4,  f12 = f10-f12;  // f4 = 0.5*X0; f12 = 3.0 - C*X0^2
          f4  = f4*f12;                 // f4 = X1 = 0.5*X0*(3.0-C*X0^2)
          f12 = f4*f4;                  // f12 = X1^2
          f12 = f12*f8;                 // f12 = C*X1^2
          f4  = f1*f4,  f12 = f10-f12;  // f4 = 0.5*X1; f12 = 3.0 - C*X1^2
          f4  = f4*f12;                 // f4 = X2 = 0.5*X1(3.0-C*X1^2)
          f12 = f4*f4;                  // f12 = X2^2
          f12 = f12*f8;                 // f12 = C*X2^2
          f4  = f1*f4,  f12 = f10-f12;  // f4 = 0.5*X2, f12 = 3.0-C*X2^2
          f4  = f4*f12;                 // f4 = X3 = 0.5*X2(3.0-C*X2^2)
abs_lp:   f8  = f4*f8, f12 = dm(i4,m6); // f8 = sqrt(f8), f12 = y[i+1], i++
        dm(i0,m6) = f8;                 // store r[len-1]


        i0  = gets(6);
        i4  = gets(5);
        i8  = gets(4);
		r1  = gets(3);
		r2  = gets(2);
		r10 = gets(1);
        leaf_exit;


_vabs_uc_DDP.end:


.endseg;
