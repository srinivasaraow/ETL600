/******************************************************************************
 *
 * PROJECT     : ETL600 COFDM modem  
 * FILE        : $Workfile: Encoder_3212.asm $
 * COPYRIGHT   : 2004 TL Consulting GmbH.
 * DESCRIPTION : Concatinated (32,12) d = 10 encoder
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * 
 * 1     31.03.04 vgp
 * Initial version
 *
 * Code (32,12) d = 10
 *
 * Output:
 * MSB                                 LSB
 * 31 30                                0  - bit number within 32 bit word
 *  |  |                                |
 * i1,i2,...,i12,r1,r2,...,r12,p1,p2...p8
 *
 *    1   2    3    4    5    6    7    8
 *  =======================================   Inner                |
 * | i1 | i4 | i7 |i10 | r1 | r4 | r7 |r10 |  --------->           |
 * | i2 | i5 | i8 |i11 | r2 | r5 | r8 |r11 |  RS(8,4) d=5 GF(2^3)  |
 * | i3 | i6 | i9 |i12 | r3 | r6 | r9 |r12 |                       |
 *  ---------------------------------------                        |
 * | p1 | p2 | p3 | p4 | p5 | p6 | p7 | p8 |                      \ /
 *  =======================================                (4,3) d=2 GF(2)
 *                                                               Outer
 *
 *****************************************************************************/
/* $Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/QAM4_FEC/Encoder_3212.asm $
 * 
 * 1     19.01.05 15:37 Chchlee
 * 
 * 3     30.09.04 8:54 Leeb02
 * 
 * 2     30.09.04 8:27 Leeb02
 */

#define N_RS84    8                // length of extended RS(8,4)
#define N1_RS84   (N_RS84-1)       // length of basic RS(7,4)
#define K_RS84    4                // number of information symbols
#define R_RS84    (N_RS84-K_RS84)  // number of check symbols       
#define R1_RS84   (R_RS84-1)
#define R2_RS84   (R_RS84+2)

#define N_3212    32               // lenght of (32,12) code in bits
#define K_3212    12               // number of information bits in (32,12) code 
#define R_3212    (N_3212-K_3212)  // number of check bits in (32,12) code 
 
#include <asm_sprt.h>

#ifdef __ADSP21160__
#include <def21160.h>
#endif
#ifdef __ADSP21161__
#include <def21161.h>
#endif

.global _Encoder_3212;
.global MulMatG8;     // array of 64 elements
.global PCbGF8;       // 32bit word
.global c84;          // array of 8 elements
.global ParityOuter;  // 32bit word
.global cw3212;       // 32bit word

		.segment/dm   seg_dmda;
		
.var MulMatG8[N_RS84*N_RS84] =  0, 0, 0, 0, 0, 0, 0, 0,
                                0, 1, 2, 3, 4, 5, 6, 7,
                                0, 2, 4, 6, 3, 1, 7, 5,
                                0, 3, 6, 5, 7, 4, 1, 2,
                                0, 4, 3, 7, 6, 2, 5, 1,
                                0, 5, 1, 4, 2, 7, 3, 6,
                                0, 6, 7, 1, 5, 3, 2, 4,
                                0, 7, 5, 2, 1, 6, 4, 3;

.var G84[K_RS84*R_RS84] = 3, 2, 1, 3, 5, 5, 1, 4, 7, 6, 1, 6, 6, 5, 6, 7;  // RS(8,4) generation matrix

.var PCbGF8 = 0x96;   // parity check bit-table

.var c84[N_RS84];
.var ParityOuter;
.var cw3212;

//		.segment/pm   seg_pmco;
		.segment/pm   seg_ext_pmco;

