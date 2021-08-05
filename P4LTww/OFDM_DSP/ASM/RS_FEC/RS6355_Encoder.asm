/******************************************************************************
 *
 * PROJECT     : ETL600
 * FILE        : $Workfile: RS6355_Encoder.asm $
 * COPYRIGHT   : 2004 by TL Consulting GmbH
 * DESCRIPTION : Reed-Solomon (63,55) encoder
 * OPERATION   : 
 * PROTOTYPE   : RS6355_Encoder(int X[], int logX[]) ;    // code word bit vector of length 63 elements
 *                                                          
 * ASSUMPTIONS :
 *               logX[64] - temporary array of length 64 words
 *               Number of symbols is equal to 55 starting with X[0]
 *               On output check symbols will occupy X[55..62] 
 *               All arrays must start on even address boundaries
 *
 *
 * Used  registers:     m6=1
 * Saved registers:     r0, r1, r2, i0, i1, i2, i4, m1, m2, m4, mode1
 * Destroyed registers: r4
 *
 * REVISION HISTORY:
 * 1     08.09.04 vgp
 *     16-bit addressing support
 * 2     12.09.04  vgp
 *     16-bit addressing mode for logarithmic and power tables
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/RS_FEC/RS6355_Encoder.asm $
 * 
 * 1     19.01.05 15:37 Chchlee
 * 
 * 4     30.09.04 8:27 Leeb02
 * 
 * 
 * 1     10.01.04 vgp
 * Initial version Complexity 2134 cycles
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
.segment/dm	seg_dmda;

.global pwrGF64;
.global logGF64;

.align 2;
.var logGF64[GF64/2] =
        0x0000FFC1, 0x00060001, 0x000C0002, 0x001A0007,
        0x00200003, 0x0023000D, 0x00300008, 0x0012001B,
		0x00180004,	0x00100021, 0x0034000E, 0x00360024, 
		0x002D0009, 0x00260031, 0x0029001C, 0x00380013, 
		0x003E0005, 0x000B0019, 0x001F0022, 0x002F0011, 
        0x0017000F, 0x00330035, 0x002C0025, 0x00280037, 
		0x003D000A, 0x001E002E, 0x00160032, 0x002B0027, 
		0x003C001D, 0x0015002A, 0x003B0014, 0x003A0039; 

.align 2;
.var  pwrGF64[4*GF64/2] =
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 		  
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 		  
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 		  
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 		  
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 		  
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 		  
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 		  
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 		  
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 		  
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 		  
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 		  
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 		  
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 		  
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 		  
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 		  
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 		  
        0x00020001, 0x00080004, 0x00200010, 0x00060003, 
	    0x0018000C, 0x00230030, 0x000A0005, 0x00280014, 
	    0x00260013, 0x001E000F, 0x003B003C, 0x00290035,
	    0x00220011, 0x000E0007, 0x0038001C, 0x00250033, 
	    0x00120009, 0x000B0024, 0x002C0016, 0x0036001B, 
        0x001D002F, 0x0037003A, 0x0019002D, 0x00270032, 
	    0x001A000D, 0x002B0034, 0x002A0015, 0x002E0017, 
	    0x003E001F, 0x003D003F, 0x00310039, 0x00010021, 
	    0x00040002, 0x00100008, 0x00030020, 0x000C0006, 
	    0x00300018, 0x00050023, 0x0014000A, 0x00130028, 
	    0x000F0026, 0x003C001E, 0x0035003B, 0x00110029, 
	    0x00070022, 0x001C000E, 0x00330038, 0x00090025, 
	    0x00240012, 0x0016000B, 0x001B002C, 0x002F0036,
	    0x003A001D, 0x002D0037, 0x00320019, 0x000D0027, 
	    0x0034001A, 0x0015002B, 0x0017002A, 0x001F002E,
	    0x003F003E, 0x0039003D, 0x00210031, 0x00000000; 

.align 2;
.var logG6355[RS6355_K*RS6355_R/2] = 
        0x0000001c, 0x00230008, 0x001c0006, 0x002a0039,
        0x00110007, 0x00090016, 0x000c0007, 0x000c000d,
        0x00060028, 0x00210031, 0x00170036, 0x00290007,
        0x002e0006, 0x0004002d, 0x000e0016, 0x002a0019,
        0x003c0007, 0x00300007, 0x001e0029, 0x002d0001,
        0x0038000a, 0x00050010, 0x002c0011, 0x0010000c,
        0x000b002c, 0x001d001b, 0x00230034, 0x002a0029,
        0x00360007, 0x00310036, 0x00100016, 0x00110029,
        0x0005002d, 0x00010016, 0x0025001e, 0x0005000a,
        0x003c0021, 0x00310035, 0x003e003e, 0x00360030,
        0x000c0013, 0x002c0009, 0x003a000b, 0x00380025,
        0x00000015, 0x0002001a, 0x00090008, 0x002f0023,
        0x001c000c, 0x002d0028, 0x00200037, 0x0008000c,
        0x00200024, 0x00090012, 0x001d0030, 0x003d0030,
        0x002f001a, 0x0029000d, 0x000d0003, 0x00190024,
        0x001f0035, 0x001e0016, 0x0019001d, 0x0007000e,
        0x00250023, 0x00120030, 0x001e003c, 0x001b0005,
        0x000c0037, 0x0025002f, 0x00360029, 0x000b0003,
        0x00300027, 0x00340026, 0x0033000d, 0x0019002b,
        0x000e0035, 0x00190038, 0x0005000a, 0x002f0016,
        0x0030000c, 0x0000002a, 0x00160003, 0x002e003b,
        0x002b000b, 0x00160031, 0x0033000e, 0x00380031,
        0x00140015, 0x00070016, 0x0028000e, 0x00180038,
        0x00350034, 0x00030016, 0x00000016, 0x00360005,
        0x00010013, 0x002e0023, 0x0033003d, 0x002e0008,
        0x0025000b, 0x00020034, 0x0021002f, 0x00370002,
        0x00300014, 0x0026002c, 0x00260016, 0x00050003,
        0x001c0021, 0x0001001a, 0x002f001d, 0x0028002a,
        0x003b0005, 0x00010018, 0x0009000a, 0x00220006,
        0x0002003e, 0x0021001a, 0x0018002c, 0x00200002,
        0x000f003c, 0x00360033, 0x000e0020, 0x002f0024,
        0x0013000c, 0x00160007, 0x0008003b, 0x00180020,
        0x000e0034, 0x00150036, 0x000f0007, 0x00000006,
        0x000e001c, 0x001c0009, 0x0031001d, 0x003c0024,
        0x001f0019, 0x00180032, 0x0031000e, 0x00050030,
        0x00010021, 0x00260028, 0x0007002e, 0x00350015,
        0x001d0012, 0x0030001e, 0x003b0011, 0x002e003e,
        0x003b000b, 0x00140028, 0x000c0009, 0x00060021,
        0x00050022, 0x002e0017, 0x0014003c, 0x00380002,
        0x00150015, 0x00160019, 0x00010010, 0x00120003,
        0x0032002e, 0x00030014, 0x003e0022, 0x003d001a,
        0x0006001a, 0x0037002b, 0x000b0009, 0x000f0012,
        0x0028002b, 0x00060035, 0x00280034, 0x003d0015,
        0x0007001a, 0x001d0025, 0x00210010, 0x000e0000,
        0x0010002a, 0x0027001e, 0x00170002, 0x00130013,
        0x0022002f, 0x00220029, 0x000b000e, 0x0028000b,
        0x00030005, 0x00090017, 0x00320024, 0x003b001a,
        0x00290018, 0x00080009, 0x001a001c, 0x001c0013,
        0x00300038, 0x002d0023, 0x0006000f, 0x0009002e,
        0x001a0025, 0x00110033, 0x0002003d, 0x002d0023,
        0x0016000a, 0x0030002c, 0x00000030, 0x0031002e,
        0x0035000e, 0x00240023, 0x002d000b, 0x00370027,
        0x002a0014, 0x000c0033, 0x0038002f, 0x00210006,
        0x0037003d, 0x0023002f, 0x0024001e, 0x00070018,
        0x002b0023, 0x00290007, 0x001d0000, 0x0023000e;
.endseg;

//==============================================

.segment/pm	seg_pmda;

.endseg;

//==============================================
//		.segment/pm   seg_pmco;
		.segment/pm   seg_ext_pmco;

        .global _RS6355_Encoder;

// m5, m13 must be 0
// m6, m14 must be 1
// m7, m15 must be -1

_RS6355_Encoder:

    leaf_entry;

    puts = r0;
    puts = r1;
    puts = r2;
    r0 = i0; puts = r0;
    r0 = i1; puts = r0;
    r0 = i2; puts = r0;
	r0 = i4; puts = r0;
    r0 = m1; puts = r0;
    r0 = m2; puts = r0;
    r0 = m4; puts = r0;
    r0 = mode1; puts = r0;
    
    bit set mode1 SSE;    //set 16 bit addressing mode sign extend

    r1 = r4;              // r1 = r4 = address of X[] in dm
    i4 = r4;

    i0 = r8;              // address of LogX vector in dm 
    i2 = 2*logGF64;       // address of logarithm table in 16 bit addressing mode
    LCNTR = RS6355_K, do (pc,TakeLog) until lce;
          r0  = dm(i4,m6);    // m1 = X[0]; i=0
          m2 = r0;
          r0  = dm(m2,i2);    // r0 = logGF64[m2]
TakeLog:  dm(i0,m6) = r0;     // logX[i] = logGF64[X[0]]; i++

    i1 = 2*pwrGF64+GF64_2;    // address of power table in 16 bit addressing mode
	r0 = RS6355_K;
	r0 = r0 + r8;          
	i4 = r0;                // i4 = &logX[K]
    m4 = -RS6355_K;
    r0 = RS6355_K;
    r0 = r0 + r1;           // r0  = address of X[K]
    i0 = r0;
    r1 = 2*logG6355-1;      // address of generation table - 1 in 16 bit addressing mode
    m2 = RS6355_R;
    LCNTR = RS6355_R, do (pc,MulMat1) until lce;
      r1 = r1 + 1, modify(i4,m4);         // get address of column of generation table, restore address of LogX
      i2 = r1;
      r0 = r0 - r0, r2 = dm(i4,m6);
	  r4 = dm(i2,m2);
      r2 = r2 + r4;
      m1 = r2;
      LCNTR = RS6355_K-1, do (pc,MulMat) until lce;
         r2 = dm(i4,m6); 
		 r4 = dm(i2,m2);
         r2 = r2 + r4,   r4 = dm(m1,i1);  // r4 = pwr[m12]
MulMat:	 r0 = r0 xor r4, m1 = r2;
      r4 = dm(m1,i1);
      r0 = r0 xor r4;
MulMat1: dm(i0,m6) = r0;

epil:
    r0 = gets( 11 );
    r1 = gets( 10 );
    r2 = gets( 9 );
    i0 = gets( 8 );
    i1 = gets( 7 );
    i2 = gets( 6 );
    i4 = gets( 5 );
    m1 = gets( 4 );
    m2 = gets( 3 );
    m4 = gets( 2 );
    mode1 = gets (1);

    leaf_exit; 

_RS6355_Encoder.end:
    
.endseg;	
