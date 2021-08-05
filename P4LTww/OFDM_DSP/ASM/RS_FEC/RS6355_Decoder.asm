/******************************************************************************
 *
 * PROJECT     : ETL600
 * FILE        : $Workfile: RS6355_Decoder.asm $
 * COPYRIGHT   : 2004 by TL Consulting GmbH
 * DESCRIPTION : Reed-Solomon (63,55) decoder
 * OPERATION   : 
 * PROTOTYPE   : int RS6355_Decoder(int Y[], int Loc[], int D[], int tmp[]) ;    
 *                                                          
 * ASSUMPTIONS :
 *               tmp must points array of 728 32bit words
 *               Number of symbols is equal to 55 starting with X[0]
 *               On output check symbols will occupy X[55..62] 
 *               All arrays must start on even address boundaries
 *
 *
 * Used  registers:     m5=0, m6=1, m7=-1
 * Saved registers:     r1, r2, r3, r13, r14, i0, i1, i2, i3, i4, i5, m1, m3, m4, mode1
 * Destroyed registers: r0,  r4, r8, r12
 * Called functions :   IFFT63(), FFT63_8()
 *
 * REVISION HISTORY:
 * 1     08.09.04 vgp
 *    16-bit addressing support
 * 2     12.09.04  vgp
 *     16-bit addressing mode for logarithmic and power tables
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/RS_FEC/RS6355_Decoder.asm $
 * 
 * 1     19.01.05 15:37 Chchlee
 * 
 * 4     30.09.04 8:27 Leeb02
 * 
 * 
 * 1     23.01.04 vgp
 * Initial version Complexity 10000 cycles
 * Maximal complexity: 8 erasures and 0 errors - 10241 cycles
 *                     4 erasures and 0 errors -  8311
 *                     6 erasures and 1 error  - 10100
 *
 *                     0 erasures and 4 errors - 9233
 *                                    3 errors - 8458
 *                                    2 errors - 7659
 *                                    1 error  - 6781
 *                                    0 errors - 1849
 *
 *****************************************************************************/

#include <asm_sprt.h>
#include "../../h/rs_fec/RS6355_defs.h"

#define nroots  RS6355_R  /* code distance = nroots+1  */
#define nroots2 (nroots+2)

#ifdef __ADSP21160__
#include <def21160.h>
#endif
#ifdef __ADSP21161__
#include <def21161.h>
#endif

.extern _FFT63_8;
.extern _IFFT63;

//==============================================
.segment/dm	seg_dmda;

.extern pwrGF64;
.extern logGF64;
.global Lo_ptr;
.global Mo_ptr;

.var ro;     // number of erasures
.var r;
.var LxT_ptr;         // points array of 2*(RS6355_R+2) =  20 words
.var Bx_ptr;          // points array of (RS6355_R+2) =  10 words
.var S_ptr;           // points array of GF64 =  64 words (spectrum)
.var E_ptr;           // points array of GF64 =  64 words (spectrum) 
.var Lo_ptr;          // points array of 472 words (spectrum calculation) 
.var Mo_ptr;          // points array of 98 words (spectrum calculation)
                      // Total: 3*(RS6355_R+2) + 2*GF64 + 570 = 728 words
.endseg;

//==============================================

//		.segment/pm   seg_pmco;
		.segment/pm   seg_ext_pmco;

		.global _RS6355_Decoder;

// m5, m13 must be 0
// m6, m14 must be 1
// m7, m15 must be -1

//  r4 = address of   Y[0..62] in dm  - input (received codewode) 
//  r8 = address of Loc[0.. 9] in dm  - input (locators of erasures, -1 termination symbol)
// r12 = address of   D[0..62] in dm  - output (decoded codeword)

