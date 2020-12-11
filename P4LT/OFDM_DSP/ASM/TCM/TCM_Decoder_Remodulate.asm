/******************************************************************************
 *
 * PROJECT     : ETL600
 * FILE        : $Workfile: TCM_Decoder_Remodulate.asm $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : Remodulates OFDM QAM vector after TCM decoding
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * 
 * Prototype:
 *	TCM_Decoder_Remodulate(int d2alloc[], int len2d);  
 * 
 *----------------------------------------------------------------------------- 
 * 
 * 
 * Initial version
 *       13.09.2003 vgp
 * Memory usage reduction
 *       16.01.2004 vgp
 *       i6 DAG REGISTER DOEN'T WORK WITH EXTERNAL MEMORY
 * Sign extension select while addressing in SW mode
 *       22.01.2004 vgp
 *****************************************************************************/
/* $Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/TCM/TCM_Decoder_Remodulate.asm $
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

//------------------------------------
	.section/pm seg_pmda;
	                          
 .var xy_ModTable[] =    -0.5, -0.5,  0.5,  0.5, -0.5,  0.5,  0.5, -0.5; 
    .endseg;
//------------------------------------
	.section/dm seg_dmda;


 .extern _Sig_RX_Remod_Ptr;
 .extern _num_abcd_ptr;     // points array of length LEN_4D_MAX_RX
 
 .extern _gr_koor_y_ptr;    // points array of length LEN_2D_MAX_RX
 .extern _gr_koor_x_ptr;    // points array of length LEN_2D_MAX_RX

    .endseg;

//-------------------------------------------------

	.section/pm seg_pmco;

    .global _TCM_Decoder_Remodulate;


_TCM_Decoder_Remodulate:

    leaf_entry;

    r0 = i2; puts = r0;    /* Save registers */
    r0 = i3; puts = r0;
    r0 = i4; puts = r0;      
    r0 = i1; puts = r0;      
    r0 = i5; puts = r0;      
    r0 = i0; puts = r0;
    r0 = m8; puts = r0;  
    r0 = i8; puts = r0;  
    r0 = i9; puts = r0;  
    r0 = m2; puts = r0;
    r0 = m4; puts = r0;
    puts = r1;
    puts = r3;
    puts = r4;
	puts = r6;
    puts = r7;
    puts = r8;
    puts = r9;
    puts = r10;
    puts = r11;
    puts = r12;
    puts = r13;

    ustat1 = mode1;             // 22.01.04 vgp
    bit set mode1 SSE;
    
    i5 = r4;                         // i5 = r4 = d2alloc[]
    r4 = lshift r8 by -1;            // r4 = len4d = len2d/2
	
    i0  = dm(_num_abcd_ptr);         // address of an array of decoded quartets numbers
    i1  = dm(_Sig_RX_Remod_Ptr);     // address of the real part of remodulated QAM vector
    r12 = xy_ModTable;
    i3  = dm(_gr_koor_x_ptr);        // address of the vector of X coordinates of decoded quartets 
    i4  = dm(_gr_koor_y_ptr);        // address of the vector of Y coordinates of decoded quartets
    m4  = 4;             

    r7 = r7 - r7;
	r6 = 0x200;                      // len6 bit field eq 8 for "fext" instruction
	r11 = 8;
    lcntr = r4, do LL3 until lce;    // r4 = len4d

      r7 = r7 + 1, r4 = dm(i5,m6);   // r7 = 1
      r4 = r4+1;                     // r2 = d2alloc[]+1

      r2 = lshift r7 by r4, r3 = dm(i0,m5);  // r3 = num_abcd[i]
      r3 = fext r3 by 2:2;            // r3 = (mum_abcd[i] & 0xc)>>2
	  r13 = r11 * r3 (ssi);
	  r13 = r13 or r6;

      r3 = r3 + r3, r7 = dm(i3,m6);
	  r7 = fext r7 by r13;            // r7 = gr_coor_x[i][i_abcd]
      r3 = r3 + r12;                  // r2 address of xy_ModTable[i_abcd]
      r2 = r2 - 1, i8 = r3;
      f2 = float r2, f9 = pm(i8,m14); // f9 = x 
      f9 = f9 - f2, f10 = pm(i8,m13); // f9 = x - sfz, f10 = y
      f10 = f10 - f2;                 // f10 = y - sfz
      r7 = r7 + r7;
      f7 = float r7;       
      f9 = f9 + f7, r7 = dm(i4,m6);
	  r7 = fext r7 by r13;            // r7 = gr_coor_y[i][i_abcd]
      r7 = r7 + r7, r3 = dm(i0,m6);   // r3 = num_abcd[i]
      f7 = float r7;
      f10 = f10 + f7,dm(i1,m6) = f9;
      
//  odd i_2d

      r7 = r7 - r7;
      r7 = r7 + 1, r4 = dm(i5,m6);    // r7 = 1
      r4 = r4+1;                      // r2 = d2alloc]+1 
      r2 = lshift r7 by r4;

      r3 = fext r3 by 0:2, dm(i1,m6) = f10; // r3 = (mum_abcd[i] & 0x3)      
	  r13 = r11 * r3 (ssi);
	  r13 = r13 or r6;

      r3 = r3 + r3, r7 = dm(i3,m6);
	  r7 = fext r7 by r13;            // r7 = gr_coor_x[i][i_abcd]

      r3 = r3 + r12;                  // r2 address of xy_ModTable[i_abcd]
      r2 = r2 - 1, i8 = r3;
      f2 = float r2, f9 = pm(i8,m14); // f9 = x 
      f9 = f9 - f2, f10 = pm(i8,m13); //f9 = x - sfz, f10 = y
      f10 = f10 - f2;                 // f10 = y - sfz
      r7 = r7 + r7;
      f7 = float r7;       
      f9 = f9 + f7, r7 = dm(i4,m6);
	  r7 = fext r7 by r13;            // r7 = gr_coor_y[i][i_abcd]
      r7 = r7 + r7;
      f7 = float r7;
      f10 = f10 + f7,dm(i1,m6) = f9;
LL3:  r7 = r7 - r7, dm(i1,m6) = f10;

epil:
    mode1 = ustat1;             // 22.01.04 vgp

    i2 = gets( 22 );        /* Restore registers */
    i3 = gets( 21 );
    i4 = gets( 20 );
    i1 = gets( 19 );
    i5 = gets( 18 );
    i0 = gets( 17 );
    m8 = gets( 16 );
    i8 = gets( 15 );
    i9 = gets( 14 );
    m2 = gets( 13 );
    m4 = gets( 12 );
    r1 = gets( 11 );
    r3 = gets( 10 );
    r4 = gets( 9 );
	r6 = gets( 8 );
    r7 = gets( 7 );
    r8 = gets( 6 );
    r9 = gets( 5 );
    r10 = gets( 4 );
    r11 = gets( 3 );
    r12 = gets( 2 );
    r13 = gets( 1 );
    leaf_exit;
    
_TCM_Decoder_Remodulate.end:

.endseg;


