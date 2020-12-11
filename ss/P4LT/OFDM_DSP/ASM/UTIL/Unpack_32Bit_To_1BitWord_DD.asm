/******************************************************************************
 *
 * PROJECT     : DRS7 for 2116x
 * FILE        : $Workfile: Unpack_32Bit_To_1BitWord_DD.asm $
 * COPYRIGHT   : 2003 by TL Consulting GmbH
 * DESCRIPTION : Converts input vector with elements of 32 bits per word into output bit vector (LSB bit) 
 * OPERATION   : 
 * PROTOTYPE   : void Unpack_32Bit_To_1BitWord_DD( int   R[],    // output vector of bit per word elements of length 32*N
 *                                          int   X[],           // input vector of 32 bit integers per word elements of length N
 *                                          int   N   );  
 * ASSUMPTIONS :
 *               output: R[32*N] vector of length 32*N, organized as 1 (LSB) bit per word
 *                      unpacking accomplished from MSB (31) first
 *               input: X[N] vector of length N, organized as 32-bit per word
 *               input: N number of words in input vector X[] 
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/Unpack_32Bit_To_1BitWord_DD.asm $
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

		.global _Unpack_32Bit_To_1BitWord_DD;
		.global _Unpack_32Bit_To_1BitWord_EE;
		.global _Unpack_32Bit_To_1BitWord_ED;
		.global _Unpack_32Bit_To_1BitWord_DE;

// m7 must be -1
// m6 must be 1

_Unpack_32Bit_To_1BitWord_DD:
_Unpack_32Bit_To_1BitWord_EE:
_Unpack_32Bit_To_1BitWord_ED:
_Unpack_32Bit_To_1BitWord_DE:

    leaf_entry;

    puts = r3;
    r0 = i0; puts = r0;
    r0 = i1; puts = r0;

    i0 = r4;              // r4 = address of R[]
    i1 = r8;              // r8 = address of X[]
                          // r12 = N

    r8=r8-r8, r3 = dm(i1,m6);       // read x[0]

    LCNTR = r12, do (pc,UnpackLp) until lce;
          r0 = fext r3 by  31:1, r4 = dm(i1,m6);
          r0 = fext r3 by  30:1, dm(i0,m6) = r0;
          r0 = fext r3 by  29:1, dm(i0,m6) = r0;
          r0 = fext r3 by  28:1, dm(i0,m6) = r0;
          r0 = fext r3 by  27:1, dm(i0,m6) = r0;
          r0 = fext r3 by  26:1, dm(i0,m6) = r0;
          r0 = fext r3 by  25:1, dm(i0,m6) = r0;
          r0 = fext r3 by  24:1, dm(i0,m6) = r0;
          r0 = fext r3 by  23:1, dm(i0,m6) = r0;
          r0 = fext r3 by  22:1, dm(i0,m6) = r0;
          r0 = fext r3 by  21:1, dm(i0,m6) = r0;
          r0 = fext r3 by  20:1, dm(i0,m6) = r0;
          r0 = fext r3 by  19:1, dm(i0,m6) = r0;
          r0 = fext r3 by  18:1, dm(i0,m6) = r0;
          r0 = fext r3 by  17:1, dm(i0,m6) = r0;
          r0 = fext r3 by  16:1, dm(i0,m6) = r0;
          r0 = fext r3 by  15:1, dm(i0,m6) = r0;
          r0 = fext r3 by  14:1, dm(i0,m6) = r0;
          r0 = fext r3 by  13:1, dm(i0,m6) = r0;
          r0 = fext r3 by  12:1, dm(i0,m6) = r0;
          r0 = fext r3 by  11:1, dm(i0,m6) = r0;
          r0 = fext r3 by  10:1, dm(i0,m6) = r0;
          r0 = fext r3 by   9:1, dm(i0,m6) = r0;
          r0 = fext r3 by   8:1, dm(i0,m6) = r0;
          r0 = fext r3 by   7:1, dm(i0,m6) = r0;
          r0 = fext r3 by   6:1, dm(i0,m6) = r0;
          r0 = fext r3 by   5:1, dm(i0,m6) = r0;
          r0 = fext r3 by   4:1, dm(i0,m6) = r0;
          r0 = fext r3 by   3:1, dm(i0,m6) = r0;
          r0 = fext r3 by   2:1, dm(i0,m6) = r0;
          r0 = fext r3 by   1:1, dm(i0,m6) = r0;
          r0 = fext r3 by   0:1, dm(i0,m6) = r0;
UnpackLp: r3 = r4+r8,           dm(i0,m6) = r0;    


epil:
    r3 = gets( 3 );
    i0 = gets( 2 );
    i1 = gets( 1 );

    leaf_exit; 

_Unpack_32Bit_To_1BitWord_DD.end:
_Unpack_32Bit_To_1BitWord_EE.end:
_Unpack_32Bit_To_1BitWord_ED.end:
_Unpack_32Bit_To_1BitWord_DE.end:

.endseg;	




