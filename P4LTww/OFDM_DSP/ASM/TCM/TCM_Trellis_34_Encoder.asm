/******************************************************************************
 *
 * PROJECT     : ETL600
 * FILE        : $Workfile: TCM_Trellis_34_Encoder.asm $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : TCM Trellis 3/4 encoder
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * 
 * Prototype:
 *	TCM_Trellis_34_Encoder(int len2d, int *Tmp_ptr);  
 *
 * Tmp_ptr - points temporarz vector of length 2*LEN_2D_MAX_TX
 * 
 *----------------------------------------------------------------------------- 
 * 
 * 
 * 1     08.09.2003  vgp
 * Initial version
 * Independent TX/RX directions
 *       13.01.2004 vgp
 * Memory usage reduction
 *       16.01.2004 vgp
 * Short word addressing of StateTable[], ZCodeTransformTable[], and EncoderTermBits[]
 *       19.01.2004 vgp
 * Dynamic temporary memory allocation for TCM encoding according to current len2d
 *       04.03.04 vgp
 *****************************************************************************/
/* $Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/TCM/TCM_Trellis_34_Encoder.asm $
 * 
 * 1     19.01.05 15:37 Chchlee
 * 
 * 4     30.09.04 8:56 Leeb02
 * 
 * 3     30.09.04 8:27 Leeb02
 */

#include <asm_sprt.h>

#include   "../../h/tcm/TCM_Defs.h"

//==============================================
	.section/pm seg_pmda;
	                          
// EncoderState (State,4D-subset mod 4) is the new state of the encoder (see trellis diagram)
.align 2;
.var StateTable[32] =    0x00020000, /*  0,  2, */
                         0x00030001, /*  1,  3, */
				         0x00060004, /*  4,  6, */  
						 0x00070005, /*  5,  7, */  
                         0x000B0009, /*  9, 11, */  
						 0x000A0008, /*  8, 10, */   
						 0x000F000D, /* 13, 15, */ 
						 0x000E000C, /* 12, 14, */
                         0x00000002, /*  2,  0, */ 
						 0x00010003, /*  3,  1, */    
						 0x00040006, /*  6,  4, */  
						 0x00050007, /*  7,  5, */
                         0x0009000B, /* 11,  9, */ 
						 0x0008000A, /* 10,  8, */   
						 0x000D000F, /* 15, 13, */ 
						 0x000C000E, /* 14, 12, */							
                         0x00030001, /*  1,  3, */  
						 0x00020000, /*  0,  2, */    
						 0x00070005, /*  5,  7, */  
						 0x00060004, /*  4,  6, */
                         0x000A0008, /*  8, 10, */  
						 0x000B0009, /*  9, 11, */   
						 0x000E000C, /* 12, 14, */ 
						 0x000F000D, /* 13, 15, */
                         0x00010003, /*  3,  1, */  
						 0x00000002, /*  2,  0, */    
						 0x00050007, /*  7,  5, */  
						 0x00040006, /*  6,  4, */
                         0x0008000A, /* 10,  8, */ 
						 0x0009000B, /* 11,  9, */   
						 0x000C000E, /* 14, 12, */ 
						 0x000D000F; /* 15, 13; */ 

//ZCodeTransformTable(row+1) 4D-z-code, where row is number of row of the table "Maping of i0,...,i3               
.align 2;
.var ZCodeTransformTable[8] = 0x00050000, /*  0,  5, */ 
                              0x000F000A, /* 10, 15, */ 
							  0x00040001, /*  1,  4, */ 
							  0x000E000B, /* 11, 14, */ 
                              0x00070002, /*  2,  7, */  
							  0x000C0009, /*  9, 12, */  
							  0x00060003, /*  3,  6, */  
							  0x000D0008; /*  8, 13; */


    .endseg;
//==============================================
.SEGMENT/dm	seg_dmda;
.global ZCode_ptr;                // Assembler output : encoded by 3/4 trellis code symbols
.global St_Sg_ptr;

.extern _TX_Code_Bits_Ptr;    // C input: pointer to inf. bit buffer to be encoded by 3/4 trellis code

            
//  EncoderTermBits(State,4D-term-branch) is the number 2*i1 + i2 (packed bits i1, i2) where
//  State            is the encoder state after encoding LEN_4D -2 branches        
//  4D-term-branch   is the number of terminating branch 1 or 2
.align 2;
.var EncoderTermBits[16] = 0x00000000, /* 0, 0, */   
                           0x00010000, /* 0, 1, */   
						   0x00020002, /* 2, 2, */   
						   0x00030002, /* 2, 3, */
                           0x00000001, /* 1, 0, */   
						   0x00010001, /* 1, 1, */   
						   0x00020003, /* 3, 2, */   
						   0x00030003, /* 3, 3, */
                           0x00000002, /* 2, 0, */   
						   0x00010002, /* 2, 1, */   
						   0x00020000, /* 0, 2, */   
						   0x00030000, /* 0, 3, */
                           0x00000003, /* 3, 0, */   
						   0x00010003, /* 3, 1, */   
						   0x00020001, /* 1, 2, */   
						   0x00030001; /* 1, 3; */
              
//output: Z encoded array 
.var ZCode_ptr;
.var St_Sg_ptr;

.ENDSEG;

//==============================================