_RS6355_Decoder:

    leaf_entry;

    puts = r1;           // 18
	puts = r2;           // 17
	puts = r3;           // 16
	puts = r13;          // 15
	puts = r14;          // 14
    r0 = i0; puts = r0;  // 13
    r0 = i1; puts = r0;  // 12
    r0 = i2; puts = r0;  // 11
    r0 = i3; puts = r0;  // 10
    r0 = i4; puts = r0;  // 9
    r0 = i5; puts = r0;  // 8
    r0 = m1; puts = r0;  // 7
    r0 = m3; puts = r0;  // 6
    r0 = m4; puts = r0;  // 5
	r0 = mode1; puts = r0;  // 4

    puts = r4;           // 3 address of Y[]
	puts = r8;           // 2 address of Loc[]
	puts = r12;          // 1 address of D[]

	bit set mode1 SSE;
	
// pointer initialization	
	r0 = reads(1);
	dm(LxT_ptr) = r0;
	r1 = 2*(RS6355_R+2);
	r0 = r0 + r1;
	dm(Bx_ptr) = r0;
	r1 = RS6355_R+2;
	r0 = r0 + r1;
	dm(S_ptr) = r0;
	r1 = GF64;
	r0 = r0 + r1;
	dm(E_ptr) = r0;
	r0 = r0 + r1;
	dm(Lo_ptr) = r0;
	r1 = 472;
	r0 = r0 + r1;
	dm(Mo_ptr) = r0;
	
//==== Syndrome calculation
    r8 = dm(S_ptr);
	ccall(_FFT63_8);

//==== Initialization
//  Lx = &LxT[0];
//	T  = &LxT[nroots2];
    r0 = dm(LxT_ptr);
	r14 = nroots2;
	r14 = r14 + r0, i0 = r0;
    r13 = r0;             // r13 points Lx

//	Lx[0] = 1;
    r0 = r0 - r0, i1=i0;
    lcntr = nroots2, do (pc,ClrLx) until lce;
ClrLx:   dm( i0, m6 ) = r0;
    r0 = r0 + 1;
    dm(i1,m5) = r0;            // Lx[0] = 1

//==== Berlecamp-Massey algorithm (BMA)
// Computation in Lx polynomial of locators of erasures of Basic word -----
    i1 = gets( 2 );                 // !!! from stack !!! i1 = address of Loc[0] 
	r0 = r0 - r0,  r1 = dm(i1,m6);  // ro = 0,  r1 = Loc[0]
    r2 = r1 + 1;                    // test Loc[0] for -1
	if eq jump DoLx2Bx(db);
    i3 = 2*logGF64;            // address of logarithmic table in 16 bit addressing mode
	i4 = 2*pwrGF64+GF64_2;   // address of power table in 16 bit addressing mode
	r0 = dm(LxT_ptr);
	r12 = nroots+1;
	r12 = r12 + r0;
	r0 = r0 - r0, i0 = r12;    // address of Lx[nroots+1], j = nroots+1
InitLx: 
        // L(x) = Lx - a*xL(x)

        LCNTR = nroots+1, do (pc,LxaLx) until lce;
		  m3 = dm(m7,i0);   // Lx[j-1]
		  r2 = dm(m3,i3);   // r2 = log(Lx[j-1])
		  r2 = r2 + r1, r8 = dm(i0,m5);    // r8 = Lx[j]
		  m1 = r2;
		  r2 = dm(m1,i4);     // pwr[ 2*GF + Loc[ro] + lgr[Lx[j-1]]] 
          r8 = r8 xor r2;
LxaLx:    dm(i0,m7) = r8;     // store Lx[j]; j--
        r1 = dm(i1,m6);       // ro = ro + 1, read next Loc[i++]
        r2 = r1 + 1;          // test r1 for -1
	 if ne jump InitLx(db);
	 r0 = r0 + 1;
     i0 = r12;

	 r2 = nroots;
	 r2 = r2 - r0;
	 if lt jump error_ret;   // error_return: number of errasures more than nroots
// copy Lx to Bx
DoLx2Bx:
	dm(ro) = r0;
	r0 = r0 + 1, r8 = r0;                // r = r0, L = r8
	dm(r)= r0;
    i1 = dm(LxT_ptr);
    i0 = dm(Bx_ptr);
	lcntr = nroots2, do (pc,Lx2Bx) until lce;
         r1 = dm(i1,m6);
