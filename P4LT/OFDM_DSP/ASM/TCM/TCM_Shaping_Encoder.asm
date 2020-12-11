/******************************************************************************
 *
 * PROJECT     : ETL600
 * FILE        : $Workfile: TCM_Shaping_Encoder.asm $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : TCM Shaping encoder
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * 
 * Prototype:
 *	TCM_Shaping_Encoder(int d2alloc[], int len2d);  
 * 
 *----------------------------------------------------------------------------- 
 * 
 * 
 * Initial version
 *       13.09.2003 vgp
 * Independent TX/RX directions
 *       13.01.2004 vgp
 * Viterby algorithm revision
 *       16.01.2004 vgp
 *       i6 DAG REGISTER DOEN'T WORK WITH EXTERNAL MEMORY
 * Sign extension select while addressing in SW mode
 *       22.01.2004 vgp
 * No i6 isage
 *       15.02.04 vgp
 * Change of calling to:     TCM_Shaping_Encoder(d2alloc_ptr, len2d, OFDM_Shaping_Flag, OFDM_Shaping_Bits)
 *       06.09.04 vgp
 *****************************************************************************/
/* $Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/TCM/TCM_Shaping_Encoder.asm $
 * 
 * 1     19.01.05 15:37 Chchlee
 * 
 * 5     30.09.04 8:56 Leeb02
 * 
 * 4     30.09.04 8:27 Leeb02
 */

#include <asm_sprt.h>

#include   "../../h/tcm/TCM_Defs.h"

#ifdef __ADSP21160__
#include <def21160.h>
#endif
#ifdef __ADSP21161__
#include <def21161.h>
#endif

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define XYsign_ptr ZCode_ptr   // temporary array of length LEN_2D_MAX_TX
                               // ZCode is defined in TCM_Trellis_34_Encoder.asm
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//==============================================
	.section/pm seg_pmda;

	                          
    .endseg;
//==============================================
.SEGMENT/dm	seg_dmda;
    
    .extern _TX_Shaping_Bits_Ptr; // C input: pointer of bit buffer to be encoded by shaping code 
    .extern _Sig_TX_Ptr;          // C input: pointer to output modulated QAM vector of complex_float
    .extern St_Sg_ptr;
    .extern XYsign_ptr;
    
.align 2;
.var AccMetric[4];
.var SaveD2;
.var Shaping_Bits;

.ENDSEG;

//==============================================
.SEGMENT/PM		seg_pmco;

		.global _TCM_Shaping_Encoder;

// m7 must be -1
// m6 must be  1

_TCM_Shaping_Encoder:
    leaf_entry;

    puts = r1;
    puts = r2;
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
    r0 = m1; puts = r0;
    r0 = m8; puts = r0;
    r0 = m9; puts = r0;

    ustat1 = mode1;             // 22.01.04 vgp
    bit set mode1 SSE;
    
    i0 = r4;                 // i0 = r4 points d2alloc[len2d]
    dm(SaveD2) = r4;
    r15 = r8;                // r8 = len2d

    r12 = pass r12;          // r12 - Shaping flag if ne then shaing is ON
    if ne jump Shaping_ON;

// ===================== QAM modulation shaping OFF ===================================
    i4 = dm(_TX_Shaping_Bits_Ptr);  // address of bit buffer for shaping encoding
    i2 = dm(_Sig_TX_Ptr);           // pointer to the real part of modulated vector     
    i3 = dm(_Sig_TX_Ptr);     
    r1 = r1 - r1, modify(i3,m6);              // pointer to the imaginary part of modulated vector
    m1 = 2;
    LCNTR = r15, do (pc,QAM_map) until lce;   // r15 = len2d
      r1 = r1 + 1, r0 = dm(i0,m6);            // r0 = d2alloc[]    
      r0 = r0 + 1;     
      if eq jump tst3 (db);                   // jump if QPSK carrier
      r0 = lshift r1 by r0;
      f0 = float r0;                          // f0 - shift from zero, 1<<d2alloc[0]+1                       
      r2 = dm(i4,m6);                         // r2 bit for shaping encoding
      r1 = fdep r2 by 1:1, r2 = dm(i4,m6);
      r1 = r1 or fdep r2 by 0:1;
      r1 = pass r1;
      if ne jump tst1;
        r1 = dm(i2,m5);
        f1 = f1 - f0, r2 = dm(i3,m5);
        jump tst3 (db);
        f2 = f2 - f0, dm(i2,m5) = f1;
        dm(i3,m5) = f2;
