/******************************************************************************
 *
 * PROJECT     : ETL600 COFDM modem  
 * FILE        : $Workfile: Decoder_3212.asm $
 * COPYRIGHT   : 2004 TL Consulting GmbH.
 * DESCRIPTION : Concatinated (32,12) d = 10 encoder
 *
 * REVISION HISTORY:
 * 1     1.04.04 vgp
 *       Initial version
 * 2     Execution from SDRAM
 *       06.04.04 vgp
 *-----------------------------------------------------------------------------
 * 
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
/* $Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/QAM4_FEC/Decoder_3212.asm $
 * 
 * 1     19.01.05 15:37 Chchlee
 * 
 * 3     30.09.04 8:53 Leeb02
 * 
 * 2     30.09.04 8:27 Leeb02
*/

#define N_RS84    8                // length of extended RS(8,4)
#define N1_RS84   (N_RS84-1)       // length of basic RS(7,4)
#define K_RS84    4                // number of information symbols
#define R_RS84    (N_RS84-K_RS84)  // number of check symbols       
#define R1_RS84   (R_RS84-1)
#define R2_RS84   (R_RS84+2)
#define SLast     (Sx+N1_RS84)

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

.extern MulMatG8;     // array of 64 elements
.extern PCbGF8;       // 32bit word
.extern c84;          // array of 8 elements
.extern ParityOuter;  // 32bit word
.extern cw3212;       // 32bit word

.global _Decoder_3212;

		.segment/dm   seg_dmda;

.var InvElemG8[N_RS84]  = 0, 1, 5, 6, 7, 2, 3, 4;

.var FFTG8[N1_RS84*K_RS84]  = 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
                              0x01, 0x02, 0x04, 0x03, 0x06, 0x07, 0x05,
                              0x01, 0x04, 0x06, 0x05, 0x02, 0x03, 0x07,
                              0x01, 0x03, 0x05, 0x04, 0x07, 0x02, 0x06;

.var TFFG8[N1_RS84*N1_RS84] = 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
                              0x01, 0x05, 0x07, 0x06, 0x03, 0x04, 0x02,
                              0x01, 0x07, 0x03, 0x02, 0x05, 0x06, 0x04,
                              0x01, 0x06, 0x02, 0x07, 0x04, 0x05, 0x03,
                              0x01, 0x03, 0x05, 0x04, 0x07, 0x02, 0x06,
                              0x01, 0x04, 0x06, 0x05, 0x02, 0x03, 0x07,
                              0x01, 0x02, 0x04, 0x03, 0x06, 0x07, 0x05;

.var PowerTable[N1_RS84]    = 1, 2, 4, 3, 6, 7, 5;

.var dw_ptr;
.var rcw;
.var Lctr;          // locator of erasures
.var Sx[N_RS84];    // array for spectrum, Sx[N_RS84-1] is SxLast
.var e84[N_RS84];   // array of errornous symbols
.var Bx[R2_RS84];
.var Lx2[2*R2_RS84];
.var Lx_ptr;
.var Tx_ptr;
.var ro;


		.segment/pm   seg_pmco;
//		.segment/pm   seg_ext_pmco;

