/******************************************************************************
 *
 * PROJECT     : DRS7 for 2116x
 * FILE        : $Workfile: Pack_1Bit_To_32BitWord_DD.asm $
 * COPYRIGHT   : 2003 by TL Consulting GmbH
 * DESCRIPTION : Converts input bit vector (one LSB per word) into output vector with elements 32 bits per word 
 * OPERATION   : 
 * PROTOTYPE   : int  Pack_1Bit_To_32BitWord_DD( int   R[],    // output vector of 32 bit integers per word elements of length N/32
 *                                        int   X[],    // input vector of bit per word elements of length N
 *                                        int   N   );  
 * ASSUMPTIONS :
 *               output: R[N/32] vector of length N/32, organized as 32-bit per word
 *               input:  X[N] vector of length N, organized as 1 (LSB) bit per word
 *                       packing accomplished from bit #31 (bit #0 - LSB) word
 *               input: N number of words in input vector X[] 
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/Pack_1Bit_To_32BitWord_DD.asm $
 * 
 * 1     19.01.05 15:38 Chchlee
 * 
 * 1     22.06.04 14:25 Unp00611
 * 
 * 
 * 1     10/03/03 13:26 vgp
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


.segment/pm		seg_pmco;

		.global _Pack_1Bit_To_32BitWord_DD;
		.global _Pack_1Bit_To_32BitWord_EE;
		.global _Pack_1Bit_To_32BitWord_ED;
		.global _Pack_1Bit_To_32BitWord_DE;

// m7 must be -1
// m6 must be 1

_Pack_1Bit_To_32BitWord_DD:
_Pack_1Bit_To_32BitWord_EE:
_Pack_1Bit_To_32BitWord_ED:
_Pack_1Bit_To_32BitWord_DE:

    leaf_entry;

    puts = r3;
    r0 = i0; puts = r0;
    r0 = i1; puts = r0;

    i0 = r4;              // r4 = address of R[]
    i1 = r8;              // r8 = address of X[]
                          // r12 = N
    r4 = 0x01f;
    r8 = r12 and r4, r0 = m7;
    if ne jump Epil;

    r12 = lshift r12 by -5;    
    r0 = r0 - r0,  r4 = dm(i1,m6);    // read x[0]

    LCNTR = r12, do (pc,UnpackLp) until lce;
          r0 = r0 or fdep r4 by 31:1, r4 = dm(i1,m6);
          r0 = r0 or fdep r4 by 30:1, r4 = dm(i1,m6);
          r0 = r0 or fdep r4 by 29:1, r4 = dm(i1,m6);
          r0 = r0 or fdep r4 by 28:1, r4 = dm(i1,m6);
          r0 = r0 or fdep r4 by 27:1, r4 = dm(i1,m6);
          r0 = r0 or fdep r4 by 26:1, r4 = dm(i1,m6);
          r0 = r0 or fdep r4 by 25:1, r4 = dm(i1,m6);
          r0 = r0 or fdep r4 by 24:1, r4 = dm(i1,m6);
          r0 = r0 or fdep r4 by 23:1, r4 = dm(i1,m6);
          r0 = r0 or fdep r4 by 22:1, r4 = dm(i1,m6);
          r0 = r0 or fdep r4 by 21:1, r4 = dm(i1,m6);
          r0 = r0 or fdep r4 by 20:1, r4 = dm(i1,m6);
          r0 = r0 or fdep r4 by 19:1, r4 = dm(i1,m6);
          r0 = r0 or fdep r4 by 18:1, r4 = dm(i1,m6);
          r0 = r0 or fdep r4 by 17:1, r4 = dm(i1,m6);
          r0 = r0 or fdep r4 by 16:1, r4 = dm(i1,m6);
          r0 = r0 or fdep r4 by 15:1, r4 = dm(i1,m6);
          r0 = r0 or fdep r4 by 14:1, r4 = dm(i1,m6);
          r0 = r0 or fdep r4 by 13:1, r4 = dm(i1,m6);
          r0 = r0 or fdep r4 by 12:1, r4 = dm(i1,m6);
          r0 = r0 or fdep r4 by 11:1, r4 = dm(i1,m6);
          r0 = r0 or fdep r4 by 10:1, r4 = dm(i1,m6);
          r0 = r0 or fdep r4 by  9:1, r4 = dm(i1,m6);
          r0 = r0 or fdep r4 by  8:1, r4 = dm(i1,m6);
          r0 = r0 or fdep r4 by  7:1, r4 = dm(i1,m6);
          r0 = r0 or fdep r4 by  6:1, r4 = dm(i1,m6);
          r0 = r0 or fdep r4 by  5:1, r4 = dm(i1,m6);
          r0 = r0 or fdep r4 by  4:1, r4 = dm(i1,m6);
          r0 = r0 or fdep r4 by  3:1, r4 = dm(i1,m6);
          r0 = r0 or fdep r4 by  2:1, r4 = dm(i1,m6);
          r0 = r0 or fdep r4 by  1:1, r4 = dm(i1,m6);
          r0 = r0 or fdep r4 by  0:1, r4 = dm(i1,m6);
UnpackLp: r0 = r0 - r0,               dm(i0,m6) = r0;    
    

Epil:
    r3 = gets( 3 );
    i0 = gets( 2 );
    i1 = gets( 1 );

    leaf_exit; 
    
_Pack_1Bit_To_32BitWord_DD.end:
_Pack_1Bit_To_32BitWord_EE.end:
_Pack_1Bit_To_32BitWord_ED.end:
_Pack_1Bit_To_32BitWord_DE.end:

.endseg;	




