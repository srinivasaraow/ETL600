//---------------------------------------------------------------------------
// COPYRIGHT:  (c) 2004 TL-Consulting GmbH, Frankfurt am Main 
//
// MODULE      ifft_c_dpd.asm
//
// PROJECT:    P4LT system board of ETL600
//
// COMPILER    VisualDSP 3.0, service pack 1
//
// DATE        21-DEC-2004
//
// AUTHOR:     Dr. Vladimir Potapov
//  
// ABSTRACT    Universal SIMD inverse complex radix-2 fast Fourier transform for 
//             maximium lenght of 2048 complex samples
//             
//                  
// PREMISES:   
//
// REMARKS:    All externs are defined in twid_c_dpd.asm
//  
// HISTORY:    Initial Version
//                21.01.2004 VGP
//             Global variables in seg_dmda common for ifft_c_dpd() & fft_c_dpd()
//                26.01.2004 VGP
//
//---------------------------------------------------------------------------
/* $Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/FFT/ifft_c_dpd.asm $
 * 
 * 1     19.01.05 15:37 Chchlee
 * 
 * 5     30.09.04 8:52 Leeb02
 * 
 * 4     30.09.04 8:51 Leeb02
 * 
 * 3     30.09.04 8:27 Leeb02
 *
 * These constants are independent of the number of points
 */
#define BFLY8           4         /*Offset between bf branches in a group of 8*/
 
#include <asm_sprt.h>

#include "../../h/fft/fft_c_dpd_defs.h"

#ifdef __ADSP21160__
#include <def21160.h>
#endif
#ifdef __ADSP21161__
#include <def21161.h>
#endif

.global _ifft8_c_DPD;
.global _ifft16_c_DPD;
.global _ifft32_c_DPD;
.global _ifft64_c_DPD;
.global _ifft128_c_DPD;
.global _ifft256_c_DPD;
.global _ifft512_c_DPD;
.global _ifft1024_c_DPD;
.global _ifft2048_c_DPD;

.extern ctwid;
.extern FFT_Stages;
.extern FFT_Stages4;
.extern FFT_N;
.extern FFT_N2;
.extern FFT_N4;
.extern FFT_N8;
.extern twid_mod;

.extern ctwid;


/*
 * The constants below must be changed for different length FFTs
 *    N         = number of points in the FFT, must be a power of 2
 *    STAGES    = log2(N)
 *    BRMODIFY  = bitrev(32 bit N/2)
 *    IREDM     = bitrev(32 bit addr of input real in dm), addr is 0,N,2N,3N,...
 *    IREPM     = bitrev(32 bit addr of input imag in pm), addr is 0,N,2N,3N,...
 */


.SEGMENT/PM   seg_pmco;

/*
 * Unused registers:
 *   I3, I4, I5, I6, I7
 *   M3, M4, M5, M6, M7
 *   I9, I12, I13, I14, I15
 *   M13, M15
 */

/*
 * int ifftXXXX_c_DPD( float dm         outR[],     <-- refft
 *                     float pm         outI[],     <-- imfft
 *                     complex_float dm in1[] );    <-- cmplx
 */

_ifft8_c_DPD:
	  r0 = 3;                 // log2(8)
	  jump ICFFTxxx;
_ifft16_c_DPD:
	  r0 = 4;                 // log2(16)
	  jump ICFFTxxx;
_ifft32_c_DPD:
	  r0 = 5;                 // log2(32)
	  jump ICFFTxxx;
_ifft64_c_DPD:
	  r0 = 6;                 // log2(64)
	  jump ICFFTxxx;
_ifft128_c_DPD:
	  r0 = 7;                 // log2(128)
	  jump ICFFTxxx;
_ifft256_c_DPD:
      r0 = 8;                 // log2(256)
	  jump ICFFTxxx;
_ifft512_c_DPD:
	  r0 = 9;                 // log2(512)
	  jump ICFFTxxx;
_ifft1024_c_DPD:
	  r0 = 10;                // log2(1024)
	  jump ICFFTxxx;
_ifft2048_c_DPD:
	  r0 = 11;                // log2(2048)