/*****************************************************
*
* int Decoder_3212(unsigned int *R, unsigned int X );
*
* Operation : for input X (32,12) codeword 
*             returns 0 if decoding is succeseful, 1 if failed
*                          decoded (32,12) codeword in *R
*                          if decoding is failed then *R == X
*
*****************************************************/
_Decoder_3212:

      leaf_entry;

      puts = r1;
      puts = r2;
      puts = r3;
      puts = r5;
      puts = r6;
      puts = r7;
      puts = r12;
      puts = r13;
      puts = r14;
      puts = r15;
      r0 = m1; puts = r0;
      r0 = i1; puts = r0;
      r0 = i2; puts = r0;
      r0 = i3; puts = r0;
      r0 = i4; puts = r0;
      
	  // r4 - pointer to decoded (32,12) codeword to encode
	  dm(dw_ptr) = r4;
	  
	  // r8 - (32,12) codeword to decode
	  // get received symbols of inner RS(8,4) code
	  // calculate parity bits for each RS(8,4) symbol
	  // calculate vector of errors in outer code 	  
	  i4 = c84;
      r0 = 0xdd;                      // r0 = len6=3:bit6 = 29
	  r12 = 3;
	  r2 = 0;                         // calculated parity vector
	  r1 = dm(PCbGF8);                // parity table
	  
	  r4 = fext r8 by r0;
	  r0 = r0 - r12, dm(i4,m6) = r4;  // r0 = len6=3:bit6 = 26, store c84[0]
	  btst r1 by r4;                  // test parity bit
      if not sz r2 = bset r2 by 7;    // set parity bit if required

      r4 = fext r8 by r0;
	  r0 = r0 - r12, dm(i4,m6) = r4;  // r0 = len6=3:bit6 = 23, store c84[1]
	  btst r1 by r4;                  // test parity bit
      if not sz r2 = bset r2 by 6;    // set parity bit if required
      
	  r4 = fext r8 by r0;	  
	  r0 = r0 - r12, dm(i4,m6) = r4;  // r0 = len6=3:bit6 = 20, store c84[2]
	  btst r1 by r4;                  // test parity bit
      if not sz r2 = bset r2 by 5;    // set parity bit if required

	  r4 = fext r8 by r0;
	  r0 = r0 - r12, dm(i4,m6) = r4;  // r0 = len6=3:bit6 = 17, store c84[3]
	  btst r1 by r4;                  // test parity bit
      if not sz r2 = bset r2 by 4;    // set parity bit if required

      r4 = fext r8 by r0;
	  r0 = r0 - r12, dm(i4,m6) = r4;  // r0 = len6=3:bit6 = 14, store c84[4]
	  btst r1 by r4;                  // test parity bit
      if not sz r2 = bset r2 by 3;    // set parity bit if required
      
	  r4 = fext r8 by r0;	  
	  r0 = r0 - r12, dm(i4,m6) = r4;  // r0 = len6=3:bit6 = 11, store c84[5]
	  btst r1 by r4;                  // test parity bit
      if not sz r2 = bset r2 by 2;    // set parity bit if required

	  r4 = fext r8 by r0;	  
	  r0 = r0 - r12, dm(i4,m6) = r4;  // r0 = len6=3:bit6 =  8, store c84[6]
	  btst r1 by r4;                  // test parity bit
      if not sz r2 = bset r2 by 1;    // set parity bit if required
                  
      r4 = fext r8 by r0;	  
      dm(i4,m6) = r4;                 // store c84[7]
	  btst r1 by r4;                  // test parity bit
      if not sz r2 = bset r2 by 0;    // set parity bit if required
      dm(cw3212) = r8;                // store received (32,12) codeword
      dm(ParityOuter) = r2;           // store partialy calculated parity word
      r4 = fext r8 by 0:8;            // extract received outer codeword
      r4 = r4 xor r2;                 // r4 = vector of errors in outer code
                                      // or vector of erasures (locators) for inner RS(8,4) code
      dm(Lctr) = r4;
            
      // initialize pointers to Lx and T polynoms      
      r0 = Lx2;
      dm(Lx_ptr) = r0;
      i4 = r0;
      r1 = R2_RS84;
      r1 = r1 + r0;
      dm(Tx_ptr) = r1;
      
      // initialize Lx = {1, 0, 0, 0, 0, 0, 0, 0}
      r0 = r0 - r0;
      lcntr = R2_RS84, do clr_Lx until lce;
clr_Lx:  dm(i4,m6) = r0;
      r0 = r0 + 1;
      dm(Lx2) = r0;
            
      // compute Lx polynomial of locators of erasures in r4 of RS(7,4) basic codeword
      r1 = N_RS84;                     // bit pointer 
      r2 = 0;                          // locator
      r3 = N_RS84;                    // loop counter
      r5 = 0;                          // erasure counter
      i1 = PowerTable;                 // address of PowerTable
      r7 = -1;                         // locator
