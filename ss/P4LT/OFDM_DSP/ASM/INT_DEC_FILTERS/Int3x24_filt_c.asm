/******************************************************************************
 *
 * PROJECT     : DRS7 COFDM Modem for 2116x
 * FILE        : $Workfile: Int3x24_filt_c.asm $
 * COPYRIGHT   : 2004 by TL Consulting GmbH.
 * DESCRIPTION : 24 tap interpolator of complex signal by 3
 * OPERATION   : 
 * PROTOTYPE   : void Int3x24_filt_c( complex_float    vOut[], 
 *                                   complex_float    vIn[],             
 *                                   int              nIn,               
 *                                   complex_float pm vCoeffs[3*8],       
 *                                   complex_float    vDelay[8+1] );    
 *
 * ASSUMPTIONS : all arrays starts with even address and there length is also even
 *               24 coefficients per stage ( overall 24 coefficients )
 *
 * REVISION HISTORY:
 * COMPLEXITY : 
 *-----------------------------------------------------------------------------
 * 
 * 1     18-04-04  VGP
 * Initial version
 * 2     10-11-04  VGP
 * Save r2
 * 
 *****************************************************************************/

#ifdef __ADSP21160__
#include <def21160.h>
#endif
#ifdef __ADSP21161__
#include <def21161.h>
#endif
#ifdef __ADSP21162__
#include <def21162.h>
#endif

#include    <asm_sprt.h>


.global _Int3x24_filt_c;

//		.segment/pm   seg_pmco;
		.segment/pm   seg_ext_pmco;

_Int3x24_filt_c:

      leaf_entry;

      r0 = i0; puts = r0;
      r0 = i1; puts = r0;
      r0 = i2; puts = r0;
      r0 = i8; puts = r0;
      r0 = i9; puts = r0;
      r0 = i10; puts = r0;
      r0 = i11; puts = r0;
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
      r0 = 16;              /* Delay line length in real samples*/
      r1 = dm(17,i0);
// restore i0
      i0 = r1;              /* Get pointer to delay line */
      l0 = r0;              /* Delay line length in real samples*/
      

// Filter coefficients 
      r1 = reads(1);        /* Address of vCoeffs0[n_taps] */
      b8 = r1;              /* Filter coefficients for filter #0 */
      r0 = 16;              /* Number of subfilter coefficients in real samples*/
      r1 = r1 + r0;         
      b9 = r1;              /* Filter coefficients for filter #1 */
      r1 = r1 + r0;         
      b10 = r1;             /* Filter coefficients for filter #2 */

      l8 = r0;              
      l9 = r0;              
      l10 = r0;              

// i1 - address of output buffer vOut[]      
      i1 = r4;              /* Address of vOut[] */
      r2 = r12-1;           /* number of samples -1 */
      
      bit set MODE1 CBUFEN | PEYEN;       // Circular Buffer Enable 
      nop;

// f12 - output sample of filt #0
// f13 - output sample of filt #1
// f14 - output sample of filt #2
    
         // delay 0
      f0 = dm(i2, m2), f4 = pm(i8,m8);   // f0 - read sample, coef filt #0         
      f12 = f0*f4, dm(i0,m2) = f0, f4 = pm(i9,m8); // store input sample in delay line, f4 coef filt #1
      f13 = f0*f4, f4 = pm(i10,m8);                // f4 coef filt #2
      f14 = f0*f4;

      lcntr = r2, do fir until lce; /* loop per sample */
         // delay 1
         f0 = dm(i0,m2), f4=pm(i8,m8);
         f5 = pm(i9,m8);
         f9 = f0*f5, f5 = pm(i10,m8);
         f9 = f0*f5, f13 = f9+f13;

         // delay 2
         f8 = f0*f4, f14 = f9+f14, f0 = dm(i0,m2), f4=pm(i8,m8);       
         f5 = pm(i9,m8);
         f9 = f0*f5, f5 = pm(i10,m8);
         f9 = f0*f5, f13 = f9+f13;
         f14 = f9+f14;

         // delay 3         
         f8 = f0*f4, f12 = f8 + f12, f0 = dm(i0,m2), f4=pm(i8,m8); 
         f5 = pm(i9,m8);
         f9 = f0*f5, f5 = pm(i10,m8);
         f9 = f0*f5, f13 = f9+f13;
         f14 = f9+f14;

         // delay 4         
         f8 = f0*f4, f12 = f8 + f12, f0 = dm(i0,m2), f4=pm(i8,m8); 
         f5 = pm(i9,m8);
         f9 = f0*f5, f5 = pm(i10,m8);
         f9 = f0*f5, f13 = f9+f13;
         f14 = f9+f14;

         // delay 5        
         f8 = f0*f4, f12 = f8 + f12, f0 = dm(i0,m2), f4=pm(i8,m8); 
         f5 = pm(i9,m8);
         f9 = f0*f5, f5 = pm(i10,m8);
         f9 = f0*f5, f13 = f9+f13;
         f14 = f9+f14;

         // delay 6        
         f8 = f0*f4, f12 = f8 + f12, f0 = dm(i0,m2), f4=pm(i8,m8); 
         f5 = pm(i9,m8);
         f9 = f0*f5, f5 = pm(i10,m8);
         f9 = f0*f5, f13 = f9+f13;
         f14 = f9+f14;

         // delay 7        
         f8 = f0*f4, f12 = f8 + f12, f0 = dm(i0,m5), f4=pm(i8,m8); 
         f5 = pm(i9,m8);
         f9 = f0*f5, f5 = pm(i10,m8);
         f9 = f0*f5, f13 = f9+f13;
         f14 = f9+f14;

         // delay 0
         f8 = f0*f4, f12 = f8 + f12, f0 = dm(i2,m2), f4=pm(i8,m8);     // f0, read sample
         f12 = f8 + f12, dm(i0,m2) = f0;
         f12 = f0*f4, dm(i1,m2) = f12, f5 = pm(i9,m8);
         f13 = f0*f5, dm(i1,m2) = f13, f5 = pm(i10,m8);