Lx2Bx:   dm( i0, m6 ) = r1;

    r2 = nroots+1;
	r2 = r2 - r0;   //nroots - ro + 1
    if eq jump BMAend(db);
    i3 = 2*logGF64;            // address of logarithmic table in 16 bit addressing mode
    i4 = 2*pwrGF64+GF64_2;      // address of power table in dm in 16 bit addressing mode
// Computation of polinomial of error locators

    i5 = 2*pwrGF64+3*GF64-1;      // address of power table for Inverse(delta) calculation
    lcntr = r2, do (pc,ErLocClc) until lce;

      // delta calculation 
       i1 = r13;               //r13 points Lx[0]
	   r1 = dm(S_ptr);
	   r1 = r1 - 1;
	   r2 = r1 + r0, r1 = dm(i1,m6);  // r2 = address of S[r-1], Lx[j]
	   m3 = r1;
	   r1 = r1 - r1, i2 = r2;  // delta
	   r2 = dm(m3,i3);         // r2 = log(Lx[j])
       r3 = dm(i2,m7);         // S[(ro-1)]
       m3 = r3;
	   r3 = dm(m3,i3);         // r2 = log(S[(ro-1-j)])
	   r2 = r2 + r3;           // log(Lx[j]) + log(S[(ro-1-j)])
	   r8 = pass r8, m1 = r2;
	   r2 = dm(m1,i4);         // pwr[ 2*GF + lgr[Lx[j]] + lgr[ S[(r-1-j)]] ]
	   if eq jump TestDelta;
       lcntr = r8, do (pc,DeltaClc) until lce;
		  r1 = r1 xor r2, r2 = dm(i1,m6);   // Lx[j]
		  m3 = r2;
		  r2 = dm(m3,i3);    // r2 = log(Lx[j])
          r3 = dm(i2,m7);    // S[(ro-1-j)]
          m3 = r3;
		  r3 = dm(m3,i3);    // r2 = log(S[(ro-1-j)])
		  r2 = r2 + r3;      // log(Lx[j]) + log(S[(ro-1-j)])
		  m1 = r2;
DeltaClc: r2 = dm(m1,i4);    // pwr[ 2*GF + lgr[Lx[j]] + lgr[ S[(r-1-j)]] ]
      
      // log(Inverse(delta)) calculating
TestDelta:
       r1 = r1 xor r2;
       if eq jump ZeroDelta;
	     m3 = r1;            // m3 = delta
         r1 = dm(m3,i3);     // r1 = log(delta)
         r1 = -r1;           // r1 = -log[delta)
		 m1 = r1;
         r1 = -r1, m3 = dm(m1,i5);    //r1 = log(delta), m3 = Inverse(delta)=pwr[ 2*GF + GF - 1 - log_delta ];
         r2 = dm(m3,i3);     // r2 = log(Inverse(delta))

         // r0 = ro
		 // r1 = log(delta)
		 // r2 = log(Inverse(delta))
         // i3 = 2*logGF64;           - address of logarithmic table
         // i4 = 2*pwrGF64+2*GF;      - address of power table in dm
         r3 = dm(Bx_ptr);
         r4 = nroots;
         r4 = r4 + r3;
         i2 = r4;             // address of Bx[nroots]
         r4 = nroots2-1;
		 r3 = r14 + r4;       // r14 points T[0] 
		 i0 = r3;             // address of T[nroots2-1]
		 r3 = r13 + r4, r4 = dm(i2,m7);      // r13 points Lx[0], Bx[j-1]
		 m3 = r4;
		 i1 = r3;             // address of Lx[nroots2-1]
		 r3  = dm(m3,i3);     // log[Bx[j-1]]
		 r3  = r3 + r1, r4 = dm(i1,m7);  // log[Bx[j-1]] + log(delta)
         m1 = r3;
		 r3 = dm(m1,i4);       // r3 = pwr[ 2*GF + lgr[Bx[j-1]] + log(delta) ]
         lcntr = nroots, do (pc,LxdeltaxBx) until lce;
		    r3 = r3 xor r4, r4 = dm(i2,m7);      //Lx[j] ^ pwr[ 2*GF + lgr[Bx[j-1]] + log(delta), Bx[j-1]
		    m3 = r4;
			dm(i0,m7) = r3;
			r3  = dm(m3,i3);    // log[Bx[j-1]]
			r3  = r3 + r1, r4 = dm(i1,m7);  // log[Bx[j-1]] + log(delta)
            m1 = r3;