init_Lx:
      r3 = r3 - 1;
      if eq jump do_fft;
        r7 = r7 + 1;                   // increment locator
        r1 = r1 - 1, m1 = r7;
        btst r4 by r1;
        if sz jump init_Lx;            // jump if no erasure
          r8 = dm(m1,i1);              // r8 = PowerTable[locator]
          // Lx = Lx - r8*Lx
          r6 = MulMatG8;
          r8 = lshift r8 by 3;         // r8 = 8*r8;
          r6 = r6 + r8;                // r6 = &MulMat[PowerTable[locator]][0]
          i2 = Lx2 + R2_RS84 - 1;      // address of Lx[R2_RS84-1]
          i3 = Lx2 + R2_RS84 - 2;      // address of Lx[R2_RS84-2]
          lcntr = R_RS84+1, do init1_Lx until lce;
            r8 = dm(i3,m7);            // get Lx[i-1]
            r8 = r8 + r6, r0 = dm(i2,m5); // r8 = &MulMat[PowerTable[locator]][Lx[i-1]], get Lx[i]
            i4 = r8;
            r8 = dm(i4,m5);            // r8 = MulMat[PowerTable[locator]][Lx[i-1]]
            r0 = r0 xor r8;
init1_Lx:   dm(i2,m7) = r0;          
          r5 = r5 + 1;                 // increment counter of erasures
          r8 = R_RS84;                 // test number of erasures
          r8 = r8 - r5;
          if ge jump init_Lx;
      
      // exception: number of erasures greater than R_RS84    
      r0 = 1;
      i4 = dm(dw_ptr);
      r1 = dm(cw3212);
      r1 = lshift r1 by -R_3212;
      dm(i4,m5) = r1;        
      jump epilogue;
        
      // perform direct FFT
do_fft:
      dm(ro) = r5;                       // store number of erasures in basic RS(7,4) codeword
      r14 = r5 + 1, r15 = r5;            // r14 = r = L+1, r15 = L = ro;
      
      i4 = Sx;
      i2 = FFTG8;
      r6 = MulMatG8;
      lcntr = R_RS84, do fft_lp2 until lce;
         r0 = r0 - r0;
         i1 = c84;
         lcntr = N1_RS84, do fft_lp1 until lce;
           r1 = dm(i2,m6);                    // load FFTG8[i][j]
           r1 = lshift r1 by 3;               // r1 = N_RS84*FFTG8[i][j]
           r1 = r6 + r1, r2 = dm(i1,m6);      // load c84[j];
           r1 = r1 + r2;                      // r1 = &MulMatG8[FFTG8[i][j]][c84[j]]
           i3 = r1;
           r1 = dm(i3,m5);
fft_lp1:   r0 = r0 xor r1;
fft_lp2: dm(i4,m6) = r0;
      r0 = dm(Sx+R1_RS84);
      dm(SLast) = r0;                    // to compute error in C84[N1_RS84]

//
//  r6 = address of MulMatG8
//  r5 = ro
// r15 = L
// r14 = r
// r13 = delta      

      // initialize Bx = Lx polynomial
      i2 = Lx2;
      i4 = Bx;
      lcntr = R2_RS84, do init_Bx until lce;
         r0 = dm(i2,m6);
init_Bx: dm(i4,m6) = r0;
      
      // =========== Berlecamp-Massey algorithm
      
      // Compute polinomial of error locators
clc_eloc:
      r1 = R1_RS84;
      r1 = r1 - r14;
      if lt jump test_4add_it;
      
      // r13 = delta calculation
      i4 = dm(Lx_ptr);                // i4 = &Lx[0]
      r1 = r14 - 1;                   // r1 = r-1
      r2 = Sx;
      r1 = r2 + r1;                   // r1 = &Sx[r-1]
      r2 = r15 + 1, i2 = r1;          // r2 = L+1, i2 = &Sx[r-1]
      r13 = r13 - r13;                // delta = 0
      lcntr = r2, do clc_delta until lce; // for (i=0; i<L+1; i++)
           r2 = dm(i4,m6);
           r2 = lshift r2 by 3;           // r2 = 8*Lx[i]
           r2 = r6 + r2, r1 = dm(i2,m7);  // r1 = Sx[r-1-i]
           r2 = r2 + r1;                  // r2 = &MulMatG8[Lx[i]][Sx[r-1-i]]
           i1 = r2;
           r2 = dm(i1,m5);
