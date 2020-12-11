
/******************************************************************************
 *
 * PROJECT     : 
 * FILE        : $Workfile: vdivz_c_DDD.asm $
 * COPYRIGHT   : 2004 TL Consulting GmbH
 * DESCRIPTION : Divide two vectors
 * OPERATION   : R = X ./ Y
 * PROTOTYPE   : void vdivz_c_DDD( complex_float    R[],
 *                                 complex_float    X[],
 *                                 complex_float    Y[],
 *                                 int              N );
 *
 * ASSUMPTIONS : if ||y[i]||^2 == 0.0 then r.re[i]=r.im[i]=0.0 
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
$Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/vdivz_c_DDD.asm $
 * 
 * 1     19.01.05 15:38 Chchlee
 * 
 * 2     30.09.04 8:27 Leeb02
 * 1     26.04.04 vgp
 * Initial version
 * 2     08.05.04 vgp
 *   r3 = r3 - r3 instead f3 = f3 - f3 which can produce NAN
 * 
 *****************************************************************************/

#include <asm_sprt.h>

.global _vdivz_c_DDD;

.segment /pm seg_pmco;

_vdivz_c_DDD:

    leaf_entry;

    puts = r2;                 
    puts = r3;       
    puts = r5;       
    puts = r6;       
    puts = r7;       
    puts = r9;       
    puts = r10;      
    puts = r11;      
    r0 = i2; puts = r0;
    r0 = i3; puts = r0;

    i2  = r4;             // Address of R[]
    i3  = r8;             // Address of X[] 
    i4  = r12;            // Address of Y[] 
    r10 = reads(1);       // N

    f11 = 2.0;
    f0  = dm(i3,m6);      // f0=x.re[0] 
    f1  = dm(i3,m6);      // f1=x.im[0]
    f4  = dm(m5,i4);                    // f4=y.re[0]
    f9  = f0 * f4,  f5 = dm(m6,i4);     // f9=x.re[0]*y.re[0], f5=y.im[0]
    f12 = f1 * f5,  f2 = dm(i4,m6);     // f12=x.im[0]*y.im[0], f2=y.re[0]
    lcntr = r10, do div_lp until lce;
        f10 = f1 * f4,  f7 = f9 + f12,  f3 = dm(i4,m6);  // f10=x.im[i]*y.re[i], f7=x.re[i]*y.re[i]+x.im[i]*y.im[i], f3=y.im[i]
        f12 = f0 * f5,  f0 = dm( i3, m6 );               // f12=x.re[i]*y.im[i], f0=x.re[i+1]
        f8  = f3 * f5,  f6 = f10 - f12,  f1 = dm(i3,m6); //  f8=y.im[i]*y.im[i], f6=x.im[i]*y.re[i]-x.re[i]*y.im[i], f1=x.im[i+1]
        f12 = f2 * f4,  f4 = dm(m5,i4);                  // f12=y.re[i]*y.re[i], f4=y.re[i+1]
        f9  = f0 * f4,  f12 = f8 + f12,  f5 = dm(m6,i4); //  f9=x.re[i+1]*y.re[i+1], f12=y.re[i]*y.re[i]+y.im[i]*y.im[i], f5=y.im[i+1] 
		// Denumerator = f12 = y.re[i]*y.re[i]+y.im[i]*y.im[i]
		// Numerator   = f7  = x.re[i]*y.re[i]+x.im[i]*y.im[i]
		r3 = r3 - r3,  f8 = f12;                         // f3 = 0.0, save D in f8 for imaginary part
		f12 = pass f12;
		if ne f3  = recips f12;                          // f3 = 8 bit seed 1/D   
        f12 = f3 * f12, f2 = f3;                         // save 8 bit seed in f2 for imaginary part
        f7  = f3 * f7,  f3 = f11 - f12;
        f12 = f3 * f12;
        f7  = f3 * f7,  f3 = f11 - f12;
        f12 = f3 * f12; 
        f7  = f3 * f7,  f3 = f11 - f12, f12 = f8;        // reload D to f12
        f8  = f3 * f7,  f3 = f2;                         // f8 = (x.re[i]*y.re[i]+x.im[i]*y.im[i])/D, reload seed to f3
		// Denumerator = f12 = y.re[i]*y.re[i]+y.im[i]*y.im[i]
		// Numerator   = f7  = x.im[i]*y.re[i]-x.re[i]*y.im[i]
        f12 = f3 * f12, f7 = f6;                         // f7 =x.im[i]*y.re[i]-x.re[i]*y.im[i]
        f7  = f3 * f7,  f3 = f11 - f12;
        f12 = f3 * f12;
        f7  = f3 * f7,  f3 = f11 - f12;
        f12 = f3 * f12, f2 = dm(i4,m6);                  // f2=y.re[i+1] 
        f7  = f3 * f7,  f3 = f11 - f12;
        f3  = f3 * f7,  dm(i2,m6) = f8;;                 // f3 = (x.im[i]*y.re[i]-x.re[i]*y.im[i])/D, Store r.re[i]
div_lp: f12 = f1 * f5,  dm(i2,m6) = f3;                  // f12=x.im[i+1]*y.im[i+1], Store r.im[i], i=i+1

    r2  = gets( 10 );
    r3  = gets(  9 );
    r5  = gets(  8 );
    r6  = gets(  7 );
    r7  = gets(  6 );
    r9  = gets(  5 );
    r10 = gets(  4 );
    r11 = gets(  3 );
    i2  = gets(  2 );
    i3  = gets(  1 );

    leaf_exit;
_vdivz_c_DDD.end:

.endseg;
