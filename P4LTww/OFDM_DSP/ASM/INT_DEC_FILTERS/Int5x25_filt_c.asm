/******************************************************************************
 *
 * PROJECT     : ETL600 COFDM Modem for 2116x
 * FILE        : $Workfile: Int5x25_filt_c.asm $
 * COPYRIGHT   : 2006 by TL Consulting GmbH.
 * DESCRIPTION : 25 tap Interpolator of complex signal by 5 using 5 polyphase filters
 * OPERATION   : 
 * PROTOTYPE   : void Int5x25_filt_c( complex_float    vOut[],         r4
 *               complex_float    vIn[],             r8
 *               int              nIn,               r12
 *               complex_float pm vCoeffs[5*5],       reads( 1 )
 *               complex_float    vDelay[5+1] );    reads( 2 )
 *
 * ASSUMPTIONS : all arrays starts with even address and there length is also even
 *
 * REVISION HISTORY:
 * COMPLEXITY : 
 *-----------------------------------------------------------------------------
 * 
 * 1     27-10-03  VGP
 * Initial version
 * 2     10-11-04  VGP
 * Save r2
 * 3     20-01-06 PL
 * Version 5x25 based on 4x24
 * 
 *****************************************************************************/

#ifdef __ADSP21160__
#include <def21160.h>
#endif
#ifdef __ADSP21161__
#include <def21161.h>
#endif

#include    <asm_sprt.h>

#define NUL_DM           m5
#define INC_DM           m6
#define DEC_DM           m7

#define NUL_PM           m13
#define INC_PM           m14
#define DEC_PM           m15

.global _Int5x25_filt_c;

//		.segment/pm   seg_pmco;
		.segment/pm   seg_ext_pmco;

_Int5x25_filt_c:

      leaf_entry;

      r0 = i0; puts = r0;
      r0 = i1; puts = r0;
      r0 = i2; puts = r0;
      r0 = i8; puts = r0;
      r0 = i9; puts = r0;
      r0 = i10; puts = r0;
      r0 = i11; puts = r0;
      r0 = i12; puts = r0; //PL
      
      r0 = m2; puts = r0;
      r0 = m8; puts = r0;
      puts = r2;
      puts = r5;
      puts = r9;
      puts = r13;
      puts = r14;
      puts = r15;

      m8 = 2;
      m2 = 2;   

// i2 - address of input buffer vIn[]
      i2 = r8;              /* Address of input buffer vIn[] */


// Delay line
      b0 = reads(2);        /* Delay line address of filter #0 */
      r0 = 10;              /* Delay line length in real samples =2*5*/
      r1 = dm(11,i0);		/* PL 2*5+1 due to complex?*/
// restore i0
      i0 = r1;              /* Get pointer to delay line */
      l0 = r0;              /* Delay line length in real samples*/
      

// Filter coefficients 
      r1 = reads(1);        /* Address of vCoeffs0[n_taps] */
      b8 = r1;              /* Filter coefficients for filter #0 */
      r0 = 10;              /* Number of subfilter coefficients in real samples*/
      r1 = r1 + r0;         
      b9 = r1;              /* Filter coefficients for filter #1 */
      r1 = r1 + r0;         
      b10 = r1;             /* Filter coefficients for filter #2 */
      r1 = r1 + r0;         
      b11 = r1;             /* Filter coefficients for filter #3 */
	  r1 = r1 + r0;         
      b12 = r1;             /* Filter coefficients for filter #4 */
      
      l8 = r0;              
      l9 = r0;              
      l10 = r0;              
      l11 = r0;              
	  l12 = r0;              

// i1 - address of output buffer vOut[]      
      i1 = r4;              /* Address of vOut[] */
      r2 = r12-1;           /* number of samples -1 */
      
      bit set MODE1 CBUFEN | PEYEN;       // Circular Buffer Enable 
      nop;

