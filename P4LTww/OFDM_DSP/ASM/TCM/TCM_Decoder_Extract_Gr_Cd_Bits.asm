/******************************************************************************
 *
 * PROJECT     : ETL600
 * FILE        : $Workfile: TCM_Decoder_Extract_Gr_Cd_Bits.asm $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : Extructs information bits encoded by 3/4 trellis and Gray codes
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * 
 * Prototype:
 *	TCM_Decoder_Extract_Gr_Cd_Bits(int d2alloc[], int len2d);  
 * 
 *----------------------------------------------------------------------------- 
 * 
 * 
 * Initial version
 *       14.09.2003 vgp
 * Memory usage reduction
 *       16.01.2004 vgp
 *       i6, i7 DAG REGISTERS DON'T WORK WITH EXTERNAL MEMORY and SW mode
 * Short word addressing of num_way[], gray_decode_table[] and s_f_z[]
 *       19.01.2004 vgp
 * Sign extension select while addressing in SW mode
 *       22.01.2004 vgp
 * i7 doesn't work with SW addressing mode, so i0<->i7
 *       22.01.2004 vgp
 * No usage of i6/i7
 *       27.02.04 vgp
 *****************************************************************************/
/* $Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/TCM/TCM_Decoder_Extract_Gr_Cd_Bits.asm $
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

.SEGMENT/DM	seg_dmda;

#define _x_sign_d_ptr _wb_sig_ptr  // _wb_sig points temporary array of length 16*LEN_4D_MAX_RX
#define _y_sign_d_ptr _wb_st_ptr   // _wb_st points temporary array of length 16*LEN_4D_MAX_RX

.extern _RX_Code_Bits_Ptr;
.extern _RX_Gray_Bits_Ptr;

.extern _gr_koor_y_ptr;  // points array of length LEN_2D_MAX_RX
.extern _gr_koor_x_ptr;  // points array of length LEN_2D_MAX_RX
.extern _x_sign_d_ptr;   // points array of length LEN_2D_MAX_RX
.extern _y_sign_d_ptr;   // points array of length LEN_2D_MAX_RX

.extern _num_abcd_ptr;   // points array of length LEN_4D_MAX_RX

.ENDSEG;

.SEGMENT/PM		seg_pmda;

.align 2;
.var  num_way[8]=  
   	0x00040000, /*  0,  4, */  
	0x000C0008, /*  8, 12, */  
	0x00010005, /*  5,  1, */ 
    0x0009000D, /* 13,  9, */
    0x000A000E, /* 14, 10, */  
    0x00060002, /*  2,  6, */ 
    0x000F000B, /* 11, 15, */
    0x00030007; /*  7,  3; */

.align 2;
.var  gray_decode_table[32] =   
              0x00010000, /*  0, 1, */
		      0x00020003, /*  3, 2, */ 
			  0x00070006, /*  6, 7, */ 
			  0x00040005, /*  5, 4, */
			  0x000D000C, /* 12,13, */
			  0x000E000F, /* 15,14, */
			  0x000B000A, /* 10,11, */ 
			  0x00080009, /*  9, 8, */
              0x00190018, /* 24,25, */
			  0x001A001B, /* 27,26, */
			  0x001F001E, /* 30,31, */
			  0x001C001D, /* 29,28, */
			  0x00150014, /* 20,21, */
			  0x00160017, /* 23,22, */
			  0x00130012, /* 18,19, */
			  0x00100011, /* 17,16, */
              0x00210020, /* 32,33, */
			  0x00220023, /* 35,34, */
			  0x00270026, /* 38,39, */
			  0x00240025, /* 37,36, */
			  0x002D002C, /* 44,45, */
			  0x002E002F, /* 47,46, */
			  0x002B002C, /* 42,43, */
			  0x00280029, /* 41,40, */
              0x00390038, /* 56,57, */
			  0x003A003B, /* 59,58, */
			  0x003F003E, /* 62,63, */
			  0x003C003D, /* 61,60, */
			  0x00350034, /* 52,53, */
			  0x00360037, /* 55,54, */
			  0x00330032, /* 50,51, */
			  0x00300031; /* 49,48; */
              
//                    QPSK  16QAM  64QAM  256QAM 1024QAM 4096QAM 16384QAM
//.var     s_f_z[7] =   0,    0,     2,     4,     8,     16,     32; // shift fron zero coordinates table upto 16384QAM

.align 2;
.var s_f_z[4] = 0x00000000,  /* 0, 0 */
                0x00040002,  /* 2, 4 */
				0x00100008,  /* 8,16 */
				0x00000020;  /*32, 0 */

.ENDSEG;

