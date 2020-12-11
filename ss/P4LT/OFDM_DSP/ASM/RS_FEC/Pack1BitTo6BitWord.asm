/******************************************************************************
 *
 * PROJECT     : DRS7 for 2116x
 * FILE        : $Workfile: Pack1BitTo6BitWord.asm $
 * COPYRIGHT   : 2003 by TL Consulting GmbH
 * DESCRIPTION : Converts input bit vector (one LSB per word) into output vector with elements of 6 LSB bits per word 
 * OPERATION   : 
 * PROTOTYPE   : int  Pack1BitTo6BitWord_DD( int   R[],    // output vector of 6 bit integers per word elements of length N/6
 *                                        int   X[],    // input vector of bit per word elements of length N
 *                                        int   N   );  
 * ASSUMPTIONS :
 *               output: R[N/6] vector of length N/6, organized as 6 (LSB) bit per word
 *               input:  X[N] vector of length N, organized as 1 (LSB) bit per word
 *                       packing accomplished from firts bit #5 (bit #0 - LSB) word
 *               input: N number of words in input vector X[] 
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/RS_FEC/Pack1BitTo6BitWord.asm $
 * 
 * 1     19.01.05 15:37 Chchlee
 * 
 * 2     30.09.04 8:27 Leeb02
 * 
 * 
 * 1     11/03/03 13:26 vgp
 * Initial version
 * 2     13/09/04 13:26 vgp
 * New testing to avoid possible underflow
 * 
 *****************************************************************************/


#include <asm_sprt.h>

#ifdef __ADSP21160__
#include <def21160.h>
#endif
#ifdef __ADSP21161__
#include <def21161.h>
#endif

.segment/pm		seg_pmco;

		.global _Pack1BitTo6BitWord_DD;

// m7 must be -1
// m6 must be 1

_Pack1BitTo6BitWord_DD:

    leaf_entry;

    puts = r12;
	r0 = i0; puts = r0;
    r0 = i1; puts = r0;

    i0 = r4;              // r4 = address of R[]
    i1 = r8;              // r8 = address of X[]
                          // r12 = N
    r12 = pass r12, r0 = m7;
    if lt jump Epil;
    
    f0 = float r12;       // numerator
    f8 = 2.0;
    f12 = 6.0;            // denominator
	f0 = recips f12,  f4 = f0;        
	f12 = f0*f12;                 
	f4 = f0*f4,  f0 = f8-f12;        
	f12 = f0*f12;                  
	f4 = f0*f4,  f0 = f8-f12;        
	f12 = f0*f12;       
	f4 = f0*f4, f0 = f8-f12;        
	f0 = f0*f4;
	r12 = fix f0;         // result                   

    r0 = gets(3);         // read N
	r4 = 6;
	r4 = r4 * r12(uui);
	r0 = r0 - r4;
	if ne jump Epil;
	
    r0 = r0 - r0,  r4 = dm(i1,m6);    // read x[0]

    LCNTR = r12, do (pc,UnpackLp) until lce;
          r0 = r0 or fdep r4 by  5:1, r4 = dm(i1,m6);
          r0 = r0 or fdep r4 by  4:1, r4 = dm(i1,m6);
          r0 = r0 or fdep r4 by  3:1, r4 = dm(i1,m6);
          r0 = r0 or fdep r4 by  2:1, r4 = dm(i1,m6);
          r0 = r0 or fdep r4 by  1:1, r4 = dm(i1,m6);
          r0 = r0 or fdep r4 by  0:1, r4 = dm(i1,m6);
UnpackLp: r0 = r0 - r0,               dm(i0,m6) = r0;    
    

Epil:
    i0 = gets( 2 );
    i1 = gets( 1 );

    leaf_exit; 

_Pack1BitTo6BitWord_DD.end:

.endseg;	