ICFFTxxx:
      leaf_entry;  

      puts = r3;
      puts = r5;
      puts = r6;
      puts = r7;
      puts = r9;
      puts = r10;
      puts = r11;
      puts = r13;
      puts = r14;
      puts = r15;

      dm(FFT_Stages) = r0;     // save log2(N)
      dm(FFT_Stages+1) = r0;   // save log2(N)
      r3 = MAX_FFT_LOG2_N;
      r3 = r3 - r0, r5 = r0;   // r5 = log2(N)
      dm(twid_mod) = r3;       // save twidle modifier log2(MAX_N)-log2(N)
      dm(twid_mod+1) = r3;
      r3 = 4;
      r0 = r0 - r3;           
      dm(FFT_Stages4) = r0;   // save log2(N)-4
      dm(FFT_Stages4+1) = r0;
      r0 = 1;
      r0 = lshift r0 by r5;    // r0 = 2^log2(N) = N
      dm(FFT_N) = r0;
      dm(FFT_N+1) = r0;        // save N
   	  r3 = lshift r0 by - 1;   // r0 = FFT_N, r3 = N/2;
      dm(FFT_N2) = r3;
      dm(FFT_N2+1) = r3;
	  r3 = lshift r3 by - 1;   // r3 = N/4;
      dm(FFT_N4) = r3;
      dm(FFT_N4+1) = r3;
	  r3 = lshift r3 by - 1;   // r3 = N/8;
      dm(FFT_N8) = r3;
      dm(FFT_N8+1) = r3;

      r0 = i0; puts = r0;
      r0 = i1; puts = r0;
      r0 = i2; puts = r0;
      r0 = i3; puts = r0;
      r0 = i4; puts = r0;
      r0 = i5; puts = r0;
      r0 = i8; puts = r0;
      r0 = i9; puts = r0;
      r0 = i10; puts = r0;
      r0 = i11; puts = r0;
      r0 = i12; puts = r0;
      r0 = i13; puts = r0;
      r0 = m0; puts = r0;
      r0 = m1; puts = r0;
      r0 = m2; puts = r0;
      r0 = m8; puts = r0;
      r0 = m9; puts = r0;
      r0 = m10; puts = r0;
      r0 = m11; puts = r0;
      r0 = m14; puts = r0;
      r0 = m13; puts = r0;                /* VGP */
      r0 = mode1; puts = r0;              /* VGP */

/*
 * Save input parameters for later use
 */
      i3 = r8;      /* i3 = refft */
      r0 = dm(FFT_N);  // r0 = N
      r0 = r0 - 1;
      r0 = r0 - 1;
      r8 = r8 + r0; 
      i4 = r8;      /* i4 = refft + N - 2 */

      i12 = r4;     /* i12 = imfft */
      r4  = r4 + r0;
      i13 = r4;     /* i13 = imfft + N - 2 */

      f4 = -1.0;
      s4 =  1.0;
      
/* 
 * enable bit reverse of i0 
 */
      bit set MODE1 BR0 | BR8 | RND32 | CBUFEN | PEYEN;

/*
 * Do bitrev and packing within first two stages
 */
      i1 = r12;  /* Get address of complex input */
      bitrev( i1, 0x0 );
      b0 = i1;

      l0 = 0;
      r1 = dm(FFT_N2);           // r1 = N/2
      i1 = r1;
      bitrev(i1, 0x0 );
      r1 = i1;
      m0 = r1;

     /* Address of second half of complex input is address of first + N */
      r0 = dm(FFT_N);           // r0 = N
      r12 = r12 + r0;
      i9 = r12;
      bitrev( i9, 0x0 );
      b8 = i9;

      l8 = 0;
      m8 = r1;
      
      /*      r0=refft; */
      r0 = i3;
      r0 = lshift r0 by -1;
      b2 = r0;
      l2 = dm(FFT_N);           // l2 = N
      m1=1;                  

      /*    r0=imfft; */
      r0 = i12;
      r0 = lshift r0 by -1;
      b10 = r0;
      l10 = dm(FFT_N);          // l10 = N
      m9 = 1;

/*
 * Do the first two stages (actually a radix-4 FFT stage)
 */

      s8=dm(i0,m0);   s9=pm(i8,m8);
      f8=f8+f9,      f9=f8-f9;      s10=dm(i0,m0);   s11=pm(i8,m8);
      f12=f10+f11,   f3=f10-f11;
      f3=f3*f4,      f2=f8+f12,      f6=f8-f12;
                
      LCNTR = dm(FFT_N4);       // LCNTR = N/4
	  do FSTAGE until LCE;    /* do N/4 simple radix-4 butterflies */
         r3<->s3;
         f3=f9+f3,    f7=f9-f3;   s8=dm(i0,m0);  s9=pm(i8,m8);
         f8=f8+f9,    f9=f8-f9;   s10=dm(i0,m0); s11=pm(i8,m8);
         f12=f10+f11, f3=f10-f11, dm(i2,m1)=f2,  pm(i10,m9)=f2;
