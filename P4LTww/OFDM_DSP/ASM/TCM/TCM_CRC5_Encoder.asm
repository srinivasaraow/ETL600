/******************************************************************************
 *
 * PROJECT     : ETL600 
 * FILE        : $Workfile: TCM_CRC5_Encoder.asm $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : TCM CRC5 encoder, dosn't depend of LEN_2D
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * 
 * 
 * Initial version
 *       08.09.2003 vgp
 * Independent TX/RX directions
 *       13.01.2004 vgp
 * Short word storage and addressing for RX and TX bit buffers
 * IN INTERNAL MEMORY (i6 & i7 DOSN'T WORK WITH EXTERNAL MEMORY)
 *       18.01.2004 vgp
 * Short word addressing of CRC5States table
 *       19.01.2004 vgp
 * Change of calling to TCM_CRC5_Encoder(NumBits)
 *       06.09. 2004 vgp
 *  
 *****************************************************************************/
/* $Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/TCM/TCM_CRC5_Encoder.asm $
 * 
 * 1     19.01.05 15:37 Chchlee
 * 
 * 4     30.09.04 8:56 Leeb02
 * 
 * 3     30.09.04 8:27 Leeb02
 */

#include <asm_sprt.h>

.SEGMENT/DM	seg_dmda;

.extern _TX_Code_Bits_Ptr;    // C input: pointer to inf. bit buffer

.ENDSEG;

//==============================================
.SEGMENT/dm	seg_dmda;
                .global CRC5States;

.align 2;

.var CRC5States[32]= 	 0x00150000,  /*  0, 21, */
                         0x00140001,  /*  1, 20, */
						 0x00170002,  /*  2, 23, */
						 0x00160003,  /*  3, 22, */
                         0x00110004,  /*  4, 17, */
						 0x00100005,  /*  5, 16, */
						 0x00130006,  /*  6, 19, */
						 0x00120007,  /*  7, 18, */
                         0x001D0008,  /*  8, 29, */
						 0x001C0009,  /*  9, 28, */
						 0x001F000A,  /* 10, 31, */
						 0x001E000B,  /* 11, 30, */
						 0x0019000C,  /* 12, 25, */
						 0x0018000D,  /* 13, 24, */
						 0x001B000E,  /* 14, 27, */
						 0x001A000F,  /* 15, 26, */
                         0x00050010,  /* 16,  5, */
						 0x00040011,  /* 17,  4, */
						 0x00070012,  /* 18,  7, */
						 0x00060013,  /* 19,  6, */
						 0x00010014,  /* 20,  1, */
						 0x00000015,  /* 21,  0, */
						 0x00030016,  /* 22,  3, */
						 0x00020017,  /* 23,  2, */
						 0x000D0018,  /* 24, 13, */
						 0x000C0019,  /* 25, 12, */
						 0x000F001A,  /* 26, 15, */
						 0x000E001B,  /* 27, 14, */
						 0x0009001C,  /* 28,  9, */
						 0x0008001D,  /* 29,  8, */
					     0x000B001E,  /* 30, 11, */
					     0x000A001F;  /* 31, 10; */
					     
.ENDSEG;

//==============================================

.SEGMENT/PM		seg_pmco;

		.global _TCM_CRC5_Encoder;

// m7 must be -1
// m6 must be 1

_TCM_CRC5_Encoder:
    leaf_entry;

    puts = r2;
    puts = r1;
    puts = r3;
    puts = r4;
    r0 = i0; puts = r0;
    r0 = i1; puts = r0;
    r0 = m1; puts = r0;

    r8 = r4;                        // number of inf. bits for CRC5 calculation
    r3 = dm(_TX_Code_Bits_Ptr);     // address of bit buffer incl. leading 5 crc bits
    r4 = 5;                         // crc length
    r0 = m6;                        // r3 points start of crc string, r0 = 1
    r8 = pass r8;
    if le jump epil;                // test number of inf. bit

    i1 = 2*CRC5States;        // SW addressing
    r2 = r3 + r4, r0 = r3;   // r0 = address of X[]
    r3 = r3 - r3, i0 = r2;   // i0 points to X[5]
    r1 = r3 + 1,  r2 = dm(i0,m6);   // r1 = 1;
    r2 = r2 and r1; 
    r2 = lshift r2 by r4;
    LCNTR = r8, do (pc,fill) until lce;
      r3 = r2 + r3,  r2 = dm(i0,m6);
      r2 = r2 and r1, m1 = r3; 
fill:	r2 = lshift r2 by r4, r3 = dm(m1,i1);

    i0 = r0;              // restore address of X[]
//store crc bits in bit buffer
    r0 = fext r3 by 0:1;
    r0 = fext r3 by 1:1, dm(i0,m6) = r0;
    r0 = fext r3 by 2:1, dm(i0,m6) = r0;
    r0 = fext r3 by 3:1, dm(i0,m6) = r0;
    r0 = fext r3 by 4:1, dm(i0,m6) = r0;
    r0 = r0 - r0,        dm(i0,m6) = r0;    

epil:
    r2 =  gets( 7 );
    r1  = gets( 6 );
    r3  = gets( 5 );
    r4  = gets( 4 );
    i0  = gets( 3 );
    i1  = gets( 2 );
    m1  = gets( 1 );

    leaf_exit; 

_TCM_CRC5_Encoder.end:
.endseg;	



