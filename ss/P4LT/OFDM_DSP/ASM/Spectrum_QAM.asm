/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600
 * FILE        : $Workfile: Spectrum_QAM.asm $
 * COPYRIGHT   : 2003 by TL Consulting GmbH.
 * DESCRIPTION : Deposit carriers scaled by real scalar in to OFDM Spectrum 
 *                               according to given vector of indecies
 * OPERATION   : 
 * PROTOTYPE   : void Spectrum_QAM( complex_float out[],
 *                                                   complex_float  in[],
 *                                             int   dm      ind4in[],
 *                                             int           Lind,
 *                                             float         scale
 *                                               );
 * ASSUMPTIONS :
 *               All arrays must start on even address boundaries
  *
 * REVISION HISTORY:
 * COMPLEXITY : for 42 carriers 337 cycles
 *-----------------------------------------------------------------------------
$Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/Spectrum_QAM.asm $
 * 
 * 1     19.01.05 15:37 Chchlee
 * 
 * 2     30.09.04 8:27 Leeb02
  * 
 * 1     12-03-02  VGP
 * Initial version
 *
 * 
 *****************************************************************************/

#ifdef __SIMD

#ifdef __ADSP21160__
#include <def21160.h>
#endif
#ifdef __ADSP21161__
#include <def21161.h>
#endif

#endif

#include <asm_sprt.h>


		.segment/pm   seg_pmco;

.global _Spectrum_QAM;

_Spectrum_QAM:
      leaf_entry;

      r1 = i3;  puts = r1;                         /* Save non-scratch registers */
      r1 = i2;  puts = r1;
      r1 = i1;  puts = r1;                       
      r1 = i0;  puts = r1;
      
      mr0f = r4;                       /* mr0f = address of complex_float out[0] */
      i1   = r8;                       /* i1 = address of complex_float in[0] */
      r12  = r12 - r12, i0    = r12;   /* r12=0, i0=address of vector of OFDM carriers's indecies */
      r12  = r12 + 1, r8   = reads(1); /* r12=1, r8=number of OFDM carriers/indexies */          
      f0   = reads(2);                 /* scale factor */

      r12  = r12 + 1, r1 = dm(i0,m6);          // r12=2, r1 = first carrier index
      r1 = mrf + r1*r12(uui), f3 = dm(i1,m6);  // r1 - address of first real carrier in input OFDM spectrum
      f3 = f3 * f0,  i3 = r1;
      r8 = r8 -  1,  dm(i3,m6) = f3;           // out.re[j] 
 	  f4 = dm(i1,m6);                          // f4 = in.im[i]
      lcntr = r8, do (pc,DepLp) until lce;
         f4 = f4 * f0,           r1 = dm(i0,m6); // r1 = next carrier index
         r1 = mrf + r1*r12(uui), dm(i3,m5) = f4; // r1 = address of output; out.im[j], store imaginary part in the output vector
         f3 = dm(i1,m6);                         // f3 = in.re[i]
         f3 = f3 * f0,           i3 = r1;
         dm(i3,m6) = f3;                         // out.re[j]
DepLp:   f4 = dm(i1,m6);                         // f4 = in.im[i]
      f4 = f4 * f0;
      dm(i3,m5) = f4;                            // f4 = out.im[j]

      i3 = gets( 4 );
      i2 = gets( 3 );
      i1 = gets( 2 );
      i0 = gets( 1 );

        leaf_exit;

_Spectrum_QAM.end:

.endseg;

