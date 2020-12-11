/******************************************************************************
 *
 * PROJECT     : ETL600
 * FILE        : $Workfile: TCM_Gray_Encoder.asm $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : TCM Gray encoder
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * 
 * Prototype:
 *	TCM_Gray_Encoder(int d2alloc[], int len2d);  
 * 
 *----------------------------------------------------------------------------- 
 * 
 * 
 * Initial version
 *       13.09.2003 vgp
 *       i6 DAG REGISTER DOEN'T WORK WITH EXTERNAL MEMORY
 * Sign extension select while addressing in SW mode
 *       22.01.2004 vgp
 *****************************************************************************/
/* $Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/TCM/TCM_Gray_Encoder.asm $
 * 
 * 1     19.01.05 15:37 Chchlee
 * 
 * 4     30.09.04 8:56 Leeb02
 * 
 * 3     30.09.04 8:27 Leeb02
 */
  
#include <asm_sprt.h>

#include   "../../h/tcm/TCM_Defs.h"

#ifdef __ADSP21160__
#include <def21160.h>
#endif
#ifdef __ADSP21161__
#include <def21161.h>
#endif

//==============================================
	.section/pm seg_pmda;

	                          
// Naming of quartet points and there coordinates 
//                                 A            B          
.var ZCode_Mapping_Table[8] = -0.5, -0.5,   0.5,  0.5,  
                              -0.5,  0.5,   0.5, -0.5;
//                                 C            D

//  GrayCodeTable(g+1) intermediate coordinate {0,1,2,..} of the Gray center,
//  where g is packed X- or Y-Gray bits

//  gray encoder table up to 256QAM 
//.var Gray_Encode_Table[4] =    0.0,   2.0,   6.0,   4.0;

//  gray encoder table up to 16384QAM 
.var Gray_Encode_Table[64] =   0.0,   2.0,   6.0,   4.0,  14.0,  12.0,   8.0,  10.0,
                              30.0,  28.0,  24.0,  26.0,  16.0,  18.0,  22.0,  20.0,
                              62.0,  60.0,  56.0,  58.0,  48.0,  50.0,  54.0,  52.0,
                              32.0,  34.0,  38.0,  36.0,  46.0,  44.0,  40.0,  42.0,
                              64.0,  66.0,  70.0,  68.0,  78.0,  76.0,  72.0,  74.0,
                              94.0,  92.0,  88.0,  90.0,  80.0,  82.0,  86.0,  84.0,
                             126.0, 124.0, 120.0, 122.0, 112.0, 114.0, 118.0, 116.0,
				      96.0,  98.0, 102.0, 100.0, 110.0, 108.0, 104.0, 106.0;

    .endseg;
    
    
//==============================================
.SEGMENT/dm	seg_dmda;

    .extern _TX_Gray_Bits_Ptr;  // C input: pointer to bit buffer with inf. bits to be encoded by gray code
    .extern _Sig_TX_Ptr;        // C output: complex_float pointer
	
    .extern ZCode_ptr;              // Assembler input: encoded by 3/4 trellis code symbols

.ENDSEG;

//==============================================

.SEGMENT/PM		seg_pmco;

		.global _TCM_Gray_Encoder;

// m7 must be -1
// m6 must be  1

_TCM_Gray_Encoder:
    leaf_entry;

    puts = r1;
    puts = r2;
    puts = r3;
    puts = r5;
    puts = r6;
    puts = r9;
    r0 = i1; puts = r0;
    r0 = i2; puts = r0;
    r0 = i3; puts = r0;
    r0 = i4; puts = r0;
    r0 = i8; puts = r0;
    r0 = i9; puts = r0;
    r0 = m8; puts = r0;

    ustat1 = mode1;             // 22.01.04 vgp
    bit set mode1 SSE;
    
    i4 = r4;                    // i4 = r4 = d2alloc[] pointer 
    r8 = r8 - 1;                // r8 = len2d - 1

    i8 = Gray_Encode_Table;     // address of Gray table
    i3 = dm(_TX_Gray_Bits_Ptr); // address of bit buffer for Gray encoding
    i1 = dm(ZCode_ptr);         // address of trellis 3/4 encoded array
    i2 = dm(_Sig_TX_Ptr);       // pointer to the output modulated vector     
    r1 = ZCode_Mapping_Table;   // address of ZCode mapping table
    r2 = dm(i1,m6);             // r2 = ZCode[0]
    r2 = r2 + r2;               // r2 = 2*ZCode[0]
    f3 = 1.0;                   // delta, per coordinate distance between QAM points
    LCNTR = r8, do Gr_Enc_Lp until lce;   // r8 = len2d-1
           r2 = r2 + r1, r4 = dm(i4,m6);  // r4 = d2alloc[i] 
           i9 = r2;
           r4 = pass r4, r2 = dm(i1,m6), r8 = pm(i9,m14);
           if ge f8 = f8 + f3;
           r4 = pass r4, dm(i2,m6) = r8, r9 = pm(i9,m13);   // store X coordinate prom r8
           if lt jump Gr_Enc_Lp (db);          // jump if QPSK carrier
           if ge f9 = f9 + f3;
           r4 = pass r4;
           if eq jump Gr_Enc_Lp (db);          // jump if 16QAM carrier
           r6 = r6 - r6;                       // r6 - fdep specifier
           r6 = bset r6 by 6;                  // set len6 field to 1
           // restore i2 by modify(i2,m7)
           r0 = r0 - r0, modify(i2,m7);
           r5 = dm(i3,m6);
           LCNTR = r4, do Gr_Enc_X until lce;  // r4 = d2alloc[i]
             r0 = r0 or fdep r5 by r6, r5 = dm(i3,m6);  