tst1:
      r1 = r1 - 1;
      if ne jump tst2;
        r1 = dm(i2,m5);
        jump tst3 (db);
        f1 = f1 - f0;
        dm(i2,m5) = f1;
tst2:
      r1 = r1 - 1;
      if ne jump tst3;
	  r2 = dm(i3,m5);
        jump tst3 (db);
        f2 = f2 - f0;
        dm(i3,m5) = r2;
tst3:
      modify(i2,m1);
      modify(i3,m1);
QAM_map: r1 = r1 - r1;

      jump epil;

// ===================== QAM modulation shaping ON ===================================
Shaping_ON:
    r2 = reads(1);                  // read number of shaping bits
	dm(Shaping_Bits) = r2;
    i2 = dm(XYsign_ptr);        
    i1 = dm(_TX_Shaping_Bits_Ptr);  // address of bit buffer for shaping encoding
    r0 = r0 - r0;
    r1 = r1 - r1;
    LCNTR = r2, do (pc,sp_mul_inv) until lce;
	  r2 = r0 xor r1, r3 = dm(i1,m6);
	  r3 = fext r3 by 0:1;
	  r0 = r2 xor r3, r1 = r0;
sp_mul_inv:  dm(i2,m6) = r0;

// Viterby forward loop

    i2 = dm(XYsign_ptr);
    i1 = dm(_Sig_TX_Ptr);     // pointer to the real part of modulated vector     
	i3 = dm(St_Sg_ptr);

    i5 = AccMetric;
    f0 = 1.0e30;
    r1 = r1 - r1;
    dm(i5,m6) = r1;
    dm(i5,m6) = f0;
    dm(i5,m6) = f0;
    dm(i5,m5) = f0;
    i5 = AccMetric;
    r14 = 2;

    r1 = r1 - r1;
    LCNTR = r15, do (pc,vit_for) until lce;  // r15 = len2d
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      r1 = r1 + 1, r6 = dm(i0,m6);           // r6 = d2alloc[]    
      r0 = r6 + 1;           
      if eq jump vit_for (db);               // jump if QPSK carrier      
      r0 = lshift r1 by r6;
      f5 = float r0;                         // f5 - shift from zero, 1<<d2alloc[0]+1                       
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!    
    
      r1 = 0x23000000;
	  r2 = -8;
      r8 = r8 - r8, r6 = dm(i2,m6);        // XYsign[i]
      r6 = pass r6;
      if ne jump none_zero;
	    r0 = 0x00031112;
        f13 = -f5,  r11 = dm(i1,m6);        // met3 = -0.5B(Q), f11 = sig.re[i]
        f10 = f5 + f8, r12 = dm(i1,m6);     // met0 = 0.5B(Q), f12 = sig.im[i]
        f11 = -f11;                         // met1 = -sig.re[i]
        f12 = -f12;                         // met2 = -sig.im[i]
		jump vit_met(db);
         f10 = f10 + f11;
         f10 = f10 + f12;                   // met0 = -met3 + met1 + met2