.SEGMENT/PM		seg_pmco;

.global _TCM_Decoder_Extract_Gr_Cd_Bits;

_TCM_Decoder_Extract_Gr_Cd_Bits:

      leaf_entry;

      puts=  r2;
      puts = r3;
      puts = r5;
      puts = r6;
      puts = r7;
      puts = r9;
      puts = r10;
      puts = r11;
      puts = r12;
      puts = r13;
      puts = r14;
      puts = r15;
      r0 = i0; puts = r0;
      r0 = i1; puts = r0;
      r0 = i2; puts = r0;
      r0 = i3; puts = r0;
      r0 = i4; puts = r0;
      r0 = i5; puts = r0;
      r0 = i8; puts = r0;
      r0 = i9; puts = r0;
      r0 = i10; puts = r0;
      r0 = m5; puts = r0;
      r0 = m8; puts = r0;
      r0 = m9; puts = r0;

      ustat1 = mode1;             // 22.01.04 vgp
      bit set mode1 SSE;

      i0 = r4;                      // i0 = r4 address of d2alloc[]   
      r15 = lshift r8 by -1;        // r15 = len4d = len2d/2
      r14 = r15 - 1, r8 = i0;       // r8 = d2alloc ptr;
      r14 = r14 - 1;                // r14 = len4d-2

      i1 = dm(_RX_Code_Bits_Ptr);     // pointer to CD bits
      i2 = dm(_num_abcd_ptr);
	  i3 = dm(_gr_koor_x_ptr);
	  i4 = dm(_gr_koor_y_ptr);				
      i5 = dm(_RX_Gray_Bits_Ptr);     //pointer to GR bits

      r9  = dm(_x_sign_d_ptr); 
      r10 = dm(_y_sign_d_ptr);

      i8 = 2*gray_decode_table;         // points gray decode table, SW addressing
      i9 = 2*num_way;                   // points way table, SW addressing
      i10 = 2*s_f_z+1;                  // points shift from zero array, SW addressing
      
      r2 = r2 - r2;                       // r2 = i_2d, r6 = 6
      r6 = 6;
      r11 = 8;
      LCNTR = r15, do Extr_Lp until lce;  // r15 = len4d
// ============ even i_2d
        r7 = lshift r11 by r6, r3 = dm(i2,m6); // r3=num_abcd[i_4d]
        r5 = fext r3 by 2:2, r0 = dm(i0,m6);   // r5 = i_abcd, r0 = d2alloc[i]        
        r12 = r5 * r11 (ssi), r4 = dm(i3,m6);  // r4 = gr_coor_x[i][3:0]
        r12 = r12 or r7, m8 = r0;              // m8 = d2alloc[]
        r4 = fext r4 by r12, r7 = dm(i4,m6);   // r4 = GrX = gr_coor_x[i][i_abcd] , r7 = gr_coor_y[i][3:0]
        r7 = fext r7 by r12, r13 = pm(m8,i10); // r7 = gr_coor_y[i][i_abcd], r13 = s_f_z[d2alloc[i]]
        
        r1 = pass r0, r8 = i0;                 // test d2alloc for QPSK, save d2alloc ptr in r8
        if lt r1 = r1 + 1;                     // if QPSK
        r1 = bset r1 by r6, i0 = r9;           // set len6 field to 1, i0 points x_sign_d
        //---- proc. gr_coor_x[j] ----
        r5 = fext r4 by r1;                // fetch sign from GrX        
        r12 = r13 * r5 (ssi), dm(i0,m6) = r5; // store SignX[j] from r5
        r4 = r4 - r12, r5 = r7;            // r4 = decoded GrX, r5 = GrY
        
        //---- proc. gr_coor_y[j] ----
        r7 = fext r5 by r1, m9 = r4;       // fetch sign from GrY, m9 = GrX
        r12 = r13 * r7 (ssi), r9 = i0;     // save x_sign_d ptr
        i0 = r10;                          // load y_sign_d ptr
        r7 = r5 - r12, dm(i0,m6) = r7;     // r7 = decoded GrY, store SigY[j] from r7         
        r0 = pass r0, r10 = i0;            // test for QPSK & 16QAM
        if le jump Extr_CD;                // (db); !!!!!!!!!!!!!!!
        r1 = r1 - r1, r4 = pm(m9,i8);      // r4 = decoded GrX
        r1 = bset r1 by r6, m9 = r7;
        r12 = pass r1, r7 = pm(m9,i8);     // r7 = decoded GrY        
        //---- store X Gray bits
        LCNTR = r0, do Extr_GrX until lce; 
          r5 = fext r4 by r1;        