LxdeltaxBx:	r3 = dm(m1,i4);       // r3 = pwr[ 2*GF + lgr[Bx[j-1]] + log(delta) ]
		 r3 = r3 xor r4, r4 = dm(i1,m5);   // Lx[j] ^ pwr[ 2*GF + lgr[Bx[j-1]] + log(delta) ]; Lx[0]
		 r1 = r8 + r8, dm(i0,m7) = r3;     // r1 = 2L
		 r12 = dm(ro);                     // r12 = ro **
         r3 = r12 - 1, dm(i0,m5) = r4;     // T[0] = Lx[0], ro - 1
		 r3 = r3 + r0;                     // r+ro-1
		 r3 = r3 - r1;
		 if lt jump TwoLLErro1; 
		   i0 = dm(Bx_ptr);
		   r1 = r12 + r0, i1 = r13;  // ro + r, r13 points Lx[0]
           r1 = r1 - r8;
		   r8 = r1;               // r8 = L = ro + r - L
           lcntr = nroots2, do (pc,Bxdelta1Lx) until lce;
		     r3 = dm(i1,m6);      // Lx[j]
		     m3 = r3;
			 r3 = dm(m3,i3);    // log(Lx[j])
			 r3 = r3 + r2;        // log(Lx[j]) + log(Inverse(delta))
			 m1 = r3;
			 r3 = dm(m1,i4);
Bxdelta1Lx:  dm(i0,m6) = r3;           
           jump SwapLxT;
TwoLLErro1:
         //  B(x) = xB(x) 
           r3 = dm(Bx_ptr);
           r1 = nroots2-1;
           r1 = r1 + r3;
           i0 = r1;         // i0 = &Bx[nroots2-1]
		   r1 = nroots;
		   r1 = r3 + r1;
		   i1 = r1;         // i1 = &Bx[nroots]
           lcntr = nroots2-1, do (pc,BxxBx) until lce;
		     r1 = dm(i1,m7);
BxxBx:       dm(i0,m7) = r1;
           r1 = r1 - r1;
           dm(i0,m5) = r1;
		 
SwapLxT:
         r1 = r13;            // r13 points Lx[0]
		 jump ContClc (db);
		 r13 = r14;           // r14 points T[0]
		 r14 = r1;
ZeroDelta:
         //  B(x) = xB(x)   
          r3 = dm(Bx_ptr);
          r1 = nroots2-1;
          r1 = r1 + r3;
          i0 = r1;            // i0 = &Bx[nroots2-1]
		  r1 = nroots;
		  r1 = r1 + r3;
		  i1 = r1;            // i1 = &Bx[nroots]
           lcntr = nroots2-1, do (pc,BxxBx1) until lce;
		     r1 = dm(i1,m7);
BxxBx1:       dm(i0,m7) = r1;
           r1 = r1 - r1;
           dm(i0,m5) = r1;
ContClc:
          nop;
ErLocClc: r0 = r0 + 1;       // r++;
//============================  End of BMA    
BMAend:
// Can decoder correct or not? (failure if deg L(x) != L

    r2 = nroots2-1;
	r1 = r13 + r2;          // r13 points Lx[0]
	i1 = r1;
	r1 = dm(i1,m7);
TestLx: r1 = pass r1;
        if ne jump TestL;
		jump TestLx (db);
		r2 = r2 - 1;
		r1 = dm(i1,m7);
    
TestL:
    // test that L = r8 is equal to r2
    r3 = r8 - r2;
	if ne jump error_ret;     // incorrectable errors
	
	r8 = pass r8;
	if eq jump No_Errors;    // 24.03.03 vgp; no errors to corrections

// Recursive extension of syndromes --------------------------------
//  r0 = r
//  r8 = L
    r13 = r13 + 1;             // r13 points Lx[1]