.SEGMENT/PM		seg_pmco;

		.global _TCM_Trellis_34_Encoder;

// m7 must be -1
// m6 must be  1

_TCM_Trellis_34_Encoder:
    leaf_entry;

    puts = r2;
    puts = r1;
    puts = r3;
    puts = r4;
    puts = r5;
    puts = r6;
    r0 = i0; puts = r0;
    r0 = i1; puts = r0;
    r0 = m1; puts = r0;
    r0 = i8; puts = r0;
    r0 = i9; puts = r0;
    r0 = m8; puts = r0;
    r0 = m9; puts = r0;

    r0 = r4;
    r4 = lshift r4 by -1;        // r4 = len4d = len2d/2
    r4 = r4 - 1;
    r4 = r4 - 1;                 // r4 = len4d-2

/*      
    ZCode_ptr = Tmp_ptr;     
    St_Sg_ptr = Tmp_ptr + Len2d    
*/

    dm(ZCode_ptr) = r8;
    r8 = r8 + r0;
    dm(St_Sg_ptr) = r8;
    

    i1 = dm(_TX_Code_Bits_Ptr);     // address of bit buffer incl. leading 5 crc bits
    i8 = 2*StateTable;              // SW addressing 
    i9 = 2*ZCodeTransformTable;     // SW addressing
    i0 = dm(ZCode_ptr);             // address of output array
    r0 = r0 - r0;            // current State = 0
    r6 = r0;                 // clear r6 = State<<2 - new state index
    LCNTR = r4, do (pc,encode) until lce;    // r4 = len4d-2
      r5 = fdep r0 by 3:1, r1 = dm(i1,m6); // r1 = CodeBit[i]
      r5 = r5 or fdep r1 by 2:1;
      r6 = r6 or fdep r1 by 1:1, r1 = dm(i1,m6); // r1 = CodeBit[i+1]
      r5 = r5 or fdep r1 by 1:1;
      r6 = r6 or fdep r1 by 0:1, r1 = dm(i1,m6); // r1 = CodeBit[i+2]
      m8 = r6;
      r5 = r5 or fdep r1 by 0:1;
      m9 = r5;
      r0 = pm(m8,i8);              // r0 = new state
      r1 = pm(m9,i9);                            // r1 = ZCode
      r5 = fext r1 by 2:2; 
      r6 = fext r1 by 0:2, dm(i0,m6)=r5;         // store Zcode[i++]
encode: r6 = lshift r0 by 2, dm(i0,m6) = r6;     // r6 new state index, store Zcode[i++]

// Last two 2D termination symbols 
    r5 = r0 + r0;     // r5 = 2*state
    r1 = 2*EncoderTermBits;                    // SW addressing
    r1 = r1 + r5;
    i2 = r1;                                   // address of first termination symbol
// first termination 2D symbol
    r5 = fdep r0 by 3:1, r1 = dm(i2,m5);       // First pair of termination bits
    r1 = lshift r1 by -1;                      // first termination bit
    r5 = r5 or fdep r1 by 2:1;

    r6 = r6 or fdep r1 by 1:1, r1 = dm(i2,m6); // r1 second termination bit
    r5 = r5 or fdep r1 by 1:1;
    r6 = r6 or fdep r1 by 0:1, r1 = dm(i1,m6); // r1 next to last inf. code bit
    m8 = r6;
    r5 = r5 or fdep r1 by 0:1;
    m9 = r5;
    r0 = pm(m8,i8);                            // r0 = new state
    r1 = pm(m9,i9);                            // r1 = ZCode
    r5 = fext r1 by 2:2; 
    r6 = fext r1 by 0:2, dm(i0,m6)=r5;         // store Zcode[i++]
    r6 = lshift r0 by 2, dm(i0,m6) = r6;       // r6 new state index, store Zcode[i++]
// second termination 2D symbol
    r5 = fdep r0 by 3:1, r1 = dm(i2,m5);        // Second pair of termination bits
    r1 = lshift r1 by -1;                      // third termination bit
    r5 = r5 or fdep r1 by 2:1;
    r6 = r6 or fdep r1 by 1:1, r1 = dm(i2,m5); // fourth termination bit
    r5 = r5 or fdep r1 by 1:1;
    r6 = r6 or fdep r1 by 0:1, r1 = dm(i1,m5); // last inf. code bit
    m8 = r6;
    r5 = r5 or fdep r1 by 0:1;
    m9 = r5;
    r0 = pm(m8,i8);    // new state must be equal to 0, for testing purpose only
    r1 = pm(m9,i9);                            // r1 = ZCode
    r5 = fext r1 by 2:2; 
    r6 = fext r1 by 0:2, dm(i0,m6)=r5;         // store Zcode[i++]
    dm(i0,m5) = r6;                            //store Zcode[i] 

epil:
    r2 =  gets( 13 );
    r1  = gets( 12 );
    r3  = gets( 11 );
    r4  = gets( 10 );
    r5  = gets( 9 );
    r6  = gets( 8 );
    i0  = gets( 7 );
    i1  = gets( 6 );
    m1  = gets( 5 );
    i8  = gets( 4 );
    i9  = gets( 3 );
    m8  = gets( 2 );
    m9  = gets( 1 );

    leaf_exit; 

_TCM_Trellis_34_Encoder.end:

.endseg;	