clc_delta: r13 = r13 xor r2;              // delta ^= MulMatG8[Lx[i]][Sx[r-1-i]]
      
      r13 = pass r13;
      if eq jump xBx_mul1;

      // T(x) = L(x) - delta*x*B(x)
      i4 = dm(Lx_ptr);
      i2 = dm(Tx_ptr);
      m1 = R2_RS84-1;                    // m1 = R+1
      r0 = dm(i4,m5);                    // r0 = L(0)
      dm(i2,m5) = r0;                    // T(0) = L(0)
      r1 = lshift r13 by 3;              // r1 = 8*delta
      r1 = r6 + r1, modify(i4,m1);       // r1 = &MulMatG8[delta][0], i4 = &L(R+1)
      i3 = r1;                           // i3 = &MulMatG8[delta][0]
      modify(i2,m1);                     // i2 = &T(R+1)
      i1 = Bx+R_RS84;                    // i1 = &B(R)
      lcntr = R2_RS84-1, do clc_Tx until lce; // for (i=0; i<L+1; i++)
        r2 = dm(i1,m7);                  // B(i-1)
        r4 = dm(i4,m7);                  // r2 = &MulMatG8[delta][B(i-1)], r4 = L(i)
        m1 = r2;
        r2 = dm(m1,i3);                  // r2 = MulMatG8[delta][B(i-1)]
        r2 = r2 xor r4;                  // r2 = L(i) ^ MulMatG8[delta][B(i-1)]        
clc_Tx: dm(i2,m7) = r2;                  // T(i) = L(i) ^ MulMatG8[delta][B(i-1)]
      
      r0 = r15 + r15;                    // r0 = 2*L
      r1 = r14 + r5;                     // r1 = r + ro
      r1 = r1 - 1;                       // r1 = r + ro - 1
      r1 = r1 - r0;
      if lt jump xBx_mul0;
        i4 = InvElemG8;
        r15 = r14 - r15, m1 = r13;        // r15 = r - L, m1 = delta
        r15 = r15 + r5, r0 = dm(m1,i4);   // L = r - L + ro, InvElemG8[delta]
        r0 = lshift r0 by 3;
        r0 = r0 + r6;                     // r0 = &MulMatG8[InvElemG8[delta]][0]
        i1 = r0;                          // i1 = &MulMatG8[InvElemG8[delta]][0]
        i4 = Bx;
        i2 = dm(Lx_ptr);
        lcntr = R2_RS84, do clc_Bx until lce; 
          r1 = dm(i2,m6);                 // L(i)
          m1 = r1;
          r1 = dm(m1,i1);                 // r1 = MulMatG8[InvElemG8[delta]][L(i)]
clc_Bx:   dm(i4,m6) = r1;        
        jump swp_LxTx;
      
      // B(x) = x*B(x)
xBx_mul0:
      i4 = Bx+R2_RS84-1;
      i2 = Bx+R_RS84;
      lcntr = R2_RS84-1, do xBx_lp0 until lce; 
         r0 = dm(i2,m7);
xBx_lp0: dm(i4,m7)=r0;
      dm(Bx) = m5;                    // B(0) = 0
      
      // swap Lx nd Tx pointers
swp_LxTx:
      r0 = dm(Lx_ptr);
      r1 = dm(Tx_ptr);
      dm(Lx_ptr) = r1;
      dm(Tx_ptr) = r0;
      jump next_it;
      
      // B(x) = x*B(x)
xBx_mul1:
      i4 = Bx+R2_RS84-1;
      i2 = Bx+R_RS84;
      lcntr = R2_RS84-1, do xBx_lp1 until lce; 
         r0 = dm(i2,m7);
