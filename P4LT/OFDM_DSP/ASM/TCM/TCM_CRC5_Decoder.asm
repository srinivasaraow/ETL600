/******************************************************************************
 *
 * PROJECT     : ETL600  
 * FILE        : $Workfile: TCM_CRC5_Decoder.asm $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : TCM CRC5 decoder, doesn't depends on LEN_2D
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * 
 * 
 * Initial version
 *       08.09.2003 vgp
 * Independent TX/RX directions
 *       13.01.2004 vgp
 * Short word addressing of CRC5States table
 *       19.01.2004 vgp
 *****************************************************************************/
/* $Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/TCM/TCM_CRC5_Decoder.asm $
 * 
 * 1     19.01.05 15:37 Chchlee
 * 
 * 3     30.09.04 8:56 Leeb02
 * 
 * 2     30.09.04 8:27 Leeb02
 */
#include <asm_sprt.h>

.SEGMENT/DM	seg_dmda;

.extern _RX_Code_Bits_Ptr;
.extern _OFDM_Num_Bits_RX;

.ENDSEG;

//==============================================
.SEGMENT/dm	seg_dmda;

.extern CRC5States;

.ENDSEG;

//==============================================

.SEGMENT/PM		seg_pmco;

		.global _TCM_CRC5_Decoder;

// m7 must be -1
// m6 must be 1

_TCM_CRC5_Decoder:
    leaf_entry;

    puts = r2;
    puts = r1;
    puts = r3;
    puts = r4;
    puts = r5;
    r0 = i0; puts = r0;
    r0 = i1; puts = r0;
    r0 = m1; puts = r0;

    i0 = dm(_RX_Code_Bits_Ptr);
    i1 = 2*CRC5States;        // SW addressing
    modify(i0,4);           // i0 points to last CRC5 bit
    r3 =  0;                // calculated CRC5
    r0 = dm(i0,m7);         // m7 = -1, r0 - received last CRC5 bit
    r1 = r3+1, r2 = dm(i0,m7);   // r1 = 1
    r2 = r2 or lshift r0 by r1,  r0=dm(i0,m7);
    r0 = r0 or lshift r2 by r1,  r2=dm(i0,m7);
    r2 = r2 or lshift r0 by r1,  r0=dm(i0,m7);
    r0 = r0 or lshift r2 by r1;  // r0 - received CRC5 result
    modify(i0,6);           // i0 points to the first inf. bit for CRC5 calculation

    r5 = dm(_OFDM_Num_Bits_RX);       // r5 - number of inf. bits for CRC5 calculation
    r4 = r4 - r4, r2 = dm(i0,m6);
    r5 = pass r5;
    if eq jump epil;        // number of inf. bits eq. to zero, CRC_RESULT will be eq to 0 

    r4 = 5;
    r2 = r2 and r1; 
    r2 = lshift r2 by r4;
    LCNTR = r5, do (pc,fill) until lce;
      r3 = r2 + r3,  r2 = dm(i0,m6);
      r2 = r2 and r1, m1 = r3; 
fill:	r2 = lshift r2 by r4, r3 = dm(m1,i1);

    r4 = r3 xor r0;  // compare received and calculated CRC5 results
    if ne r4 = m6;
epil:
    r0 = r4;

    r2 =  gets( 8 );
    r1  = gets( 7 );
    r3  = gets( 6 );
    r4  = gets( 5 );
    r5  = gets( 4 );
    i0  = gets( 3 );
    i1  = gets( 2 );
    m1  = gets( 1 );

    leaf_exit; 

_TCM_CRC5_Decoder.end:

.endseg;	




