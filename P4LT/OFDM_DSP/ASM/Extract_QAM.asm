/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600
 * FILE        : $Workfile: Extract_QAM.asm $
 * COPYRIGHT   : 2003 by TL Consulting GmbH.
 * DESCRIPTION : Extract QAM modulated carriers
 * OPERATION   : 
 * PROTOTYPE   : void Extract_QAM(complex_float out[],complex_float in[],int ind4in[],int Lind,);
 * ASSUMPTIONS :
 *
 * REVISION HISTORY:
 * COMPLEXITY : 
 *-----------------------------------------------------------------------------
$Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/Extract_QAM.asm $
 * 
 * 1     19.01.05 15:36 Chchlee
 * 
 * 2     30.09.04 8:27 Leeb02
 * 
 * 1     20-03-03  VGP
 * Initial version
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

.global _Extract_QAM;

_Extract_QAM:
      leaf_entry;

                                        /* Save non-scratch registers */
      r0 = i4;  puts = r0;                       
      r0 = i1;  puts = r0;
      r0 = i0;  puts = r0;

/*  r4 pointer to complex_float out[] in dm */
/*  r8 pointer to complex_float in[] in dm */
/* r12 pointer to int ind4in[Lind] in dm (vector of indecies) */
/* (1) int, number of indecies Lind, */
      

    r12 = r12 - r12, i1 = r12;       // i1 - points vector of indecies
    mr0f = r8;                       // mr0f = address of complex_float in[0]
    r12 = r12 + 1,  i0 = r4;         // i0 - points complex_float out[]
	r12 = r12 + 1,  r8   = reads(1); // number of OFDM carriers/indecies 
	f0   = reads(2);                 // scale factor 
    
    r1 = dm(i1,m6);                  // r1 = first carrier index
    r1 = mrf + r1*r12(uui);          // r1 - address of first real carrier in output OFDM spectrum
    i4 = r1;
    f3 = dm(i4,m6);                  // real part of input
    r8 = r8 -  1,  dm(i0,m6) = f3;   // out.re[j] 
    f4 = dm(i4,m5);                  // f4 = in.im[i]
    lcntr = r8, do (pc,ExtrLp) until lce;
         r1 = dm(i1,m6);                           // r1 = next carrier index
         r1 = mrf + r1*r12(uui), dm(i0,m6) = f4; // r1 = address of output; out.im[j], store imaginary part in the output vector
         i4 = r1;        
         f3 = dm(i4,m6);                         // f3 = in.re[i]
         dm(i0,m6) = f3;                         // out.re[j]
ExtrLp:  f4 = dm(i4,m5);                         // f4 = in.im[i]
      dm(i0,m5) = f4;                            // f4 = out.im[j]

     i4 = gets( 3 );
     i1 = gets( 2 );
     i0 = gets( 1 );

    leaf_exit;

_Extract_QAM.end:

.endseg;