FSTAGE:  f3=f3*f4,    f2=f8+f12,  f6=f8-f12,     dm(i2,m1)=f6,   pm(i10,m9)=f6;

/*
 * middle stages loop 
 */

      bit clr MODE1 BR0 | BR8;      /*finished with bitreversal*/
      b2 = i3;
      b8 = i12;
      l8 = dm(FFT_N);           // l8 = N
      
      /*    b0=refft; */
      b0 = i3;
      l0 = dm(FFT_N);           // l0 = N

      l1 = MAX_FFT_N2;
      r2 = ctwid;
      r2 = lshift r2 by -1;         // LW addressing
      b1 = r2;

      /*    b10=imfft; */
      b10 = i12;
      l10 = dm(FFT_N);          // l10 = N
        
      /*    b11=imfft; */
      b11 = i12;
      l11 = dm(FFT_N);          // l11 = N

      m0  = -BFLY8;
	  r15 = dm(FFT_N8);         // r15 = N/8, i.e. # OF GROUPS IN THIRD STAGE
	  r2  = -r15;

	  m1 = -MAX_FFT_N8;
      m2  = -BFLY8-2;
      m11 = -2;

      r2  = 2;
      r3  = -BFLY8;            /*initializes m0,10 - incr for butterf branches   */
      r5  = BFLY8/2;            /*counts # butterflies per a group            */
      r9  = (-2*BFLY8)-2;      /*initializes m12 - wrap around to next grp + 2   */
      r10 = -2*BFLY8;         /*initializes m8 - incr between groups         */
      r13 = -BFLY8-2;         /*initializes m2,13 - wrap to bgn of 1st group   */

      f1=dm(i1,m1);         /*set pointers to tables to 1st coeff.   */

	  r1  = 3;                     // test if N/8 == 1 or N/8 == 2
	  r1 = r15 and r1;
	  if ne jump last2_stages;
            
/*
 * # OF STAGES TO BE HANDLED = LOG2(N)-4 
 */
      LCNTR = dm(FFT_Stages4);   // LCNTR = log2(N)-4
	  do end_stage until LCE;
         m8  = r10;
         m10 = r3;
         m12 = r9;
         /* i0=refft+N-2; */
         i0 = i4;
         /* i2=refft+N-2; */
         i2 = i4;
         /* i8=imfft+N-2; */
         i8 = i13;
         /* i10=imfft+N-2; */
         i10 = i13;
         /* i11=imfft+N-2; */
         i11 = i13;
         r15 = r15-r2,     m13=r13;       /*CALCULATE # OF CORE  */
                                        /*BFLIES/GROUP IN THIS STAGE */

         s1=dm(i1,m1); /* load s1 with cos and s0 with sin values */

         r1 = dm(i1,m1);   f7=pm(i8,m8); 
         f12 = f0*f7,      f6=dm(i0,m0);
         f8 = f1*f6,       modify(i11,m10);
         f11 = f1*f7;                                              
         f14=f0*f6,  f12=f8+f12,                 f8=dm(i0,m0),   f7=pm(i8,m8);
         f12=f0*f7,  f13=f8+f12, f10=f8-f12,     f6=dm(i0,m0);   

/*
 * Each iteration does another set of bttrflys in each group 
 */

/*
 * # OF BUTTERFLIES/GROUP IN THIS STAGE 
 */
         LCNTR=r5,   do end_group until LCE; 

/*core butterfly loop */

/*
 * Do a butterfly in each group - 2 
 */
            LCNTR=r15,  do end_bfly until LCE;
               f8=f1*f6,  f14=f11-f14, dm(i2,m0)=f10, f9=pm(i11,m8);
               f11=f1*f7, f3=f9+f14,   f9=f9-f14,  dm(i2,m0)=f13,  f7=pm(i8,m8);
               f14=f0*f6, f12=f8+f12,  f8=dm(i0,m0),  pm(i10,m10)=f9;
end_bfly:      f12=f0*f7, f13=f8+f12,  f10=f8-f12, f6=dm(i0,m0), pm(i10,m10)=f3;