//         S S S S S S S S
//         T G T G T G T G
//         0 0 1 1 2 2 3 3
//
// Def  0x 0 0 0 3 1 1 1 2
//
// Mod0 0x 2 3 0 0 0 0 0 0
//         2 3 x x x x x x
// Mod1 0x 0 0 2 3 0 0 0 0
//         x x 2 0 x x x x
// Mod2 0x 0 0 0 0 2 3 0 0
//         x x x x 3 2 x x
// Mod3 0x 0 0 0 0 0 0 2 3
//         x x x x x x 3 1 
none_zero:
      r0 = 0x02011310;
      f11 = -f5, r13 = dm(i1,m6);           // met1 = -0.5B(Q), f13 = sig.re[i]
      f12 = f5 + f8, r10 = dm(i1,m6);       // met2 = 0.5B(Q), f10 = sig.im[i]
      f13 = -f13;          // met3 = -sig.re[i]
      f10 = -f10;          // met0 = -sig.im[i]
      f12 = f12 + f13;
      f12 = f12 + f10;     // met2 = -met1 + met0 + met3 

//         S S S S S S S S
//         T G T G T G T G
//         0 0 1 1 2 2 3 3
//
// Def  0x 0 2 0 1 1 3 1 0
//
// Mod0 0x 2 3 0 0 0 0 0 0
//         2 1 x x x x x x
// Mod1 0x 0 0 2 3 0 0 0 0
//         x x 2 2 x x x x
// Mod2 0x 0 0 0 0 2 3 0 0
//         x x x x 3 0 x x
// Mod3 0x 0 0 0 0 0 0 2 3
//         x x x x x x 3 3 
vit_met:
      r6 = dm(i5,m5);                  // summet = f6 = AccMetric[0]
      f6 = f6 + f10, r7 = dm(2,i5);    // summet = AccMetric[0] + met0, newsum = f7 = AccMetric[2]
      f7 = f7 + f13;                   // newsum = AccMetric[2] + met3 
      comp(f6,f7);                     // compare f6=summet and f7=newsum
	  if gt r0 = r0 xor r1, f6 = f7;   // summet = newsum

	  r1 = lshift r1 by r2, f9 = f6;   // summet = f6 = AccMetric[0]
      r6 = dm(i5,m5);                  // summet = f6 = AccMetric[0]
      f6 = f6 + f13, r7 = dm(2,i5);    // summet = AccMetric[0] + met3, newsum = AccMetric[2]
      f7 = f7 + f10, dm(i5,m5) = f9;   // newsum = AccMetric[2] + met0,  AccMetric[0] = tmp
      comp(f6,f7);                     // compare f6=summet and f7=newsum
	  if gt r0 = r0 xor r1, f6 = f7;   // summet = newsum

	  r1 = lshift r1 by r2, f9 = f6;   // f9 = tmp = summet
      r6 = dm(1,i5);                   // summet = f6 = AccMetric[1]
      f6 = f6 + f11, r7 = dm(3,i5);    // summet = AccMetric[1] + met1, newsum = AccMetric[3]
      f7 = f7 + f12;                   // newsum = AccMetric[3] + met2
      comp(f6,f7);                     // compare f6=summet and f7=newsum
	  if gt r0 = r0 xor r1, f6 = f7;   // summet = newsum

	  r1 = lshift r1 by r2, dm(2,i5) = f6; // AccMetric[2] = summet
      r6 = dm(1,i5);                   // summet = f6 = AccMetric[1]
      f6 = f6 + f12, r7 = dm(3,i5);    // summet = AccMetric[1] + met2, newsum = AccMetric[3]
      f7 = f7 + f11, dm(1,i5) = f9;    // newsum = AccMetric[3] + met1, AccMetric[1] = tmp
      comp(f6,f7);                     // compare f6=summet and f7=newsum
	  if gt r0 = r0 xor r1, f6 = f7;   // summet = newsum

	  dm(3,i5) = f6;                   // AccMetric[3] = summet
      dm(i3,m6) = r0;                  // save state[i][0:3] & sig[i][0:3]
vit_for: r1 = r1 - r1;

// Viterby backward loop
    r0 = r0 - r0;
    r1 = r0 + 1, f6 = dm(i5,m6);    // AccMetric[0]
    r1 = r1 + 1, f7 = dm(i5,m6);    // AccMetric[1]
    comp(f6,f7), f8 = dm(i5,m6);    // AccMetric[2]
    if gt r0 = r0 + 1, f6 = f7;

    r2 = r2 - r2, f7 = dm(i5,m6);   // AccMetric[3]
    comp(f8,f7);
    if gt r1 = r1 + 1, f8 = f7;
    
    comp(f6,f8);
    if gt r0 = r1 + r2;
    
