//#define FIR_ACC
#undef FIR_ACC
/******************************************************************************
 *
 * PROJECT     : DRS7 COFDM Modem for 2116x
 * FILE        : $Workfile: dec4x24_filt_c.asm $
 * COPYRIGHT   : 2004 by TL Consulting GmbH.
 * DESCRIPTION : 24 tap decimator of complex signal by 4 
 * OPERATION   : 
 * PROTOTYPE   : void Dec4x24_filt_c( complex_float    vOut[], 
 *                                   complex_float    vIn[],             
 *                                   int              nIn,               
 *                                   complex_float pm vCoeffs[24],       
 *                                   complex_float    vDelay[24+1] );    
 *
 * ASSUMPTIONS : all arrays starts with even address and there length is also even
 *               24 coefficients
 *               Size "nIn" of input array "vIn[]" must contain 4
 *
 * REVISION HISTORY:
 * COMPLEXITY : 
 *-----------------------------------------------------------------------------
 * 
 * 1     19-04-04  VGP
 * Initial version
 * 2     09-11-04  VGP
 * non strided impulse respose version of fir filter
 * 
 *****************************************************************************/

#ifdef __ADSP21160__
#include <def21160.h>
#endif
#ifdef __ADSP21161__
#include <def21161.h>
#endif
#ifdef __ADSP21262__
#include <def21262.h>
#endif

#include    <asm_sprt.h>

#define RCOEF 24
#define NCOEF (2*RCOEF)
#define DECIMATION_FACTOR 4
#define SCALE_FACTOR (1.0/DECIMATION_FACTOR)

.global _Dec4x24_filt_c;

//		.segment/pm   seg_pmco;
		.segment/pm   seg_ext_pmco;

_Dec4x24_filt_c:

      leaf_entry;

      r0 = i0; puts = r0;
      r0 = i1; puts = r0;
      r0 = i2; puts = r0;
      r0 = i8; puts = r0;
      r0 = m2; puts = r0;
      r0 = m8; puts = r0;
      puts = r2;
      puts = r9;
      puts = r13;
      puts = r14;
      puts = r15;

      m8 = 2;
      m2 = 2;   

// i2 - address of input buffer vIn[]
      i2 = r8;              /* Address of input buffer vIn[] */


// Delay line
      b0 = reads(2);        /* Delay line address of filter  */
      r0 = NCOEF;           /* Delay line length in real samples  */
      r1 = dm(NCOEF+1,i0);
// restore i0
      i0 = r1;              /* Get pointer to delay line */
      l0 = r0;              /* Delay line length in real samples*/
      

// Filter coefficients 
      r1 = reads(1);        /* Address of vCoeffs0[n_taps] */
      b8 = r1;              /* Filter coefficients for filter */
      l8  = r0;              

// i1 - address of output buffer vOut[]      
      i1 = r4;              /* Address of vOut[] */

	  r2 = r12;             // r2 = nIn
      
      bit set MODE1 CBUFEN | PEYEN;       // Circular Buffer Enable 
      nop;

      // i8 - points coefficients of filter #0
      // f12 - output sample of FIR filter
      // f13 - output sample of decimation filter

	  // clear f13
	  r13 = r13 - r13;

	  f9 = SCALE_FACTOR;
	  
      // delay 0
      f0 = dm(i2, m2), f4 = pm(i8,m8);   // f0 - read complex sample
#ifdef FIR_ACC
      f0 = f0*f9;
#else
      r9 = r9 - r9;
#endif
      f12 = f0*f4, dm(i0,m2) = f0;       // store input sample in delay line
      
      r14 = DECIMATION_FACTOR;
      r15 = DECIMATION_FACTOR;
      lcntr = r2, do sample_lp until lce; /* loop per sample */

         // delay 1
         f0 = dm(i0,m2), f4 = pm(i8,m8); // get sample from delay line
		 f8 = f0*f4, f0 = dm(i0,m2), f4=pm(i8,m8);

         // delay 2-22
		 lcntr = RCOEF-3, do fir_lp until lce; /* loop per sample */
fir_lp:    f8  = f0*f4, f12 = f8+f12, f0 = dm(i0,m2), f4=pm(i8,m8);
		 
         // delay 23
		 f8  = f0*f4, f12 = f8+f12;
		 
         // delay 0
		 f12 = f8+f12,  f0 = dm(i2, m2), f4 = pm(i8,m8);   // f0 - read complex sample
#ifdef FIR_ACC
		 f0 = f0*f9;
		 f13 = f13 + f12, modify(i0,m7);
#else
		 f13 = f9 + f12, modify(i0,m7);
#endif
		 r14 = r14 - 1, modify(i0,m7);
		 if eq r13 = r13 - r13, dm(i1,m2) = f13;
		 if eq r14 = r15;		 
sample_lp:  f12 = f0*f4, dm(i0,m2) = f0;       // store input sample in delay line

      modify(i0,-2);
         
      bit clr MODE1 CBUFEN | PEYEN; 
      nop;

//store i0
      i1 = reads(2);        /* Delay line address of filter #0 */
      r0 = i0;
      dm(NCOEF+1,i1) = r0;
      
      l0 = 0;
      l8 = 0;              

      i0 = gets( 11 );
      i1 = gets( 10 );
      i2 = gets( 9 );
      i8 = gets( 8 );
      m2 = gets( 7 );
      m8 = gets( 6 );
      r2 = gets( 5 );
      r9 = gets( 4 );
      r13= gets( 3 );
      r14= gets( 2 );
      r15= gets( 1 );
      
      leaf_exit;

_Dec4x24_filt_c.end:
.endseg;