/*
 * finish up last bttrfly and set up for next butterfly in each group 
 */
            f8=f1*f6, f14=f11-f14,  dm(i2,m0)=f10,  f9=pm(i11,m8);
            f11=f1*f7,  f4=f9+f14,  f9=f9-f14,   dm(i2,m0)=f13,  f14=pm(i8,m11);
            f14=f0*f6,  f12=f8+f12, f8=dm(i0,m2),   pm(i10,m10)=f9;
            s1=dm(i1,m1);
            f13=f8+f12, f10=f8-f12, r1=dm(i1,m1);   f7=pm(i8,m8);
            f14=f11-f14,    dm(i2,m0)=f10,  f9=pm(i11,m12);

/*
 * start on next butterfly in each group 
 */
            f12=f0*f7,  f3=f9+f14,  f9=f9-f14,      f6=dm(i0,m0);
            f8=f1*f6,   dm(i2,m2)=f13,  pm(i10,m10)=f4;
            f11=f1*f7,  pm(i10,m10)=f9;
            f14=f0*f6,  f12=f8+f12,  f8=dm(i0,m0),   f7=pm(i8,m8);
end_group:  f12=f0*f7,  f13=f8+f12, f10=f8-f12,  f6=dm(i0,m0),   pm(i10,m13)=f3;

                                        
         r4 = r15+r2,          i1=b1;          /*PREPARE R4 FOR #OF BFLIES CALC */
         r15 = ashift r4 by -1;                /*# OF BFLIES/GRP IN NEXT STAGE */
         r4 = -r15;

         r3 = dm(twid_mod);
	     r4 = ashift r4 by r3;
         m1 = r4;                              /*update inc for sin & cos  */

         r5=ashift r5 by 1,  f1=dm(i1,m1);   /*update # bttrfly in a grp */
         r3=ashift r5 by 1;
         r3=-r3;                             /* inc for bttrfly branch */
         r13=r3-r2,           m0=r3;         /* wrap to 1st grp        */
         r10=ashift r3 by 1;                 /* inc between grps       */
end_stage:  r9=r10-r2,           m2=r13;     /* wrap to grp +1         */

/*
 * next to last stage
 */
last2_stages:
 
      r0 = dm(FFT_N8);
      r0 = r0 - 1;
      if eq jump lastone_stage;
 
      r0 = -2;
      r1 = dm(twid_mod);
      r0 = ashift r0 by r1;
      m1 = r0;  

      m8 = r10;                 /*incr between groups            */
      m10 = r3;                 /*incr between bttrfly branches  */
      m12 = r9;                 /*wrap around to next grp + 1    */
      m13 = r13;                /*wrap to bgn of 1st group       */

      /*    i0=refft+N-2; */
      i0 = i4;
        
	  r0 = dm(twid_mod);
      r1 = dm(FFT_N2);
	  r1 = r1 - 1;
	  r1 = r1 - 1;              // r1 = N/2-2
	  r1 = ashift r1 by r0;
	  
      r0 = b1;
      r0 = r0 + r1;
      i1 = r0;                  /*pntr to 1st twiddle coeffs     */
        
      /*    i2=refft+N-2; */
      i2 = i4;
      /*    i8=imfft+N-2; */
      i8=i13;
      /*    i10=imfft+N-2; */
      i10 = i13;
      /*    i11=imfft+N-2; */
      i11 = i13;

      s1 = dm(i1,m1);

      r1=dm(i1,m1);   f7=pm(i8,m8);
      f12=f0*f7,                        f6=dm(i0,m0);
      f8=f1*f6,                        modify(i11,m10);
      f11=f1*f7,                                    f7=pm(i8,m12);
      f14=f0*f6, f12=f8+f12,               f8=dm(i0,m0);
      f12=f0*f7, f13=f8+f12,  f10=f8-f12,      f6=dm(i0,m0);                           
/*
 * Do the N/4 butterflies in the two groups of this stage 
 */

      LCNTR = dm(FFT_N8);        // LCNTR = N/8
	  do end_group2 until LCE;   
         f8=f1*f6,  f14=f11-f14,    dm(i2,m0)=f10,  f9=pm(i11,m8);
         f11=f1*f7, f3=f9+f14,      f9=f9-f14,      dm(i2,m0)=f13;
         f14=f0*f6, f12=f8+f12,     f8=dm(i0,m2),   pm(i10,m10)=f9;
         s1=dm(i1,m1);
         f13=f8+f12,     f10=f8-f12,     r1=dm(i1,m1);   f7=pm(i8,m8); 
         f12=f0*f7, f14=f11-f14,    f6=dm(i0,m0),   f9=pm(i11,m12);
         f8=f1*f6,   f3=f9+f14,      f9=f9-f14, dm(i2,m0)=f10,  pm(i10,m10)=f3;
         f11=f1*f7,                             dm(i2,m2)=f13,  pm(i10,m10)=f9; 
         f14=f0*f6, f12=f8+f12,                 f8=dm(i0,m0),   f7=pm(i8,m12);