xBx_lp1: dm(i4,m7)=r0;
      dm(Bx) = m5;                    // B(0) = 0
      
next_it:      
      r14 = r14+ 1;                   // r = r + 1
      jump clc_eloc;
      
      // One more iteration?
test_4add_it:            
      r0 = dm(Lctr);
      btst r0 by 0;
      if not sz jump do_SynExt;       // if extended c84[N1_RS84] is erased jump to syndrome extension
      
      r1 = R1_RS84;
      r1 = r5 - r1;                   
      if lt jump ver_last;            // ro ? R1_RS84
        // if ro >= R1_RS84 then Sx[R1_RS84] ^= c84[N1_RS84]
        r0 = dm(Sx+R1_RS84);          
        r1 = dm(c84+N1_RS84);
        r0 = r0 xor r1;
        dm(Sx+R1_RS84) = r0;
        jump do_SynExt;      
        
ver_last:
      r1 = R_RS84;
      r1 = r1 - r5;                   // R_RS84-ro
      r1 = ashift r1 by -1;           // (R_RS84-ro)/2
      r1 = r1 + r5;
      r1 = r1 - 1;                    // r1 = R_RS84 + (int)((R_RS84-ro)/2) - 1
      r1 = r1 - r15;                  // (R_RS84 + (int)((R_RS84-ro)/2) - 1) ? L
      if lt jump do_add_it;           // jump if (R_RS84 + (int)((R_RS84-ro)/2) - 1) < L
      r13 = pass r13;                 // test delta
      if eq jump do_SynExt;           // jump if delta == 0;
      
      // perform one more iteration
      // Sx[R1_RS84] ^= c84[N1_RS84]
do_add_it:      
      r0 = dm(Sx+R1_RS84);          
      r1 = dm(c84+N1_RS84);
      r0 = r0 xor r1;
      dm(Sx+R1_RS84) = r0;
      
      // delta calculation in last iteration
      // r13 = delta calculation
      i4 = dm(Lx_ptr);                // i4 = &Lx[0]
      r1 = r14 - 1;                   // r1 = r-1
      r2 = Sx;
      r1 = r2 + r1;                   // r1 = &Sx[r-1]
      r14 = r14 + 1, i2 = r1;         // r = r + 1
      r2 = r15 + 1;                   // r2 = L+1
      r13 = r13 - r13;                // delta = 0
      lcntr = r2, do clc_delta1 until lce; // for (i=0; i<L+1; i++)
            r2 = dm(i4,m6);
            r2 = lshift r2 by 3;           // r2 = 8*Lx[i]
            r2 = r6 + r2, r1 = dm(i2,m7);  // r1 = Sx[r-1-i]
            r2 = r2 + r1;                  // r2 = &MulMatG8[Lx[i]][Sx[r-1-i]]
            i1 = r2;
            r2 = dm(i1,m5);
clc_delta1: r13 = r13 xor r2;              // delta ^= MulMatG8[Lx[i]][Sx[r-1-i]]
      
      r13 = pass r13;
      if eq jump do_SynExt;
      
      // T(x) = L(x) - delta*x*B(x)
      i4 = dm(Lx_ptr);
      i2 = dm(Tx_ptr);
      m1 = R2_RS84-1;                    // m1 = R+1
      r0 = dm(i4,m5);                    // r0 = L(0)
      dm(i2,m5) = r0;                    // T(0) = L(0)
      r1 = lshift r13 by 3;              // r1 = 8*delta
      r1 = r6 + r1, modify(i4,m1);       // r1 = &MulMatG8[delta][0], i4 = &L(R+1)
      i3 = r1;                           // i3 = &MulMatG8[delta][0]
      modify(i2,m1);                     // i2 = &T(R+1)
      i1 = Bx+R_RS84;                    // i1 = &B(R)
      lcntr = R2_RS84-1, do clc_Tx1 until lce; // for (i=0; i<L+1; i++)
         r2 = dm(i1,m7);                  // B(i-1)
         r4 = dm(i4,m7);                  // r2 = &MulMatG8[delta][B(i-1)], r4 = L(i)
         m1 = r2;
         r2 = dm(m1,i3);                  // r2 = MulMatG8[delta][B(i-1)]
         r2 = r2 xor r4;                  // r2 = L(i) ^ MulMatG8[delta][B(i-1)]        
