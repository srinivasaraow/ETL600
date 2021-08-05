/******************************************************************************
 *
 * PROJECT     : DRS7 for 2116x
 * FILE        : $Workfile: Unpack6BitTo1BitWord.asm $
 * COPYRIGHT   : 2003 by TL Consulting GmbH
 * DESCRIPTION : Converts input vector with elements of 6 LSB bits per word  into output bit vector (LSB bit) 
 * OPERATION   : 
 * PROTOTYPE   : void Unpack6BitTo1BitWord_DD( int   R[],    // output vector of bit per word elements of length 8*N
 *                                          int   X[],    // input vector of 6 bit integers per word elements of length N
 *                                          int   N   );  
 * ASSUMPTIONS :
 *               output: R[6*N] vector of length 6*N, organized as 1 (LSB) bit per word
 *                      unpacking accomplished from 6 LSB word from MSB (5) first
 *               input: X[N] vector of length N, organized as 6 (LSB) bit per word
 *               input: N number of words in input vector X[] 
 *               All arrays must start on even address boundaries
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/RS_FEC/Unpack6BitTo1BitWord.asm $
 * 
 * 1     19.01.05 15:37 Chchlee
 * 
 * 2     30.09.04 8:27 Leeb02
 * 
 * 
 * 1     11/03/03 13:26 vgp
 * Initial version
 * 
 *****************************************************************************/


#include <asm_sprt.h>

#ifdef __ADSP21160__
#include <def21160.h>
#endif
#ifdef __ADSP21161__
#include <def21161.h>
#endif

//.segment/pm		seg_pmco;
		.segment/pm   seg_ext_pmco;

		.global _Unpack6BitTo1BitWord_DD;

// m7 must be -1
// m6 must be 1

_Unpack6BitTo1BitWord_DD:

    leaf_entry;

    puts = r3;
	r0 = i0; puts = r0;
    r0 = i1; puts = r0;

    i0 = r4;              // r4 = address of R[]
    i1 = r8;              // r8 = address of X[]
                          // r12 = N

    r8=r8-r8, r3 = dm(i1,m6);       // read x[0]

    LCNTR = r12, do (pc,UnpackLp) until lce;
          r0 = fext r3 by  5:1, r4 = dm(i1,m6);
          r0 = fext r3 by  4:1, dm(i0,m6) = r0;
          r0 = fext r3 by  3:1, dm(i0,m6) = r0;
          r0 = fext r3 by  2:1, dm(i0,m6) = r0;
          r0 = fext r3 by  1:1, dm(i0,m6) = r0;
          r0 = fext r3 by  0:1, dm(i0,m6) = r0;
UnpackLp: r3 = r4+r8,           dm(i0,m6) = r0;    


epil:
    r3 = gets( 3 );
    i0 = gets( 2 );
    i1 = gets( 1 );

    leaf_exit; 

_Unpack6BitTo1BitWord_DD.end:

.endseg;	