/*****************************************************
*
* unsigned int Encoder_3212( unsigned int X );
*
* Operation : for input X in 12 LSB bits of 32 bit word 
*             returns (32,12) codeword  
*
*****************************************************/
_Encoder_3212:

      leaf_entry;

      puts = r1;
      puts = r2;
      puts = r8;
      puts = r12;
      r0 = i0; puts = r0;  
      r0 = i1; puts = r0;  
      r0 = i2; puts = r0;  
      r0 = i4; puts = r0;  
            
	  // r4 - information word to encode
	  // get information symbols of inner RS(8,4) code
	  // calculate parity bits for information RS(8,4) symbols
	  // deposit information bits of (32,12) codeword
	  
      r0 = 0xc9;                      // r0 = len6=3:bit6 = 9
	  r12 = 3;
	  i4 = c84;                       // address of RS(8,4) codeword
	  r1 = dm(PCbGF8);
	  r2 = 0;                         // parity
	  r8 = fext r4 by r0;
	  r0 = r0 - r12, dm(i4,m6) = r8;  // r0 = len6=3:bit6 = 6, store c84[0]
	  btst r1 by r8;                  // test parity bit
      if not sz r2 = bset r2 by 7;    // set parity bit if required

      r8 = fext r4 by r0;
	  r0 = r0 - r12, dm(i4,m6) = r8;  // r0 = len6=3:bit6 = 3, store c84[1]
	  btst r1 by r8;                  // test parity bit
      if not sz r2 = bset r2 by 6;    // set parity bit if required
      
	  r8 = fext r4 by r0;	  
	  r0 = r0 - r12, dm(i4,m6) = r8;  // r0 = len6=3:bit6 = 0, store c84[2]
	  btst r1 by r8;                  // test parity bit
      if not sz r2 = bset r2 by 5;    // set parity bit if required

      r8 = fext r4 by r0;	  
      dm(i4,m6) = r8;                 // store c84[3]
	  btst r1 by r8;                  // test parity bit
      if not sz r2 = bset r2 by 4;    // set parity bit if required
      dm(ParityOuter) = r2;           // store partialy calculated parity word
      r4 = lshift r4 by R_3212;       // shift inf. bits to the left by number of check bits
      dm(cw3212) = r4;                // store information part of (32,12) codeword

      // Reed-Solomon (8,4) encoder
      i0 = c84+4;                     // address of 4-th symbol (first check symbol) of RS(8,4) codeword 
      i4 = G84;
      r4 = MulMatG8;
      lcntr = R_RS84, do outer_lp until lce;
          i1 = c84;
          r0 = r0 - r0;
          lcntr = K_RS84, do inner_lp until lce;
            r1 = dm(i1,m6);                // load c85[i]
            r1 = lshift r1 by 3;           // 8*c85[i]
            r1 = r4 + r1, r2 = dm(i4,m6);  // address of MulMatG8[c85[j]][0]
            r1 = r1 + r2;                  // address of MulMatG8[c85[j]][G84[i][j]]
            i2 = r1;
            r1 = dm(i2,m5);                        
inner_lp:   r0 = r0 xor r1;      
outer_lp: dm(i0,m6) = r0;      
      // end of Reed-Solomon encoder
            
	  // deposit check symbols of inner RS(8,4) codeword
	  // into (32,12) codeword
	  // calculate parity bits for RS(8,4) check symbols
	  // deposit outer parity check bits into (32,12) codeword
	  i4 = c84+4;                     // address of 4-th symbol (first check symbol) of RS(8,4) codeword 
	  r1 = dm(PCbGF8);                    // parity check table
	  r2 = dm(ParityOuter);           // partial encoded parity word
      r4 = dm(cw3212);                // information part of (32,12) codeword
      r8 = dm(i4,m6);                 // first RS(8,4) check symbol	  
	  btst r1 by r8;                  // test parity bit
      if not sz r2 = bset r2 by 3;    // set parity bit if required
      r4 = r4 or fdep r8 by 17:3;
      
      r8 = dm(i4,m6);                 // second RS(8,4) check symbol	  
	  btst r1 by r8;                  // test parity bit
      if not sz r2 = bset r2 by 2;    // set parity bit if required
      r4 = r4 or fdep r8 by 14:3;

      r8 = dm(i4,m6);                 // third RS(8,4) check symbol	  
	  btst r1 by r8;                  // test parity bit
      if not sz r2 = bset r2 by 1;    // set parity bit if required
      r4 = r4 or fdep r8 by 11:3;

      r8 = dm(i4,m5);                 // fourth RS(8,4) check symbol	  
	  btst r1 by r8;                  // test parity bit
      if not sz r2 = bset r2 by 0;    // set parity bit if required
      r4 = r4 or fdep r8 by 8:3;
      
      r0 = r2 or r4;                  // return (32,12) codeword

      r1  = gets( 8 );
      r2  = gets( 7 );
      r8  = gets( 6 );
      r12 = gets( 5 );
      i0  = gets( 4 );
      i1  = gets( 3 );
      i2  = gets( 2 );
      i4  = gets( 1 );

      leaf_exit;

_Encoder_3212.end:

.endseg;

