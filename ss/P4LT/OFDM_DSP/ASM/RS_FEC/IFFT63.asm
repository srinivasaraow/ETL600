/******************************************************************************
 *
 * PROJECT     : ETL600
 * FILE        : $Workfile: IFFT63.asm $
 * COPYRIGHT   : 2004 by TL Consulting GmbH
 * DESCRIPTION : Calculates inverse Fast Fourier Transform over GF(64)
 *               for 63 element input vector in frequency domain
 * OPERATION   : 
 * PROTOTYPE   : IFFT63(int   S[], int Y[]) ;    
 *                                                          
 * ASSUMPTIONS :
 *               Number of elements in input S[] and  vector is equal to 63 
 *               Number of elements in output vector Y[] is equal to 63 
 *
 *
 * Used  registers:     m6=1
 * Saved registers:     i0, i1, i2, i3, i5
 * Destroyed registers: r0, r1, r2, r4, i4, m4
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/RS_FEC/IFFT63.asm $
 * 
 * 1     19.01.05 15:37 Chchlee
 * 
 * 4     30.09.04 8:27 Leeb02
 * 
 * 
 * 1     10.01.04 vgp
 * Initial version Complexity  3552 cycles
 * 2     17.01.04  vgp
 * additional _logGF64 table in dm memory is inserted to improve execution time
 * 3     09.09.04  vgp
 * avoid usage of m5 register
 * 4     12.09.04  vgp
 * 16-bit addressing mode for logarithmic and power tables
 * 
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
//.section/dm	sdram_data;
.section/dm	seg_dmda;

.extern pwrGF64;
.extern  logGF64;
.extern Lo_ptr;
.extern Mo_ptr;

.endseg;

//==============================================

//		.segment/pm   seg_pmco;
		.segment/pm   seg_ext_pmco;

		.global _IFFT63;

// m5, m13 must be 0
// m6, m14 must be 1
// m7, m15 must be -1

// r4 - address of X[]
// r8 - address of S[]

_IFFT63:

    leaf_entry;

    r0 = i0; puts = r0;
    r0 = i1; puts = r0;
    r0 = i2; puts = r0;
    r0 = i3; puts = r0;
    r0 = i5; puts = r0;


    i4 = r4;              // i4 address of X[]

    i0 = dm(Lo_ptr);
//lo[ 0] = a[7] ^ a[43] ;
    r0 = dm( 7,i4);
	r1 = dm(43,i4);
	r2 = r0 xor r1, i1 = i0; // i0 and i1 point lo[] 
//lo[ 1] = a[15] ^ a[51] ;
    r0 = dm(15,i4);
	r1 = dm(51,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[0]
//lo[ 2] = a[33] ^ a[54] ;
    r0 = dm(33,i4);
	r1 = dm(54,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[1]
//lo[ 3] = a[12] ^ a[48] ;
    r0 = dm(12,i4);
	r1 = dm(48,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[2]
//lo[ 4] = a[23] ^ a[32] ;
    r0 = dm(23,i4);
	r1 = dm(32,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[3]
//lo[ 5] = a[25] ^ a[52] ;
    r0 = dm(25,i4);
	r1 = dm(52,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[4]
//lo[ 6] = a[14] ^ a[34] ;
    r0 = dm(14,i4);
	r1 = dm(34,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[5]
//lo[ 7] = a[ 4] ^ a[49] ;
    r0 = dm( 4,i4);
	r1 = dm(49,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[6]
//lo[ 8] = a[ 7] ^ a[35] ;
    r0 = dm( 7,i4);
	r1 = dm(35,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[7]
//lo[ 9] = a[ 2] ^ a[44] ;
    r0 = dm( 2,i4);
	r1 = dm(44,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[8]
//lo[10] = a[17] ^ a[38] ;
    r0 = dm(17,i4);
	r1 = dm(38,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[9]
//lo[11] = a[46] ^ a[56] ;
    r0 = dm(46,i4);
	r1 = dm(56,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[10]
//lo[12] = a[ 3] ^ a[21] ;
    r0 = dm( 3,i4);
	r1 = dm(21,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[11]
//lo[13] = a[31] ^ a[40] ;
    r0 = dm(31,i4);
	r1 = dm(40,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[12]
//lo[14] = a[17] ^ a[62] ;
    r0 = dm(17,i4);
	r1 = dm(62,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[13]
//lo[15] = a[34] ^ a[52] ;
    r0 = dm(34,i4);
	r1 = dm(52,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[14]
//lo[16] = a[39] ^ a[57] ;
    r0 = dm(39,i4);
	r1 = dm(57,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[15]
//lo[17] = a[29] ^ a[50] ;
    r0 = dm(29,i4);
	r1 = dm(50,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[16]
//lo[18] = a[37] ^ a[58] ;
    r0 = dm(37,i4);
	r1 = dm(58,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[17]
//lo[19] = a[12] ^ a[54] ;
    r0 = dm(12,i4);
	r1 = dm(54,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[18]
//lo[20] = a[33] ^ a[44] ;
    r0 = dm(33,i4);
	r1 = dm(44,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[19]
//lo[21] = a[42] ^ a[60] ;
    r0 = dm(42,i4);
	r1 = dm(60,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[20]
//lo[22] = a[ 7] ^ a[28] ;
    r0 = dm( 7,i4);
	r1 = dm(28,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[21]
//lo[23] = a[59] ^ a[61] ;
    r0 = dm(59,i4);
	r1 = dm(61,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[22]
//lo[24] = a[ 6] ^ a[33] ;
    r0 = dm( 6,i4);
	r1 = dm(33,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[23]
//lo[25] = a[10] ^ a[23] ;
    r0 = dm(10,i4);
	r1 = dm(23,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[24]
//lo[26] = a[29] ^ a[56] ;
    r0 = dm(29,i4);
	r1 = dm(56,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[25]
//lo[27] = a[ 8] ^ a[46] ;
    r0 = dm( 8,i4);
	r1 = dm(46,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[26]
//lo[28] = a[ 9] ^ a[51] ;
    r0 = dm( 9,i4);
	r1 = dm(51,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[27]
//lo[29] = a[49] ^ a[55] ;
    r0 = dm(49,i4);
	r1 = dm(55,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[28]
//lo[30] = a[10] ^ a[22] ;
    r0 = dm(10,i4);
	r1 = dm(22,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[29]
//lo[31] = a[18] ^ a[54] ;
    r0 = dm(18,i4);
	r1 = dm(54,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[30]
//lo[32] = a[ 3] ^ a[53] ;
    r0 = dm( 3,i4);
	r1 = dm(53,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[31]
//lo[33] = a[39] ^ a[59] ;
    r0 = dm(39,i4);
	r1 = dm(59,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[32]
//lo[34] = a[ 6] ^ a[34] ;
    r0 = dm( 6,i4);
	r1 = dm(34,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[33]
//lo[35] = a[36] ^ a[38] ;
    r0 = dm(36,i4);
	r1 = dm(38,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[34]
//lo[36] = a[10] ^ a[19] ;
    r0 = dm(10,i4);
	r1 = dm(19,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[35]
//lo[37] = a[41] ^ a[59] ;
    r0 = dm(41,i4);
	r1 = dm(59,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[36]
//lo[38] = a[ 1] ^ a[ 2] ;
    r0 = dm( 1,i4);
	r1 = dm( 2,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[37]
//lo[39] = a[20] ^ a[29] ;
    r0 = dm(20,i4);
	r1 = dm(29,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[38]
//lo[40] = a[ 4] ^ a[46] ;
    r0 = dm( 4,i4);
	r1 = dm(46,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[39]
//lo[41] = a[14] ^ a[50] ;
    r0 = dm(14,i4);
	r1 = dm(50,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[40]
//lo[42] = a[35] ^ a[53] ;
    r0 = dm(35,i4);
	r1 = dm(53,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[41]
//lo[43] = a[17] ^ a[43] ;
    r0 = dm(17,i4);
	r1 = dm(43,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[42]
//lo[44] = a[44] ^ a[53] ;
    r0 = dm(44,i4);
	r1 = dm(53,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[43]
//lo[45] = a[49] ^ a[56] ;
    r0 = dm(49,i4);
	r1 = dm(56,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[44]
//lo[46] = a[43] ^ a[47] ;
    r0 = dm(43,i4);
	r1 = dm(47,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[45]
//lo[47] = a[13] ^ a[55] ;
    r0 = dm(13,i4);
	r1 = dm(55,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[46]
//lo[48] = a[41] ^ a[57] ;
    r0 = dm(41,i4);
	r1 = dm(57,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[47]
//lo[49] = a[21] ^ a[42] ;
    r0 = dm(21,i4);
	r1 = dm(42,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[48]
//lo[50] = a[15] ^ a[43] ;
    r0 = dm(15,i4);
	r1 = dm(43,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[49]
//lo[51] = a[38] ^ a[55] ;
    r0 = dm(38,i4);
	r1 = dm(55,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[50]
//lo[52] = a[25] ^ a[58] ;
    r0 = dm(25,i4);
	r1 = dm(58,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[51]
//lo[53] = a[ 5] ^ a[28] ;
    r0 = dm( 5,i4);
	r1 = dm(28,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[52]
//lo[54] = a[ 9] ^ a[13] ;
    r0 = dm( 9,i4);
	r1 = dm(13,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[53]
//lo[55] = a[26] ^ a[56] ;
    r0 = dm(26,i4);
	r1 = dm(56,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[54]
//lo[56] = a[30] ^ a[37] ;
    r0 = dm(30,i4);
	r1 = dm(37,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[55]
//lo[57] = a[21] ^ a[30] ;
    r0 = dm(21,i4);
	r1 = dm(30,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[56]
//lo[58] = a[13] ^ a[34] ;
    r0 = dm(13,i4);
	r1 = dm(34,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[57]
//lo[59] = a[11] ^ a[32] ;
    r0 = dm(11,i4);
	r1 = dm(32,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[58]
//lo[60] = a[47] ^ a[52] ;
    r0 = dm(47,i4);
	r1 = dm(52,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[59]
//lo[61] = a[25] ^ a[61] ;
    r0 = dm(25,i4);
	r1 = dm(61,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[60]
//lo[62] = a[ 2] ^ a[14] ;
    r0 = dm( 2,i4);
	r1 = dm(14,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[61]
//lo[63] = a[10] ^ a[18] ;
    r0 = dm(10,i4);
	r1 = dm(18,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[62]
//lo[64] = a[32] ^ a[37] ;
    r0 = dm(32,i4);
	r1 = dm(37,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[63]
//lo[65] = a[22] ^ a[61] ;
    r0 = dm(22,i4);
	r1 = dm(61,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[64]
//lo[66] = a[26] ^ a[29] ;
    r0 = dm(26,i4);
	r1 = dm(29,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[65]
//lo[67] = a[48] ^ a[57] ;
    r0 = dm(48,i4);
	r1 = dm(57,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[66]
//lo[68] = a[ 3] ^ a[45] ;
    r0 = dm( 3,i4);
	r1 = dm(45,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[67]
//lo[69] = a[ 6] ^ a[27] ;
    r0 = dm( 6,i4);
	r1 = dm(27,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[68]
//lo[70] = a[16] ^ a[23] ;
    r0 = dm(16,i4);
	r1 = dm(23,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[69]
//lo[71] = a[35] ^ a[56] ;
    r0 = dm(35,i4);
	r1 = dm(56,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[70]
//lo[72] = a[11] ^ a[22] ;
    r0 = dm(11,i4);
	r1 = dm(22,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[71]
//lo[73] = a[ 5] ^ a[55] ;
    r0 = dm( 5,i4);
	r1 = dm(55,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[72]
//lo[74] = a[ 6] ^ a[36] ;
    r0 = dm( 6,i4);
	r1 = dm(36,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[73]
//lo[75] = a[49] ^ a[61] ;
    r0 = dm(49,i4);
	r1 = dm(61,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[74]
//lo[76] = a[28] ^ a[47] ;
    r0 = dm(28,i4);
	r1 = dm(47,i4);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[75]

//lo[77] = a[ 7] ^ lo[68] ;
    r0 = dm( 7,i4);
	r1 = dm(68,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[76]
//lo[78] = a[ 3] ^ lo[ 3] ;
    r0 = dm( 3,i4);
	r1 = dm( 3,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[77]
//lo[79] = a[16] ^ lo[ 0] ;
    r0 = dm(16,i4);
	r1 = dm( 0,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[78]
//lo[80] = a[53] ^ lo[59] ;
    r0 = dm(53,i4);
	r1 = dm(59,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[79]
//lo[81] = a[ 9] ^ a[24] ^ a[25] ;
    r0 = dm( 9,i4);
	r1 = dm(24,i4);
    r0 = r0 xor r1;
	r1 = dm(25,i4);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[80]
//lo[82] = a[30] ^ lo[28] ;
    r0 = dm(30,i4);
	r1 = dm(28,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[81]
//lo[83] = a[47] ^ lo[13] ;
    r0 = dm(47,i4);
	r1 = dm(13,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[82]
//lo[84] = a[47] ^ lo[23] ;
    r0 = dm(47,i4);
	r1 = dm(23,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[83]
//lo[85] = a[19] ^ a[24] ^ a[27] ;
    r0 = dm(19,i4);
	r1 = dm(24,i4);
    r0 = r0 xor r1;
	r1 = dm(27,i4);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[84]
//lo[86] = a[39] ^ lo[ 5] ;
    r0 = dm(39,i4);
	r1 = dm( 5,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[85]
//lo[87] = a[26] ^ lo[ 8] ;
    r0 = dm(26,i4);
	r1 = dm( 8,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[86]
//lo[88] = a[11] ^ a[37] ^ a[44] ;
    r0 = dm(11,i4);
	r1 = dm(37,i4);
    r0 = r0 xor r1;
	r1 = dm(44,i4);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[87]
//lo[89] = a[60] ^ lo[49] ;
    r0 = dm(60,i4);
	r1 = dm(49,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[88]
//lo[90] = a[ 8] ^ lo[17] ;
    r0 = dm( 8,i4);
	r1 = dm(17,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[89]
//lo[91] = a[36] ^ lo[23] ;
    r0 = dm(36,i4);
	r1 = dm(23,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[90]
//lo[92] = a[13] ^ lo[ 8] ;
    r0 = dm(13,i4);
	r1 = dm( 8,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[91]
//lo[93] = a[19] ^ a[40] ^ a[61] ;
    r0 = dm(19,i4);
	r1 = dm(40,i4);
    r0 = r0 xor r1;
	r1 = dm(61,i4);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[92]
//lo[94] = a[ 5] ^ a[26] ^ a[47] ;
    r0 = dm( 5,i4);
	r1 = dm(26,i4);
    r0 = r0 xor r1;
	r1 = dm(47,i4);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[93]
//lo[95] = a[13] ^ a[22] ^ a[23] ;
    r0 = dm(13,i4);
	r1 = dm(22,i4);
    r0 = r0 xor r1;
	r1 = dm(23,i4);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[94]
//lo[96] = a[13] ^ a[43] ^ a[46] ;
    r0 = dm(13,i4);
	r1 = dm(43,i4);
    r0 = r0 xor r1;
	r1 = dm(46,i4);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[95]
//lo[97] = a[28] ^ a[58] ^ a[60] ;
    r0 = dm(28,i4);
	r1 = dm(58,i4);
    r0 = r0 xor r1;
	r1 = dm(60,i4);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[96]
//lo[98] = a[34] ^ a[42] ^ a[46] ;
    r0 = dm(34,i4);
	r1 = dm(42,i4);
    r0 = r0 xor r1;
	r1 = dm(46,i4);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[97]
//lo[99] = a[ 4] ^ a[30] ^ a[51] ;
    r0 = dm( 4,i4);
	r1 = dm(30,i4);
    r0 = r0 xor r1;
	r1 = dm(51,i4);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[98]
//lo[100] = a[32] ^ lo[19] ;
    r0 = dm(32,i4);
	r1 = dm(19,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[99]
//lo[101] = a[56] ^ lo[53] ;
    r0 = dm(56,i4);
	r1 = dm(53,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[100]
//lo[102] = a[41] ^ a[60] ^ a[62] ;
    r0 = dm(41,i4);
	r1 = dm(60,i4);
    r0 = r0 xor r1;
	r1 = dm(62,i4);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[101]
//lo[103] = a[ 6] ^ lo[ 1] ;
    r0 = dm( 6,i4);
	r1 = dm( 1,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[102]
//lo[104] = a[18] ^ a[39] ^ a[60] ;
    r0 = dm(18,i4);
	r1 = dm(39,i4);
    r0 = r0 xor r1;
	r1 = dm(60,i4);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[103]
//lo[105] = a[25] ^ lo[40] ;
    r0 = dm(25,i4);
	r1 = dm(40,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[104]
//lo[106] = a[31] ^ a[55] ^ a[62] ;
    r0 = dm(31,i4);
	r1 = dm(55,i4);
    r0 = r0 xor r1;
	r1 = dm(62,i4);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[105]
//lo[107] = a[52] ^ lo[62] ;
    r0 = dm(52,i4);
	r1 = dm(62,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[106]
//lo[108] = a[20] ^ a[41] ^ a[55] ;
    r0 = dm(20,i4);
	r1 = dm(41,i4);
    r0 = r0 xor r1;
	r1 = dm(55,i4);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[107]
//lo[109] = a[26] ^ a[40] ^ a[46] ;
    r0 = dm(26,i4);
	r1 = dm(40,i4);
    r0 = r0 xor r1;
	r1 = dm(46,i4);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[108]
//lo[110] = a[ 1] ^ lo[ 2] ;
    r0 = dm( 1,i4);
	r1 = dm( 2,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[109]
//lo[111] = a[33] ^ lo[19] ;
    r0 = dm(33,i4);
	r1 = dm(19,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[110]
//lo[112] = a[24] ^ a[45] ^ a[59] ;
    r0 = dm(24,i4);
	r1 = dm(45,i4);
    r0 = r0 xor r1;
	r1 = dm(59,i4);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[111]
//lo[113] = a[ 5] ^ a[20] ^ a[40] ;
    r0 = dm( 5,i4);
	r1 = dm(20,i4);
    r0 = r0 xor r1;
	r1 = dm(40,i4);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[112]
//lo[114] = a[10] ^ a[17] ^ a[34] ;
    r0 = dm(10,i4);
	r1 = dm(17,i4);
    r0 = r0 xor r1;
	r1 = dm(34,i4);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[113]
//lo[115] = a[43] ^ lo[13] ;
    r0 = dm(43,i4);
	r1 = dm(13,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[114]
//lo[116] = a[50] ^ lo[14] ;
    r0 = dm(50,i4);
	r1 = dm(14,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[115]
//lo[117] = a[12] ^ a[18] ^ a[33] ;
    r0 = dm(12,i4);
	r1 = dm(18,i4);
    r0 = r0 xor r1;
	r1 = dm(33,i4);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[116]
//lo[118] = a[ 9] ^ a[27] ^ a[59] ;
    r0 = dm( 9,i4);
	r1 = dm(27,i4);
    r0 = r0 xor r1;
	r1 = dm(59,i4);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[117]
//lo[119] = a[ 5] ^ a[16] ^ a[45] ;
    r0 = dm( 5,i4);
	r1 = dm(16,i4);
    r0 = r0 xor r1;
	r1 = dm(45,i4);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[118]
//lo[120] = a[22] ^ a[58] ^ lo[ 7] ;
    r0 = dm(22,i4);
	r1 = dm(58,i4);
    r0 = r0 xor r1;
	r1 = dm( 7,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[119]
//lo[121] = a[ 1] ^ a[36] ^ lo[50] ;
    r0 = dm( 1,i4);
	r1 = dm(36,i4);
    r0 = r0 xor r1;
	r1 = dm(50,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[120]
//lo[122] = a[14] ^ a[28] ^ lo[45] ;
    r0 = dm(14,i4);
	r1 = dm(28,i4);
    r0 = r0 xor r1;
	r1 = dm(45,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[121]
//lo[123] = a[18] ^ a[40] ^ lo[29] ;
    r0 = dm(18,i4);
	r1 = dm(40,i4);
    r0 = r0 xor r1;
	r1 = dm(29,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[122]
//lo[124] = a[27] ^ a[48] ^ lo[47] ;
    r0 = dm(27,i4);
	r1 = dm(48,i4);
    r0 = r0 xor r1;
	r1 = dm(47,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[123]
//lo[125] = a[ 4] ^ a[18] ^ a[19] ^ a[48] ;
    r0 = dm( 4,i4);
	r1 = dm(18,i4);
    r0 = r0 xor r1;
	r1 = dm(19,i4);
    r0 = r0 xor r1;
	r1 = dm(48,i4);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[124]
//lo[126] = a[ 8] ^ a[38] ^ lo[39] ;
    r0 = dm( 8,i4);
	r1 = dm(38,i4);
    r0 = r0 xor r1;
	r1 = dm(39,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[125]
//lo[127] = a[24] ^ a[31] ^ lo[30] ;
    r0 = dm(24,i4);
	r1 = dm(31,i4);
    r0 = r0 xor r1;
	r1 = dm(30,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[126]
//lo[128] = a[ 8] ^ a[17] ^ a[26] ^ a[44] ;
    r0 = dm( 8,i4);
	r1 = dm(17,i4);
    r0 = r0 xor r1;
	r1 = dm(26,i4);
    r0 = r0 xor r1;
	r1 = dm(44,i4);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[127]
//lo[129] = lo[37] ^ lo[ 4] ;
    r0 = dm(37,i1);
	r1 = dm( 4,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[128]
//lo[130] = a[11] ^ a[19] ^ lo[51] ;
    r0 = dm(11,i4);
	r1 = dm(19,i4);
    r0 = r0 xor r1;
	r1 = dm(51,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[129]
//lo[131] = a[ 6] ^ a[55] ^ lo[68] ;
    r0 = dm( 6,i4);
	r1 = dm(55,i4);
    r0 = r0 xor r1;
	r1 = dm(68,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[130]
//lo[132] = a[38] ^ lo[83] ;
    r0 = dm(38,i4);
	r1 = dm(83,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[131]
//lo[133] = a[10] ^ a[26] ^ lo[23] ;
    r0 = dm(10,i4);
	r1 = dm(26,i4);
    r0 = r0 xor r1;
	r1 = dm(23,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[132]
//lo[134] = a[16] ^ a[58] ^ lo[28] ;
    r0 = dm(16,i4);
	r1 = dm(58,i4);
    r0 = r0 xor r1;
	r1 = dm(28,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[133]
//lo[135] = a[ 2] ^ a[20] ^ a[47] ^ a[56] ;
    r0 = dm( 2,i4);
	r1 = dm(20,i4);
    r0 = r0 xor r1;
	r1 = dm(47,i4);
    r0 = r0 xor r1;
	r1 = dm(56,i4);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[134]
//lo[136] = a[51] ^ a[60] ^ lo[16] ;
    r0 = dm(51,i4);
	r1 = dm(60,i4);
    r0 = r0 xor r1;
	r1 = dm(16,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[135]
//lo[137] = a[23] ^ a[41] ^ a[52] ^ a[58] ;
    r0 = dm(23,i4);
	r1 = dm(41,i4);
    r0 = r0 xor r1;
	r1 = dm(52,i4);
    r0 = r0 xor r1;
	r1 = dm(58,i4);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[136]
//lo[138] = lo[18] ^ lo[ 9] ;
    r0 = dm(18,i1);
	r1 = dm( 9,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[137]
//lo[139] = a[32] ^ a[34] ^ lo[ 0] ;
    r0 = dm(32,i4);
	r1 = dm(34,i4);
    r0 = r0 xor r1;
	r1 = dm( 0,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[138]
//lo[140] = a[ 8] ^ a[15] ^ a[36] ^ lo[68] ;
    r0 = dm( 8,i4);
	r1 = dm(15,i4);
    r0 = r0 xor r1;
	r1 = dm(36,i4);
    r0 = r0 xor r1;
	r1 = dm(68,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[139]
//lo[141] = a[42] ^ lo[69] ^ lo[57] ;
    r0 = dm(42,i4);
	r1 = dm(69,i1);
    r0 = r0 xor r1;
	r1 = dm(57,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[140]
//lo[142] = a[ 4] ^ a[16] ^ a[32] ^ lo[38] ;
    r0 = dm( 4,i4);
	r1 = dm(16,i4);
    r0 = r0 xor r1;
	r1 = dm(32,i4);
    r0 = r0 xor r1;
	r1 = dm(38,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[141]
//lo[143] = a[19] ^ a[37] ^ a[42] ^ lo[ 1] ;
    r0 = dm(19,i4);
	r1 = dm(37,i4);
    r0 = r0 xor r1;
	r1 = dm(42,i4);
    r0 = r0 xor r1;
	r1 = dm( 1,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[142]
//lo[144] = a[36] ^ a[51] ^ a[53] ^ lo[ 2] ;
    r0 = dm(36,i4);
	r1 = dm(51,i4);
    r0 = r0 xor r1;
	r1 = dm(53,i4);
    r0 = r0 xor r1;
	r1 = dm( 2,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[143]
//lo[145] = a[15] ^ a[45] ^ lo[81] ;
    r0 = dm(15,i4);
	r1 = dm(45,i4);
    r0 = r0 xor r1;
	r1 = dm(81,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[144]
//lo[146] = a[49] ^ lo[21] ^ lo[ 3] ;
    r0 = dm(49,i4);
	r1 = dm(21,i1);
    r0 = r0 xor r1;
	r1 = dm( 3,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[145]
//lo[147] = a[ 1] ^ a[46] ^ a[62] ^ lo[ 4] ;
    r0 = dm( 1,i4);
	r1 = dm(46,i4);
    r0 = r0 xor r1;
	r1 = dm(62,i4);
    r0 = r0 xor r1;
	r1 = dm( 4,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[146]
//lo[148] = a[ 4] ^ a[44] ^ lo[108] ;
    r0 = dm( 4,i4);
	r1 = dm(44,i4);
    r0 = r0 xor r1;
	r1 = dm(108,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[147]
//lo[149] = lo[112] ^ lo[10] ;
    r0 = dm(112,i1);
	r1 = dm( 10,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[148]
//lo[150] = a[31] ^ a[49] ^ a[50] ^ lo[42] ;
    r0 = dm(31,i4);
	r1 = dm(49,i4);
    r0 = r0 xor r1;
	r1 = dm(50,i4);
    r0 = r0 xor r1;
	r1 = dm(42,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[149]
//lo[151] = a[44] ^ a[62] ^ lo[87] ;
    r0 = dm(44,i4);
	r1 = dm(62,i4);
    r0 = r0 xor r1;
	r1 = dm(87,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[150]
//lo[152] = a[13] ^ lo[38] ^ lo[11] ;
    r0 = dm(13,i4);
	r1 = dm(38,i1);
    r0 = r0 xor r1;
	r1 = dm(11,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[151]
//lo[153] = a[50] ^ lo[31] ^ lo[ 5] ;
    r0 = dm(50,i4);
	r1 = dm(31,i1);
    r0 = r0 xor r1;
	r1 = dm( 5,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[152]
//lo[154] = a[15] ^ a[30] ^ lo[84] ;
    r0 = dm(15,i4);
	r1 = dm(30,i4);
    r0 = r0 xor r1;
	r1 = dm(84,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[153]
//lo[155] = lo[77] ^ lo[ 6] ;
    r0 = dm(77,i1);
	r1 = dm( 6,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[154]
//lo[156] = a[ 4] ^ lo[53] ^ lo[ 6] ;
    r0 = dm( 4,i4);
	r1 = dm(53,i1);
    r0 = r0 xor r1;
	r1 = dm( 6,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[155]
//lo[157] = a[ 2] ^ a[35] ^ a[41] ^ lo[ 7] ;
    r0 = dm( 2,i4);
	r1 = dm(35,i4);
    r0 = r0 xor r1;
	r1 = dm(41,i4);
    r0 = r0 xor r1;
	r1 = dm( 7,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[156]
//lo[158] = a[23] ^ a[25] ^ a[28] ^ lo[41] ;
    r0 = dm(23,i4);
	r1 = dm(25,i4);
    r0 = r0 xor r1;
	r1 = dm(28,i4);
    r0 = r0 xor r1;
	r1 = dm(41,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[157]
//lo[159] = lo[122] ^ lo[ 8] ;
    r0 = dm(122,i1);
	r1 = dm(  8,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[158]
//lo[160] = a[17] ^ a[22] ^ a[31] ^ a[45] ^ lo[20] ;
    r0 = dm(17,i4);
	r1 = dm(22,i4);
    r0 = r0 xor r1;
	r1 = dm(31,i4);
    r0 = r0 xor r1;
	r1 = dm(45,i4);
    r0 = r0 xor r1;
	r1 = dm(20,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[159]
//lo[161] = a[ 0] ^ lo[110] ^ lo[74] ;
    r0 = dm(  0,i4);
	r1 = dm(110,i1);
    r0 = r0 xor r1;
	r1 = dm( 74,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[160]
//lo[162] = a[59] ^ lo[111] ^ lo[10] ;
    r0 = dm( 59,i4);
	r1 = dm(111,i1);
    r0 = r0 xor r1;
	r1 = dm( 10,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[161]
//lo[163] = lo[124] ^ lo[34] ;
    r0 = dm(124,i1);
	r1 = dm( 34,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[162]
//lo[164] = a[10] ^ a[50] ^ lo[128] ;
    r0 = dm( 10,i4);
	r1 = dm( 50,i4);
    r0 = r0 xor r1;
	r1 = dm(128,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[163]
//lo[165] = a[ 1] ^ a[22] ^ a[43] ^ lo[90] ;
    r0 = dm( 1,i4);
	r1 = dm(22,i4);
    r0 = r0 xor r1;
	r1 = dm(43,i4);
    r0 = r0 xor r1;
	r1 = dm(90,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[164]
//lo[166] = a[23] ^ lo[82] ^ lo[ 9] ;
    r0 = dm(23,i4);
	r1 = dm(82,i1);
    r0 = r0 xor r1;
	r1 = dm( 9,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[165]
//lo[167] = lo[105] ^ lo[80] ;
    r0 = dm(105,i1);
	r1 = dm( 80,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[166]
//lo[168] = a[ 0] ^ a[14] ^ lo[12] ^ lo[10] ;
    r0 = dm( 0,i4);
	r1 = dm(14,i4);
    r0 = r0 xor r1;
	r1 = dm(12,i1);
    r0 = r0 xor r1;
	r1 = dm(10,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[167]
//lo[169] = a[24] ^ lo[78] ^ lo[24] ;
    r0 = dm(24,i4);
	r1 = dm(78,i1);
    r0 = r0 xor r1;
	r1 = dm(24,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[168]
//lo[170] = a[35] ^ lo[79] ^ lo[64] ;
    r0 = dm(35,i4);
	r1 = dm(79,i1);
    r0 = r0 xor r1;
	r1 = dm(64,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[169]
//lo[171] = a[10] ^ a[37] ^ lo[76] ^ lo[11] ;
    r0 = dm(10,i4);
	r1 = dm(37,i4);
    r0 = r0 xor r1;
	r1 = dm(76,i1);
    r0 = r0 xor r1;
	r1 = dm(11,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[170]
//lo[172] = a[ 0] ^ a[23] ^ a[31] ^ lo[77] ;
    r0 = dm( 0,i4);
	r1 = dm(23,i4);
    r0 = r0 xor r1;
	r1 = dm(31,i4);
    r0 = r0 xor r1;
	r1 = dm(77,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[171]
//lo[173] = a[15] ^ a[26] ^ lo[95] ^ lo[28] ;
    r0 = dm(15,i4);
	r1 = dm(26,i4);
    r0 = r0 xor r1;
	r1 = dm(95,i1);
    r0 = r0 xor r1;
	r1 = dm(28,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[172]
//lo[174] = lo[78] ^ lo[57] ^ lo[16] ;
    r0 = dm(78,i1);
	r1 = dm(57,i1);
    r0 = r0 xor r1;
	r1 = dm(16,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[173]
//lo[175] = a[13] ^ a[27] ^ a[52] ^ lo[121] ;
    r0 = dm(13,i4);
	r1 = dm(27,i4);
    r0 = r0 xor r1;
	r1 = dm(52,i4);
    r0 = r0 xor r1;
	r1 = dm(121,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[174]
//lo[176] = a[35] ^ lo[126] ^ lo[12] ;
    r0 = dm( 35,i4);
	r1 = dm(126,i1);
    r0 = r0 xor r1;
	r1 = dm( 12,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[175]
//lo[177] = lo[79] ^ lo[61] ^ lo[15] ;
    r0 = dm(79,i1);
	r1 = dm(61,i1);
    r0 = r0 xor r1;
	r1 = dm(15,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[176]
//lo[178] = a[13] ^ lo[120] ^ lo[13] ;
    r0 = dm( 13,i4);
	r1 = dm(120,i1);
    r0 = r0 xor r1;
	r1 = dm( 13,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[177]
//lo[179] = a[19] ^ a[41] ^ lo[73] ^ lo[15] ^ lo[14] ;
    r0 = dm(19,i4);
	r1 = dm(41,i4);
    r0 = r0 xor r1;
	r1 = dm(73,i1);
    r0 = r0 xor r1;
	r1 = dm(15,i1);
    r0 = r0 xor r1;
	r1 = dm(14,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[178]
//lo[180] = lo[142] ^ lo[113] ;
    r0 = dm(142,i1);
	r1 = dm(113,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[179]
//lo[181] = a[ 2] ^ a[11] ^ lo[120] ^ lo[26] ;
    r0 = dm( 2,i4);
	r1 = dm(11,i4);
    r0 = r0 xor r1;
	r1 = dm(120,i1);
    r0 = r0 xor r1;
	r1 = dm( 26,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[180]
//lo[182] = a[21] ^ a[22] ^ a[50] ^ lo[106] ^ lo[88] ;
    r0 = dm(21,i4);
	r1 = dm(22,i4);
    r0 = r0 xor r1;
	r1 = dm(50,i4);
    r0 = r0 xor r1;
	r1 = dm(106,i1);
    r0 = r0 xor r1;
	r1 = dm(88,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[181]
//lo[183] = a[29] ^ a[39] ^ lo[150] ^ lo[43] ;
    r0 = dm(29,i4);
	r1 = dm(39,i4);
    r0 = r0 xor r1;
	r1 = dm(150,i1);
    r0 = r0 xor r1;
	r1 = dm( 43,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[182]
//lo[184] = a[16] ^ a[18] ^ lo[141] ^ lo[16] ;
    r0 = dm(16,i4);
	r1 = dm(18,i4);
    r0 = r0 xor r1;
	r1 = dm(141,i1);
    r0 = r0 xor r1;
	r1 = dm( 16,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[183]
//lo[185] = lo[163] ^ lo[80] ;
    r0 = dm(163,i1);
	r1 = dm( 80,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[184]
//lo[186] = a[24] ^ a[57] ^ lo[140] ^ lo[17] ;
    r0 = dm(24,i4);
	r1 = dm(57,i4);
    r0 = r0 xor r1;
	r1 = dm(140,i1);
    r0 = r0 xor r1;
	r1 = dm( 17,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[185]
//lo[187] = a[20] ^ a[39] ^ a[54] ^ lo[117] ^ lo[102] ;
    r0 = dm(20,i4);
	r1 = dm(39,i4);
    r0 = r0 xor r1;
	r1 = dm(54,i4);
    r0 = r0 xor r1;
	r1 = dm(117,i1);
    r0 = r0 xor r1;
	r1 = dm(102,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[186]
//lo[188] = a[16] ^ lo[166] ^ lo[18] ;
    r0 = dm( 16,i4);
	r1 = dm(166,i1);
    r0 = r0 xor r1;
	r1 = dm( 18,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[187]
//lo[189] = a[31] ^ a[44] ^ lo[107] ^ lo[71] ^ lo[25] ;
    r0 = dm(31,i4);
	r1 = dm(44,i4);
    r0 = r0 xor r1;
	r1 = dm(107,i1);
    r0 = r0 xor r1;
	r1 = dm(71,i1);
    r0 = r0 xor r1;
	r1 = dm(25,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[188]
//lo[190] = a[14] ^ a[23] ^ a[36] ^ a[61] ^ lo[153] ;
    r0 = dm(14,i4);
	r1 = dm(23,i4);
    r0 = r0 xor r1;
	r1 = dm(36,i4);
    r0 = r0 xor r1;
	r1 = dm(61,i4);
    r0 = r0 xor r1;
	r1 = dm(153,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[189]
//lo[191] = a[32] ^ lo[134] ^ lo[49] ^ lo[32] ^ lo[22] ;
    r0 = dm(32,i4);
	r1 = dm(134,i1);
    r0 = r0 xor r1;
	r1 = dm(49,i1);
    r0 = r0 xor r1;
	r1 = dm(32,i1);
    r0 = r0 xor r1;
	r1 = dm(22,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[190]
//lo[192] = a[10] ^ a[44] ^ lo[140] ^ lo[89] ^ lo[19] ;
    r0 = dm(10,i4);
	r1 = dm(44,i4);
    r0 = r0 xor r1;
	r1 = dm(140,i1);
    r0 = r0 xor r1;
	r1 = dm(89,i1);
    r0 = r0 xor r1;
	r1 = dm(19,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[191]
//lo[193] = a[22] ^ a[57] ^ lo[121] ^ lo[105] ^ lo[94] ;
    r0 = dm(22,i4);
	r1 = dm(57,i4);
    r0 = r0 xor r1;
	r1 = dm(121,i1);
    r0 = r0 xor r1;
	r1 = dm(105,i1);
    r0 = r0 xor r1;
	r1 = dm(94,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[192]
//lo[194] = a[19] ^ a[38] ^ a[53] ^ lo[137] ^ lo[96] ^ lo[66] ;
    r0 = dm(19,i4);
	r1 = dm(38,i4);
    r0 = r0 xor r1;
	r1 = dm(53,i4);
    r0 = r0 xor r1;
	r1 = dm(137,i1);              // lo[137]
    r0 = r0 xor r1;
	r1 = dm(96,i1);               // lo[96]
    r0 = r0 xor r1;
	r1 = dm(66,i1);               // lo[66]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[193]
//lo[195] = a[ 7] ^ a[14] ^ a[20] ^ lo[141] ^ lo[81] ^ lo[20] ;
    r0 = dm( 7,i4);
	r1 = dm(14,i4);
    r0 = r0 xor r1;
	r1 = dm(20,i4);
    r0 = r0 xor r1;
	r1 = dm(141,i1);              // lo[141]
    r0 = r0 xor r1;
	r1 = dm(81,i1);               // lo[81]
    r0 = r0 xor r1;
	r1 = dm(20,i1);               // lo[20]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[194]
//lo[196] = a[24] ^ a[33] ^ lo[174] ^ lo[103] ^ lo[21] ;
    r0 = dm(24,i4);
	r1 = dm(33,i4);
    r0 = r0 xor r1;
	r1 = dm(174,i1);
    r0 = r0 xor r1;
	r1 = dm(103,i1);
    r0 = r0 xor r1;
	r1 = dm(21,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[195]
//lo[197] = a[49] ^ lo[189] ^ lo[82] ^ lo[22] ;
    r0 = dm( 49,i4);
	r1 = dm(189,i1);
    r0 = r0 xor r1;
	r1 = dm( 82,i1);
    r0 = r0 xor r1;
	r1 = dm( 22,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[196]
//lo[198] = a[27] ^ a[45] ^ lo[143] ^ lo[123] ^ lo[97] ^ lo[54] ;
    r0 = dm(27,i4);
	r1 = dm(45,i4);
    r0 = r0 xor r1;
	r1 = dm(143,i1);              // lo[143]
    r0 = r0 xor r1;
	r1 = dm(123,i1);              // lo[123]
    r0 = r0 xor r1;
	r1 = dm(97,i1);               // lo[97]
    r0 = r0 xor r1;
	r1 = dm(54,i1);               // lo[54]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[197]
//lo[199] = lo[179] ^ lo[132] ^ lo[122] ;
    r0 = dm(179,i1);
	r1 = dm(132,i1);
    r0 = r0 xor r1;
	r1 = dm(122,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[198]
//lo[200] = a[16] ^ a[54] ^ lo[164] ^ lo[123] ^ lo[24] ^ lo[23] ;
    r0 = dm(16,i4);
	r1 = dm(54,i4);
    r0 = r0 xor r1;
	r1 = dm(164,i1);              // lo[164]
    r0 = r0 xor r1;
	r1 = dm(123,i1);              // lo[123]
    r0 = r0 xor r1;
	r1 = dm(24,i1);               // lo[24]
    r0 = r0 xor r1;
	r1 = dm(23,i1);               // lo[23]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[199]
//lo[201] = a[ 0] ^ a[53] ^ lo[147] ^ lo[91] ^ lo[86] ^ lo[85] ;
    r0 = dm( 0,i4);
	r1 = dm(53,i4);
    r0 = r0 xor r1;
	r1 = dm(147,i1);              // lo[147]
    r0 = r0 xor r1;
	r1 = dm(91,i1);               // lo[91]
    r0 = r0 xor r1;
	r1 = dm(86,i1);               // lo[86]
    r0 = r0 xor r1;
	r1 = dm(85,i1);               // lo[85]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[200]
//lo[202] = a[ 1] ^ lo[170] ^ lo[158] ^ lo[44] ^ lo[27] ;
    r0 = dm( 1,i4);
	r1 = dm(170,i1);
    r0 = r0 xor r1;
	r1 = dm(158,i1);
    r0 = r0 xor r1;
	r1 = dm(44,i1);
    r0 = r0 xor r1;
	r1 = dm(27,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[201]
//lo[203] = a[34] ^ lo[175] ^ lo[131] ^ lo[127] ^ lo[67] ;
    r0 = dm(34,i4);
	r1 = dm(175,i1);
    r0 = r0 xor r1;
	r1 = dm(131,i1);
    r0 = r0 xor r1;
	r1 = dm(127,i1);
    r0 = r0 xor r1;
	r1 = dm(67,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[202]
//lo[204] = lo[186] ^ lo[162] ^ lo[94] ;
    r0 = dm(186,i1);
	r1 = dm(162,i1);
    r0 = r0 xor r1;
	r1 = dm( 94,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[203]
//lo[205] = a[ 2] ^ lo[183] ^ lo[155] ^ lo[100] ^ lo[35] ;
    r0 = dm( 2,i4);
	r1 = dm(183,i1);
    r0 = r0 xor r1;

	r1 = dm(155,i1);
    r0 = r0 xor r1;
	r1 = dm(100,i1);
    r0 = r0 xor r1;
	r1 = dm(35,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[204]
//lo[206] = a[ 5] ^ a[53] ^ a[58] ^ lo[182] ^ lo[136] ^ lo[39] ^ lo[25] ;
    r0 = dm(  5,i4);
	r1 = dm( 53,i4);
    r0 = r0 xor r1;
	r1 = dm( 58,i4);               
    r0 = r0 xor r1;
	r1 = dm(182,i1);               // lo[182]               
    r0 = r0 xor r1;
	r1 = dm(136,i1);               // lo[136]
	r0 = r0 xor r1;
	r1 = dm( 39,i1);               // lo[39]
	r0 = r0 xor r1;
	r1 = dm( 25,i1);               // lo[25]
    r2 = r0 xor r1, dm(i0,m6)=r2;  // lo[205]
//lo[207] = a[15] ^ a[21] ^ a[29] ^ lo[190] ^ lo[146] ^ lo[83] ;
    r0 = dm(15,i4);
	r1 = dm(21,i4);
    r0 = r0 xor r1;
	r1 = dm(29,i4);
    r0 = r0 xor r1;
	r1 = dm(190,i1);               // lo[190]
    r0 = r0 xor r1;
	r1 = dm(146,i1);               // lo[146]
    r0 = r0 xor r1;
	r1 = dm(83,i1);                // lo[83]
    r2 = r0 xor r1, dm(i0,m6)=r2;  // lo[206]
//lo[208] = a[25] ^ a[42] ^ lo[182] ^ lo[169] ^ lo[84] ;
    r0 = dm(25,i4);
	r1 = dm(42,i4);
    r0 = r0 xor r1;
	r1 = dm(182,i1);
    r0 = r0 xor r1;
	r1 = dm(169,i1);
    r0 = r0 xor r1;
	r1 = dm(84,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[207]
//lo[209] = a[12] ^ a[14] ^ a[35] ^ lo[191] ^ lo[124] ^ lo[36] ;
    r0 = dm(12,i4);
	r1 = dm(14,i4);
    r0 = r0 xor r1;
	r1 = dm(35,i4);
    r0 = r0 xor r1;
	r1 = dm(191,i1);               // lo[191]
    r0 = r0 xor r1;
	r1 = dm(124,i1);               // lo[124]
    r0 = r0 xor r1;
	r1 = dm(36,i1);                // lo[36]
    r2 = r0 xor r1, dm(i0,m6)=r2;  // lo[208]
//lo[210] = a[ 1] ^ a[30] ^ lo[183] ^ lo[125] ^ lo[118] ^ lo[60] ;
    r0 = dm(1,i4);
	r1 = dm(30,i4);
    r0 = r0 xor r1;
	r1 = dm(183,i1);               // lo[183]
    r0 = r0 xor r1;
	r1 = dm(125,i1);               // lo[125]
    r0 = r0 xor r1;
	r1 = dm(118,i1);               // lo[118]
    r0 = r0 xor r1;
	r1 = dm(60,i1);                // lo[60]
    r2 = r0 xor r1, dm(i0,m6)=r2;  // lo[209]
//lo[211] = a[ 8] ^ lo[194] ^ lo[142] ^ lo[49] ;
    r0 = dm( 8,i4);
	r1 = dm(194,i1);
    r0 = r0 xor r1;
	r1 = dm(142,i1);
    r0 = r0 xor r1;
	r1 = dm( 49,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[210]
//lo[212] = a[ 5] ^ lo[173] ^ lo[148] ^ lo[85] ^ lo[27] ^ lo[26] ;
    r0 = dm( 5,i4);
	r1 = dm(173,i1);
    r0 = r0 xor r1;
	r1 = dm(148,i1);               // lo[148]
    r0 = r0 xor r1;
	r1 = dm( 85,i1);               // lo[85]
    r0 = r0 xor r1;
	r1 = dm( 27,i1);               // lo[27]
    r0 = r0 xor r1;
	r1 = dm( 26,i1);               // lo[26]
    r2 = r0 xor r1, dm(i0,m6)=r2;  // lo[211]
//lo[213] = lo[180] ^ lo[143] ^ lo[86] ^ lo[72] ^ lo[48] ;
    r0 = dm(180,i1);
	r1 = dm(143,i1);
    r0 = r0 xor r1;
	r1 = dm(86,i1);
    r0 = r0 xor r1;
	r1 = dm(72,i1);
    r0 = r0 xor r1;
	r1 = dm(48,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[212]
//lo[214] = a[ 1] ^ a[17] ^ lo[184] ^ lo[144] ^ lo[109] ^ lo[92] ;
    r0 = dm(1,i4);
	r1 = dm(17,i4);
    r0 = r0 xor r1;
	r1 = dm(184,i1);               // lo[184]
    r0 = r0 xor r1;
	r1 = dm(144,i1);               // lo[144]
    r0 = r0 xor r1;
	r1 = dm(109,i1);               // lo[109]
    r0 = r0 xor r1;
	r1 = dm(92,i1);                // lo[92]
    r2 = r0 xor r1, dm(i0,m6)=r2;  // lo[213]
//lo[215] = a[ 1] ^ a[11] ^ a[55] ^ lo[192] ^ lo[101] ^ lo[33] ^ lo[28] ;
    r0 = dm(  1,i4);
	r1 = dm( 11,i4);
    r0 = r0 xor r1;
	r1 = dm( 55,i4);               
    r0 = r0 xor r1;
	r1 = dm(192,i1);               // lo[192]               
    r0 = r0 xor r1;
	r1 = dm(101,i1);               // lo[101]
	r0 = r0 xor r1;
	r1 = dm( 33,i1);               // lo[33]
	r0 = r0 xor r1;
	r1 = dm( 28,i1);               // lo[28]
    r2 = r0 xor r1, dm(i0,m6)=r2;  // lo[214]
//lo[216] = lo[184] ^ lo[145] ^ lo[135] ^ lo[30] ^ lo[29] ;
    r0 = dm(184,i1);
	r1 = dm(145,i1);
    r0 = r0 xor r1;
	r1 = dm(135,i1);
    r0 = r0 xor r1;
	r1 = dm(30,i1);
    r0 = r0 xor r1;
	r1 = dm(29,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[215]
//lo[217] = a[34] ^ lo[195] ^ lo[46] ^ lo[33] ^ lo[32] ^ lo[31] ;
    r0 = dm(34,i4);
	r1 = dm(195,i1);
    r0 = r0 xor r1;
	r1 = dm(46,i1);               // lo[46] 
    r0 = r0 xor r1;
	r1 = dm(33,i1);               // lo[33]
    r0 = r0 xor r1;
	r1 = dm(32,i1);               // lo[32]
    r0 = r0 xor r1;
	r1 = dm(31,i1);               // lo[31]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[216]
//lo[218] = a[22] ^ lo[192] ^ lo[125] ^ lo[87] ^ lo[34] ;
    r0 = dm( 22,i4);
	r1 = dm(192,i1);
    r0 = r0 xor r1;
	r1 = dm(125,i1);
    r0 = r0 xor r1;
	r1 = dm(87,i1);
    r0 = r0 xor r1;
	r1 = dm(34,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[217]
//lo[219] = a[ 8] ^ a[12] ^ a[52] ^ a[57] ^ lo[195] ^ lo[119] ^ lo[35] ;
    r0 = dm(  8,i4);
	r1 = dm( 12,i4);
    r0 = r0 xor r1;
	r1 = dm( 52,i4);               
    r0 = r0 xor r1;
	r1 = dm( 57,i4);               // lo[57]               
    r0 = r0 xor r1;
	r1 = dm(195,i1);               // lo[195]
	r0 = r0 xor r1;
	r1 = dm(119,i1);               // lo[119]
	r0 = r0 xor r1;
	r1 = dm( 35,i1);               // lo[35]
    r2 = r0 xor r1, dm(i0,m6)=r2;  // lo[218]
//lo[220] = a[ 0] ^ a[11] ^ a[49] ^ lo[191] ^ lo[149] ^ lo[104] ^ lo[56] ;
    r0 = dm(  0,i4);
	r1 = dm( 11,i4);
    r0 = r0 xor r1;
	r1 = dm( 49,i4);               
    r0 = r0 xor r1;
	r1 = dm(191,i1);               // lo[191]               
    r0 = r0 xor r1;
	r1 = dm(149,i1);               // lo[149]
	r0 = r0 xor r1;
	r1 = dm(104,i1);               // lo[104]
	r0 = r0 xor r1;
	r1 = dm( 56,i1);               // lo[56]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[219]
//lo[221] = lo[193] ^ lo[185] ^ lo[93] ;
    r0 = dm(193,i1);
	r1 = dm(185,i1);
    r0 = r0 xor r1;
	r1 = dm( 93,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[220]
//lo[222] = a[ 1] ^ a[52] ^ lo[204] ^ lo[115] ^ lo[65] ^ lo[36] ;
    r0 = dm(1,i4);
	r1 = dm(52,i4);
    r0 = r0 xor r1;
	r1 = dm(204,i1);              // lo[204]
    r0 = r0 xor r1;
	r1 = dm(115,i1);              // lo[115]
    r0 = r0 xor r1;
	r1 = dm(65,i1);               // lo[65]
    r0 = r0 xor r1;
	r1 = dm(36,i1);               // lo[36]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[221]
//lo[223] = lo[203] ^ lo[126] ^ lo[116] ^ lo[37] ;
    r0 = dm(203,i1);
	r1 = dm(126,i1);
    r0 = r0 xor r1;
	r1 = dm(116,i1);
    r0 = r0 xor r1;
	r1 = dm( 37,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[222]
//lo[224] = lo[197] ^ lo[185] ^ lo[104] ;
    r0 = dm(197,i1);
	r1 = dm(185,i1);
    r0 = r0 xor r1;
	r1 = dm(104,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[223]
//lo[225] = lo[193] ^ lo[187] ^ lo[159] ;
    r0 = dm(193,i1);
	r1 = dm(187,i1);
    r0 = r0 xor r1;
	r1 = dm(159,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[224]
//lo[226] = lo[197] ^ lo[186] ^ lo[93] ;
    r0 = dm(197,i1);
	r1 = dm(186,i1);
    r0 = r0 xor r1;
	r1 = dm( 93,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[225]
//lo[227] = lo[196] ^ lo[171] ^ lo[130] ^ lo[39] ^ lo[38] ;
    r0 = dm(196,i1);
	r1 = dm(171,i1);
    r0 = r0 xor r1;
	r1 = dm(130,i1);
    r0 = r0 xor r1;
	r1 = dm(39,i1);
    r0 = r0 xor r1;
	r1 = dm(38,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[226]
//lo[228] = lo[202] ^ lo[179] ^ lo[133] ;
    r0 = dm(202,i1);
	r1 = dm(179,i1);
    r0 = r0 xor r1;
	r1 = dm(133,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[227]
//lo[229] = lo[198] ^ lo[161] ^ lo[127] ^ lo[40] ;
    r0 = dm(198,i1);
	r1 = dm(161,i1);
    r0 = r0 xor r1;
	r1 = dm(127,i1);
    r0 = r0 xor r1;
	r1 = dm( 40,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[228]
//lo[230] = a[ 5] ^ lo[227] ^ lo[129] ^ lo[41] ;
    r0 = dm(  5,i4);
	r1 = dm(227,i1);
    r0 = r0 xor r1;
	r1 = dm(129,i1);
    r0 = r0 xor r1;
	r1 = dm( 41,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[229]
//lo[231] = a[62] ^ lo[227] ^ lo[178] ^ lo[128] ^ lo[42] ;
    r0 = dm( 62,i4);
	r1 = dm(227,i1);
    r0 = r0 xor r1;
	r1 = dm(178,i1);
    r0 = r0 xor r1;
	r1 = dm(128,i1);
    r0 = r0 xor r1;
	r1 = dm( 42,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[230]
//lo[232] = a[ 9] ^ lo[76] ^ lo[117] ^ lo[172] ^ lo[213] ;
    r0 = dm(  9,i4);
	r1 = dm( 76,i1);
    r0 = r0 xor r1;
	r1 = dm(117,i1);
    r0 = r0 xor r1;
	r1 = dm(172,i1);
    r0 = r0 xor r1;
	r1 = dm(213,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[231]
//lo[233] = a[23] ^ lo[116] ^ lo[171] ^ lo[219] ;
    r0 = dm( 23,i4);
	r1 = dm(116,i1);
    r0 = r0 xor r1;
	r1 = dm(171,i1);
    r0 = r0 xor r1;
	r1 = dm(219,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[232]
//lo[234] = a[11] ^ a[45] ^ lo[74] ^ lo[75] ^ lo[170] ^ lo[212] ;
    r0 = dm(11,i4);
	r1 = dm(45,i4);
    r0 = r0 xor r1;
	r1 = dm(74,i1);              // lo[74]
    r0 = r0 xor r1;
	r1 = dm(75,i1);              // lo[75]
    r0 = r0 xor r1;
	r1 = dm(170,i1);             // lo[170]
    r0 = r0 xor r1;
	r1 = dm(212,i1);               // lo[212]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[233]
//lo[235] = lo[73] ^ lo[115] ^ lo[158] ^ lo[218] ;
    r0 = dm( 73,i1);
	r1 = dm(115,i1);
    r0 = r0 xor r1;
	r1 = dm(158,i1);
    r0 = r0 xor r1;
	r1 = dm(218,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[234]
//lo[236] = a[ 5] ^ a[13] ^ a[17] ^ lo[72] ^ lo[157] ^ lo[217] ;
    r0 = dm( 5,i4);
	r1 = dm( 13,i4);
    r0 = r0 xor r1;
	r1 = dm( 17,i4);
    r0 = r0 xor r1;
	r1 = dm( 72,i1);               // lo[72]
    r0 = r0 xor r1;
	r1 = dm(157,i1);               // lo[157]
    r0 = r0 xor r1;
	r1 = dm(217,i1);               // lo[217]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[235]
//lo[237] = lo[113] ^ lo[114] ^ lo[169] ^ lo[211] ;
    r0 = dm(113,i1);
	r1 = dm(114,i1);
    r0 = r0 xor r1;
	r1 = dm(169,i1);
    r0 = r0 xor r1;
	r1 = dm(211,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[236]
//lo[238] = a[42] ^ lo[71] ^ lo[112] ^ lo[168] ^ lo[221] ;
    r0 = dm( 42,i4);
	r1 = dm( 71,i1);
    r0 = r0 xor r1;
	r1 = dm(112,i1);
    r0 = r0 xor r1;
	r1 = dm(168,i1);
    r0 = r0 xor r1;
	r1 = dm(221,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[237]
//lo[239] = lo[111] ^ lo[167] ^ lo[226] ;
    r0 = dm(111,i1);
	r1 = dm(167,i1);
    r0 = r0 xor r1;
	r1 = dm(226,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[238]
//lo[240] = lo[166] ^ lo[204] ;
    r0 = dm(166,i1);
	r1 = dm(204,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[239]
//lo[241] = a[48] ^ lo[69] ^ lo[70] ^ lo[138] ^ lo[225] ;
    r0 = dm( 48,i4);
	r1 = dm( 69,i1);
    r0 = r0 xor r1;
	r1 = dm( 70,i1);
    r0 = r0 xor r1;
	r1 = dm(138,i1);
    r0 = r0 xor r1;
	r1 = dm(225,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[240]
//lo[242] = a[24] ^ lo[68] ^ lo[165] ^ lo[224] ;
    r0 = dm( 24,i4);
	r1 = dm( 68,i1);
    r0 = r0 xor r1;
	r1 = dm(165,i1);
    r0 = r0 xor r1;
	r1 = dm(224,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[241]
//lo[243] = lo[181] ^ lo[202] ;
    r0 = dm(181,i1);
	r1 = dm(202,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[242]
//lo[244] = a[40] ^ lo[67] ^ lo[110] ^ lo[168] ^ lo[212] ;
    r0 = dm( 40,i4);
	r1 = dm( 67,i1);
    r0 = r0 xor r1;
	r1 = dm(110,i1);
    r0 = r0 xor r1;
	r1 = dm(168,i1);
    r0 = r0 xor r1;
	r1 = dm(212,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[243]
//lo[245] = a[ 1] ^ lo[137] ^ lo[156] ^ lo[216] ;
    r0 = dm(  1,i4);
	r1 = dm(137,i1);
    r0 = r0 xor r1;
	r1 = dm(156,i1);
    r0 = r0 xor r1;
	r1 = dm(216,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[244]
//lo[246] = a[11] ^ lo[108] ^ lo[109] ^ lo[155] ^ lo[210] ;
    r0 = dm( 11,i4);
	r1 = dm(108,i1);
    r0 = r0 xor r1;
	r1 = dm(109,i1);
    r0 = r0 xor r1;
	r1 = dm(155,i1);
    r0 = r0 xor r1;
	r1 = dm(210,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[245]
//lo[247] = a[17] ^ lo[66] ^ lo[107] ^ lo[139] ^ lo[215] ;
    r0 = dm( 17,i4);
	r1 = dm( 66,i1);
    r0 = r0 xor r1;
	r1 = dm(107,i1);
    r0 = r0 xor r1;
	r1 = dm(139,i1);
    r0 = r0 xor r1;
	r1 = dm(215,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[246]
//lo[248] = a[59] ^ lo[64] ^ lo[65] ^ lo[156] ^ lo[214] ;
    r0 = dm( 59,i4);
	r1 = dm( 64,i1);
    r0 = r0 xor r1;
	r1 = dm( 65,i1);
    r0 = r0 xor r1;
	r1 = dm(156,i1);
    r0 = r0 xor r1;
	r1 = dm(214,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[247]
//lo[249] = lo[106] ^ lo[136] ^ lo[154] ^ lo[211] ;
    r0 = dm(106,i1);
	r1 = dm(136,i1);
    r0 = r0 xor r1;
	r1 = dm(154,i1);
    r0 = r0 xor r1;
	r1 = dm(211,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[248]
//lo[250] = lo[178] ^ lo[230] ;
    r0 = dm(178,i1);
	r1 = dm(230,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[249]
//lo[251] = lo[177] ^ lo[230] ;
    r0 = dm(177,i1);
	r1 = dm(230,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[250]
//lo[252] = lo[105] ^ lo[189] ^ lo[220] ;
    r0 = dm(105,i1);
	r1 = dm(189,i1);
    r0 = r0 xor r1;
	r1 = dm(220,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[251]
//lo[253] = lo[188] ^ lo[223] ;
    r0 = dm(188,i1);
	r1 = dm(223,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[252]
//lo[254] = lo[104] ^ lo[167] ^ lo[222] ;
    r0 = dm(104,i1);
	r1 = dm(167,i1);
    r0 = r0 xor r1;
	r1 = dm(222,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[253]
//lo[255] = a[30] ^ a[43] ^ lo[62] ^ lo[63] ^ lo[103] ^ lo[176] ^ lo[201] ;
    r0 = dm(30,i4);
	r1 = dm(43,i4);
    r0 = r0 xor r1;
	r1 = dm(62,i1);               
    r0 = r0 xor r1;
	r1 = dm(63,i1);               // lo[63]               
    r0 = r0 xor r1;
	r1 = dm(103,i1);               // lo[103]
	r0 = r0 xor r1;
	r1 = dm(176,i1);               // lo[176]
	r0 = r0 xor r1;
	r1 = dm(201,i1);               // lo[201]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[254]
//lo[256] = a[ 4] ^ a[ 8] ^ a[24] ^ a[40] ^ lo[102] ^ lo[153] ^ lo[209] ;
    r0 = dm(  4,i4);
	r1 = dm(  8,i4);
    r0 = r0 xor r1;
	r1 = dm( 24,i4);               
    r0 = r0 xor r1;
	r1 = dm( 40,i4);               // lo[40]               
    r0 = r0 xor r1;
	r1 = dm(102,i1);               // lo[102]
	r0 = r0 xor r1;
	r1 = dm(153,i1);               // lo[153]
	r0 = r0 xor r1;
	r1 = dm(209,i1);               // lo[209]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[255]
//lo[257] = lo[61] ^ lo[100] ^ lo[101] ^ lo[138] ^ lo[210] ;
    r0 = dm( 61,i1);
	r1 = dm(100,i1);
    r0 = r0 xor r1;
	r1 = dm(101,i1);
    r0 = r0 xor r1;
	r1 = dm(138,i1);
    r0 = r0 xor r1;
	r1 = dm(210,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[256]
//lo[258] = a[31] ^ a[58] ^ a[59] ^ lo[66] ^ lo[157] ^ lo[219] ;
    r0 = dm(31,i4);
	r1 = dm(58,i4);
    r0 = r0 xor r1;
	r1 = dm(59,i4);
    r0 = r0 xor r1;
	r1 = dm(66,i1);              // lo[66]
    r0 = r0 xor r1;
	r1 = dm(157,i1);               // lo[157]
    r0 = r0 xor r1;
	r1 = dm(219,i1);               // lo[219]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[257]
//lo[259] = a[29] ^ lo[60] ^ lo[75] ^ lo[152] ^ lo[218] ;
    r0 = dm( 29,i4);
	r1 = dm( 60,i1);
    r0 = r0 xor r1;
	r1 = dm( 75,i1);
    r0 = r0 xor r1;
	r1 = dm(152,i1);
    r0 = r0 xor r1;
	r1 = dm(218,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[258]
//lo[260] = lo[194] ^ lo[208] ;
    r0 = dm(194,i1);
	r1 = dm(208,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[259]
//lo[261] = a[ 5] ^ a[37] ^ a[39] ^ a[48] ^ lo[98] ^ lo[99] ^ lo[172] ^ lo[200] ;
    r0 = dm(  5,i4);
	r1 = dm( 37,i4);
    r0 = r0 xor r1;
	r1 = dm( 39,i4);               
    r0 = r0 xor r1;
	r1 = dm( 48,i4);               
    r0 = r0 xor r1;
	r1 = dm( 98,i1);               // lo[98]               
    r0 = r0 xor r1;
	r1 = dm( 99,i1);               // lo[99]
	r0 = r0 xor r1;
	r1 = dm(172,i1);               // lo[172]
	r0 = r0 xor r1;
	r1 = dm(200,i1);               // lo[200]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[260]
//lo[262] = a[20] ^ a[27] ^ lo[59] ^ lo[99] ^ lo[151] ^ lo[207] ;
    r0 = dm(20,i4);
	r1 = dm(27,i4);
    r0 = r0 xor r1;
	r1 = dm(59,i1);
    r0 = r0 xor r1;
	r1 = dm(99,i1);              // lo[99]
    r0 = r0 xor r1;
	r1 = dm(151,i1);               // lo[151]
    r0 = r0 xor r1;
	r1 = dm(207,i1);               // lo[207]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[261]
//lo[263] = a[14] ^ a[41] ^ lo[97] ^ lo[135] ^ lo[175] ^ lo[200] ;
    r0 = dm(14,i4);
	r1 = dm(41,i4);
    r0 = r0 xor r1;
	r1 = dm(97,i1);
    r0 = r0 xor r1;
	r1 = dm(135,i1);              // lo[135]
    r0 = r0 xor r1;
	r1 = dm(175,i1);               // lo[175]
    r0 = r0 xor r1;
	r1 = dm(200,i1);               // lo[200]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[262]
//lo[264] = a[31] ^ a[37] ^ lo[96] ^ lo[151] ^ lo[216] ;
    r0 = dm( 31,i4);
	r1 = dm( 37,i4);
    r0 = r0 xor r1;
	r1 = dm( 96,i1);
    r0 = r0 xor r1;
	r1 = dm(151,i1);
    r0 = r0 xor r1;
	r1 = dm(216,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[263]
//lo[265] = a[38] ^ a[47] ^ lo[95] ^ lo[150] ^ lo[215] ;
    r0 = dm( 38,i4);
	r1 = dm( 47,i4);
    r0 = r0 xor r1;
	r1 = dm( 95,i1);
    r0 = r0 xor r1;
	r1 = dm(150,i1);
    r0 = r0 xor r1;
	r1 = dm(215,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[264]
//lo[266] = a[38] ^ a[60] ^ lo[57] ^ lo[58] ^ lo[164] ^ lo[213] ;
    r0 = dm(38,i4);
	r1 = dm(60,i4);
    r0 = r0 xor r1;
	r1 = dm(57,i1);
    r0 = r0 xor r1;
	r1 = dm(58,i1);              // lo[58]
    r0 = r0 xor r1;
	r1 = dm(164,i1);               // lo[164]
    r0 = r0 xor r1;
	r1 = dm(213,i1);               // lo[213]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[265]
//lo[267] = lo[93] ^ lo[94] ^ lo[165] ^ lo[220] ;
    r0 = dm( 93,i1);
	r1 = dm( 94,i1);
    r0 = r0 xor r1;
	r1 = dm(165,i1);
    r0 = r0 xor r1;
	r1 = dm(220,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[266]
//lo[268] = lo[94] ^ lo[163] ^ lo[226] ;
    r0 = dm( 94,i1);
	r1 = dm(163,i1);
    r0 = r0 xor r1;
	r1 = dm(226,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[267]
//lo[269] = lo[56] ^ lo[134] ^ lo[203] ;
    r0 = dm( 56,i1);
	r1 = dm(134,i1);
    r0 = r0 xor r1;
	r1 = dm(203,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[268]
//lo[270] = a[ 3] ^ a[55] ^ lo[58] ^ lo[149] ^ lo[225] ;
    r0 = dm(  3,i4);
	r1 = dm( 55,i4);
    r0 = r0 xor r1;
	r1 = dm( 58,i1);
    r0 = r0 xor r1;
	r1 = dm(149,i1);
    r0 = r0 xor r1;
	r1 = dm(225,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[269]
//lo[271] = lo[93] ^ lo[162] ^ lo[224] ;
    r0 = dm( 93,i1);
	r1 = dm(162,i1);
    r0 = r0 xor r1;
	r1 = dm(224,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[270]
//lo[272] = a[20] ^ lo[92] ^ lo[133] ^ lo[199] ;
    r0 = dm( 20,i4);
	r1 = dm( 92,i1);
    r0 = r0 xor r1;
	r1 = dm(133,i1);
    r0 = r0 xor r1;
	r1 = dm(199,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[271]
//lo[273] = lo[174] ^ lo[177] ^ lo[229] ;
    r0 = dm(174,i1);
	r1 = dm(177,i1);
    r0 = r0 xor r1;
	r1 = dm(229,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[272]
//lo[274] = a[13] ^ a[20] ^ lo[132] ^ lo[181] ^ lo[228] ;
    r0 = dm( 13,i4);
	r1 = dm( 20,i4);
    r0 = r0 xor r1;
	r1 = dm(132,i1);
    r0 = r0 xor r1;
	r1 = dm(181,i1);
    r0 = r0 xor r1;
	r1 = dm(228,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[273]
//lo[275] = a[14] ^ a[48] ^ lo[54] ^ lo[55] ^ lo[161] ^ lo[206] ;
    r0 = dm(14,i4);
	r1 = dm(48,i4);
    r0 = r0 xor r1;
	r1 = dm(54,i1);
    r0 = r0 xor r1;
	r1 = dm(55,i1);              // lo[55]
    r0 = r0 xor r1;
	r1 = dm(161,i1);               // lo[161]
    r0 = r0 xor r1;
	r1 = dm(206,i1);               // lo[206]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[274]
//lo[276] = a[43] ^ lo[53] ^ lo[131] ^ lo[152] ^ lo[207] ;
    r0 = dm( 43,i4);
	r1 = dm( 53,i1);
    r0 = r0 xor r1;
	r1 = dm(131,i1);
    r0 = r0 xor r1;
	r1 = dm(152,i1);
    r0 = r0 xor r1;
	r1 = dm(207,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[275]
//lo[277] = a[ 1] ^ a[37] ^ a[57] ^ lo[52] ^ lo[63] ^ lo[148] ^ lo[205] ;
    r0 = dm(  1,i4);
	r1 = dm( 37,i4);
    r0 = r0 xor r1;
	r1 = dm( 57,i4);               
    r0 = r0 xor r1;
	r1 = dm( 52,i1);               // lo[52]               
    r0 = r0 xor r1;
	r1 = dm( 63,i1);               // lo[63]
	r0 = r0 xor r1;
	r1 = dm(148,i1);               // lo[148]
	r0 = r0 xor r1;
	r1 = dm(205,i1);               // lo[205]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[276]
//lo[278] = a[28] ^ lo[51] ^ lo[55] ^ lo[147] ^ lo[217] ;
    r0 = dm( 28,i4);
	r1 = dm( 51,i1);
    r0 = r0 xor r1;
	r1 = dm( 55,i1);
    r0 = r0 xor r1;
	r1 = dm(147,i1);
    r0 = r0 xor r1;
	r1 = dm(217,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[277]
//lo[279] = a[23] ^ lo[50] ^ lo[91] ^ lo[160] ^ lo[209] ;
    r0 = dm( 23,i4);
	r1 = dm( 50,i1);
    r0 = r0 xor r1;
	r1 = dm( 91,i1);
    r0 = r0 xor r1;
	r1 = dm(160,i1);
    r0 = r0 xor r1;
	r1 = dm(209,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[278]
//lo[280] = a[ 8] ^ lo[114] ^ lo[180] ^ lo[208] ;
    r0 = dm(  8,i4);
	r1 = dm(114,i1);
    r0 = r0 xor r1;
	r1 = dm(180,i1);
    r0 = r0 xor r1;
	r1 = dm(208,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[279]
//lo[281] = a[ 0] ^ lo[49] ^ lo[159] ^ lo[223] ;
    r0 = dm(  0,i4);
	r1 = dm( 49,i1);
    r0 = r0 xor r1;
	r1 = dm(159,i1);
    r0 = r0 xor r1;
	r1 = dm(223,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[280]
//lo[282] = lo[188] ^ lo[222] ;
    r0 = dm(188,i1);
	r1 = dm(222,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[281]
//lo[283] = lo[90] ^ lo[187] ^ lo[221] ;
    r0 = dm( 90,i1);
	r1 = dm(187,i1);
    r0 = r0 xor r1;
	r1 = dm(221,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[282]
//lo[284] = a[ 7] ^ lo[47] ^ lo[48] ^ lo[146] ^ lo[160] ^ lo[201] ;
    r0 = dm( 7,i4);
	r1 = dm(47,i1);
    r0 = r0 xor r1;
	r1 = dm(48,i1);
    r0 = r0 xor r1;
	r1 = dm(146,i1);              // lo[146]
    r0 = r0 xor r1;
	r1 = dm(160,i1);               // lo[160]
    r0 = r0 xor r1;
	r1 = dm(201,i1);               // lo[201]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[283]
//lo[285] = lo[60] ^ lo[89] ^ lo[173] ^ lo[205] ;
    r0 = dm( 60,i1);
	r1 = dm( 89,i1);
    r0 = r0 xor r1;
	r1 = dm(173,i1);
    r0 = r0 xor r1;
	r1 = dm(205,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[284]
//lo[286] = a[ 4] ^ lo[70] ^ lo[88] ^ lo[144] ^ lo[145] ^ lo[199] ;
    r0 = dm( 4,i4);
	r1 = dm(70,i1);
    r0 = r0 xor r1;
	r1 = dm(88,i1);
    r0 = r0 xor r1;
	r1 = dm(144,i1);              // lo[144]
    r0 = r0 xor r1;
	r1 = dm(145,i1);               // lo[145]
    r0 = r0 xor r1;
	r1 = dm(199,i1);               // lo[199]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[285]

//lo[287] = lo[45] ^ lo[46] ^ lo[52] ^ lo[130] ^ lo[214] ;
    r0 = dm( 45,i1);
	r1 = dm( 46,i1);
    r0 = r0 xor r1;
	r1 = dm( 52,i1);
    r0 = r0 xor r1;
	r1 = dm(130,i1);
    r0 = r0 xor r1;
	r1 = dm(214,i1);
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[286]
//lo[288] = a[56] ^ lo[44] ^ lo[67] ^ lo[129] ^ lo[176] ^ lo[198] ;
    r0 = dm(56,i4);
	r1 = dm(44,i1);
    r0 = r0 xor r1;
	r1 = dm(67,i1);
    r0 = r0 xor r1;
	r1 = dm(129,i1);              // lo[129]
    r0 = r0 xor r1;
	r1 = dm(176,i1);               // lo[176]
    r0 = r0 xor r1;
	r1 = dm(198,i1);               // lo[198]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[287]
//lo[289] = a[25] ^ a[40] ^ lo[43] ^ lo[98] ^ lo[154] ^ lo[206] ;
    r0 = dm(25,i4);
	r1 = dm(40,i4);
    r0 = r0 xor r1;
	r1 = dm(43,i1);
    r0 = r0 xor r1;
	r1 = dm(98,i1);              // lo[98]
    r0 = r0 xor r1;
	r1 = dm(154,i1);               // lo[154]
    r0 = r0 xor r1;
	r1 = dm(206,i1);               // lo[206]
    r2 = r0 xor r1, dm(i0,m6)=r2; // lo[288]
    dm(i0,m6)=r2; // lo[289]

//lo[290] = lo[234] ^ lo[236] ;
    r0 = dm(234,i1);
	r1 = dm(236,i1);
//	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[289]
	r2 = r0 xor r1;
//lo[291] = lo[235] ^ lo[236] ;
    r0 = dm(235,i1);
	r1 = dm(236,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[290]
//lo[292] = lo[235] ^ lo[237] ;
    r0 = dm(235,i1);
	r1 = dm(237,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[291]
//lo[293] = lo[232] ^ lo[235] ;
    r0 = dm(232,i1);
	r1 = dm(235,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[292]
//lo[294] = lo[233] ^ lo[235] ;
    r0 = dm(233,i1);
	r1 = dm(235,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[293]
//lo[295] = lo[237] ^ lo[290] ;
    r0 = dm(237,i1);
	r1 = dm(290,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[294]
//lo[296] = lo[234] ^ lo[294] ;
    r0 = dm(234,i1);
	r1 = dm(294,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[295]
//lo[297] = lo[233] ^ lo[290] ;
    r0 = dm(233,i1);
	r1 = dm(290,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[296]
//lo[298] = lo[234] ^ lo[292] ;
    r0 = dm(234,i1);
	r1 = dm(292,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[297]
	dm(i0,m6)=r2; // lo[298]
//lo[299] = lo[236] ^ lo[298] ; **
    r0 = dm(236,i1);
	r1 = dm(298,i1);
	r2 = r0 xor r1;
	dm(i0,m6)=r2; // lo[299]
//lo[300] = lo[233] ^ lo[299] ; **
    r0 = dm(233,i1);
    r1 = dm(299,i1);
	r2 = r0 xor r1; 
//lo[301] = lo[236] ^ lo[293] ;
    r0 = dm(236,i1);
	r1 = dm(293,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[300]
//lo[302] = lo[232] ^ lo[299] ;
    r0 = dm(232,i1);
	r1 = dm(299,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[301]
//lo[303] = lo[232] ^ lo[296] ;
    r0 = dm(232,i1);
	r1 = dm(296,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[302]
//lo[304] = lo[232] ^ lo[233] ;
    r0 = dm(232,i1);
	r1 = dm(233,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[303]
//lo[305] = lo[240] ^ lo[242] ;
    r0 = dm(240,i1);
	r1 = dm(242,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[304]
//lo[306] = lo[241] ^ lo[242] ;
    r0 = dm(241,i1);
	r1 = dm(242,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[305]
//lo[307] = lo[241] ^ lo[243] ;
    r0 = dm(241,i1);
	r1 = dm(243,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[306]
//lo[308] = lo[238] ^ lo[241] ;
    r0 = dm(238,i1);
	r1 = dm(241,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[307]
//lo[309] = lo[239] ^ lo[241] ;
    r0 = dm(239,i1);
	r1 = dm(241,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[308]
//lo[310] = lo[243] ^ lo[305] ;
    r0 = dm(243,i1);
	r1 = dm(305,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[309]
//lo[311] = lo[240] ^ lo[309] ;
    r0 = dm(240,i1);
	r1 = dm(309,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[310]
//lo[312] = lo[239] ^ lo[305] ;
    r0 = dm(239,i1);
	r1 = dm(305,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[311]
//lo[313] = lo[240] ^ lo[307] ;
    r0 = dm(240,i1);
	r1 = dm(307,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[312]
	dm(i0,m6)=r2; // lo[313]
//lo[314] = lo[242] ^ lo[313] ; **
    r0 = dm(242,i1);
	r1 = dm(313,i1);
	r2 = r0 xor r1; 
	dm(i0,m6)=r2;   // lo[314]
//lo[315] = lo[239] ^ lo[314] ; **
    r0 = dm(239,i1);
	r1 = dm(314,i1);
	r2 = r0 xor r1;
//lo[316] = lo[242] ^ lo[308] ;
    r0 = dm(242,i1);
	r1 = dm(308,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[315]
//lo[317] = lo[238] ^ lo[314] ;
    r0 = dm(238,i1);
	r1 = dm(314,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[316]
//lo[318] = lo[238] ^ lo[311] ;
    r0 = dm(238,i1);
	r1 = dm(311,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[317]
//lo[319] = lo[238] ^ lo[239] ;
    r0 = dm(238,i1);
	r1 = dm(239,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[318]
//lo[320] = lo[246] ^ lo[248] ;
    r0 = dm(246,i1);
	r1 = dm(248,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[319]
//lo[321] = lo[247] ^ lo[248] ;
    r0 = dm(247,i1);
	r1 = dm(248,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[320]
//lo[322] = lo[247] ^ lo[249] ;
    r0 = dm(247,i1);
	r1 = dm(249,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[321]
//lo[323] = lo[244] ^ lo[247] ;
    r0 = dm(244,i1);
	r1 = dm(247,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[322]
//lo[324] = lo[245] ^ lo[247] ;
    r0 = dm(245,i1);
	r1 = dm(247,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[323]
//lo[325] = lo[249] ^ lo[320] ;
    r0 = dm(249,i1);
	r1 = dm(320,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[324]
//lo[326] = lo[246] ^ lo[324] ;
    r0 = dm(246,i1);
	r1 = dm(324,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[325]
//lo[327] = lo[245] ^ lo[320] ;
    r0 = dm(245,i1);
	r1 = dm(320,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[326]
//lo[328] = lo[246] ^ lo[322] ;
    r0 = dm(246,i1);
	r1 = dm(322,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[327]
	dm(i0,m6)=r2; // lo[328]
//lo[329] = lo[248] ^ lo[328] ; **
    r0 = dm(248,i1);
	r1 = dm(328,i1);
	r2 = r0 xor r1;
	dm(i0,m6)=r2; // lo[329]
//lo[330] = lo[245] ^ lo[329] ; **
    r0 = dm(245,i1);
	r1 = dm(329,i1);
	r2 = r0 xor r1; 
//lo[331] = lo[248] ^ lo[323] ;
    r0 = dm(248,i1);
	r1 = dm(323,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[330]
//lo[332] = lo[244] ^ lo[329] ;
    r0 = dm(244,i1);
	r1 = dm(329,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[331]
//lo[333] = lo[244] ^ lo[326] ;
    r0 = dm(244,i1);
	r1 = dm(326,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[332]
//lo[334] = lo[244] ^ lo[245] ;
    r0 = dm(244,i1);
	r1 = dm(245,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[333]
//lo[335] = lo[228] ^ lo[251] ;
    r0 = dm(228,i1);
	r1 = dm(251,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[334]
//lo[336] = lo[250] ^ lo[251] ;
    r0 = dm(250,i1);
	r1 = dm(251,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[335]
//lo[337] = lo[250] ^ lo[196] ;
    r0 = dm(250,i1);
	r1 = dm(196,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[336]
//lo[338] = lo[229] ^ lo[250] ;
    r0 = dm(229,i1);
	r1 = dm(250,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[337]
//lo[339] = lo[231] ^ lo[250] ;
    r0 = dm(231,i1);
	r1 = dm(250,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[338]
//lo[340] = lo[196] ^ lo[335] ;
    r0 = dm(196,i1);
	r1 = dm(335,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[339]
//lo[341] = lo[228] ^ lo[339] ;
    r0 = dm(228,i1);
	r1 = dm(339,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[340]
//lo[342] = lo[231] ^ lo[335] ;
    r0 = dm(231,i1);
	r1 = dm(335,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[341]
//lo[343] = lo[228] ^ lo[337] ;
    r0 = dm(228,i1);
	r1 = dm(337,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[342]
	dm(i0,m6)=r2; // lo[343]
//lo[344] = lo[251] ^ lo[343] ; **
    r0 = dm(251,i1);
	r1 = dm(343,i1);
	r2 = r0 xor r1;
	dm(i0,m6)=r2; // lo[344]
//lo[345] = lo[231] ^ lo[344] ; **
    r0 = dm(231,i1);
	r1 = dm(344,i1);
	r2 = r0 xor r1;
//lo[346] = lo[251] ^ lo[338] ;
    r0 = dm(251,i1);
	r1 = dm(338,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[345]
//lo[347] = lo[229] ^ lo[344] ;
    r0 = dm(229,i1);
	r1 = dm(344,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[346]
//lo[348] = lo[229] ^ lo[341] ;
    r0 = dm(229,i1);
	r1 = dm(341,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[347]
//lo[349] = lo[229] ^ lo[231] ;
    r0 = dm(229,i1);
	r1 = dm(231,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[348]
//lo[350] = lo[253] ^ lo[254] ;
    r0 = dm(253,i1);
	r1 = dm(254,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[349]
//lo[351] = lo[252] ^ lo[254] ;
    r0 = dm(252,i1);
	r1 = dm(254,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[350]
//lo[352] = lo[252] ^ lo[253] ;
    r0 = dm(252,i1);
	r1 = dm(253,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[351]
//lo[353] = lo[257] ^ lo[259] ;
    r0 = dm(257,i1);
	r1 = dm(259,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[352]
//lo[354] = lo[258] ^ lo[259] ;
    r0 = dm(258,i1);
	r1 = dm(259,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[353]
//lo[355] = lo[258] ^ lo[260] ;
    r0 = dm(258,i1);
	r1 = dm(260,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[354]
//lo[356] = lo[255] ^ lo[258] ;
    r0 = dm(255,i1);
	r1 = dm(258,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[355]
//lo[357] = lo[256] ^ lo[258] ;
    r0 = dm(256,i1);
	r1 = dm(258,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[356]
//lo[358] = lo[260] ^ lo[353] ;
    r0 = dm(260,i1);
	r1 = dm(353,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[357]
//lo[359] = lo[257] ^ lo[357] ;
    r0 = dm(257,i1);
	r1 = dm(357,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[358]
//lo[360] = lo[256] ^ lo[353] ;
    r0 = dm(256,i1);
	r1 = dm(353,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[359]
//lo[361] = lo[257] ^ lo[355] ;
    r0 = dm(257,i1);
	r1 = dm(355,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[360]
	dm(i0,m6)=r2; // lo[361]
//lo[362] = lo[259] ^ lo[361] ; **
    r0 = dm(259,i1);
	r1 = dm(361,i1);
	r2 = r0 xor r1;
	dm(i0,m6)=r2; // lo[362]
//lo[363] = lo[256] ^ lo[362] ; **
    r0 = dm(256,i1);
	r1 = dm(362,i1);
	r2 = r0 xor r1;
//lo[364] = lo[259] ^ lo[356] ;
    r0 = dm(259,i1);
	r1 = dm(356,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[363]
//lo[365] = lo[255] ^ lo[362] ;
    r0 = dm(255,i1);
	r1 = dm(362,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[364]
//lo[366] = lo[255] ^ lo[359] ;
    r0 = dm(255,i1);
	r1 = dm(359,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[365]
//lo[367] = lo[255] ^ lo[256] ;
    r0 = dm(255,i1);
	r1 = dm(256,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[366]
//lo[368] = lo[263] ^ lo[265] ;
    r0 = dm(263,i1);
	r1 = dm(265,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[367]
//lo[369] = lo[264] ^ lo[265] ;
    r0 = dm(264,i1);
	r1 = dm(265,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[368]
//lo[370] = lo[264] ^ lo[266] ;
    r0 = dm(264,i1);
	r1 = dm(266,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[369]
//lo[371] = lo[261] ^ lo[264] ;
    r0 = dm(261,i1);
	r1 = dm(264,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[370]
//lo[372] = lo[262] ^ lo[264] ;
    r0 = dm(262,i1);
	r1 = dm(264,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[371]
//lo[373] = lo[266] ^ lo[368] ;
    r0 = dm(266,i1);
	r1 = dm(368,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[372]
//lo[374] = lo[263] ^ lo[372] ;
    r0 = dm(263,i1);
	r1 = dm(372,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[373]
//lo[375] = lo[262] ^ lo[368] ;
    r0 = dm(262,i1);
	r1 = dm(368,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[374]
//lo[376] = lo[263] ^ lo[370] ;
    r0 = dm(263,i1);
	r1 = dm(370,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[375]
	dm(i0,m6)=r2; // lo[376]
//lo[377] = lo[265] ^ lo[376] ; **
    r0 = dm(265,i1);
	r1 = dm(376,i1);
	r2 = r0 xor r1;
	dm(i0,m6)=r2; // lo[377]
//lo[378] = lo[262] ^ lo[377] ; **
    r0 = dm(262,i1);
	r1 = dm(377,i1);
	r2 = r0 xor r1;
//lo[379] = lo[265] ^ lo[371] ;
    r0 = dm(265,i1);
	r1 = dm(371,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[378]
//lo[380] = lo[261] ^ lo[377] ;
    r0 = dm(261,i1);
	r1 = dm(377,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[379]
//lo[381] = lo[261] ^ lo[374] ;
    r0 = dm(261,i1);
	r1 = dm(374,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[380]
//lo[382] = lo[261] ^ lo[262] ;
    r0 = dm(261,i1);
	r1 = dm(262,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[381]
//lo[383] = lo[269] ^ lo[271] ;
    r0 = dm(269,i1);
	r1 = dm(271,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[382]
//lo[384] = lo[270] ^ lo[271] ;
    r0 = dm(270,i1);
	r1 = dm(271,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[383]
//lo[385] = lo[270] ^ lo[272] ;
    r0 = dm(270,i1);
	r1 = dm(272,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[384]
//lo[386] = lo[267] ^ lo[270] ;
    r0 = dm(267,i1);
	r1 = dm(270,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[385]
//lo[387] = lo[268] ^ lo[270] ;
    r0 = dm(268,i1);
	r1 = dm(270,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[386]
//lo[388] = lo[272] ^ lo[383] ;
    r0 = dm(272,i1);
	r1 = dm(383,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[387]
//lo[389] = lo[269] ^ lo[387] ;
    r0 = dm(269,i1);
	r1 = dm(387,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[388]
//lo[390] = lo[268] ^ lo[383] ;
    r0 = dm(268,i1);
	r1 = dm(383,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[389]
//lo[391] = lo[269] ^ lo[385] ;
    r0 = dm(269,i1);
	r1 = dm(385,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[390]
    dm(i0,m6)=r2; // lo[391]
//lo[392] = lo[271] ^ lo[391] ; **
    r0 = dm(271,i1);
	r1 = dm(391,i1);
	r2 = r0 xor r1; 
	dm(i0,m6)=r2; // lo[392]
//lo[393] = lo[268] ^ lo[392] ; **
    r0 = dm(268,i1);
	r1 = dm(392,i1);
	r2 = r0 xor r1;
//lo[394] = lo[271] ^ lo[386] ;
    r0 = dm(271,i1);
	r1 = dm(386,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[393]
//lo[395] = lo[267] ^ lo[392] ;
    r0 = dm(267,i1);
	r1 = dm(392,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[394]
//lo[396] = lo[267] ^ lo[389] ;
    r0 = dm(267,i1);
	r1 = dm(389,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[395]
//lo[397] = lo[267] ^ lo[268] ;
    r0 = dm(267,i1);
	r1 = dm(268,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[396]
//lo[398] = lo[273] ^ lo[274] ;
    r0 = dm(273,i1);
	r1 = dm(274,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[397]
//lo[399] = lo[277] ^ lo[279] ;
    r0 = dm(277,i1);
	r1 = dm(279,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[398]
//lo[400] = lo[278] ^ lo[279] ;
    r0 = dm(278,i1);
	r1 = dm(279,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[399]
//lo[401] = lo[278] ^ lo[280] ;
    r0 = dm(278,i1);
	r1 = dm(280,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[400]
//lo[402] = lo[275] ^ lo[278] ;
    r0 = dm(275,i1);
	r1 = dm(278,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[401]
//lo[403] = lo[276] ^ lo[278] ;
    r0 = dm(276,i1);
	r1 = dm(278,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[402]
//lo[404] = lo[280] ^ lo[399] ;
    r0 = dm(280,i1);
	r1 = dm(399,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[403]
//lo[405] = lo[277] ^ lo[403] ;
    r0 = dm(277,i1);
	r1 = dm(403,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[404]
//lo[406] = lo[276] ^ lo[399] ;
    r0 = dm(276,i1);
	r1 = dm(399,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[405]
//lo[407] = lo[277] ^ lo[401] ;
    r0 = dm(277,i1);
	r1 = dm(401,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[406]
	dm(i0,m6)=r2;                 // lo[407]
//lo[408] = lo[279] ^ lo[407] ; **
    r0 = dm(279,i1);
	r1 = dm(407,i1);
	r2 = r0 xor r1;
	dm(i0,m6)=r2; // lo[408]
//lo[409] = lo[276] ^ lo[408] ; **
    r0 = dm(276,i1);
    r1 = dm(408,i1);
	r2 = r0 xor r1;
//lo[410] = lo[279] ^ lo[402] ;
    r0 = dm(279,i1);
	r1 = dm(402,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[409]
//lo[411] = lo[275] ^ lo[408] ;
    r0 = dm(275,i1);
	r1 = dm(408,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[410]
//lo[412] = lo[275] ^ lo[405] ;
    r0 = dm(275,i1);
	r1 = dm(405,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[411]
//lo[413] = lo[275] ^ lo[276] ;
    r0 = dm(275,i1);
	r1 = dm(276,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[412]
//lo[414] = lo[282] ^ lo[283] ;
    r0 = dm(282,i1);
	r1 = dm(283,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[413]
//lo[415] = lo[281] ^ lo[283] ;
    r0 = dm(281,i1);
	r1 = dm(283,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[414]
//lo[416] = lo[281] ^ lo[282] ;
    r0 = dm(281,i1);
	r1 = dm(282,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[415]
//lo[417] = lo[286] ^ lo[288] ;
    r0 = dm(286,i1);
	r1 = dm(288,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[416]
//lo[418] = lo[287] ^ lo[288] ;
    r0 = dm(287,i1);
	r1 = dm(288,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[417]
//lo[419] = lo[287] ^ lo[289] ;
    r0 = dm(287,i1);
	r1 = dm(289,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[418]
//lo[420] = lo[284] ^ lo[287] ;
    r0 = dm(284,i1);
	r1 = dm(287,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[419]
//lo[421] = lo[285] ^ lo[287] ;
    r0 = dm(285,i1);
	r1 = dm(287,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[420]
//lo[422] = lo[289] ^ lo[417] ;
    r0 = dm(289,i1);
	r1 = dm(417,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[421]
//lo[423] = lo[286] ^ lo[421] ;
    r0 = dm(286,i1);
	r1 = dm(421,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[422]
//lo[424] = lo[285] ^ lo[417] ;
    r0 = dm(285,i1);
	r1 = dm(417,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[423]
//lo[425] = lo[286] ^ lo[419] ;
    r0 = dm(286,i1);
	r1 = dm(419,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[424]
	dm(i0,m6)=r2; // lo[425]
//lo[426] = lo[288] ^ lo[425] ; **
    r0 = dm(288,i1);
    r1 = dm(425,i1);
	r2 = r0 xor r1;
	dm(i0,m6)=r2; // lo[426]
//lo[427] = lo[285] ^ lo[426] ; **
    r0 = dm(285,i1);
    r1 = dm(426,i1);
	r2 = r0 xor r1;
//lo[428] = lo[288] ^ lo[420] ;
    r0 = dm(288,i1);
	r1 = dm(420,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[427]
//lo[429] = lo[284] ^ lo[426] ;
    r0 = dm(284,i1);
	r1 = dm(426,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[428]
//lo[430] = lo[284] ^ lo[423] ;
    r0 = dm(284,i1);
	r1 = dm(423,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[429]
//lo[431] = lo[284] ^ lo[285] ;
    r0 = dm(284,i1);
	r1 = dm(285,i1);
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[430]
	dm(i0,m6)=r2;                 // lo[431]


	i5 = 2*logGF64;                // address of logarithm table in dm in 16 bit addressing mode
    i3 = dm(Mo_ptr);
//m1[ 0] = pwr[ 2*GF + 19 + lgr[ lo[234]] ];
    m4 = dm(234,i1);             // m4 = lo[234]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[234]]
    r1  = 2*pwrGF64+GF64_2+19;
	r0  = r0+r1;
	i2  = r0;
	r2  = dm(i2,m4);
//m1[ 1] = pwr[ 2*GF + 42 + lgr[ lo[237]] ];
    m4 = dm(237,i1);             // m4 = lo[237]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[237]]
    r1  = 2*pwrGF64+GF64_2+42;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[0]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[ 2] = pwr[ 2*GF + 13 + lgr[ lo[298]] ];
    m4 = dm(298,i1);             // m4 = lo[298]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[298]]
    r1  = 2*pwrGF64+GF64_2+13;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[1]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[ 3] = pwr[ 2*GF + 33 + lgr[ lo[294]] ];
    m4 = dm(294,i1);             // m4 = lo[294]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[294]]
    r1  = 2*pwrGF64+GF64_2+33;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[2]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[ 4] = pwr[ 2*GF +  4 + lgr[ lo[300]] ];
    m4 = dm(300,i1);             // m4 = lo[300]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[300]]
    r1  = 2*pwrGF64+GF64_2+4;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[3]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[ 5] = pwr[ 2*GF +  8 + lgr[ lo[297]] ];
    m4 = dm(297,i1);             // m4 = lo[297]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[297]]
    r1  = 2*pwrGF64+GF64_2+8;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[4]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[ 6] = pwr[ 2*GF +  3 + lgr[ lo[296]] ];
    m4 = dm(296,i1);             // m4 = lo[296]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[296]]
    r1  = 2*pwrGF64+GF64_2+3;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[5]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[ 7] = pwr[ 2*GF + 14 + lgr[ lo[292]] ];
    m4 = dm(292,i1);             // m4 = lo[292]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[292]]
    r1  = 2*pwrGF64+GF64_2+14;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[6]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[ 8] = pwr[ 2*GF +  1 + lgr[ lo[295]] ];
    m4 = dm(295,i1);             // m4 = lo[295]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[295]]
    r1  = 2*pwrGF64+GF64_2+1;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[7]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[ 9] = pwr[ 2*GF +  2 + lgr[ lo[291]] ];
    m4 = dm(291,i1);             // m4 = lo[291]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[291]]
    r1  = 2*pwrGF64+GF64_2+2;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[8]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[10] = pwr[ 2*GF + 19 + lgr[ lo[240]] ];
    m4 = dm(240,i1);             // m4 = lo[240]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[240]]
    r1  = 2*pwrGF64+GF64_2+19;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[9]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[11] = pwr[ 2*GF + 42 + lgr[ lo[243]] ];
    m4 = dm(243,i1);             // m4 = lo[243]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[243]]
    r1  = 2*pwrGF64+GF64_2+42;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[10]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[12] = pwr[ 2*GF + 13 + lgr[ lo[313]] ];
    m4 = dm(313,i1);             // m4 = lo[313]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[313]]
    r1  = 2*pwrGF64+GF64_2+13;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[11]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[13] = pwr[ 2*GF + 33 + lgr[ lo[309]] ];
    m4 = dm(309,i1);             // m4 = lo[309]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[309]]
    r1  = 2*pwrGF64+GF64_2+33;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[12]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[14] = pwr[ 2*GF +  4 + lgr[ lo[315]] ];
    m4 = dm(315,i1);             // m4 = lo[315]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[315]]
    r1  = 2*pwrGF64+GF64_2+4;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[13]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[15] = pwr[ 2*GF +  8 + lgr[ lo[312]] ];
    m4 = dm(312,i1);             // m4 = lo[312]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[312]]
    r1  = 2*pwrGF64+GF64_2+8;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[14]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[16] = pwr[ 2*GF +  3 + lgr[ lo[311]] ];
    m4 = dm(311,i1);             // m4 = lo[311]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[311]]
    r1  = 2*pwrGF64+GF64_2+3;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[15]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[17] = pwr[ 2*GF + 14 + lgr[ lo[307]] ];
    m4 = dm(307,i1);             // m4 = lo[307]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[307]]
    r1  = 2*pwrGF64+GF64_2+14;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[16]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[18] = pwr[ 2*GF +  1 + lgr[ lo[310]] ];
    m4 = dm(310,i1);             // m4 = lo[310]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[310]]
    r1  = 2*pwrGF64+GF64_2+1;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[17]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[19] = pwr[ 2*GF +  2 + lgr[ lo[306]] ];
    m4 = dm(306,i1);             // m4 = lo[306]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[306]]
    r1  = 2*pwrGF64+GF64_2+2;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[18]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[20] = pwr[ 2*GF + 19 + lgr[ lo[246]] ];
    m4 = dm(246,i1);             // m4 = lo[246]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[246]]
    r1  = 2*pwrGF64+GF64_2+19;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[19]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[21] = pwr[ 2*GF + 42 + lgr[ lo[249]] ];
    m4 = dm(249,i1);             // m4 = lo[249]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[249]]
    r1  = 2*pwrGF64+GF64_2+42;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[20]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[22] = pwr[ 2*GF + 13 + lgr[ lo[328]] ];
    m4 = dm(328,i1);             // m4 = lo[328]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[328]]
    r1  = 2*pwrGF64+GF64_2+13;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[21]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[23] = pwr[ 2*GF + 33 + lgr[ lo[324]] ];
    m4 = dm(324,i1);             // m4 = lo[324]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[324]]
    r1  = 2*pwrGF64+GF64_2+33;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[22]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[24] = pwr[ 2*GF +  4 + lgr[ lo[330]] ];
    m4 = dm(330,i1);             // m4 = lo[330]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[330]]
    r1  = 2*pwrGF64+GF64_2+4;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[23]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[25] = pwr[ 2*GF +  8 + lgr[ lo[327]] ];
    m4 = dm(327,i1);             // m4 = lo[327]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[327]]
    r1  = 2*pwrGF64+GF64_2+8;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[24]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[26] = pwr[ 2*GF +  3 + lgr[ lo[326]] ];
    m4 = dm(326,i1);             // m4 = lo[326]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[326]]
    r1  = 2*pwrGF64+GF64_2+3;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[25]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[27] = pwr[ 2*GF + 14 + lgr[ lo[322]] ];
    m4 = dm(322,i1);             // m4 = lo[322]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[322]]
    r1  = 2*pwrGF64+GF64_2+14;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[26]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[28] = pwr[ 2*GF +  1 + lgr[ lo[325]] ];
    m4 = dm(325,i1);             // m4 = lo[325]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[325]]
    r1  = 2*pwrGF64+GF64_2+1;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[27]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[29] = pwr[ 2*GF +  2 + lgr[ lo[321]] ];
    m4 = dm(321,i1);             // m4 = lo[321]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[321]]
    r1  = 2*pwrGF64+GF64_2+2;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[28]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[30] = pwr[ 2*GF + 19 + lgr[ lo[228]] ];
    m4 = dm(228,i1);             // m4 = lo[228]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[228]]
    r1  = 2*pwrGF64+GF64_2+19;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[29]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[31] = pwr[ 2*GF + 42 + lgr[ lo[196]] ];
    m4 = dm(196,i1);             // m4 = lo[196]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[196]]
    r1  = 2*pwrGF64+GF64_2+42;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[30]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[32] = pwr[ 2*GF + 13 + lgr[ lo[343]] ];
    m4 = dm(343,i1);             // m4 = lo[343]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[343]]
    r1  = 2*pwrGF64+GF64_2+13;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[31]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[33] = pwr[ 2*GF + 33 + lgr[ lo[339]] ];
    m4 = dm(339,i1);             // m4 = lo[339]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[339]]
    r1  = 2*pwrGF64+GF64_2+33;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[32]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[34] = pwr[ 2*GF +  4 + lgr[ lo[345]] ];
    m4 = dm(345,i1);             // m4 = lo[345]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[345]]
    r1  = 2*pwrGF64+GF64_2+4;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[33]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[35] = pwr[ 2*GF +  8 + lgr[ lo[342]] ];
    m4 = dm(342,i1);             // m4 = lo[342]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[342]]
    r1  = 2*pwrGF64+GF64_2+8;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[34]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[36] = pwr[ 2*GF +  3 + lgr[ lo[341]] ];
    m4 = dm(341,i1);             // m4 = lo[341]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[341]]
    r1  = 2*pwrGF64+GF64_2+3;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[35]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[37] = pwr[ 2*GF + 14 + lgr[ lo[337]] ];
    m4 = dm(337,i1);             // m4 = lo[337]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[337]]
    r1  = 2*pwrGF64+GF64_2+14;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[36]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[38] = pwr[ 2*GF +  1 + lgr[ lo[340]] ];
    m4 = dm(340,i1);             // m4 = lo[340]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[340]]
    r1  = 2*pwrGF64+GF64_2+1;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[37]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[39] = pwr[ 2*GF +  2 + lgr[ lo[336]] ];
    m4 = dm(336,i1);             // m4 = lo[336]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[336]]
    r1  = 2*pwrGF64+GF64_2+2;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[38]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[40] = pwr[ 2*GF +  9 + lgr[ lo[350]] ];
    m4 = dm(350,i1);             // m4 = lo[350]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[350]]
    r1  = 2*pwrGF64+GF64_2+9;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[39]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[41] = pwr[ 2*GF + 54 + lgr[ lo[254]] ];
    m4 = dm(254,i1);             // m4 = lo[254]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[254]]
    r1  = 2*pwrGF64+GF64_2+54;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[40]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[42] = pwr[ 2*GF + 18 + lgr[ lo[253]] ];
    m4 = dm(253,i1);             // m4 = lo[253]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[253]]
    r1  = 2*pwrGF64+GF64_2+18;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[41]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[43] = pwr[ 2*GF + 19 + lgr[ lo[257]] ];
    m4 = dm(257,i1);             // m4 = lo[257]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[257]]
    r1  = 2*pwrGF64+GF64_2+19;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[42]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[44] = pwr[ 2*GF + 42 + lgr[ lo[260]] ];
    m4 = dm(260,i1);             // m4 = lo[260]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[260]]
    r1  = 2*pwrGF64+GF64_2+42;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[43]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[45] = pwr[ 2*GF + 13 + lgr[ lo[361]] ];
    m4 = dm(361,i1);             // m4 = lo[361]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[361]]
    r1  = 2*pwrGF64+GF64_2+13;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[44]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[46] = pwr[ 2*GF + 33 + lgr[ lo[357]] ];
    m4 = dm(357,i1);             // m4 = lo[357]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[357]]
    r1  = 2*pwrGF64+GF64_2+33;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[45]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[47] = pwr[ 2*GF +  4 + lgr[ lo[363]] ];
    m4 = dm(363,i1);             // m4 = lo[363]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[363]]
    r1  = 2*pwrGF64+GF64_2+4;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[46]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[48] = pwr[ 2*GF +  8 + lgr[ lo[360]] ];
    m4 = dm(360,i1);             // m4 = lo[360]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[360]]
    r1  = 2*pwrGF64+GF64_2+8;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[47]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[49] = pwr[ 2*GF +  3 + lgr[ lo[359]] ];
    m4 = dm(359,i1);             // m4 = lo[359]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[359]]
    r1  = 2*pwrGF64+GF64_2+3;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[48]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[50] = pwr[ 2*GF + 14 + lgr[ lo[355]] ];
    m4 = dm(355,i1);             // m4 = lo[355]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[355]]
    r1  = 2*pwrGF64+GF64_2+14;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[49]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[51] = pwr[ 2*GF +  1 + lgr[ lo[358]] ];
    m4 = dm(358,i1);             // m4 = lo[358]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[358]]
    r1  = 2*pwrGF64+GF64_2+1;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[50]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[52] = pwr[ 2*GF +  2 + lgr[ lo[354]] ];
    m4 = dm(354,i1);             // m4 = lo[354]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[354]]
    r1  = 2*pwrGF64+GF64_2+2;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[51]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[53] = pwr[ 2*GF + 19 + lgr[ lo[263]] ];
    m4 = dm(263,i1);             // m4 = lo[263]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[263]]
    r1  = 2*pwrGF64+GF64_2+19;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[52]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[54] = pwr[ 2*GF + 42 + lgr[ lo[266]] ];
    m4 = dm(266,i1);             // m4 = lo[266]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[266]]
    r1  = 2*pwrGF64+GF64_2+42;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[53]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[55] = pwr[ 2*GF + 13 + lgr[ lo[376]] ];
    m4 = dm(376,i1);             // m4 = lo[376]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[376]]
    r1  = 2*pwrGF64+GF64_2+13;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[54]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[56] = pwr[ 2*GF + 33 + lgr[ lo[372]] ];
    m4 = dm(372,i1);             // m4 = lo[372]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[372]]
    r1  = 2*pwrGF64+GF64_2+33;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[55]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[57] = pwr[ 2*GF +  4 + lgr[ lo[378]] ];
    m4 = dm(378,i1);             // m4 = lo[378]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[378]]
    r1  = 2*pwrGF64+GF64_2+4;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[56]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[58] = pwr[ 2*GF +  8 + lgr[ lo[375]] ];
    m4 = dm(375,i1);             // m4 = lo[375]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[375]]
    r1  = 2*pwrGF64+GF64_2+8;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[57]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[59] = pwr[ 2*GF +  3 + lgr[ lo[374]] ];
    m4 = dm(374,i1);             // m4 = lo[374]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[374]]
    r1  = 2*pwrGF64+GF64_2+3;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[58]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[60] = pwr[ 2*GF + 14 + lgr[ lo[370]] ];
    m4 = dm(370,i1);             // m4 = lo[370]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[370]]
    r1  = 2*pwrGF64+GF64_2+14;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[59]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[61] = pwr[ 2*GF +  1 + lgr[ lo[373]] ];
    m4 = dm(373,i1);             // m4 = lo[373]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[373]]
    r1  = 2*pwrGF64+GF64_2+1;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[60]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[62] = pwr[ 2*GF +  2 + lgr[ lo[369]] ];
    m4 = dm(369,i1);             // m4 = lo[369]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[369]]
    r1  = 2*pwrGF64+GF64_2+2;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[61]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[63] = pwr[ 2*GF + 19 + lgr[ lo[269]] ];
    m4 = dm(269,i1);             // m4 = lo[269]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[269]]
    r1  = 2*pwrGF64+GF64_2+19;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[62]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[64] = pwr[ 2*GF + 42 + lgr[ lo[272]] ];
    m4 = dm(272,i1);             // m4 = lo[272]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[272]]
    r1  = 2*pwrGF64+GF64_2+42;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[63]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[65] = pwr[ 2*GF + 13 + lgr[ lo[391]] ];
    m4 = dm(391,i1);             // m4 = lo[391]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[391]]
    r1  = 2*pwrGF64+GF64_2+13;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[64]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[66] = pwr[ 2*GF + 33 + lgr[ lo[387]] ];
    m4 = dm(387,i1);             // m4 = lo[387]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[387]]
    r1  = 2*pwrGF64+GF64_2+33;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[65]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[67] = pwr[ 2*GF +  4 + lgr[ lo[393]] ];
    m4 = dm(393,i1);             // m4 = lo[393]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[393]]
    r1  = 2*pwrGF64+GF64_2+4;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[66]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[68] = pwr[ 2*GF +  8 + lgr[ lo[390]] ];
    m4 = dm(390,i1);             // m4 = lo[390]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[390]]
    r1  = 2*pwrGF64+GF64_2+8;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[67]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[69] = pwr[ 2*GF +  3 + lgr[ lo[389]] ];
    m4 = dm(389,i1);             // m4 = lo[389]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[389]]
    r1  = 2*pwrGF64+GF64_2+3;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[68]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[70] = pwr[ 2*GF + 14 + lgr[ lo[385]] ];
    m4 = dm(385,i1);             // m4 = lo[385]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[385]]
    r1  = 2*pwrGF64+GF64_2+14;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[69]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[71] = pwr[ 2*GF +  1 + lgr[ lo[388]] ];
    m4 = dm(388,i1);             // m4 = lo[388]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[388]]
    r1  = 2*pwrGF64+GF64_2+1;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[70]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[72] = pwr[ 2*GF +  2 + lgr[ lo[384]] ];
    m4 = dm(384,i1);             // m4 = lo[384]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[384]]
    r1  = 2*pwrGF64+GF64_2+2;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[71]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[73] = pwr[ 2*GF + 21 + lgr[ lo[274]] ];
    m4 = dm(274,i1);             // m4 = lo[274]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[274]]
    r1  = 2*pwrGF64+GF64_2+21;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[72]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[74] = pwr[ 2*GF + 19 + lgr[ lo[277]] ];
    m4 = dm(277,i1);             // m4 = lo[277]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[277]]
    r1  = 2*pwrGF64+GF64_2+19;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[73]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[75] = pwr[ 2*GF + 42 + lgr[ lo[280]] ];
    m4 = dm(280,i1);             // m4 = lo[280]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[280]]
    r1  = 2*pwrGF64+GF64_2+42;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[74]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[76] = pwr[ 2*GF + 13 + lgr[ lo[407]] ];
    m4 = dm(407,i1);             // m4 = lo[407]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[407]]
    r1  = 2*pwrGF64+GF64_2+13;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[75]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[77] = pwr[ 2*GF + 33 + lgr[ lo[403]] ];
    m4 = dm(403,i1);             // m4 = lo[403]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[403]]
    r1  = 2*pwrGF64+GF64_2+33;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[76]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[78] = pwr[ 2*GF +  4 + lgr[ lo[409]] ];
    m4 = dm(409,i1);             // m4 = lo[409]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[409]]
    r1  = 2*pwrGF64+GF64_2+4;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[77]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[79] = pwr[ 2*GF +  8 + lgr[ lo[406]] ];
    m4 = dm(406,i1);             // m4 = lo[406]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[406]]
    r1  = 2*pwrGF64+GF64_2+8;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[78]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[80] = pwr[ 2*GF +  3 + lgr[ lo[405]] ];
    m4 = dm(405,i1);             // m4 = lo[405]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[405]]
    r1  = 2*pwrGF64+GF64_2+3;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[79]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[81] = pwr[ 2*GF + 14 + lgr[ lo[401]] ];
    m4 = dm(401,i1);             // m4 = lo[401]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[401]]
    r1  = 2*pwrGF64+GF64_2+14;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[80]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[82] = pwr[ 2*GF +  1 + lgr[ lo[404]] ];
    m4 = dm(404,i1);             // m4 = lo[404]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[404]]
    r1  = 2*pwrGF64+GF64_2+1;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[81]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[83] = pwr[ 2*GF +  2 + lgr[ lo[400]] ];
    m4 = dm(400,i1);             // m4 = lo[400]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[400]]
    r1  = 2*pwrGF64+GF64_2+2;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[82]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[84] = pwr[ 2*GF +  9 + lgr[ lo[414]] ];
    m4 = dm(414,i1);             // m4 = lo[414]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[414]]
    r1  = 2*pwrGF64+GF64_2+9;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[83]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[85] = pwr[ 2*GF + 54 + lgr[ lo[283]] ];
    m4 = dm(283,i1);             // m4 = lo[283]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[283]]
    r1  = 2*pwrGF64+GF64_2+54;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[84]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[86] = pwr[ 2*GF + 18 + lgr[ lo[282]] ];
    m4 = dm(282,i1);             // m4 = lo[282]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[282]]
    r1  = 2*pwrGF64+GF64_2+18;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[85]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[87] = pwr[ 2*GF + 19 + lgr[ lo[286]] ];
    m4 = dm(286,i1);             // m4 = lo[286]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[286]]
    r1  = 2*pwrGF64+GF64_2+19;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[86]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[88] = pwr[ 2*GF + 42 + lgr[ lo[289]] ];
    m4 = dm(289,i1);             // m4 = lo[289]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[289]]
    r1  = 2*pwrGF64+GF64_2+42;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[87]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[89] = pwr[ 2*GF + 13 + lgr[ lo[425]] ];
    m4 = dm(425,i1);             // m4 = lo[425]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[425]]
    r1  = 2*pwrGF64+GF64_2+13;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[88]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[90] = pwr[ 2*GF + 33 + lgr[ lo[421]] ];
    m4 = dm(421,i1);             // m4 = lo[421]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[421]]
    r1  = 2*pwrGF64+GF64_2+33;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[89]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[91] = pwr[ 2*GF +  4 + lgr[ lo[427]] ];
    m4 = dm(427,i1);             // m4 = lo[427]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[427]]
    r1  = 2*pwrGF64+GF64_2+4;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[90]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[92] = pwr[ 2*GF +  8 + lgr[ lo[424]] ];
    m4 = dm(424,i1);             // m4 = lo[424]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[424]]
    r1  = 2*pwrGF64+GF64_2+8;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[91]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[93] = pwr[ 2*GF +  3 + lgr[ lo[423]] ];
    m4 = dm(423,i1);             // m4 = lo[423]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[423]]
    r1  = 2*pwrGF64+GF64_2+3;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[92]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[94] = pwr[ 2*GF + 14 + lgr[ lo[419]] ];
    m4 = dm(419,i1);             // m4 = lo[419]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[419]]
    r1  = 2*pwrGF64+GF64_2+14;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[93]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[95] = pwr[ 2*GF +  1 + lgr[ lo[422]] ];
    m4 = dm(422,i1);             // m4 = lo[422]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[422]]
    r1  = 2*pwrGF64+GF64_2+1;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[94]
	i2  = r0;
	r2  = dm(i2,m4);
//m1[96] = pwr[ 2*GF +  2 + lgr[ lo[418]] ];
    m4 = dm(418,i1);             // m4 = lo[418]
    r0  = dm(m4,i5);            // r0 = logGF64[lo[418]]
    r1  = 2*pwrGF64+GF64_2+2;
	r0  = r0+r1, dm(i3,m6) = r2;  // store ml[95]
	i2  = r0;
	r2  = dm(i2,m4);
    dm(i3,m6) = r2;               // store ml[96]

    i3  = dm(Mo_ptr); 
//lo[432] = m1[31] ^ m1[37] ;
	r0 = dm(31,i3);               // ma[31]               
	r1 = dm(37,i3);               // ma[37]
	r2 = r0 xor r1;
//lo[433] = m1[44] ^ m1[50] ;
	r0 = dm(44,i3);               // ma[44]               
	r1 = dm(50,i3);               // ma[50]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[432]
//lo[434] = m1[21] ^ m1[27] ;
	r0 = dm(21,i3);               // ma[21]               
	r1 = dm(27,i3);               // ma[27]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[433]
//lo[435] = m1[64] ^ m1[65] ;
	r0 = dm(64,i3);               // ma[64]               
	r1 = dm(65,i3);               // ma[65]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[434]
//lo[436] = m1[54] ^ m1[55] ;
	r0 = dm(54,i3);               // ma[54]               
	r1 = dm(55,i3);               // ma[55]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[435]
//lo[437] = m1[10] ^ m1[11] ;
	r0 = dm(10,i3);               // ma[10]               
	r1 = dm(11,i3);               // ma[11]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[436]
//lo[438] = m1[ 0] ^ m1[ 1] ;
	r0 = dm( 0,i3);               // ma[0]               
	r1 = dm( 1,i3);               // ma[1]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[437]
//lo[439] = m1[87] ^ m1[92] ;
	r0 = dm(87,i3);               // ma[87]               
	r1 = dm(92,i3);               // ma[92]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[438]
//lo[440] = m1[94] ^ m1[95] ;
	r0 = dm(94,i3);               // ma[94]               
	r1 = dm(95,i3);               // ma[95]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[439]
//lo[441] = m1[89] ^ m1[93] ;
	r0 = dm(89,i3);               // ma[89]               
	r1 = dm(93,i3);               // ma[93]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[440]
//lo[442] = m1[74] ^ m1[79] ;
	r0 = dm(74,i3);               // ma[74]               
	r1 = dm(79,i3);               // ma[79]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[441]
//lo[443] = m1[81] ^ m1[83] ;
	r0 = dm(81,i3);               // ma[81]               
	r1 = dm(83,i3);               // ma[83]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[442]
//lo[444] = m1[87] ^ m1[91] ;
	r0 = dm(87,i3);               // ma[87]               
	r1 = dm(91,i3);               // ma[91]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[443]
//lo[445] = m1[81] ^ m1[82] ;
	r0 = dm(81,i3);               // ma[81]               
	r1 = dm(82,i3);               // ma[82]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[444]
//lo[446] = m1[76] ^ m1[77] ;
	r0 = dm(76,i3);               // ma[76]               
	r1 = dm(77,i3);               // ma[77]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[445]
//lo[447] = m1[63] ^ m1[64] ^ m1[70] ;
	r0 = dm(63,i3);               // ma[63]               
	r1 = dm(64,i3);               // ma[64]
	r0 = r0 xor r1;
	r1 = dm(70,i3);               // ma[70]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[446]
//lo[448] = m1[88] ^ m1[94] ^ m1[96] ;
	r0 = dm(88,i3);               // ma[88]               
	r1 = dm(94,i3);               // ma[94]
	r0 = r0 xor r1;
	r1 = dm(96,i3);               // ma[96]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[447]
//lo[449] = m1[51] ^ lo[433] ;
	r0 = dm(51,i3);               // ma[51]               
	r1 = dm(433,i1);              // lo[433]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[448]
//lo[450] = m1[75] ^ m1[76] ^ m1[80] ;
	r0 = dm(75,i3);               // ma[75]               
	r1 = dm(76,i3);               // ma[76]
	r0 = r0 xor r1;
	r1 = dm(80,i3);               // ma[80]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[449]
//lo[451] = m1[53] ^ m1[54] ^ m1[60] ;
	r0 = dm(53,i3);               // ma[53]               
	r1 = dm(54,i3);               // ma[54]
	r0 = r0 xor r1;
	r1 = dm(60,i3);               // ma[60]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[450]
//lo[452] = m1[70] ^ lo[435] ;
	r0 = dm(70,i3);               // ma[70]               
	r1 = dm(435,i1);              // lo[435]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[451]
//lo[453] = m1[39] ^ lo[432] ;
	r0 = dm(39,i3);               // ma[39]               
	r1 = dm(432,i1);              // lo[432]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[452]
//lo[454] = m1[29] ^ lo[434] ;
	r0 = dm(29,i3);               // ma[29]               
	r1 = dm(434,i1);              // lo[434]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[453]
//lo[455] = m1[11] ^ m1[12] ^ m1[17] ;
	r0 = dm(11,i3);               // ma[11]               
	r1 = dm(12,i3);               // ma[12]
	r0 = r0 xor r1;
	r1 = dm(17,i3);               // ma[17]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[454]
//lo[456] = m1[88] ^ m1[89] ^ m1[90] ;
	r0 = dm(88,i3);               // ma[88]               
	r1 = dm(89,i3);               // ma[89]
	r0 = r0 xor r1;
	r1 = dm(90,i3);               // ma[90]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[455]
//lo[457] = m1[38] ^ lo[432] ;
	r0 = dm(38,i3);               // ma[38]               
	r1 = dm(432,i1);              // lo[432]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[456]
//lo[458] = m1[59] ^ lo[436] ;
	r0 = dm(59,i3);               // ma[59]               
	r1 = dm(436,i1);              // lo[436]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[457]
//lo[459] = m1[52] ^ lo[433] ;
	r0 = dm(52,i3);               // ma[52]               
	r1 = dm(433,i1);              // lo[433]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[458]
//lo[460] = m1[74] ^ m1[75] ^ m1[78] ;
	r0 = dm(74,i3);               // ma[74]               
	r1 = dm(75,i3);               // ma[75]
	r0 = r0 xor r1;
	r1 = dm(78,i3);               // ma[78]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[459]
//lo[461] = m1[28] ^ lo[434] ;
	r0 = dm(28,i3);               // ma[28]               
	r1 = dm(434,i1);              // lo[434]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[460]
//lo[462] = m1[ 1] ^ m1[ 2] ^ m1[ 7] ;
	r0 = dm( 1,i3);               // ma[1]               
	r1 = dm( 2,i3);               // ma[2]
	r0 = r0 xor r1;
	r1 = dm( 7,i3);               // ma[7]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[461]
//lo[463] = m1[63] ^ lo[435] ;
	r0 = dm(63,i3);               // ma[63]               
	r1 = dm(435,i1);              // lo[435]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[462]
//lo[464] = m1[56] ^ lo[436] ;
	r0 = dm(56,i3);               // ma[56]               
	r1 = dm(436,i1);              // lo[436]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[463]
//lo[465] = m1[17] ^ lo[437] ;
	r0 = dm(17,i3);               // ma[17]               
	r1 = dm(437,i1);              // lo[437]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[464]
//lo[466] = m1[43] ^ m1[44] ^ m1[45] ;
	r0 = dm(43,i3);               // ma[43]               
	r1 = dm(44,i3);               // ma[44]
	r0 = r0 xor r1;
	r1 = dm(45,i3);               // ma[45]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[465]
//lo[467] = m1[30] ^ m1[31] ^ m1[32] ;
	r0 = dm(30,i3);               // ma[30]               
	r1 = dm(31,i3);               // ma[31]
	r0 = r0 xor r1;
	r1 = dm(32,i3);               // ma[32]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[466]
//lo[468] = m1[20] ^ m1[21] ^ m1[22] ;
	r0 = dm(20,i3);               // ma[20]               
	r1 = dm(21,i3);               // ma[21]
	r0 = r0 xor r1;
	r1 = dm(22,i3);               // ma[22]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[467]
//lo[469] = m1[ 7] ^ lo[438] ;
	r0 = dm( 7,i3);               // ma[ 7]               
	r1 = dm(438,i1);              // lo[438]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[468]
//lo[470] = m1[12] ^ lo[437] ;
	r0 = dm(12,i3);               // ma[12]               
	r1 = dm(437,i1);              // lo[437]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[469]
//lo[471] = m1[ 2] ^ lo[438] ;
	r0 = dm( 2,i3);               // ma[ 2]               
	r1 = dm(438,i1);              // lo[438]
	r2 = r0 xor r1, dm(i0,m6)=r2; // lo[470]
    dm(i0,m6)=r2;                 // lo[471]

	i0 = r8;
//A1[ 0] = a[ 0] ^ a[41] ^ lo[118] ^ lo[119] ^ lo[139] ^ lo[190] ^ lo[231] ; **
    r0 = dm( 0,i4);
	r1 = dm(41,i4);                              
	r0 = r0 xor r1;
	r1 = dm(118,i1);               // lo[118]               
	r0 = r0 xor r1;
	r1 = dm(119,i1);               // lo[119]               
	r0 = r0 xor r1;
	r1 = dm(139,i1);               // lo[139]
	r0 = r0 xor r1;
	r1 = dm(190,i1);               // lo[190]
	r0 = r0 xor r1;
	r1 = dm(231,i1);               // lo[231]
	r2 = r0 xor r1;
//A1[ 1] = m1[ 3] ^ m1[ 4] ^ lo[232] ^ lo[471] ;
    r0 = dm( 3,i3);                // ma[ 3]
	r1 = dm( 4,i3);                // ma[ 4]               
	r0 = r0 xor r1;
	r1 = dm(232,i1);               // lo[232]               
	r0 = r0 xor r1;
	r1 = dm(471,i1);               // lo[471]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[0]
//A1[ 2] = m1[ 5] ^ m1[ 6] ^ lo[293] ^ lo[471] ;
    r0 = dm( 5,i3);                // ma[ 5]
	r1 = dm( 6,i3);                // ma[ 6]               
	r0 = r0 xor r1;
	r1 = dm(293,i1);               // lo[293]               
	r0 = r0 xor r1;
	r1 = dm(471,i1);               // lo[471]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[1]
//A1[ 3] = m1[13] ^ m1[14] ^ lo[238] ^ lo[470] ;
    r0 = dm(13,i3);                // ma[13]
	r1 = dm(14,i3);                // ma[14]               
	r0 = r0 xor r1;
	r1 = dm(238,i1);               // lo[238]               
	r0 = r0 xor r1;
	r1 = dm(470,i1);               // lo[470]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[2]
//A1[ 4] = m1[ 4] ^ m1[ 8] ^ lo[301] ^ lo[469] ;
    r0 = dm( 4,i3);                // ma[ 4]
	r1 = dm( 8,i3);                // ma[ 8]               
	r0 = r0 xor r1;
	r1 = dm(301,i1);               // lo[301]               
	r0 = r0 xor r1;
	r1 = dm(469,i1);               // lo[469]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[3]
//A1[ 5] = m1[23] ^ m1[24] ^ lo[244] ^ lo[468] ;
    r0 = dm(23,i3);                // ma[23]
	r1 = dm(24,i3);                // ma[24]               
	r0 = r0 xor r1;
	r1 = dm(244,i1);               // lo[244]               
	r0 = r0 xor r1;
	r1 = dm(468,i1);               // lo[468]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[4]
//A1[ 6] = m1[15] ^ m1[16] ^ lo[308] ^ lo[470] ;
    r0 = dm(15,i3);                // ma[15]
	r1 = dm(16,i3);                // ma[16]               
	r0 = r0 xor r1;
	r1 = dm(308,i1);               // lo[308]               
	r0 = r0 xor r1;
	r1 = dm(470,i1);               // lo[470]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[5]
//A1[ 7] = m1[33] ^ m1[34] ^ lo[229] ^ lo[467] ;
    r0 = dm(33,i3);                // ma[33]
	r1 = dm(34,i3);                // ma[34]               
	r0 = r0 xor r1;
	r1 = dm(229,i1);               // lo[229]               
	r0 = r0 xor r1;
	r1 = dm(467,i1);               // lo[467]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[6]
//A1[ 8] = m1[ 5] ^ m1[ 9] ^ lo[302] ^ lo[469] ;
    r0 = dm( 5,i3);                // ma[ 5]
	r1 = dm( 9,i3);                // ma[ 9]               
	r0 = r0 xor r1;
	r1 = dm(302,i1);               // lo[302]               
	r0 = r0 xor r1;
	r1 = dm(469,i1);               // lo[469]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[7]
//A1[ 9] = m1[40] ^ m1[41] ^ lo[252] ;
    r0 = dm(40,i3);                // ma[40]
	r1 = dm(41,i3);                // ma[41]               
	r0 = r0 xor r1;
	r1 = dm(252,i1);               // lo[252]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[8]
//A1[10] = m1[25] ^ m1[26] ^ lo[323] ^ lo[468] ;
    r0 = dm(25,i3);                // ma[25]
	r1 = dm(26,i3);                // ma[26]               
	r0 = r0 xor r1;
	r1 = dm(323,i1);               // lo[323]               
	r0 = r0 xor r1;
	r1 = dm(468,i1);               // lo[468]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[9]
//A1[11] = m1[46] ^ m1[47] ^ lo[255] ^ lo[466] ;
    r0 = dm(46,i3);                // ma[46]
	r1 = dm(47,i3);                // ma[47]               
	r0 = r0 xor r1;
	r1 = dm(255,i1);               // lo[255]               
	r0 = r0 xor r1;
	r1 = dm(466,i1);               // lo[466]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[10]
//A1[12] = m1[14] ^ m1[18] ^ lo[316] ^ lo[465] ;
    r0 = dm(14,i3);                // ma[14]
	r1 = dm(18,i3);                // ma[18]               
	r0 = r0 xor r1;
	r1 = dm(316,i1);               // lo[316]               
	r0 = r0 xor r1;
	r1 = dm(465,i1);               // lo[465]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[11]
//A1[13] = m1[53] ^ m1[57] ^ lo[261] ^ lo[464] ;
    r0 = dm(53,i3);                // ma[53]
	r1 = dm(57,i3);                // ma[57]               
	r0 = r0 xor r1;
	r1 = dm(261,i1);               // lo[261]               
	r0 = r0 xor r1;
	r1 = dm(464,i1);               // lo[464]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[12]
//A1[14] = m1[35] ^ m1[36] ^ lo[338] ^ lo[467] ;
    r0 = dm(35,i3);                // ma[35]
	r1 = dm(36,i3);                // ma[36]               
	r0 = r0 xor r1;
	r1 = dm(338,i1);               // lo[338]               
	r0 = r0 xor r1;
	r1 = dm(467,i1);               // lo[467]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[13]
//A1[15] = m1[66] ^ m1[67] ^ lo[267] ^ lo[463] ;
    r0 = dm(66,i3);                // ma[66]
	r1 = dm(67,i3);                // ma[67]               
	r0 = r0 xor r1;
	r1 = dm(267,i1);               // lo[267]               
	r0 = r0 xor r1;
	r1 = dm(463,i1);               // lo[463]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[14]
//A1[16] = m1[ 3] ^ m1[ 8] ^ lo[303] ^ lo[462] ;
    r0 = dm( 3,i3);                // ma[ 3]
	r1 = dm( 8,i3);                // ma[ 8]               
	r0 = r0 xor r1;
	r1 = dm(303,i1);               // lo[303]               
	r0 = r0 xor r1;
	r1 = dm(462,i1);               // lo[462]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[15]
//A1[17] = m1[22] ^ m1[23] ^ lo[333] ^ lo[461] ;
    r0 = dm(22,i3);                // ma[22]
	r1 = dm(23,i3);                // ma[23]               
	r0 = r0 xor r1;
	r1 = dm(333,i1);               // lo[333]               
	r0 = r0 xor r1;
	r1 = dm(461,i1);               // lo[461]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[16]
//A1[18] = m1[41] ^ m1[42] ^ lo[351] ;
    r0 = dm(41,i3);                // ma[41]
	r1 = dm(42,i3);                // ma[42]               
	r0 = r0 xor r1;
	r1 = dm(351,i1);               // lo[351]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[17]
//A1[19] = m1[60] ^ m1[61] ^ lo[381] ^ lo[464] ;
    r0 = dm(60,i3);                // ma[60]
	r1 = dm(61,i3);                // ma[61]               
	r0 = r0 xor r1;
	r1 = dm(381,i1);               // lo[381]               
	r0 = r0 xor r1;
	r1 = dm(464,i1);               // lo[464]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[18]
//A1[20] = m1[20] ^ m1[24] ^ lo[331] ^ lo[461] ;
    r0 = dm(20,i3);                // ma[20]
	r1 = dm(24,i3);                // ma[24]               
	r0 = r0 xor r1;
	r1 = dm(331,i1);               // lo[331]               
	r0 = r0 xor r1;
	r1 = dm(461,i1);               // lo[461]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[19]
//A1[21] = m1[73] ^ lo[273] ;
    r0 = dm(73,i3);                // ma[73]
	r1 = dm(273,i1);               // lo[273]               
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[20]
//A1[22] = m1[48] ^ m1[49] ^ lo[356] ^ lo[466] ;
    r0 = dm(48,i3);                // ma[48]
	r1 = dm(49,i3);                // ma[49]               
	r0 = r0 xor r1;
	r1 = dm(356,i1);               // lo[356]               
	r0 = r0 xor r1;
	r1 = dm(466,i1);               // lo[466]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[21]
//A1[23] = lo[275] ^ lo[446] ^ lo[460] ;
    r0 = dm(275,i1);               // lo[275]
	r1 = dm(446,i1);               // lo[446]               
	r0 = r0 xor r1;
	r1 = dm(460,i1);               // lo[460]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[22]
//A1[24] = m1[15] ^ m1[19] ^ lo[317] ^ lo[465] ;
    r0 = dm(15,i3);                // ma[15]
	r1 = dm(19,i3);                // ma[19]               
	r0 = r0 xor r1;
	r1 = dm(317,i1);               // lo[317]               
	r0 = r0 xor r1;
	r1 = dm(465,i1);               // lo[465]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[23]
//A1[25] = m1[43] ^ m1[48] ^ lo[365] ^ lo[459] ;
    r0 = dm(43,i3);                // ma[43]
	r1 = dm(48,i3);                // ma[48]               
	r0 = r0 xor r1;
	r1 = dm(365,i1);               // lo[365]               
	r0 = r0 xor r1;
	r1 = dm(459,i1);               // lo[459]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[24]
//A1[26] = m1[53] ^ m1[58] ^ lo[371] ^ lo[458] ;
    r0 = dm(53,i3);                // ma[53]
	r1 = dm(58,i3);                // ma[58]               
	r0 = r0 xor r1;
	r1 = dm(371,i1);               // lo[371]               
	r0 = r0 xor r1;
	r1 = dm(458,i1);               // lo[458]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[25]
//A1[27] = m1[84] ^ m1[85] ^ lo[281] ;
    r0 = dm(84,i3);                // ma[84]
	r1 = dm(85,i3);                // ma[85]               
	r0 = r0 xor r1;
	r1 = dm(281,i1);               // lo[281]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[26]
//A1[28] = m1[30] ^ m1[34] ^ lo[346] ^ lo[457] ;
    r0 = dm(30,i3);                // ma[30]
	r1 = dm(34,i3);                // ma[34]               
	r0 = r0 xor r1;
	r1 = dm(346,i1);               // lo[346]               
	r0 = r0 xor r1;
	r1 = dm(457,i1);               // lo[457]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[27]
//A1[29] = lo[410] ^ lo[445] ^ lo[460] ;
    r0 = dm(410,i1);               // lo[410]
	r1 = dm(445,i1);               // lo[445]               
	r0 = r0 xor r1;
	r1 = dm(460,i1);               // lo[460]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[28]
//A1[30] = m1[68] ^ m1[69] ^ lo[386] ^ lo[463] ;
    r0 = dm(68,i3);                // m1[68]
	r1 = dm(69,i3);                // m1[69]               
	r0 = r0 xor r1;
	r1 = dm(386,i1);               // lo[386]               
	r0 = r0 xor r1;
	r1 = dm(463,i1);               // lo[463]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[29]
//A1[31] = lo[284] ^ lo[444] ^ lo[456] ;
    r0 = dm(284,i1);               // lo[284]
	r1 = dm(444,i1);               // lo[444]               
	r0 = r0 xor r1;
	r1 = dm(456,i1);               // lo[456]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[30]
//A1[32] = m1[ 6] ^ m1[ 9] ^ lo[304] ^ lo[462] ;
    r0 = dm( 6,i3);                // m1[ 6]
	r1 = dm( 9,i3);                // m1[ 9]               
	r0 = r0 xor r1;
	r1 = dm(304,i1);               // lo[304]               
	r0 = r0 xor r1;
	r1 = dm(462,i1);               // lo[462]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[31]
//A1[33] = m1[16] ^ m1[19] ^ lo[319] ^ lo[455] ;
    r0 = dm(16,i3);                // m1[16]
	r1 = dm(19,i3);                // m1[19]               
	r0 = r0 xor r1;
	r1 = dm(319,i1);               // lo[319]               
	r0 = r0 xor r1;
	r1 = dm(455,i1);               // lo[455]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[32]
//A1[34] = m1[22] ^ m1[26] ^ lo[334] ^ lo[454] ;
    r0 = dm(22,i3);                // m1[22]
	r1 = dm(26,i3);                // m1[26]               
	r0 = r0 xor r1;
	r1 = dm(334,i1);               // lo[334]               
	r0 = r0 xor r1;
	r1 = dm(454,i1);               // lo[454]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[33]
//A1[35] = m1[32] ^ m1[36] ^ lo[349] ^ lo[453] ;
    r0 = dm(32,i3);                // m1[32]
	r1 = dm(36,i3);                // m1[36]               
	r0 = r0 xor r1;
	r1 = dm(349,i1);               // lo[349]               
	r0 = r0 xor r1;
	r1 = dm(453,i1);               // lo[453]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[34]
//A1[36] = m1[40] ^ m1[42] ^ lo[352] ;
    r0 = dm(40,i3);                // m1[40]
	r1 = dm(42,i3);                // m1[42]               
	r0 = r0 xor r1;
	r1 = dm(352,i1);               // lo[352]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[35]
//A1[37] = m1[45] ^ m1[49] ^ lo[367] ^ lo[459] ;
    r0 = dm(45,i3);                // m1[45]
	r1 = dm(49,i3);                // m1[49]               
	r0 = r0 xor r1;
	r1 = dm(367,i1);               // lo[367]               
	r0 = r0 xor r1;
	r1 = dm(459,i1);               // lo[459]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[36]
//A1[38] = m1[60] ^ m1[62] ^ lo[382] ^ lo[458] ;
    r0 = dm(60,i3);                // m1[60]
	r1 = dm(62,i3);                // m1[62]               
	r0 = r0 xor r1;
	r1 = dm(382,i1);               // lo[382]               
	r0 = r0 xor r1;
	r1 = dm(458,i1);               // lo[458]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[37]
//A1[39] = m1[69] ^ m1[72] ^ lo[397] ^ lo[452] ;
    r0 = dm(69,i3);                // m1[69]
	r1 = dm(72,i3);                // m1[72]               
	r0 = r0 xor r1;
	r1 = dm(397,i1);               // lo[397]               
	r0 = r0 xor r1;
	r1 = dm(452,i1);               // lo[452]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[38]
//A1[40] = m1[20] ^ m1[25] ^ lo[332] ^ lo[454] ;
    r0 = dm(20,i3);                // m1[20]
	r1 = dm(25,i3);                // m1[25]               
	r0 = r0 xor r1;
	r1 = dm(332,i1);               // lo[332]               
	r0 = r0 xor r1;
	r1 = dm(454,i1);               // lo[454]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[39]
//A1[41] = m1[58] ^ m1[62] ^ lo[380] ^ lo[451] ;
    r0 = dm(58,i3);                // m1[58]
	r1 = dm(62,i3);                // m1[62]               
	r0 = r0 xor r1;
	r1 = dm(380,i1);               // lo[380]               
	r0 = r0 xor r1;
	r1 = dm(451,i1);               // lo[451]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[40]
//A1[42] = m1[73] ^ lo[398] ;
    r0 = dm(73,i3);                // m1[73]
	r1 = dm(398,i1);               // lo[398]               
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[41]
//A1[43] = lo[413] ^ lo[443] ^ lo[450] ;
    r0 = dm(413,i1);               // lo[413]
	r1 = dm(443,i1);               // lo[443]               
	r0 = r0 xor r1;
	r1 = dm(450,i1);               // lo[450]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[42]
//A1[44] = m1[43] ^ m1[47] ^ lo[364] ^ lo[449] ;
    r0 = dm(43,i3);                // m1[43]
	r1 = dm(47,i3);                // m1[47]               
	r0 = r0 xor r1;
	r1 = dm(364,i1);               // lo[364]               
	r0 = r0 xor r1;
	r1 = dm(449,i1);               // lo[449]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[43]
//A1[45] = m1[84] ^ m1[86] ^ lo[416] ;
    r0 = dm(84,i3);                // m1[84]
	r1 = dm(86,i3);                // m1[86]               
	r0 = r0 xor r1;
	r1 = dm(416,i1);               // lo[416]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[44]
//A1[46] = lo[402] ^ lo[442] ^ lo[450] ;
    r0 = dm(402,i1);               // lo[402]
	r1 = dm(442,i1);               // lo[442]               
	r0 = r0 xor r1;
	r1 = dm(450,i1);               // lo[450]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[45]
//A1[47] = lo[431] ^ lo[441] ^ lo[448] ;
    r0 = dm(431,i1);               // lo[431]
	r1 = dm(441,i1);               // lo[441]               
	r0 = r0 xor r1;
	r1 = dm(448,i1);               // lo[448]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[46]
//A1[48] = m1[13] ^ m1[18] ^ lo[318] ^ lo[455] ;
    r0 = dm(13,i3);                // m1[13]
	r1 = dm(18,i3);                // m1[18]               
	r0 = r0 xor r1;
	r1 = dm(318,i1);               // lo[318]               
	r0 = r0 xor r1;
	r1 = dm(455,i1);               // lo[455]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[47]
//A1[49] = m1[32] ^ m1[33] ^ lo[348] ^ lo[457] ;
    r0 = dm(32,i3);                // m1[32]
	r1 = dm(33,i3);                // m1[33]               
	r0 = r0 xor r1;
	r1 = dm(348,i1);               // lo[348]               
	r0 = r0 xor r1;
	r1 = dm(457,i1);               // lo[457]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[48]
//A1[50] = m1[45] ^ m1[46] ^ lo[366] ^ lo[449] ;
    r0 = dm(45,i3);                // m1[45]
	r1 = dm(46,i3);                // m1[46]               
	r0 = r0 xor r1;
	r1 = dm(366,i1);               // lo[366]               
	r0 = r0 xor r1;
	r1 = dm(449,i1);               // lo[449]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[49]
//A1[51] = m1[66] ^ m1[71] ^ lo[396] ^ lo[452] ;
    r0 = dm(66,i3);                // m1[66]
	r1 = dm(71,i3);                // m1[71]               
	r0 = r0 xor r1;
	r1 = dm(396,i1);               // lo[396]               
	r0 = r0 xor r1;
	r1 = dm(452,i1);               // lo[452]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[50]
//A1[52] = m1[57] ^ m1[61] ^ lo[379] ^ lo[451] ;
    r0 = dm(57,i3);                // m1[57]
	r1 = dm(61,i3);                // m1[61]               
	r0 = r0 xor r1;
	r1 = dm(379,i1);               // lo[379]               
	r0 = r0 xor r1;
	r1 = dm(451,i1);               // lo[451]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[51]
//A1[53] = m1[75] ^ lo[412] ^ lo[445] ^ lo[446] ;
    r0 = dm(75,i3);                // m1[75]
	r1 = dm(412,i1);                // lo[412]               
	r0 = r0 xor r1;
	r1 = dm(445,i1);               // lo[445]               
	r0 = r0 xor r1;
	r1 = dm(446,i1);               // lo[446]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[52]
//A1[54] = m1[85] ^ m1[86] ^ lo[415] ;
    r0 = dm(85,i3);               // m1[85]
	r1 = dm(86,i3);               // m1[86]               
	r0 = r0 xor r1;
	r1 = dm(415,i1);               // lo[415]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[53]
//A1[55] = lo[430] ^ lo[440] ^ lo[456] ;
    r0 = dm(430,i1);               // lo[430]
	r1 = dm(440,i1);               // lo[440]               
	r0 = r0 xor r1;
	r1 = dm(456,i1);               // lo[456]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[54]
//A1[56] = m1[30] ^ m1[35] ^ lo[347] ^ lo[453] ;
    r0 = dm(30,i3);                // m1[30]
	r1 = dm(35,i3);                // m1[35]               
	r0 = r0 xor r1;
	r1 = dm(347,i1);               // lo[347]               
	r0 = r0 xor r1;
	r1 = dm(453,i1);               // lo[453]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[55]
//A1[57] = m1[68] ^ m1[72] ^ lo[395] ^ lo[447] ;
    r0 = dm(68,i3);                // m1[68]
	r1 = dm(72,i3);                // m1[72]               
	r0 = r0 xor r1;
	r1 = dm(395,i1);               // lo[395]               
	r0 = r0 xor r1;
	r1 = dm(447,i1);               // lo[447]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[56]
//A1[58] = m1[75] ^ lo[411] ^ lo[442] ^ lo[443] ;
    r0 = dm(75,i3);                // m1[75]
	r1 = dm(411,i1);                // lo[411]               
	r0 = r0 xor r1;
	r1 = dm(442,i1);               // lo[442]               
	r0 = r0 xor r1;
	r1 = dm(443,i1);               // lo[443]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[57]
//A1[59] = lo[429] ^ lo[439] ^ lo[448] ;
    r0 = dm(429,i1);               // lo[429]
	r1 = dm(439,i1);               // lo[439]               
	r0 = r0 xor r1;
	r1 = dm(448,i1);               // lo[448]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[58]
//A1[60] = m1[67] ^ m1[71] ^ lo[394] ^ lo[447] ;
    r0 = dm(67,i3);                // m1[67]
	r1 = dm(71,i3);                // m1[71]               
	r0 = r0 xor r1;
	r1 = dm(394,i1);               // lo[394]               
	r0 = r0 xor r1;
	r1 = dm(447,i1);               // lo[447]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[59]
//A1[61] = m1[88] ^ lo[428] ^ lo[440] ^ lo[444] ;
    r0 = dm(88,i3);                // m1[88]
	r1 = dm(428,i1);                // lo[428]               
	r0 = r0 xor r1;
	r1 = dm(440,i1);               // lo[440]               
	r0 = r0 xor r1;
	r1 = dm(444,i1);               // lo[444]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[60]
//A1[62] = m1[88] ^ lo[420] ^ lo[439] ^ lo[441] ;
    r0 = dm(88,i3);                // m1[88]
	r1 = dm(420,i1);                // lo[420]               
	r0 = r0 xor r1;
	r1 = dm(439,i1);               // lo[439]               
	r0 = r0 xor r1;
	r1 = dm(441,i1);               // lo[441]
	r2 = r0 xor r1, dm(i0,m6)=r2;  // A[61]
    dm(i0,m6)=r2;                  // A[62]

//	bit set mode1 IRPTEN;

epil:
    i0  = gets( 5 );
    i1  = gets( 4 );
    i2  = gets( 3 );
    i3  = gets( 2 );
    i5  = gets( 1 );

    leaf_exit; 
    
_IFFT63.end:

.endseg;	