// calculate log(Lx[j]) for j=1..L
	i0 = r13;
    lcntr = r8, do (pc,LogLx1) until lce;
		r3 = dm(i0,m5);     // Lx[j]
		m3 = r3;
		r3 = dm(m3,i3);     // log[Lx[j]]
LogLx1: dm(i0,m6) = r3;     // store log[Lx[j]] in Lx[j]

    r2 = GF64;
	r2 = r2 - r0;              // GF-r
    r3 = dm(S_ptr);
    r3 = r3 - 1;
	r0 = r3 + r0;              // r0 = address of S[r-1]
	r1 = r8-1, i0 = r0;        // r1 = L-1
    lcntr = r2, do (pc,RecExt1) until lce;
	    i1 = r0;               // i0 points S[r-1]
		modify(i1,m7);         // i1 points S[r-2]
		i2 = r13;              // r13 points Lx[1]
	    r3 = dm(i2,m6);        // log(Lx[j])
	    r4 = dm(i1,m7);        // S[r-2-j]
	    m3 = r4;
	    r4 = dm(m3,i3);        // log[S[r-2-j]]
	    r3 = r3 + r4;
		r1 = pass r1, m1 = r3;
	    if eq jump RecExt0 (db);
	    r3 = dm(m1,i4);        // pwr[ 2*GF+(log[Lx[j]] + log[S[r-1-j]]) ]
		r2 = r2 - r2;
        lcntr = r1, do (pc,RecExt) until lce;
		  r2 = r2 xor r3, r3 = dm(i2,m6);      // log(Lx[j])
		  r4 = dm(i1,m7);      // S[r-2-j]
		  m3 = r4;
		  r4 = dm(m3,i3);      // log[S[r-2-j]]
		  r3 = r3 + r4;
		  m1 = r3;
RecExt:   r3 = dm(m1,i4);      // pwr[ 2*GF+(log[Lx[j]] + log[S[r-1-j]]) ]
RecExt0:  r2 = r2 xor r3, i0=r0;
RecExt1: r0 = r0 + 1, dm(i0,m6) = r2;    

//Inverse Fourier transform S -> e  (error vector) 4844 cycles
//==== Syndrome calculation
    r4 = dm(S_ptr);
	r8 = dm(E_ptr);
	ccall(_IFFT63);

// restore calling registers
//  i1 = address of   Y[0..62] in dm  - input (received codewode) 
//  i0 = address of   D[0..62] in dm  - output (decoded codeword)
    i1  = gets( 3 );
    i0  = gets( 1 );
//Reconstruction of codeword d in time domain  --------------------------------------------
    i2 = dm(E_ptr);
    lcntr = GF64-1, do (pc,Reconst) until lce;
	     r0 = dm(i1,m6);
	     r1 = dm(i2,m6);
	     r0 = r0 xor r1;
Reconst: dm(i0,m6) = r0;

    jump normal_ret;

No_Errors:
    i1  = gets( 3 );
    i0  = gets( 1 );
    lcntr = GF64-1, do (pc,CopyLp) until lce;
	     r0 = dm(i1,m6);
CopyLp:  dm(i0,m6) = r0;

    jump normal_ret;
    
error_ret:
    i1  = gets( 3 );
    i0  = gets( 1 );
    lcntr = GF64-1, do (pc,CopyLpEr) until lce;
	      r0 = dm(i1,m6);
CopyLpEr: dm(i0,m6) = r0;
    r0 = 1;
	jump epil;

normal_ret:
    r0 = r0 - r0;

epil:
    r1  = gets( 18 );
    r2  = gets( 17 );
    r3  = gets( 16 );
    r13 = gets( 15 );
    r14 = gets( 14 );
    i0  = gets( 13 );
    i1  = gets( 12 );
    i2  = gets( 11 );
    i3  = gets( 10 );
    i4  = gets( 9 );
    i5  = gets( 8 );
    m1  = gets( 7 );
    m3  = gets( 6 );
    m4  = gets( 5 );
    mode1  = gets( 4 );

    leaf_exit; 

_RS6355_Decoder.end:    
    
.endseg;

