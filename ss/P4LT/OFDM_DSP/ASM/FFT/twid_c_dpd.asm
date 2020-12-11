//---------------------------------------------------------------------------
// COPYRIGHT:  (c) 2004 TL-Consulting GmbH, Frankfurt am Main 
//
// MODULE      twid_c_dpd.asm
//
// PROJECT:    P4LT system board of ETL600
//
// COMPILER    VisualDSP 3.0, service pack 1
//
// DATE        21-DEC-2004
//
// AUTHOR:     Dr. Vladimir Potapov
//  
// ABSTRACT    Twiddle factors for universal complex radix-2 fast 
//             direct and inverse Fourier transform for maximium lenght of 2048
//             complex samples (fft_c_dpd.asm & ifft_c_dpd.asm)
//             
//                  
// PREMISES:   
//
// REMARKS:    
//  
// HISTORY:    Initial Version
//                21.01.2004 VGP
//             Global variables in seg_dmda common for ifft_c_dpd() & fft_c_dpd()
//                26.01.2004 VGP
//
//---------------------------------------------------------------------------
/* $Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/FFT/twid_c_dpd.asm $
 * 
 * 1     19.01.05 15:37 Chchlee
 * 
 * 4     30.09.04 8:52 Leeb02
 * 
 * 3     30.09.04 8:27 Leeb02
*/
#include "../../h/fft/fft_c_dpd_defs.h"

.global ctwid;
.global FFT_Stages;
.global FFT_Stages4;
.global FFT_N;
.global FFT_N2;
.global FFT_N4;
.global FFT_N8;
.global twid_mod;

.segment/dm             seg_dmda;
.align 2;
.var FFT_Stages[2];        // log2(N)
.align 2;
.var FFT_Stages4[2];       // log2(N)-4
.align 2;
.var FFT_N[2];             // N
.align 2;
.var FFT_N2[2];            // N/2
.align 2;
.var FFT_N4[2];            // N/4
.align 2;
.var FFT_N8[2];          // N/8
.align 2;
.var twid_mod[2];        //log2(MAX_N)-log2(N)
.endseg;

.segment/dm             seg_twid;
#if MAX_FFT_N==64
.align 2;
.var     ctwid[MAX_FFT_N]  =   {
#include "../../asm/fft/twid64.dat"
};
#endif
#if MAX_FFT_N==128
.align 2;
.var     ctwid[MAX_FFT_N]  =   {
#include "../../asm/fft/twid128.dat"
};
#endif
#if MAX_FFT_N==256
.align 2;
.var     ctwid[MAX_FFT_N]  =   {
#include "../../asm/fft/twid256.dat"
};
#endif
#if MAX_FFT_N==512
.align 2;
.var     ctwid[MAX_FFT_N]  =   {
#include "../../asm/fft/twid512.dat"
};
#endif
#if MAX_FFT_N==1024
.align 2;
.var     ctwid[MAX_FFT_N]  =   {
#include "../../asm/fft/twid1024.dat"
};
#endif
#if MAX_FFT_N==2048
.align 2;
.var     ctwid[MAX_FFT_N]  =   {
#include "../../asm/fft/twid2048.dat"
};
#endif
.endseg;

