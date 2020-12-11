/******************************************************************************
 *
 * PROJECT     : ETL600
 * FILE        : $Workfile: FFT63_8.asm $
 * COPYRIGHT   : 2004 by TL Consulting GmbH
 * DESCRIPTION : Calculates 8  coefficients of Fast Fourier Transform over GF(64)
 *               for 63 element input vector
 * OPERATION   : 
 * PROTOTYPE   : FFT63_8(int   X[], int S[]) ;    
 *                                                          
 * ASSUMPTIONS :
 *               Number of elements in input X[] vector is equal to 63 
 *
 *
 * Used  registers:     m6=1
 * Saved registers:     i0, i1, i2, i3, i5
 * Destroyed registers: r0, r1, r2, r4, i4, m4
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/RS_FEC/FFT63_8.asm $
 * 
 * 1     19.01.05 15:37 Chchlee
 * 
 * 4     30.09.04 8:27 Leeb02
 * 
 * 
 * 1     12.01.04 vgp
 * Initial version Complexity  1155 cycles
 * 2     17.01.04  vgp
 * additional _logGF64 table in dm memory is inserted to improve execution time
 * 3     09.09.04  vgp
 * avoid usage of m5 register
 * 4     12.09.04  vgp
 * 16-bit addressing mode for logarithmic and power tables
 * 
 *****************************************************************************/

#include <asm_sprt.h>
#include "../../h/rs_fec/RS6355_defs.h"

#ifdef __ADSP21160__
#include <def21160.h>
#endif
#ifdef __ADSP21161__
#include <def21161.h>
#endif

//==============================================
.section/dm	seg_dmda;

.extern pwrGF64;
.extern logGF64;
.extern Lo_ptr;
.extern Mo_ptr;

.endseg;


//==============================================

//		.segment/pm   seg_pmco;
		.segment/pm   seg_ext_pmco;

		.global _FFT63_8;

// m5, m13 must be 0
// m6, m14 must be 1
// m7, m15 must be -1

// r4 - address of X[]
// r8 - address of S[]

_FFT63_8:

    leaf_entry;

    r0 = i0; puts = r0;
    r0 = i1; puts = r0;
    r0 = i2; puts = r0;
    r0 = i3; puts = r0;
    r0 = i5; puts = r0;

    i4 = r4;              // i4 address of X[]

    i0 = dm(Lo_ptr);
//lo[ 0] = a[30] ^ a[51] ;
    r0 = dm(30,i4);
	r1 = dm(51,i4);
	r2 = r0 xor r1, i1 = i0; // i0 and i1 point lo[] 