end_group2: f12=f0*f7, f13=f8+f12,  f10=f8-f12, f6=dm(i0,m0),   pm(i10,m13)=f3;

/*
 * The last stage      
 */
lastone_stage:
 
      r0 = dm(FFT_N2);
	  r0 = -r0;
	  m0 = r0;                 // m0 = -N/2
	  r0 = r0 - 1;
	  r0 = r0 - 1;             
	  m2 = r0;                 // m2 = -N/2 - 2

      m10 = m0;
      m13 = m2;
      /*    i0=refft+N-2; */
      i0 = i4;
        
      r1 = MAX_FFT_N2;
      r0 = b1;
      r0 = r0 + r1;
      r3 = -1;
      r4 = dm(twid_mod);
      r3 = ashift r3 by r4;
      m1 = r3;
      r0 = r0 + r3;      
      i1 = r0;               /*pntr to 1st twiddle coeffs     */

      /*    i2=refft+N-2; */
      i2 = i4;
      /*    i8=imfft+N-2; */
      i8 = i13;
        
      /*    i10=imfft+N-2; */
      i10 = i13;
      /*    i11=imfft+N-2; */
      i11 = i13;

/*start first bttrfly */
      s1=dm(i1,m1);
      r1=dm(i1,m1);   f7=pm(i8,m11);
      f12=f0*f7,                        f6=dm(i0,m0);                                    
      f8=f1*f6,                        modify(i11,m10);
      f11=f1*f7;                                      
      f14=f0*f6, f12=f8+f12,               f8=dm(i0,m2),   f9=pm(i11,m11);

/*do N/2 bttrflys in the last stage */

      LCNTR=dm(FFT_N4);       // LCNTR = N/4  
	  do last_stage until LCE;   
         s1=dm(i1,m1);
         f13=f8+f12, f10=f8-f12,  r1=dm(i1,m1);   f7=pm(i8,m11);
         f12=f0*f7,  f14=f11-f14, f6=dm(i0,m0);
         f8=f1*f6,   f3=f9+f14,   f15=f9-f14,  dm(i2,m0)=f10,  f9=pm(i11,m11);
         f11=f1*f7,  dm(i2,m2)=f13,  pm(i10,m10)=f15;
last_stage: f14=f0*f6, f12=f8+f12,   f8=dm(i0,m2),   pm(i10,m13)=f3;

   
      bit clr mode1 PEYEN | CBUFEN;     /* VGP */

      r3  = gets( 32 );
      r5  = gets( 31 );
      r6  = gets( 30 );
      r7  = gets( 29 );
      r9  = gets( 28 );
      r10 = gets( 27 );
      r11 = gets( 26 );
      r13 = gets( 25 );
      r14 = gets( 24 );
      r15 = gets( 23 );
      i0  = gets( 22 );
      i1  = gets( 21 );
      i2  = gets( 20 );
      i3  = gets( 19 );
      i4  = gets( 18 );
      i5  = gets( 17 );
      i8  = gets( 16 );
      i9  = gets( 15 );
      i10 = gets( 14 );
      i11 = gets( 13 );
      i12 = gets( 12 );
      i13 = gets( 11 );
      m0  = gets( 10 );
      m1  = gets( 9 );
      m2  = gets( 8 );
      m8  = gets( 7 );
      m9  = gets( 6 );
      m10 = gets( 5 );
      m11 = gets( 4 );
      m14 = gets( 3 );
      m13 = gets( 2 );     /* VGP */
      r0  = gets( 1 );     /* VGP */
      mode1 = r0;          /* VGP */


      l0 = 0;
      l1 = 0;
      l8 = 0;
      l2 = 0;
      l10 = 0;
      l11 = 0;

      r0 = r0 - r0;

      leaf_exit;  /* VGP */

_ifft8_c_DPD.end:
_ifft16_c_DPD.end:
_ifft32_c_DPD.end:
_ifft64_c_DPD.end:
_ifft128_c_DPD.end:
_ifft256_c_DPD.end:
_ifft512_c_DPD.end:
_ifft1024_c_DPD.end:
_ifft2048_c_DPD.end:

.endseg;