clc_Tx1: dm(i2,m7) = r2;                  // T(i) = L(i) ^ MulMatG8[delta][B(i-1)]
      
      // swap Lx nd Tx pointers
      r0 = dm(Lx_ptr);
      r1 = dm(Tx_ptr);
      dm(Lx_ptr) = r1;
      dm(Tx_ptr) = r0;

//  r6 = address of MulMatG8
//  r5 = ro
// r15 = L
// r14 = r
                  
      // Recursive extension of syndromes
do_SynExt:
      r0 = N1_RS84;
      r0 = r0 - r14;
      if lt jump do_TFF;
        r1 = r14 - 1;                     // r1 = r-1
        r2 = Sx;
        r1 = r1 + r2;                     // r2 = &Sx(r-1)
        i4 = r1;                          // i4 = &Sx(r-1)
        i1 = dm(Lx_ptr);                  // i1 = Lx[0]
        r0 = r0 - r0, i2 = r1;            // i2 = &Sx(r-1)          
        r2 = r15 + 1, dm(i4,m5) = r0;     // r2 = L+1, Sx[r-1] = 0;
        lcntr = r2, do SynExt_lp until lce; // for (i=0; i<L+1; i++)        
           r1 = dm(i1,m6);                  // load Lx[i]
           r1 = lshift r1 by 3;             // 8*Lx[i]
           r1 = r1 + r6, r2 = dm(i2,m7);    // r2 = S[r-1-i]
           r1 = r1 + r2;                    // r1 = &MulMatG8[Lx[i]][S[r-1-i]]
           i3 = r1;
           r1 = dm(i3,m5);                  // r1 = MulMatG8[Lx[i]][S[r-1-i]]
SynExt_lp: r0 = r0 xor r1;           
        dm(i4,m5) = r0;
              
        r14 = r14 + 1;
        jump do_SynExt;
        
      // Inverse Fourier transform
do_TFF:
      i4 = e84;
      i2 = TFFG8;
      r6 = MulMatG8;
      lcntr = N1_RS84, do tff_lp2 until lce;
         r0 = r0 - r0;
         i1 = Sx;
         lcntr = N1_RS84, do tff_lp1 until lce;
           r1 = dm(i2,m6);                    // load TFFG8[i][j]
           r1 = lshift r1 by 3;               // r1 = N_RS84*TFFG8[i][j]
           r1 = r6 + r1, r2 = dm(i1,m6);      // load Sx[j];
           r1 = r1 + r2;                      // r1 = &MulMatG8[TFFG8[i][j]][Sx[j]]
           i3 = r1;
           r1 = dm(i3,m5);
tff_lp1:   r0 = r0 xor r1;
         
tff_lp2: dm(i4,m6) = r0;
      r0 = dm(SLast);
      r1 = dm(Sx+R1_RS84);
      r0 = r0 xor r1;
      r1 = dm(c84+N1_RS84);
      r0 = r0 xor r1;                         // r0 = Sx[R1_RS84] ^ SLast ^ c[N1_RS84]
      dm(e84+N1_RS84) = r0;
      
      // reconstruction of RS(8,4) codeword in time domain
      i4 = c84;
      i2 = e84;
      r4 = r4 - r4;                          // number of corrected errors
      r2 = dm(Lctr);
      r3 = 7;
      lcntr = N_RS84, do rec_lp until lce;
        r6 = r6 - r6, r0 = dm(i4,m5);        // c84[i]
        r7 = r7 - r7, r1 = dm(i2,m6);        // e84[i]
        r0 = r0 xor r1;
        btst r2 by r3;
        if sz r6 = r6 + 1;
        r1 = pass r1;
        if ne r7 = r7 + 1;
        r6 = r6 and r7;
        r4 = r4 + r6;