Extr_GrX: r1 = r1 + 1, dm(i5,m6) = r5;
        //---- store Y Gray bits
        LCNTR = r0, do Extr_GrY until lce;
          r5 = fext r7 by r12;
Extr_GrY: r12 = r12 + 1, dm(i5,m6) = r5;
                
Extr_CD:
        r0 = lshift r2 by -1;
        m9 = r3;
        comp(r14,r0), r5 = pm(m9,i9);
        if le jump Extr_TCD (db);
          r7 = m6;                           // r7 = 1
          r4 = fext r5 by 2:1;
          r4 = fext r5 by 1:1, dm(i1,m6) = r4;
          dm(i1,m6) = r4;
Extr_TCD:
        r4 = r5 and r7;
        r2 = r2 + 1, dm(i1,m6) = r4;         // inrement i_2d counter
        
//============ odd i_2d
        r7 = lshift r11 by r6, i0 = r8;       // i0 points d2alloc 
        r5 = fext r3 by 0:2, r0 = dm(i0,m6);  // r5 = i_abcd, r0 = d2alloc[i]        r7 = lshift r11 by r6, r4 = dm(i3,m6); // r4 = gr_coor_x[i][3:0]
        r12 = r5 * r11 (ssi), m8 = r0;        // m8 = d2alloc[]
        r12 = r12 or r7, r4 = dm(i3,m6);      // r4 = gr_coor_x[i][3:0]
        r4 = fext r4 by r12, r7 = dm(i4,m6);   // r4 = GrX = gr_coor_x[i][i_abcd] , r7 = gr_coor_y[i][3:0]
        r7 = fext r7 by r12, r13 = pm(m8,i10); // r7 = gr_coor_y[i][i_abcd], r13 = s_f_z[d2alloc[i]]
        
        r1 = pass r0, r8 = i0;            // test d2alloc for QPSK
        if lt r1 = r1 + 1;                // if QPSK
        r1 = bset r1 by r6, i0 = r9;      // set len6 field to 1

        //---- proc. gr_coor_x[j] ----
        r5 = fext r4 by r1;                // fetch sign from GrX        
        r12 = r13 * r5 (ssi), dm(i0,m6) = r5; // store SignX[j] from r5
        r4 = r4 - r12, r5 = r7;            // r4 = decoded GrX, r5 = GrY

        //---- proc. gr_coor_y[j] ----
        r7 = fext r5 by r1, m9 = r4;       // fetch sign from GrY, m9 = GrX
        r12 = r13 * r7 (ssi), r9 = i0;
        i0 = r10;
        r7 = r5 - r12, dm(i0,m6) = r7;     // r7 = decoded GrY, store SigY[j] from r7         
        r0 = pass r0, r10 = i0;            // test for QPSK & QAM16.
        if le jump Extr_Lp;                // (db); !!!!!!!!!!!!!!!!!!!
        r1 = r1 - r1, r4 = pm(m9,i8);      // r4 = decoded GrX
        r1 = bset r1 by r6, m9 = r7;
        r12 = pass r1, r7 = pm(m9,i8);     // r7 = decoded GrY        
        //---- store X Gray bits
        LCNTR = r0, do Extr1_GrX until lce; 
           r5 = fext r4 by r1;        
Extr1_GrX: r1 = r1 + 1, dm(i5,m6) = r5;
        //---- store Y Gray bits
        LCNTR = r0, do Extr1_GrY until lce;
           r5 = fext r7 by r12;
Extr1_GrY: r12 = r12 + 1, dm(i5,m6) = r5;
         
Extr_Lp: r2 =  r2 + 1, i0 = r8;            // incr. i_2d


//=========== epilogue
Epil:
    mode1 = ustat1;             // 22.01.04 vgp
       
      r2  = gets( 24 );
      r3  = gets( 23 );
      r5  = gets( 22 );
      r6  = gets( 21 );
      r7  = gets( 20 );
      r9  = gets( 19 );
      r10 = gets( 18 );
      r11 = gets( 17 );
      r12 = gets( 16 );
      r13 = gets( 15 );
      r14 = gets( 14 );
      r15 = gets( 13 );
      i0  = gets( 12 );
      i1  = gets( 11 );
      i2  = gets( 10 );
      i3  = gets( 9 );
      i4  = gets( 8 );
      i5  = gets( 7 );
      i8  = gets( 6 );
      i9  = gets( 5 );
      i10 = gets( 4 );
      m5  = gets( 3 );
      m8  = gets( 2 );
      m9  = gets( 1 );

      leaf_exit;

_TCM_Decoder_Extract_Gr_Cd_Bits.end:

.endseg;	