// f12 - output sample of filt #0
// f13 - output sample of filt #1
// f14 - output sample of filt #2
// f15 - output sample of filt #3
// f11 - output sample of filt #4

    
         // delay 0
      f0 = dm(i2, m2), f4 = pm(i8,m8);   // f0 - read sample, coef0 filt #0         
      f12 = f0*f4, dm(i0,m2) = f0, f4 = pm(i9,m8); // store input sample in delay line, f4 coef0 filt #1
      f13 = f0*f4, f4 = pm(i10,m8);                // f4 coef0 filt #2
      f14 = f0*f4, f4 = pm(i11,m8);                // f4 coef0 filt #3
      f15 = f0*f4, f4 = pm(i12,m8);                // f4 coef0 filt #4
      f11 = f0*f4;
      
      lcntr = r2, do fir until lce; /* loop per sample */
         // delay 1
         f0 = dm(i0,m2), f4=pm(i8,m8);
         f5 = pm(i9,m8);
         f9 = f0*f5, f5 = pm(i10,m8);
         f9 = f0*f5, f13 = f9+f13, f5 = pm(i11,m8);
         f9 = f0*f5, f14 = f9+f14, f5 = pm(i12,m8);
         f9 = f0*f5, f15 = f9+f15;
         f11 = f9+f11;
         // delay 2
         f8 = f0*f4, f0 = dm(i0,m2), f4=pm(i8,m8);       // delay 2
         f5 = pm(i9,m8);
         f9 = f0*f5, f5 = pm(i10,m8);
         f9 = f0*f5, f13 = f9+f13, f5 = pm(i11,m8);
         f9 = f0*f5, f14 = f9+f14, f5 = pm(i12,m8);
         f9 = f0*f5, f15 = f9+f15;
         f11 = f9+f11;
         // delay 3         
         f8 = f0*f4, f12 = f8 + f12, f0 = dm(i0,m2), f4=pm(i8,m8); // delay 3
         f5 = pm(i9,m8);
         f9 = f0*f5, f5 = pm(i10,m8);
         f9 = f0*f5, f13 = f9+f13, f5 = pm(i11,m8);
         f9 = f0*f5, f14 = f9+f14, f5 = pm(i12,m8);
         f9 = f0*f5, f15 = f9+f15;
         f11 = f9+f11;
         // delay 4        
         f8 = f0*f4, f12 = f8 + f12, f0 = dm(i0,NUL_DM), f4=pm(i8,m8); // delay 3
         f5 = pm(i9,m8);
         f9 = f0*f5, f5 = pm(i10,m8);
         f9 = f0*f5, f13 = f9+f13, f5 = pm(i11,m8);
         f9 = f0*f5, f14 = f9+f14, f5 = pm(i12,m8);
         f9 = f0*f5, f15 = f9+f15;
         f11 = f9+f11;
         // delay 0
         f8 = f0*f4, f12 = f8 + f12, f0 = dm(i2,m2), f4=pm(i8,m8);     // f0, read sample
         f12 = f8 + f12, dm(i0,m2) = f0;
         f12 = f0*f4, dm(i1,m2) = f12, f5 = pm(i9,m8);
         f13 = f0*f5, dm(i1,m2) = f13, f5 = pm(i10,m8);
         f14 = f0*f5, dm(i1,m2) = f14, f5 = pm(i11,m8);
    	 f15 = f0*f5, dm(i1,m2) = f15, f5 = pm(i12,m8);
fir:	 f11 = f0*f5, dm(i1,m2) = f11;
      // delay 1
      f0 = dm(i0,m2), f4=pm(i8,m8);
      f5 = pm(i9,m8);
      f9 = f0*f5, f5 = pm(i10,m8);
      f9 = f0*f5, f13 = f9+f13, f5 = pm(i11,m8);
      f9 = f0*f5, f14 = f9+f14, f5 = pm(i12,m8);
      f9 = f0*f5, f15 = f9+f15;
      f11 = f9+f11;
      // delay 2
      f8 = f0*f4, f0 = dm(i0,m2), f4=pm(i8,m8); 
      f5 = pm(i9,m8);
      f9 = f0*f5, f5 = pm(i10,m8);
      f9 = f0*f5, f13 = f9+f13, f5 = pm(i11,m8);
      f9 = f0*f5, f14 = f9+f14, f5 = pm(i12,m8);
      f9 = f0*f5, f15 = f9+f15;
      f11 = f9+f11;
      // delay 3               
      f8 = f0*f4, f12 = f8 + f12, f0 = dm(i0,m2), f4=pm(i8,m8); // delay 3
      f5 = pm(i9,m8);
      f9 = f0*f5, f5 = pm(i10,m8);
      f9 = f0*f5, f13 = f9+f13, f5 = pm(i11,m8);
      f9 = f0*f5, f14 = f9+f14, f5 = pm(i12,m8);
      f9 = f0*f5, f15 = f9+f15;
      f11 = f9+f11;
      // delay 4               
      f8 = f0*f4, f12 = f8 + f12, f0 = dm(i0,NUL_DM), f4=pm(i8,m8); // delay 3
      f5 = pm(i9,m8);
      f9 = f0*f5, f5 = pm(i10,m8);
      f9 = f0*f5, f13 = f9+f13, f5 = pm(i11,m8);
      f9 = f0*f5, f14 = f9+f14, f5 = pm(i12,m8);
      f9 = f0*f5, f15 = f9+f15;
      f11 = f9+f11;

      
      f8 = f0*f4, f12 = f8 + f12;    
      f12 = f8 + f12;
      dm(i1,m2) = f12; 
      dm(i1,m2) = f13; 
      dm(i1,m2) = f14; 
      dm(i1,m2) = f15; 
	  dm(i1,m2) = f11; 

      bit clr MODE1 CBUFEN | PEYEN; 
      nop;

//store i0
      i1 = reads(2);        /* Delay line address of filter #0 */
      r0 = i0;
      dm(11,i1) = r0;
      
      l0 = 0;
      l8 = 0;              
      l9 = 0;              
      l10 = 0;              
      l11 = 0; 
      l12 = 0;              

      i0 = gets( 16 );
      i1 = gets( 15 );
      i2 = gets( 14 );
      i8 = gets( 13 );
      i9 = gets( 12 );
      i10 = gets( 11 );
      i11 = gets( 10 );
      i12 = gets( 9 );
      m2 = gets( 8 );
      m8 = gets( 7 );
      r2 = gets( 6 );
      r5 = gets( 5 );
      r9 = gets( 4 );
      r13= gets( 3 );
      r14= gets( 2 );
      r15= gets( 1 );
      
      leaf_exit;

_Int5x25_filt_c.end:
.endseg;