DoBack:
//	i3 = State;
//	i4 = Sign;
//========================== Viterbi back step

    i2 = dm(XYsign_ptr);
    i3 = dm(St_Sg_ptr);
    r2 = dm(Shaping_Bits);
    r3 = r2 - 1;             // r3 = # of shaping bits - 1
    m1 = r3;
    modify(i2,m1);
    r4 = r4 - r4, modify(i3,m1);
    r3 = 3;
    r8 = -8;
    lcntr = r2, do (pc,Vit_Back) until lce;  
          r5 = r3 - r0, r2 = dm(i3,m7);
          r5 = r8*r5(ssi);
          r2 = lshift r2 by r5;
          r6 = fext r2 by 0:4;  // Sign[i][j] bits 3:0
Vit_Back: r0 = fext r2 by 4:4, dm(i2,m7) = r6;  // State[i][j] bits 7:4


// mapping of signal with shaping flag ON
    
    i0 = dm(SaveD2);
    f0 = float r0;          // shift from zero, 1<<d2alloc[0]+1
    i4 = dm(XYsign_ptr);        
    i2 = dm(_Sig_TX_Ptr);     // pointer to the real part of modulated vector     
    i3 = dm(_Sig_TX_Ptr);     
    r1 = r1 - r1, modify(i3,m6);           // pointer to the imaginary part of modulated vector
    m1 = 2;
    LCNTR = r15, do (pc,QAM_sp_map) until lce;   // r15 = len2d

      r1 = r1 + 1, r0 = dm(i0,m6);         // r0 = d2alloc[]    
      r0 = r0 + 1;     
      if eq jump sp_tst3 (db);             // jump if QPSK carrier
      r0 = lshift r1 by r0;
      f0 = float r0;                       // f0 - shift from zero, 1<<d2alloc[0]+1                       
    
        
      r1 = dm(i4,m6);       // r1 sign
      r1 = pass r1;
      if ne jump sp_tst1;
        r1 = dm(i2,m5);
        f1 = f1 - f0, r2 = dm(i3,m5);
        jump sp_tst3 (db);
        f2 = f2 - f0, dm(i2,m5) = f1;
        dm(i3,m5) = f2;
sp_tst1:
      r1 = r1 - 1;
      if ne jump sp_tst2;
        r1 = dm(i2,m5);
        jump sp_tst3 (db);
        f1 = f1 - f0;
        dm(i2,m5) = f1;
sp_tst2:
      r1 = r1 - 1;
      if ne jump sp_tst3;
        r2 = dm(i3,m5);
        jump sp_tst3 (db);
        f2 = f2 - f0;
        dm(i3,m5) = r2;
sp_tst3:
      modify(i2,m1);
      modify(i3,m1);
QAM_sp_map: r1 = r1 - r1;

// =====================
epil:

    mode1 = ustat1;             // 22.01.04 vgp

    r1   = gets( 24 );
    r2   = gets( 23 );
    r3   = gets( 22 );
    r5   = gets( 21 );
    r6   = gets( 20 );
    r7   = gets( 19 );
    r9   = gets( 18 );
    r10  = gets( 17 );
    r11  = gets( 16 );
    r12  = gets( 15 );
    r13  = gets( 14 );
    r14  = gets( 13 );
    r15  = gets( 12 );
    i0   = gets( 11 );
    i1   = gets( 10 );
    i2   = gets( 9 );
    i3   = gets( 8 );
    i4   = gets( 7 );
    i5   = gets( 6 );
    i8   = gets( 5 );
    i9   = gets( 4 );
    m1   = gets( 3 );
    m8   = gets( 2 );
    m9   = gets( 1 );

    leaf_exit; 

_TCM_Shaping_Encoder.end:

.endseg;	