//lo[ 1] = a[ 8] ^ a[44] ;
    r0 = dm( 8,i4);
	r1 = dm(44,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[0]
//lo[ 2] = a[10] ^ a[28] ;
    r0 = dm(10,i4);
	r1 = dm(28,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[1]
//lo[ 3] = a[ 9] ^ a[27] ;
    r0 = dm( 9,i4);
	r1 = dm(27,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[2]
//lo[ 4] = a[24] ^ a[33] ;
    r0 = dm(24,i4);
	r1 = dm(33,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[3]
//lo[ 5] = a[36] ^ a[45] ;
    r0 = dm(36,i4);
	r1 = dm(45,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[4]
//lo[ 6] = a[23] ^ a[59] ;
    r0 = dm(23,i4);
	r1 = dm(59,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[5]
//lo[ 7] = a[21] ^ a[42] ;
    r0 = dm(21,i4);
	r1 = dm(42,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[6]
//lo[ 8] = a[16] ^ a[43] ;
    r0 = dm(16,i4);
	r1 = dm(43,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[7]
//lo[ 9] = a[52] ^ a[61] ;
    r0 = dm(52,i4);
	r1 = dm(61,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[8]
//lo[10] = a[ 5] ^ a[14] ;
    r0 = dm( 5,i4);
	r1 = dm(14,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[9]
//lo[11] = a[31] ^ a[47] ;
    r0 = dm(31,i4);
	r1 = dm(47,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[10]
//lo[12] = a[ 7] ^ a[34] ;
    r0 = dm( 7,i4);
	r1 = dm(34,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[11]
//lo[13] = a[13] ^ a[18] ;
    r0 = dm(13,i4);
	r1 = dm(18,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[12]
//lo[14] = a[11] ^ a[32] ;
    r0 = dm(11,i4);
	r1 = dm(32,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[13]
//lo[15] = a[11] ^ a[54] ;
    r0 = dm(11,i4);
	r1 = dm(54,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[14]
//lo[16] = a[ 9] ^ a[30] ;
    r0 = dm( 9,i4);
	r1 = dm(30,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[15]
//lo[17] = a[35] ^ a[38] ;
    r0 = dm(35,i4);
	r1 = dm(38,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[16]
//lo[18] = a[ 8] ^ a[15] ;
    r0 = dm( 8,i4);
	r1 = dm(15,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[17]
//lo[19] = a[ 3] ^ a[20] ;
    r0 = dm( 3,i4);
	r1 = dm(20,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[18]
//lo[20] = a[17] ^ a[26] ;
    r0 = dm(17,i4);
	r1 = dm(26,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[19]
//lo[21] = a[25] ^ a[37] ;
    r0 = dm(25,i4);
	r1 = dm(37,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[20]
//lo[22] = a[ 4] ^ a[16] ;
    r0 = dm( 4,i4);
	r1 = dm(16,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[21]
//lo[23] = a[ 2] ^ a[38] ;
    r0 = dm( 2,i4);
	r1 = dm(38,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[22]
//lo[24] = a[29] ^ a[53] ;
    r0 = dm(29,i4);
	r1 = dm(53,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[23]
//lo[25] = a[46] ^ a[58] ;
    r0 = dm(46,i4);
	r1 = dm(58,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[24]
//lo[26] = a[39] ^ a[61] ;
    r0 = dm(39,i4);
	r1 = dm(61,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[25]
//lo[27] = a[15] ^ a[28] ;
    r0 = dm(15,i4);
	r1 = dm(28,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[26]
//lo[28] = a[38] ^ a[57] ;
    r0 = dm(38,i4);
	r1 = dm(57,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[27]
//lo[29] = a[22] ^ a[43] ;
    r0 = dm(22,i4);
	r1 = dm(43,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[28]
//lo[30] = a[13] ^ a[49] ;
    r0 = dm(13,i4);
	r1 = dm(49,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[29]
//lo[31] = a[ 0] ^ a[60] ;
    r0 = dm( 0,i4);
	r1 = dm(60,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[30]
//lo[32] = a[18] ^ a[27] ;
    r0 = dm(18,i4);
	r1 = dm(27,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[31]
	dm(i0,m6)=r2;                 // lo[32]

    

//lo[33] = a[34] ^ a[40] ^ a[55] ;
    r0 = dm(34,i4);
	r1 = dm(40,i4);
    r0 = r0 xor r1;
	r1 = dm(55,i4);
	r2 = r0 xor r1;
//lo[34] = a[ 6] ^ lo[ 0] ;
    r0 = dm(6,i4);
	r1 = dm(0,i1);                // r1 = lo[0]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[33]
//lo[35] = a[48] ^ a[49] ^ a[58] ;
    r0 = dm(48,i4);
	r1 = dm(49,i4);
    r0 = r0 xor r1;
	r1 = dm(58,i4);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[34]
//lo[36] = a[36] ^ a[41] ^ a[62] ;
    r0 = dm(36,i4);
	r1 = dm(41,i4);
    r0 = r0 xor r1;
	r1 = dm(62,i4);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[35]
//lo[37] = a[24] ^ a[45] ^ a[57] ;
    r0 = dm(24,i4);
	r1 = dm(45,i4);
    r0 = r0 xor r1;
	r1 = dm(57,i4);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[36]
//lo[38] = a[17] ^ a[23] ^ a[44] ;
    r0 = dm(17,i4);
	r1 = dm(23,i4);
    r0 = r0 xor r1;
	r1 = dm(44,i4);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[37]
//lo[39] = a[16] ^ a[20] ^ a[52] ;
    r0 = dm(16,i4);
	r1 = dm(20,i4);
    r0 = r0 xor r1;
	r1 = dm(52,i4);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[38]
//lo[40] = a[ 2] ^ a[37] ^ a[58] ;
    r0 = dm( 2,i4);
	r1 = dm(37,i4);
    r0 = r0 xor r1;
	r1 = dm(58,i4);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[39]
//lo[41] = a[ 1] ^ a[19] ^ a[55] ;
    r0 = dm( 1,i4);
	r1 = dm(19,i4);
    r0 = r0 xor r1;
	r1 = dm(55,i4);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[40]
//lo[42] = a[27] ^ a[36] ^ a[57] ;
    r0 = dm(27,i4);
	r1 = dm(36,i4);
    r0 = r0 xor r1;
	r1 = dm(57,i4);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[41]
//lo[43] = a[54] ^ lo[ 5] ;
    r0 = dm(54,i4);
	r1 = dm( 5,i1);               // r1 = lo[5]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[42]
//lo[44] = a[32] ^ a[41] ^ lo[ 6] ;
    r0 = dm(32,i4);
	r1 = dm(41,i4);
    r0 = r0 xor r1;
	r1 = dm( 6,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[43]
//lo[45] = a[35] ^ a[62] ^ lo[20] ;
    r0 = dm(35,i4);
	r1 = dm(62,i4);
    r0 = r0 xor r1;
	r1 = dm(20,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[44]
//lo[46] = a[ 5] ^ a[10] ^ a[61] ^ a[62] ;
    r0 = dm( 5,i4);
	r1 = dm(10,i4);
    r0 = r0 xor r1;
	r1 = dm(61,i4);
    r0 = r0 xor r1;
	r1 = dm(62,i4);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[45]
//lo[47] = a[ 4] ^ a[29] ^ lo[ 2] ;
    r0 = dm( 4,i4);
	r1 = dm(29,i4);
    r0 = r0 xor r1;
	r1 = dm( 2,i1);               // lo[2]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[46]
//lo[48] = a[25] ^ a[39] ^ a[42] ^ a[46] ;
    r0 = dm(25,i4);
	r1 = dm(39,i4);
    r0 = r0 xor r1;
	r1 = dm(42,i4);
    r0 = r0 xor r1;
	r1 = dm(46,i4);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[47]
//lo[49] = a[ 7] ^ a[14] ^ a[28] ^ a[52] ^ a[56] ;
    r0 = dm( 7,i4);
	r1 = dm(14,i4);
    r0 = r0 xor r1;
	r1 = dm(28,i4);
    r0 = r0 xor r1;
	r1 = dm(52,i4);
    r0 = r0 xor r1;
	r1 = dm(56,i4);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[48]
//lo[50] = a[40] ^ a[47] ^ lo[17] ^ lo[ 8] ;
    r0 = dm(40,i4);
	r1 = dm(47,i4);
    r0 = r0 xor r1;
	r1 = dm(17,i1);               //lo[17]
    r0 = r0 xor r1;
	r1 = dm( 8,i1);               //lo[8]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[49]
//lo[51] = a[ 3] ^ a[ 6] ^ a[12] ^ a[48] ^ lo[ 4] ;
    r0 = dm( 3,i4);
	r1 = dm( 6,i4);
    r0 = r0 xor r1;
	r1 = dm(12,i4);
    r0 = r0 xor r1;
	r1 = dm(48,i4);
    r0 = r0 xor r1;
	r1 = dm( 4,i1);               // lo[4]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[50]
//lo[52] = a[37] ^ lo[38] ^ lo[18] ;
    r0 = dm(37,i4);
	r1 = dm(38,i1);               // lo[38]
    r0 = r0 xor r1;
	r1 = dm(18,i1);               // lo[18]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[51]
//lo[53] = a[ 8] ^ a[18] ^ lo[24] ^ lo[ 7] ;
    r0 = dm( 8,i4);
	r1 = dm(18,i4);
    r0 = r0 xor r1;
	r1 = dm(24,i1);               // lo[24]
    r0 = r0 xor r1;
	r1 = dm( 7,i1);               // lo[7]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[52]
//lo[54] = a[12] ^ a[19] ^ a[54] ^ lo[33] ;
    r0 = dm(12,i4);
	r1 = dm(19,i4);
    r0 = r0 xor r1;
	r1 = dm(54,i4);               
    r0 = r0 xor r1;
	r1 = dm(33,i1);               // lo[33]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[53]
//lo[55] = a[48] ^ lo[34] ^ lo[ 3] ;
    r0 = dm(48,i4);
	r1 = dm(34,i1);               // lo[34]
    r0 = r0 xor r1;
	r1 = dm( 3,i1);               // lo[ 3]               
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[54]
//lo[56] = a[15] ^ a[39] ^ a[57] ^ a[60] ^ lo[ 0] ;
    r0 = dm(15,i4);
	r1 = dm(39,i4);
    r0 = r0 xor r1;
	r1 = dm(57,i4);
    r0 = r0 xor r1;
	r1 = dm(60,i4);
    r0 = r0 xor r1;
	r1 = dm( 0,i1);               // lo[0]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[55]
//lo[57] = a[21] ^ a[41] ^ a[50] ^ a[57] ^ lo[16] ;
    r0 = dm(21,i4);
	r1 = dm(41,i4);
    r0 = r0 xor r1;
	r1 = dm(50,i4);
    r0 = r0 xor r1;
	r1 = dm(57,i4);
    r0 = r0 xor r1;
	r1 = dm(16,i1);               // lo[16]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[56]
//lo[58] = a[18] ^ a[39] ^ a[48] ^ a[54] ^ lo[ 1] ;
    r0 = dm(18,i4);
	r1 = dm(39,i4);
    r0 = r0 xor r1;
	r1 = dm(48,i4);
    r0 = r0 xor r1;
	r1 = dm(54,i4);
    r0 = r0 xor r1;
	r1 = dm( 1,i1);               // lo[1]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[57]
//lo[59] = a[12] ^ a[24] ^ a[31] ^ a[62] ^ lo[ 9] ;
    r0 = dm(12,i4);
	r1 = dm(24,i4);
    r0 = r0 xor r1;
	r1 = dm(31,i4);
    r0 = r0 xor r1;
	r1 = dm(62,i4);
    r0 = r0 xor r1;
	r1 = dm( 9,i1);               // lo[9]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[58]
//lo[60] = a[44] ^ a[45] ^ lo[44] ;
    r0 = dm(44,i4);
	r1 = dm(45,i4);
    r0 = r0 xor r1;
	r1 = dm(44,i1);               // lo[44]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[59]
//lo[61] = a[ 4] ^ a[22] ^ a[31] ^ a[40] ^ a[58] ^ lo[30] ;
    r0 = dm( 4,i4);
	r1 = dm(22,i4);
    r0 = r0 xor r1;
	r1 = dm(31,i4);
    r0 = r0 xor r1;
	r1 = dm(40,i4);
    r0 = r0 xor r1;
	r1 = dm(58,i4);
    r0 = r0 xor r1;
	r1 = dm(30,i1);               // lo[30]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[60]
//lo[62] = a[11] ^ a[20] ^ a[29] ^ a[47] ^ a[56] ^ lo[23] ;
    r0 = dm(11,i4);
	r1 = dm(20,i4);
    r0 = r0 xor r1;
	r1 = dm(29,i4);
    r0 = r0 xor r1;
	r1 = dm(47,i4);
    r0 = r0 xor r1;
	r1 = dm(56,i4);
    r0 = r0 xor r1;
	r1 = dm(23,i1);               // lo[23]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[61]
//lo[63] = a[50] ^ lo[44] ^ lo[10] ;
    r0 = dm(50,i4);
	r1 = dm(44,i1);               // lo[44]
    r0 = r0 xor r1;
	r1 = dm(10,i1);               // lo[10]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[62]
//lo[64] = a[53] ^ lo[45] ^ lo[ 1] ;
    r0 = dm(53,i4);
	r1 = dm(45,i1);               // lo[45]
    r0 = r0 xor r1;
	r1 = dm( 1,i1);               // lo[ 1]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[63]
//lo[65] = a[37] ^ a[46] ^ lo[41] ^ lo[ 2] ;
    r0 = dm(37,i4);
	r1 = dm(46,i4);
    r0 = r0 xor r1;
	r1 = dm(41,i1);               // lo[41]               
    r0 = r0 xor r1;
	r1 = dm( 2,i1);               // lo[ 2]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[64]
//lo[66] = a[17] ^ a[20] ^ a[59] ^ lo[33] ^ lo[11] ;
    r0 = dm(17,i4);
	r1 = dm(20,i4);
    r0 = r0 xor r1;
	r1 = dm(59,i4);
    r0 = r0 xor r1;
	r1 = dm(33,i1);               // lo[33]
    r0 = r0 xor r1;
	r1 = dm(11,i1);               // lo[11]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[65]
//lo[67] = a[19] ^ a[37] ^ a[51] ^ a[56] ^ lo[19] ^ lo[ 3] ;
    r0 = dm(19,i4);
	r1 = dm(37,i4);
    r0 = r0 xor r1;
	r1 = dm(51,i4);
    r0 = r0 xor r1;
	r1 = dm(56,i4);
    r0 = r0 xor r1;
	r1 = dm(19,i1);               // lo[19]
    r0 = r0 xor r1;
	r1 = dm( 3,i1);               // lo[ 3]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[66]
//lo[68] = a[ 6] ^ a[ 7] ^ a[59] ^ lo[36] ^ lo[35] ;
    r0 = dm( 6,i4);
	r1 = dm( 7,i4);
    r0 = r0 xor r1;
	r1 = dm(59,i4);
    r0 = r0 xor r1;
	r1 = dm(36,i1);               // lo[36]
    r0 = r0 xor r1;
	r1 = dm(35,i1);               // lo[35]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[67]
//lo[69] = a[46] ^ a[49] ^ a[56] ^ lo[47] ^ lo[ 4] ;
    r0 = dm(46,i4);
	r1 = dm(49,i4);
    r0 = r0 xor r1;
	r1 = dm(56,i4);
    r0 = r0 xor r1;
	r1 = dm(47,i1);               // lo[47]
    r0 = r0 xor r1;
	r1 = dm( 4,i1);               // lo[ 4]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[68]
//lo[70] = a[33] ^ a[60] ^ lo[54] ^ lo[26] ^ lo[13] ;
    r0 = dm(33,i4);
	r1 = dm(60,i4);
    r0 = r0 xor r1;
	r1 = dm(54,i1);               // lo[54]*
    r0 = r0 xor r1;
	r1 = dm(26,i1);               // lo[26]
    r0 = r0 xor r1;
	r1 = dm(13,i1);               // lo[13]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[69]
//lo[71] = lo[56] ^ lo[51] ^ lo[ 7] ;
	r0 = dm(56,i1);               // lo[56]
	r1 = dm(51,i1);               // lo[51]
    r0 = r0 xor r1;
	r1 = dm( 7,i1);               // lo[ 7]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[70]
//lo[72] = lo[62] ^ lo[61] ;
	r0 = dm(62,i1);               // lo[62]
	r1 = dm(61,i1);               // lo[61]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[71]
//lo[73] = a[12] ^ a[27] ^ lo[57] ^ lo[45] ^ lo[ 6] ^ lo[ 5] ;
    r0 = dm(12,i4);
	r1 = dm(27,i4);
    r0 = r0 xor r1;
	r1 = dm(57,i1);               // lo[57]
    r0 = r0 xor r1;
	r1 = dm(45,i1);               // lo[45]
    r0 = r0 xor r1;
	r1 = dm( 6,i1);               // lo[ 6]
    r0 = r0 xor r1;
	r1 = dm( 5,i1);               // lo[ 5]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[72]
//lo[74] = a[19] ^ a[26] ^ a[41] ^ lo[66] ^ lo[49] ;
    r0 = dm(19,i4);
	r1 = dm(26,i4);
    r0 = r0 xor r1;
	r1 = dm(41,i4);
    r0 = r0 xor r1;
	r1 = dm(66,i1);               // lo[66]
    r0 = r0 xor r1;
	r1 = dm(49,i1);               // lo[49]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[73]
//lo[75] = a[18] ^ a[21] ^ a[26] ^ a[56] ^ a[58] ^ lo[50] ^ lo[34] ^ lo[15] ;
    r0 = dm(18,i4);
	r1 = dm(21,i4);
    r0 = r0 xor r1;
	r1 = dm(26,i4);               
    r0 = r0 xor r1;
	r1 = dm(56,i4);               
    r0 = r0 xor r1;
	r1 = dm(58,i4);               
    r0 = r0 xor r1;
	r1 = dm(50,i1);               // lo[50]
	r0 = r0 xor r1;
	r1 = dm(34,i1);               // lo[34]
	r0 = r0 xor r1;
	r1 = dm(15,i1);               // lo[15]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[74]
//lo[76] = a[11] ^ a[22] ^ a[44] ^ a[50] ^ lo[66] ^ lo[46] ^ lo[21] ^ lo[ 7] ;
    r0 = dm(11,i4);
	r1 = dm(22,i4);
    r0 = r0 xor r1;
	r1 = dm(44,i4);               
    r0 = r0 xor r1;
	r1 = dm(50,i4);               
    r0 = r0 xor r1;
	r1 = dm(66,i1);               // lo[66]               
    r0 = r0 xor r1;
	r1 = dm(46,i1);               // lo[46]
	r0 = r0 xor r1;
	r1 = dm(21,i1);               // lo[21]
	r0 = r0 xor r1;
	r1 = dm( 7,i1);               // lo[ 7]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[75]
//lo[77] = a[14] ^ a[59] ^ a[60] ^ lo[69] ^ lo[43] ^ lo[39] ^ lo[29] ;
    r0 = dm(14,i4);
	r1 = dm(59,i4);
    r0 = r0 xor r1;
	r1 = dm(60,i4);               
    r0 = r0 xor r1;
	r1 = dm(69,i1);               // lo[69]               
    r0 = r0 xor r1;
	r1 = dm(43,i1);               // lo[43]
	r0 = r0 xor r1;
	r1 = dm(39,i1);               // lo[39]
	r0 = r0 xor r1;
	r1 = dm(29,i1);               // lo[29]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[76]
//lo[78] = a[35] ^ a[55] ^ a[60] ^ lo[67] ^ lo[53] ^ lo[35] ;
    r0 = dm(35,i4);
	r1 = dm(55,i4);
    r0 = r0 xor r1;
	r1 = dm(60,i4);               
    r0 = r0 xor r1;
	r1 = dm(67,i1);               // lo[67]
	r0 = r0 xor r1;
	r1 = dm(53,i1);               // lo[53]
	r0 = r0 xor r1;
	r1 = dm(35,i1);               // lo[35]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[77]
//lo[79] = a[22] ^ a[61] ^ lo[68] ^ lo[50] ^ lo[37] ^ lo[10] ;
    r0 = dm(22,i4);
	r1 = dm(61,i4);
    r0 = r0 xor r1;
	r1 = dm(68,i1);               // lo[68]               
    r0 = r0 xor r1;
	r1 = dm(50,i1);               // lo[50]
	r0 = r0 xor r1;
	r1 = dm(37,i1);               // lo[37]
	r0 = r0 xor r1;
	r1 = dm(10,i1);               // lo[10]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[78]
//lo[80] = a[50] ^ lo[68] ^ lo[52] ^ lo[48] ^ lo[31] ;
    r0 = dm(50,i4);
	r1 = dm(68,i1);               // lo[68]               
    r0 = r0 xor r1;
	r1 = dm(52,i1);               // lo[52]
	r0 = r0 xor r1;
	r1 = dm(48,i1);               // lo[48]
	r0 = r0 xor r1;
	r1 = dm(31,i1);               // lo[31]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[79]
//lo[81] = a[10] ^ a[31] ^ a[35] ^ a[49] ^ a[53] ^ lo[70] ^ lo[49] ^ lo[14] ;
    r0 = dm(10,i4);
	r1 = dm(31,i4);
    r0 = r0 xor r1;
	r1 = dm(35,i4);               
    r0 = r0 xor r1;
	r1 = dm(49,i4);                              
    r0 = r0 xor r1;
	r1 = dm(53,i4);               
	r0 = r0 xor r1;
	r1 = dm(70,i1);               // lo[70]
	r0 = r0 xor r1;
	r1 = dm(49,i1);               // lo[49]
	r0 = r0 xor r1;
	r1 = dm(14,i1);               // lo[14]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[80]
//lo[82] = a[25] ^ lo[71] ^ lo[64] ^ lo[12] ^ lo[ 9] ^ lo[ 8] ;
    r0 = dm(25,i4);
	r1 = dm(71,i1);               // lo[71]
    r0 = r0 xor r1;
	r1 = dm(64,i1);               // lo[64]               
    r0 = r0 xor r1;
	r1 = dm(12,i1);               // lo[12]
	r0 = r0 xor r1;
	r1 = dm( 9,i1);               // lo[ 9]
	r0 = r0 xor r1;
	r1 = dm( 8,i1);               // lo[ 8]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[81]
    dm(i0,m6)=r2; // lo[82]
//lo[83] = lo[82] ^ lo[61] ;   ***
	r0 = dm(61,i1);               // lo[61]
	r1 = dm(82,i1);
    r2 = r0 xor r1;
//lo[84] = lo[82] ^ lo[65] ^ lo[63] ;
	r0 = dm(82,i1);               // lo[82]
	r1 = dm(65,i1);               // lo[65]
    r0 = r0 xor r1;
	r1 = dm(63,i1);               // lo[63]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[83]
//lo[85] = a[22] ^ a[48] ^ lo[31] ^ lo[59] ^ lo[60] ^ lo[75] ;
    r0 = dm(22,i4);
	r1 = dm(48,i4);               
    r0 = r0 xor r1;
	r1 = dm(31,i1);               // lo[31]               
    r0 = r0 xor r1;
	r1 = dm(59,i1);               // lo[59]
	r0 = r0 xor r1;
	r1 = dm(60,i1);               // lo[60]
	r0 = r0 xor r1;
	r1 = dm(75,i1);               // lo[75]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[84]
//lo[86] = a[ 7] ^ a[17] ^ a[33] ^ a[53] ^ lo[30] ^ lo[41] ^ lo[42] ^ lo[48] ^ lo[75] ;
    r0 = dm( 7,i4);
	r1 = dm(17,i4);
    r0 = r0 xor r1;
	r1 = dm(33,i4);               
    r0 = r0 xor r1;
	r1 = dm(53,i4);                              
    r0 = r0 xor r1;
	r1 = dm(30,i1);               // lo[30]               
	r0 = r0 xor r1;
	r1 = dm(41,i1);               // lo[41]               
	r0 = r0 xor r1;
	r1 = dm(42,i1);               // lo[42]
	r0 = r0 xor r1;
	r1 = dm(48,i1);               // lo[48]
	r0 = r0 xor r1;
	r1 = dm(75,i1);               // lo[75]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[85]
//lo[87] = a[12] ^ a[50] ^ lo[29] ^ lo[40] ^ lo[42] ^ lo[58] ^ lo[74] ;
    r0 = dm(12,i4);
	r1 = dm(50,i4);
    r0 = r0 xor r1;
	r1 = dm(29,i1);               // lo[29]               
	r0 = r0 xor r1;
	r1 = dm(40,i1);               // lo[40]               
	r0 = r0 xor r1;
	r1 = dm(42,i1);               // lo[42]
	r0 = r0 xor r1;
	r1 = dm(58,i1);               // lo[58]
	r0 = r0 xor r1;
	r1 = dm(74,i1);               // lo[74]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[86]
//lo[88] = a[13] ^ a[40] ^ lo[27] ^ lo[28] ^ lo[60] ^ lo[78] ;
    r0 = dm(13,i4);
	r1 = dm(40,i4);
    r0 = r0 xor r1;
	r1 = dm(27,i1);               // lo[27]               
	r0 = r0 xor r1;
	r1 = dm(28,i1);               // lo[28]               
	r0 = r0 xor r1;
	r1 = dm(60,i1);               // lo[60]
	r0 = r0 xor r1;
	r1 = dm(78,i1);               // lo[78]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[87]
//lo[89] = a[19] ^ a[38] ^ a[42] ^ a[58] ^ lo[26] ^ lo[57] ^ lo[77] ;
    r0 = dm(19,i4);
	r1 = dm(38,i4);
    r0 = r0 xor r1;
	r1 = dm(42,i4);               
    r0 = r0 xor r1;
	r1 = dm(58,i4);                              
    r0 = r0 xor r1;
	r1 = dm(26,i1);               // lo[26]               
	r0 = r0 xor r1;
	r1 = dm(57,i1);               // lo[57]
	r0 = r0 xor r1;
	r1 = dm(77,i1);               // lo[77]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[88]
//lo[90] = a[23] ^ a[43] ^ lo[24] ^ lo[25] ^ lo[56] ^ lo[76] ;
    r0 = dm(23,i4);
	r1 = dm(43,i4);
    r0 = r0 xor r1;
	r1 = dm(24,i1);               // lo[24]                            
    r0 = r0 xor r1;
	r1 = dm(25,i1);               // lo[25]               
	r0 = r0 xor r1;
	r1 = dm(56,i1);               // lo[56]
	r0 = r0 xor r1;
	r1 = dm(76,i1);               // lo[76]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[89]
//lo[91] = a[ 2] ^ a[21] ^ a[31] ^ lo[28] ^ lo[32] ^ lo[39] ^ lo[47] ^ lo[80] ;
    r0 = dm( 2,i4);
	r1 = dm(21,i4);
    r0 = r0 xor r1;
	r1 = dm(31,i4);               
    r0 = r0 xor r1;
	r1 = dm(28,i1);               // lo[28]                              
    r0 = r0 xor r1;
	r1 = dm(32,i1);               // lo[32]               
	r0 = r0 xor r1;
	r1 = dm(39,i1);               // lo[39]               
	r0 = r0 xor r1;
	r1 = dm(47,i1);               // lo[47]
	r0 = r0 xor r1;
	r1 = dm(80,i1);               // lo[80]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[90]
//lo[92] = a[59] ^ lo[23] ^ lo[38] ^ lo[55] ^ lo[81] ;
    r0 = dm(59,i4);
	r1 = dm(23,i1);               // lo[23]               
	r0 = r0 xor r1;
	r1 = dm(38,i1);               // lo[38]               
	r0 = r0 xor r1;
	r1 = dm(55,i1);               // lo[55]
	r0 = r0 xor r1;
	r1 = dm(81,i1);               // lo[81]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[91]
//lo[93] = lo[21] ^ lo[22] ^ lo[25] ^ lo[55] ^ lo[70] ;
    r0 = dm(21,i1);               // lo[21]
	r1 = dm(22,i1);               // lo[22]               
	r0 = r0 xor r1;
	r1 = dm(25,i1);               // lo[25]               
	r0 = r0 xor r1;
	r1 = dm(55,i1);               // lo[55]
	r0 = r0 xor r1;
	r1 = dm(70,i1);               // lo[70]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[92]
//lo[94] = a[ 1] ^ a[30] ^ lo[20] ^ lo[27] ^ lo[67] ^ lo[79] ;
    r0 = dm( 1,i4);
	r1 = dm(30,i4);
    r0 = r0 xor r1;
	r1 = dm(20,i1);               // lo[20]               
	r0 = r0 xor r1;
	r1 = dm(27,i1);               // lo[27]               
	r0 = r0 xor r1;
	r1 = dm(67,i1);               // lo[67]
	r0 = r0 xor r1;
	r1 = dm(79,i1);               // lo[79]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[93]
//lo[95] = a[29] ^ a[50] ^ lo[18] ^ lo[19] ^ lo[36] ^ lo[37] ^ lo[81] ;
    r0 = dm(29,i4);
	r1 = dm(50,i4);
    r0 = r0 xor r1;
	r1 = dm(18,i1);               // lo[18]               
    r0 = r0 xor r1;
	r1 = dm(19,i1);               // lo[19]                              
    r0 = r0 xor r1;
	r1 = dm(36,i1);               // lo[36]               
	r0 = r0 xor r1;
	r1 = dm(37,i1);               // lo[37]               
	r0 = r0 xor r1;
	r1 = dm(81,i1);               // lo[81]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[94]
//lo[96] = lo[17] ^ lo[30] ^ lo[46] ^ lo[74] ;
	r0 = dm(17,i1);               // lo[17]               
	r1 = dm(30,i1);               // lo[30]
	r0 = r0 xor r1;
	r1 = dm(46,i1);               // lo[46]
	r0 = r0 xor r1;
	r1 = dm(74,i1);               // lo[74]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[95]
//lo[97] = lo[16] ^ lo[29] ^ lo[54] ^ lo[80] ;
	r0 = dm(16,i1);               // lo[16]               
	r1 = dm(29,i1);               // lo[29]
	r0 = r0 xor r1;
	r1 = dm(54,i1);               // lo[54]
	r0 = r0 xor r1;
	r1 = dm(80,i1);               // lo[80]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[96]
//lo[98] = a[33] ^ a[39] ^ lo[15] ^ lo[53] ^ lo[79] ;
    r0 = dm(33,i4);
	r1 = dm(39,i4);                              
	r0 = r0 xor r1;
	r1 = dm(15,i1);               // lo[15]               
	r0 = r0 xor r1;
	r1 = dm(53,i1);               // lo[53]
	r0 = r0 xor r1;
	r1 = dm(79,i1);               // lo[79]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[97]
//lo[99] = a[34] ^ a[62] ^ lo[13] ^ lo[14] ^ lo[52] ^ lo[77] ;
    r0 = dm(34,i4);
	r1 = dm(62,i4);                              
	r0 = r0 xor r1;
	r1 = dm(13,i1);               // lo[13]               
	r0 = r0 xor r1;
	r1 = dm(14,i1);               // lo[14]               
	r0 = r0 xor r1;
	r1 = dm(52,i1);               // lo[52]
	r0 = r0 xor r1;
	r1 = dm(77,i1);               // lo[77]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[98]
//lo[100] = a[ 4] ^ a[46] ^ lo[12] ^ lo[15] ^ lo[59] ^ lo[78] ;
    r0 = dm( 4,i4);
	r1 = dm(46,i4);                              
	r0 = r0 xor r1;
	r1 = dm(12,i1);               // lo[12]               
	r0 = r0 xor r1;
	r1 = dm(15,i1);               // lo[15]               
	r0 = r0 xor r1;
	r1 = dm(59,i1);               // lo[59]
	r0 = r0 xor r1;
	r1 = dm(78,i1);               // lo[78]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[99]
//lo[101] = a[ 6] ^ a[42] ^ lo[11] ^ lo[40] ^ lo[69] ^ lo[73] ;
    r0 = dm( 6,i4);
	r1 = dm(42,i4);                              
	r0 = r0 xor r1;
	r1 = dm(11,i1);               // lo[11]               
	r0 = r0 xor r1;
	r1 = dm(40,i1);               // lo[40]               
	r0 = r0 xor r1;
	r1 = dm(69,i1);               // lo[69]
	r0 = r0 xor r1;
	r1 = dm(73,i1);               // lo[73]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[100]
//lo[102] = a[ 1] ^ a[ 2] ^ a[ 8] ^ a[32] ^ lo[22] ^ lo[51] ^ lo[76] ;
    r0 = dm( 1,i4);
	r1 = dm( 2,i4);
    r0 = r0 xor r1;
	r1 = dm( 8,i4);               
    r0 = r0 xor r1;
	r1 = dm(32,i4);                              
    r0 = r0 xor r1;
	r1 = dm(22,i1);               // lo[22]               
	r0 = r0 xor r1;
	r1 = dm(51,i1);               // lo[51]
	r0 = r0 xor r1;
	r1 = dm(76,i1);               // lo[76]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[101]
//lo[103] = a[ 0] ^ a[ 3] ^ a[32] ^ a[53] ^ lo[10] ^ lo[58] ^ lo[73] ;
    r0 = dm( 0,i4);
	r1 = dm( 3,i4);
    r0 = r0 xor r1;
	r1 = dm(32,i4);               
    r0 = r0 xor r1;
	r1 = dm(53,i4);                              
    r0 = r0 xor r1;
	r1 = dm(10,i1);               // lo[10]               
	r0 = r0 xor r1;
	r1 = dm(58,i1);               // lo[58]
	r0 = r0 xor r1;
	r1 = dm(73,i1);               // lo[73]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[102]
//lo[104] = lo[64] ^ lo[65] ^ lo[72] ;
	r0 = dm(64,i1);               // lo[64]               
	r1 = dm(65,i1);               // lo[65]
	r0 = r0 xor r1;
	r1 = dm(72,i1);               // lo[72]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[103]
//lo[105] = lo[63] ^ lo[83] ;
	r0 = dm(63,i1);               // lo[63]               
	r1 = dm(83,i1);               // lo[83]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[104]
//lo[106] = lo[62] ^ lo[83] ;
	r0 = dm(62,i1);               // lo[62]               
	r1 = dm(83,i1);               // lo[83]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[105]
//lo[107] = lo[88] ^ lo[90] ;
	r0 = dm(88,i1);               // lo[88]               
	r1 = dm(90,i1);               // lo[90]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[106]
//lo[108] = lo[87] ^ lo[89] ;
	r0 = dm(87,i1);               // lo[87]               
	r1 = dm(89,i1);               // lo[89]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[107]
//lo[109] = lo[85] ^ lo[88] ;
	r0 = dm(85,i1);               // lo[85]               
	r1 = dm(88,i1);               // lo[88]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[108]
//lo[110] = lo[86] ^ lo[88] ;
	r0 = dm(86,i1);               // lo[86]               
	r1 = dm(88,i1);               // lo[88]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[109]
//lo[111] = lo[87] ^ lo[107] ;
	r0 = dm(87,i1);               // lo[87]               
	r1 = dm(107,i1);              // lo[107]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[110]
    dm(i0,m6)=r2; // lo[111]
//lo[112] = lo[86] ^ lo[89] ^ lo[111] ;
	r0 = dm(86,i1);               // lo[86]               
	r1 = dm(89,i1);               // lo[89]
	r0 = r0 xor r1;
	r1 = dm(111,i1);              // lo[111]
	r2 = r0 xor r1;
//lo[113] = lo[86] ^ lo[108] ;
	r0 = dm(86,i1);               // lo[86]               
	r1 = dm(108,i1);              // lo[108]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[112]
//lo[114] = lo[87] ^ lo[110] ;
	r0 = dm(87,i1);               // lo[87]               
	r1 = dm(110,i1);              // lo[110]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[113]
//lo[115] = lo[89] ^ lo[109] ;
	r0 = dm(89,i1);               // lo[89]               
	r1 = dm(109,i1);              // lo[109]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[114]
//lo[116] = lo[90] ^ lo[108] ;
	r0 = dm(90,i1);               // lo[90]               
	r1 = dm(108,i1);              // lo[108]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[115]
//lo[117] = lo[92] ^ lo[95] ;
	r0 = dm(92,i1);               // lo[92]               
	r1 = dm(95,i1);               // lo[95]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[116]
//lo[118] = lo[92] ^ lo[94] ;
	r0 = dm(92,i1);               // lo[92]               
	r1 = dm(94,i1);               // lo[94]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[117]
//lo[119] = lo[93] ^ lo[94] ^ lo[96] ;
	r0 = dm(93,i1);               // lo[93]               
	r1 = dm(94,i1);               // lo[94]
	r0 = r0 xor r1;
	r1 = dm(96,i1);               // lo[96]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[118]
	dm(i0,m6)=r2;                 // lo[119]
//lo[120] = lo[117] ^ lo[119] ; ***
	r0 = dm(117,i1);              // lo[117]
    r1 = dm(119,i1);              // lo[119]           
    r2 = r0 xor r1;
//lo[121] = lo[91] ^ lo[94] ;
	r0 = dm(91,i1);               // lo[91]               
	r1 = dm(94,i1);               // lo[94]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[120]
//lo[122] = lo[93] ^ lo[117] ;
	r0 = dm(93,i1);               // lo[93]               
	r1 = dm(117,i1);              // lo[117]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[121]
//lo[123] = lo[93] ^ lo[118] ;
	r0 = dm(93,i1);               // lo[93]               
	r1 = dm(118,i1);              // lo[118]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[122]
//lo[124] = lo[99] ^ lo[100] ^ lo[102] ;
	r0 = dm(99,i1);               // lo[99]               
	r1 = dm(100,i1);              // lo[100]
	r0 = r0 xor r1;
	r1 = dm(102,i1);              // lo[102]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[123]
//lo[125] = lo[98] ^ lo[100] ;
	r0 = dm(98,i1);               // lo[98]               
	r1 = dm(100,i1);              // lo[100]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[124]
//lo[126] = lo[98] ^ lo[101] ^ lo[124] ;
	r0 = dm(98,i1);               // lo[98]               
	r1 = dm(101,i1);              // lo[101]
	r0 = r0 xor r1;
	r1 = dm(124,i1);              // lo[124]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[125]
//lo[127] = lo[104] ^ lo[105] ^ lo[71] ;
	r0 = dm(104,i1);              // lo[104]               
	r1 = dm(105,i1);              // lo[105]
	r0 = r0 xor r1;
	r1 = dm(71,i1);               // lo[71]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[126]
//lo[128] = lo[84] ^ lo[105] ;
	r0 = dm(84,i1);               // lo[84]               
	r1 = dm(105,i1);              // lo[105]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[127]
//lo[129] = lo[84] ^ lo[106] ^ lo[127] ;
	r0 = dm(84,i1);               // lo[84]               
	r1 = dm(106,i1);              // lo[106]
	r0 = r0 xor r1;
	r1 = dm(127,i1);              // lo[127]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[128]
    dm(i0,m6)=r2;                 // lo[129]

	i5 = 2*logGF64;                // address of logarithm table in dm in 16 bit addressing mode
    i3 = dm(Mo_ptr);
//m1[ 0] = pwr[ 2*GF + 19 + lgr[l[87]]  ];
    m4 = dm(87,i1);              // m4 = lo[87]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[87]]
    r1  = 2*pwrGF64+GF64_2+19;
	r0  = r0+r1;
	i2  = r0;
	r2  = dm(i2,m4);
//m1[ 1] = pwr[ 2*GF + 42 + lgr[l[90]]  ];
    m4 = dm(90,i1);              // m4 = lo[90]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[90]]
    r1  = 2*pwrGF64+GF64_2+42;
	r0  = r0+r1, dm(i3,m6) = r2;  // store mo[0]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[ 2] = pwr[ 2*GF + 13 + lgr[l[111]] ];
    m4 = dm(111,i1);             // m4 = lo[111]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[111]]
    r1  = 2*pwrGF64+GF64_2+13;
	r0  = r0+r1, dm(i3,m6) = r2;  // store mo[1]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[ 3] = pwr[ 2*GF + 33 + lgr[l[110]] ];
    m4 = dm(110,i1);             // m4 = lo[110]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[110]]
    r1  = 2*pwrGF64+GF64_2+33;
	r0  = r0+r1, dm(i3,m6) = r2;  // store mo[2]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[ 4] = pwr[ 2*GF +  4 + lgr[l[112]] ];
    m4 = dm(112,i1);             // m4 = lo[112]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[112]]
    r1  = 2*pwrGF64+GF64_2+4;
	r0  = r0+r1, dm(i3,m6) = r2;  // store mo[3]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[ 5] = pwr[ 2*GF +  8 + lgr[l[113]] ];
    m4 = dm(113,i1);             // m4 = lo[113]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[113]]
    r1  = 2*pwrGF64+GF64_2+8;
	r0  = r0+r1, dm(i3,m6) = r2;  // store mo[4]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[ 6] = pwr[ 2*GF +  3 + lgr[l[114]] ];
    m4 = dm(114,i1);             // m4 = lo[114]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[114]]
    r1  = 2*pwrGF64+GF64_2+3;
	r0  = r0+r1, dm(i3,m6) = r2;  // store mo[5]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[ 7] = pwr[ 2*GF + 14 + lgr[l[107]] ];
    m4 = dm(107,i1);             // m4 = lo[107]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[107]]
    r1  = 2*pwrGF64+GF64_2+14;
	r0  = r0+r1, dm(i3,m6) = r2;  // store mo[6]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[ 8] = pwr[ 2*GF +  1 + lgr[l[116]] ];
    m4 = dm(116,i1);             // m4 = lo[116]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[116]]
    r1  = 2*pwrGF64+GF64_2+1;
	r0  = r0+r1, dm(i3,m6) = r2;  // store mo[7]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[ 9] = pwr[ 2*GF + 19 + lgr[l[93]]  ];
    m4 = dm(93,i1);              // m4 = lo[93]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[93]]
    r1  = 2*pwrGF64+GF64_2+19;
	r0  = r0+r1, dm(i3,m6) = r2;  // store mo[8]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[10] = pwr[ 2*GF + 42 + lgr[l[96]]  ];
    m4 = dm(96,i1);              // m4 = lo[96]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[96]]
    r1  = 2*pwrGF64+GF64_2+42;
	r0  = r0+r1, dm(i3,m6) = r2;  // store mo[9]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[11] = pwr[ 2*GF + 13 + lgr[l[119]] ];
    m4 = dm(119,i1);             // m4 = lo[119]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[119]]
    r1  = 2*pwrGF64+GF64_2+13;
	r0  = r0+r1, dm(i3,m6) = r2;  // store mo[10]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[12] = pwr[ 2*GF + 33 + lgr[l[118]] ];
    m4 = dm(118,i1);             // m4 = lo[118]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[118]]
    r1  = 2*pwrGF64+GF64_2+33;
	r0  = r0+r1, dm(i3,m6) = r2;  // store mo[11]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[13] = pwr[ 2*GF +  4 + lgr[l[120]] ];
    m4 = dm(120,i1);             // m4 = lo[120]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[120]]
    r1  = 2*pwrGF64+GF64_2+4;
	r0  = r0+r1, dm(i3,m6) = r2;  // store mo[12]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[14] = pwr[ 2*GF +  8 + lgr[l[122]] ];
    m4 = dm(122,i1);             // m4 = lo[122]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[122]]
    r1  = 2*pwrGF64+GF64_2+8;
	r0  = r0+r1, dm(i3,m6) = r2;  // store mo[13]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[15] = pwr[ 2*GF +  3 + lgr[l[123]] ];
    m4 = dm(123,i1);             // m4 = lo[123]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[123]]
    r1  = 2*pwrGF64+GF64_2+3;
	r0  = r0+r1, dm(i3,m6) = r2;  // store mo[14]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[16] = pwr[ 2*GF + 19 + lgr[l[99]]  ];
    m4 = dm(99,i1);              // m4 = lo[99]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[99]]
    r1  = 2*pwrGF64+GF64_2+19;
	r0  = r0+r1, dm(i3,m6) = r2;  // store mo[15]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[17] = pwr[ 2*GF + 42 + lgr[l[102]] ];
    m4 = dm(102,i1);             // m4 = lo[102]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[102]]
    r1  = 2*pwrGF64+GF64_2+42;
	r0  = r0+r1, dm(i3,m6) = r2;  // store mo[16]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[18] = pwr[ 2*GF + 13 + lgr[l[124]] ];
    m4 = dm(124,i1);             // m4 = lo[124]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[124]]
    r1  = 2*pwrGF64+GF64_2+13;
	r0  = r0+r1, dm(i3,m6) = r2;  // store mo[17]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[19] = pwr[ 2*GF + 33 + lgr[l[125]] ];
    m4 = dm(125,i1);             // m4 = lo[125]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[125]]
    r1  = 2*pwrGF64+GF64_2+33;
	r0  = r0+r1, dm(i3,m6) = r2;  // store mo[18]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[20] = pwr[ 2*GF +  4 + lgr[l[126]] ];
    m4 = dm(126,i1);             // m4 = lo[126]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[126]]
    r1  = 2*pwrGF64+GF64_2+4;
	r0  = r0+r1, dm(i3,m6) = r2;  // store mo[19]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[21] = pwr[ 2*GF + 19 + lgr[l[104]] ];
    m4 = dm(104,i1);             // m4 = lo[104]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[104]]
    r1  = 2*pwrGF64+GF64_2+19;
	r0  = r0+r1, dm(i3,m6) = r2;  // store mo[20]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[22] = pwr[ 2*GF + 42 + lgr[l[71]]  ];
    m4 = dm(71,i1);              // m4 = lo[71]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[71]]
    r1  = 2*pwrGF64+GF64_2+42;
	r0  = r0+r1, dm(i3,m6) = r2;  // store mo[21]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[23] = pwr[ 2*GF + 13 + lgr[l[127]] ];
    m4 = dm(127,i1);             // m4 = lo[127]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[127]]
    r1  = 2*pwrGF64+GF64_2+13;
	r0  = r0+r1, dm(i3,m6) = r2;  // store mo[22]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[24] = pwr[ 2*GF + 33 + lgr[l[128]] ];
    m4 = dm(128,i1);             // m4 = lo[128]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[128]]
    r1  = 2*pwrGF64+GF64_2+33;
	r0  = r0+r1, dm(i3,m6) = r2;  // store mo[23]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[25] = pwr[ 2*GF +  4 + lgr[l[129]] ];
    m4 = dm(129,i1);             // m4 = lo[129]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[129]]
    r1  = 2*pwrGF64+GF64_2+4;
	r0  = r0+r1, dm(i3,m6) = r2;  // store mo[24]
	i2  = r0;
	r2  = dm(i2,m4);
	dm(i3,m6) = r2;               // store mo[25]

    i3  = dm(Mo_ptr); 
//l[130] = m1[ 0] ^ m1[ 1] ;
	r0 = dm( 0,i3);              // ma[0]               
	r1 = dm( 1,i3);              // ma[1]
	r2 = r0 xor r1;
//l[131] = m1[ 9] ^ m1[10] ^ m1[11] ;
	r0 = dm( 9,i3);              // ma[9]               
	r1 = dm(10,i3);              // ma[10]
	r0 = r0 xor r1;
	r1 = dm(11,i3);              // ma[11]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[130]
//l[132] = m1[ 2] ^ l[130] ;
	r0 = dm( 2,i3);              // ma[2]               
	r1 = dm(130,i1);             // lo[130]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[131]
	dm(i0,m6)=r2;                // lo[132]

	i0 = r8;
//A1[ 0] = a[ 0] ^ a[ 9] ^ lo[32] ^ lo[43] ^ lo[72] ^ lo[84] ;
    r0 = dm( 0,i4);
	r1 = dm( 9,i4);                              
	r0 = r0 xor r1;
	r1 = dm(32,i1);               // lo[32]               
	r0 = r0 xor r1;
	r1 = dm(43,i1);               // lo[43]               
	r0 = r0 xor r1;
	r1 = dm(72,i1);               // lo[72]
	r0 = r0 xor r1;
	r1 = dm(84,i1);               // lo[84]
	r2 = r0 xor r1;
//A1[ 1] = m1[ 3] ^ m1[ 4] ^ l[85] ^ l[132] ;
    r0 = dm( 3,i3);               // ma[3]
	r1 = dm( 4,i3);               // ma[4]               
	r0 = r0 xor r1;
	r1 = dm(85,i1);               // lo[85]               
	r0 = r0 xor r1;
	r1 = dm(132,i1);              // lo[132]
	r2 = r0 xor r1, dm(i0,m6)=r2; // A[0]
//A1[ 2] = m1[ 5] ^ m1[ 6] ^ l[109] ^ l[132] ;
    r0 = dm( 5,i3);               // ma[5]
	r1 = dm( 6,i3);               // ma[6]               
	r0 = r0 xor r1;
	r1 = dm(109,i1);              // lo[109]               
	r0 = r0 xor r1;
	r1 = dm(132,i1);              // lo[132]
	r2 = r0 xor r1, dm(i0,m6)=r2; // A[1]
//A1[ 3] = m1[12] ^ m1[13] ^ l[91] ^ l[131] ;
    r0 = dm(12,i3);               // ma[12]
	r1 = dm(13,i3);               // ma[13]               
	r0 = r0 xor r1;
	r1 = dm(91,i1);               // lo[91]               
	r0 = r0 xor r1;
	r1 = dm(131,i1);              // lo[131]
	r2 = r0 xor r1, dm(i0,m6)=r2; // A[2]
//A1[ 4] = m1[ 4] ^ m1[ 7] ^ m1[ 8] ^ l[115] ^ l[130] ;
    r0 = dm( 4,i3);               // ma[4]
	r1 = dm( 7,i3);               // ma[7]               
	r0 = r0 xor r1;
	r1 = dm( 8,i3);               // ma[8]               
	r0 = r0 xor r1;
	r1 = dm(115,i1);              // lo[115]               
	r0 = r0 xor r1;
	r1 = dm(130,i1);              // lo[130]
	r2 = r0 xor r1, dm(i0,m6)=r2; // A[3]
//A1[ 5] = m1[16] ^ m1[17] ^ m1[18] ^ m1[19] ^ m1[20] ^ l[97] ;
    r0 = dm(16,i3);               // ma[16]
	r1 = dm(17,i3);               // ma[17]               
	r0 = r0 xor r1;
	r1 = dm(18,i3);               // ma[18]               
	r0 = r0 xor r1;
	r1 = dm(19,i3);               // ma[19]               
	r0 = r0 xor r1;
	r1 = dm(20,i3);               // ma[20]               
	r0 = r0 xor r1;
	r1 = dm(97,i1);               // lo[97]
	r2 = r0 xor r1, dm(i0,m6)=r2; // A[4]
//A1[ 6] = m1[14] ^ m1[15] ^ l[121] ^ l[131] ;
    r0 = dm(14,i3);               // ma[14]
	r1 = dm(15,i3);               // ma[15]               
	r0 = r0 xor r1;
	r1 = dm(121,i1);              // lo[121]               
	r0 = r0 xor r1;
	r1 = dm(131,i1);              // lo[131]
	r2 = r0 xor r1, dm(i0,m6)=r2; // A[5]
//A1[ 7] = m1[21] ^ m1[22] ^ m1[23] ^ m1[24] ^ m1[25] ^ l[103] ;
    r0 = dm(21,i3);               // ma[21]
	r1 = dm(22,i3);               // ma[22]               
	r0 = r0 xor r1;
	r1 = dm(23,i3);               // ma[23]               
	r0 = r0 xor r1;
	r1 = dm(24,i3);               // ma[24]               
	r0 = r0 xor r1;
	r1 = dm(25,i3);               // ma[25]               
	r0 = r0 xor r1;
	r1 = dm(103,i1);              // lo[103]
	r2 = r0 xor r1, dm(i0,m6)=r2; // A[6]
	dm(i0,m6)=r2;                 // A[7]

epil:
    i0  = gets( 5 );
    i1  = gets( 4 );
    i2  = gets( 3 );
    i3  = gets( 2 );
    i5  = gets( 1 );

    leaf_exit; 
_FFT63_8.end:

.endseg;	