Gr_Enc_X:  r6 = r6 + 1, m8 = r0;
           r6 = r6 - r6, r0 = pm(m8,i8);
           f8 = f8 + f0, modify(i3,m7);
           r0 = r0 - r0, dm(i2,m6) = r8;
           r6 = bset r6 by 6, r5 = dm(i3,m6);
           LCNTR = r4, do Gr_Enc_Y until lce;  // r4 = d2alloc[i]
             r0 = r0 or fdep r5 by r6, r5 = dm(i3,m6);
Gr_Enc_Y:  r6 = r6 + 1, m8 = r0;
           r6 = pm(m8,i8);
           f9 = f9 + f6, modify(i3,m7);
Gr_Enc_Lp: r2 = r2 + r2, dm(i2,m6) = r9;           // store Y coordinate from r9   
    r2 = r2 + r1, r4 = dm(i4,m5);
    i9 = r2;
    r4 = pass r4, r8 = pm(i9,m14);
    if ge f8 = f8 + f3;
    r4 = pass r4, dm(i2,m6) = r8, r9 = pm(i9,m13); // store X coordinate from r8
    if lt jump epil (db);                          // jump if QPSK carrier
    if ge f9 = f9 + f3;    
    dm(i2,m6) = r9;   // store Y coordinate from r9
    r4 = pass r4;
    if eq jump epil (db);           // jump if 16QAM carrier
    r6 = r6 - r6;                       // r6 - fdep specifier
    r6 = bset r6 by 6;                  // set len6 field to 1
    // restore i2 by 2 x modify(i2,m7)
    modify(i2,m7);
    modify(i2,m7);
    r0 = r0 - r0, r5 = dm(i3,m6);
    LCNTR = r4, do Gr_Enc_X1 until lce;  // r4 = d2alloc[i]
        r0 = r0 or fdep r5 by r6, r5 = dm(i3,m6);
Gr_Enc_X1:  r6 = r6 + 1, m8 = r0;
    r6 = pm(m8,i8);
    f8 = f8 + f6, modify(i3,m7);
    r0 = r0 - r0, dm(i2,m6) = r8;
    r6 = r6 - r6;
    r6 = bset r6 by 6, r5 = dm(i3,m6);
    LCNTR = r4, do Gr_Enc_Y1 until lce;   // r4 = d2alloc[i]
        r0 = r0 or fdep r5 by r6, r5 = dm(i3,m6);
Gr_Enc_Y1:  r6 = r6 + 1, m8 = r0;
    r6 = pm(m8,i8);
    f9 = f9 + f6, modify(i3,m7);
    dm(i2,m6) = r9;           // store Y coordinate from r9           
    
epil:
    mode1 = ustat1;             // 22.01.04 vgp

    r1  = gets( 13 );
    r2 =  gets( 12 );
    r3  = gets( 11 );
    r5  = gets( 10 );
    r6  = gets( 9 );
    r9  = gets( 8 );
    i1  = gets( 7 );
    i2  = gets( 6 );
    i3  = gets( 5 );
    i4  = gets( 4 );
    i8  = gets( 3 );
    i9  = gets( 2 );
    m8  = gets( 1 );

    leaf_exit; 

_TCM_Gray_Encoder.end:

.endseg;	