rec_lp: r3 = r3 - 1, dm(i4,m6) = r0;
      
      r2 = dm(Lctr);
      r4 = r4 + r4, r0 = m6;
      r0 = r2 and r0;
      r5 = r5 + r0;
      r0 = r0 - r0;
      r5 = r5 + r4;
      r1 = R_RS84;
      r5 = r5 - r1;
      if le jump Dec_Outer;

      // exception: number of corrected errors more than possible    
      r0 = 1;
      i4 = dm(dw_ptr);
      r1 = dm(cw3212);
      r1 = lshift r1 by -R_3212;
      dm(i4,m5) = r1;        
      jump epilogue;
      
      // decode outer parity check code and (32,12) codeword
Dec_Outer:
	  i4 = c84;
      r0 = 0xdd;                      // r0 = len6=3:bit6 = 29
	  r12 = 3;
	  r2 = 0;                         // calculated parity vector
	  r1 = dm(PCbGF8);                // parity table
	  
	  r4 = dm(i4,m6);                 // load decoded c84[0]
	  r8 = fdep r4 by r0;             
	  btst r1 by r4;
      if not sz r2 = bset r2 by 7;    // set parity bit if required
      
      r0 = r0 - r12, r4 = dm(i4,m6);  // r0 = len6=3:bit6 = 26, r4 = c84[1]
	  r8 = r8 or fdep r4 by r0;
	  btst r1 by r4;
      if not sz r2 = bset r2 by 6;    // set parity bit if required
      
      r0 = r0 - r12, r4 = dm(i4,m6);  // r0 = len6=3:bit6 = 23, r4 = c84[2]
	  r8 = r8 or fdep r4 by r0;
	  btst r1 by r4;
      if not sz r2 = bset r2 by 5;    // set parity bit if required
      	  
      r0 = r0 - r12, r4 = dm(i4,m6);  // r0 = len6=3:bit6 = 20, r4 = c84[3]
	  r8 = r8 or fdep r4 by r0;
	  btst r1 by r4;
      if not sz r2 = bset r2 by 4;    // set parity bit if required

      r0 = r0 - r12, r4 = dm(i4,m6);  // r0 = len6=3:bit6 = 17, r4 = c84[4]
	  r8 = r8 or fdep r4 by r0;
	  btst r1 by r4;
      if not sz r2 = bset r2 by 3;    // set parity bit if required
      
      r0 = r0 - r12, r4 = dm(i4,m6);  // r0 = len6=3:bit6 = 14, r4 = c84[5]
	  r8 = r8 or fdep r4 by r0;
	  btst r1 by r4;
      if not sz r2 = bset r2 by 2;    // set parity bit if required
      
      r0 = r0 - r12, r4 = dm(i4,m6);  // r0 = len6=3:bit6 = 11, r4 = c84[6]
	  r8 = r8 or fdep r4 by r0;
	  btst r1 by r4;
      if not sz r2 = bset r2 by 1;    // set parity bit if required
      
      r0 = r0 - r12, r4 = dm(i4,m6);  // r0 = len6=3:bit6 = 8, r4 = c84[7]
	  r8 = r8 or fdep r4 by r0;
	  btst r1 by r4;
      if not sz r2 = bset r2 by 0;    // set parity bit if required
      
      r0 = r0 - r0;
      i4 = dm(dw_ptr);
      r8 = r8 or r2;
      r8 = lshift r8 by -R_3212;
      dm(i4,m5) = r8;
                  
epilogue:      
       r1 = gets( 15);
       r2 = gets( 14 );
       r3 = gets( 13 );
       r5 = gets( 12 );
       r6 = gets( 11 );
       r7 = gets( 10 );
      r12 = gets(  9 );
      r13 = gets(  8 );
      r14 = gets(  7 );
      r15 = gets(  6 );
       m1 = gets(  5 );
       i1 = gets(  4 );
       i2 = gets(  3 );
       i3 = gets(  2 );
       i4 = gets(  1 );

      leaf_exit;

_Decoder_3212.end:

.endseg;