fir:     f14 = f0*f5, dm(i1,m2) = f14;

      // delay 1
      f0 = dm(i0,m2), f4=pm(i8,m8);
      f5 = pm(i9,m8);
      f9 = f0*f5, f5 = pm(i10,m8);
      f9 = f0*f5, f13 = f9+f13;

      // delay 2
      f8 = f0*f4, f14 = f9+f14, f0 = dm(i0,m2), f4=pm(i8,m8); 
      f5 = pm(i9,m8);
      f9 = f0*f5, f5 = pm(i10,m8);
      f9 = f0*f5, f13 = f9+f13;
      f14 = f9+f14;

      // delay 3               
      f8 = f0*f4, f12 = f8 + f12, f0 = dm(i0,m2), f4=pm(i8,m8); 
      f5 = pm(i9,m8);
      f9 = f0*f5, f5 = pm(i10,m8);
      f9 = f0*f5, f13 = f9+f13;
      f14 = f9+f14;

      // delay 4               
      f8 = f0*f4, f12 = f8 + f12, f0 = dm(i0,m2), f4=pm(i8,m8); 
      f5 = pm(i9,m8);
      f9 = f0*f5, f5 = pm(i10,m8);
      f9 = f0*f5, f13 = f9+f13;
      f14 = f9+f14;

      // delay 5               
      f8 = f0*f4, f12 = f8 + f12, f0 = dm(i0,m2), f4=pm(i8,m8); 
      f5 = pm(i9,m8);
      f9 = f0*f5, f5 = pm(i10,m8);
      f9 = f0*f5, f13 = f9+f13;
      f14 = f9+f14;

      // delay 6               
      f8 = f0*f4, f12 = f8 + f12, f0 = dm(i0,m2), f4=pm(i8,m8); 
      f5 = pm(i9,m8);
      f9 = f0*f5, f5 = pm(i10,m8);
      f9 = f0*f5, f13 = f9+f13;
      f14 = f9+f14;

      // delay 7               
      f8 = f0*f4, f12 = f8 + f12, f0 = dm(i0,m5), f4=pm(i8,m8); 
      f5 = pm(i9,m8);
      f9 = f0*f5, f5 = pm(i10,m8);
      f9 = f0*f5, f13 = f9+f13;
      f14 = f9+f14;
      
      f8 = f0*f4, f12 = f8 + f12;    
      f12 = f8 + f12;
      dm(i1,m2) = f12; 
      dm(i1,m2) = f13; 
      dm(i1,m2) = f14; 

      bit clr MODE1 CBUFEN | PEYEN; 
      nop;

//store i0
      i1 = reads(2);        /* Delay line address of filter #0 */
      r0 = i0;
      dm(17,i1) = r0;
      
      l0 = 0;
      l8 = 0;              
      l9 = 0;              
      l10 = 0;              

      i0  = gets( 15 );
      i1  = gets( 14 );
      i2  = gets( 13 );
      i8  = gets( 12 );
      i9  = gets( 11 );
      i10 = gets( 10 );
      i11 = gets( 9 );
      m2  = gets( 8 );
      m8  = gets( 7 );
      r2  = gets( 6 );
      r5  = gets( 5 );
      r9  = gets( 4 );
      r13 = gets( 3 );
      r14 = gets( 2 );
      r15 = gets( 1 );
      
      leaf_exit;


_Int3x24_filt_c.end:
.endseg;

