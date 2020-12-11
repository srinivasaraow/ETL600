/******************************************************************************
 *
 * PROJECT     : ETL600 COFDM Modem for 2116x
 * FILE        : $Workfile: Interp4_filt_c_L16.asm $
 * COPYRIGHT   : 2003 by TL Consulting GmbH.
 * DESCRIPTION : Interpolator of complex signal by 4 
 * OPERATION   : 
 * PROTOTYPE   : void Interp4_filt_c_L16( complex_float    vOut[],         r4
 *               complex_float    vIn[],             r8
 *               int              nIn,               r12
 *               complex_float pm vCoeffs[4*4],       reads( 1 )
 *               complex_float    vDelay[4+1] );    reads( 2 )
 *
 * ASSUMPTIONS : all arrays starts with even address and there length is also even
 *
 * REVISION HISTORY:
 * COMPLEXITY : 
 *-----------------------------------------------------------------------------
 * 
 * $Log: /Source/P4LT Prototype 2/UC_DSP/ASM/Interp4_filt_c_L16.asm $
 * 
 * 1     20.01.05 8:32 Chchlee
 * 
 * 1     28.04.04 16:57 Leeb02
 * 
 *
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

.global _Interp4_filt_c_L16;
.global _Interp4_filt_c_L16.end;

		.segment/pm   seg_pmco;

_Interp4_filt_c_L16:

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
      r0 = 8;               /* Delay line length in real samples*/
      r1 = dm(9,i0);
      i0 = r1;              /* Get pointer to delay line */

      l0 = r0;              /* Delay line length in complex samples*/
      
      // restore i0


// Filter coefficients 
      r1 = reads(1);        /* Address of vCoeffs0[n_taps] */
      b8 = r1;              /* Filter coefficients for filter #0 */
      r0 = 8;               /* Number of subfilter coefficients in real samples*/
      r1 = r1 + r0;         
      b9 = r1;              /* Filter coefficients for filter #1 */
      r1 = r1 + r0;         
      b10 = r1;             /* Filter coefficients for filter #2 */
      r1 = r1 + r0;         
      b11 = r1;             /* Filter coefficients for filter #3 */

      l8 = r0;              
      l9 = r0;              
      l10 = r0;              
      l11 = r0;              

// i1 - address of output buffer vOut[]      
      i1 = r4;              /* Address of vOut[] */
      r2 = r12-1;           /* number of samples -1 */
      
      bit set MODE1 CBUFEN | PEYEN;       // Circular Buffer Enable 
      nop;
      
         // delay 0
      f0 = dm(i2, m2), f4 = pm(i8,m8);   // 0 - read sample         
      f12 = f0*f4, dm(i0,m2) = f0, f4 = pm(i9,m8);
      f13 = f0*f4, f4 = pm(i10,m8);
      f14 = f0*f4, f4 = pm(i11,m8);
      f15 = f0*f4;
      lcntr = r2, do fir until lce; /* loop per sample */
         // delay 1
         f0 = dm(i0,m2), f4=pm(i8,m8);
         f5 = pm(i9,m8);
         f9 = f0*f5, f5 = pm(i10,m8);
         f9 = f0*f5, f13 = f9+f13, f5 = pm(i11,m8);
         f9 = f0*f5, f14 = f9+f14;
         // delay 2
         f8 = f0*f4, f15 = f9+f15, f0 = dm(i0,m2), f4=pm(i8,m8); // delay 2
         f5 = pm(i9,m8);
         f9 = f0*f5, f5 = pm(i10,m8);
         f9 = f0*f5, f13 = f9+f13, f5 = pm(i11,m8);
         f9 = f0*f5, f14 = f9+f14;
         f15 = f9+f15;
         // delay 3         
         f8 = f0*f4, f12 = f8 + f12, f0 = dm(i0,NUL_DM), f4=pm(i8,m8); // delay 3
         f5 = pm(i9,m8);
         f9 = f0*f5, f5 = pm(i10,m8);
         f9 = f0*f5, f13 = f9+f13, f5 = pm(i11,m8);
         f9 = f0*f5, f14 = f9+f14;
         f15 = f9+f15;
         // delay 0
         f8 = f0*f4, f12 = f8 + f12, f0 = dm(i2,m2), f4=pm(i8,m8);     // read sample
         f12 = f8 + f12, dm(i0,m2) = f0;
         f12 = f0*f4, dm(i1,m2) = f12, f5 = pm(i9,m8);
         f13 = f0*f5, dm(i1,m2) = f13, f5 = pm(i10,m8);
         f14 = f0*f5, dm(i1,m2) = f14, f5 = pm(i11,m8);
fir:     f15 = f0*f5, dm(i1,m2) = f15;
      // delay 1
      f0 = dm(i0,m2), f4=pm(i8,m8);
      f5 = pm(i9,m8);
      f9 = f0*f5, f5 = pm(i10,m8);
      f9 = f0*f5, f13 = f9+f13, f5 = pm(i11,m8);
      f9 = f0*f5, f14 = f9+f14;
      // delay 2
      f8 = f0*f4, f15 = f9+f15, f0 = dm(i0,m2), f4=pm(i8,m8); 
      f5 = pm(i9,m8);
      f9 = f0*f5, f5 = pm(i10,m8);
      f9 = f0*f5, f13 = f9+f13, f5 = pm(i11,m8);
      f9 = f0*f5, f14 = f9+f14;
      f15 = f9+f15;
      // delay 3               
      f8 = f0*f4, f12 = f8 + f12, f0 = dm(i0,NUL_DM), f4=pm(i8,m8); // delay 3
      f5 = pm(i9,m8);
      f9 = f0*f5, f5 = pm(i10,m8);
      f9 = f0*f5, f13 = f9+f13, f5 = pm(i11,m8);
      f9 = f0*f5, f14 = f9+f14;
      f15 = f9+f15;
      
      f8 = f0*f4, f12 = f8 + f12;    
      f12 = f8 + f12;
      dm(i1,m2) = f12; 
      dm(i1,m2) = f13; 
      dm(i1,m2) = f14; 
      dm(i1,m2) = f15; 

      bit clr MODE1 CBUFEN | PEYEN; 
      nop;

//store i0
      i1 = reads(2);        /* Delay line address of filter #0 */
      r0 = i0;
      dm(9,i1) = r0;
      
      l0 = 0;
      l8 = 0;              
      l9 = 0;              
      l10 = 0;              
      l11 = 0;              

      i0 = gets( 14 );
      i1 = gets( 13 );
      i2 = gets( 12 );
      i8 = gets( 11 );
      i9 = gets( 10 );
      i10 = gets( 9 );
      i11 = gets( 8 );
      m2 = gets( 7 );
      m8 = gets( 6 );
      r5 = gets( 5 );
      r9 = gets( 4 );
      r13= gets( 3 );
      r14= gets( 2 );
      r15= gets( 1 );
      
      leaf_exit;

_Interp4_filt_c_L16.end:
.endseg;

