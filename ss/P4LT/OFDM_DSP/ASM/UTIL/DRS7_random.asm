/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: DRS7_random.asm $
 * COPYRIGHT   : 2003 TL consulting GmbH
 * DESCRIPTION : Generates a series of random uniform integers deviates
 *               between 0 and 2^32-1.
 *               Uses the "Even Quicker Generator" technique found on page 284
 *               of "Numerical Recipes in C", which uses the linear congruential
 *               technique to generate random numbers.
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/UTIL/DRS7_random.asm $
 * 
 * 1     19.01.05 15:38 Chchlee
 * 
 * 1     22.06.04 14:25 Unp00611
 *  
 * 1     17/07/03 vgp
 * Initial version
 *****************************************************************************/

#include <asm_sprt.h>
#ifdef __ADSP21160__
#include <def21160.h>
#endif
#ifdef __ADSP21161__
#include <def21161.h>
#endif

#define a_value   1664525   
#define c_value 1013904223

.global _DRS7_rand; 
.global _DRS7_vrand_D; 
.global _DRS7_vrand_E; 
.global _DRS7_setseed;
.global _DRS7_getseed;
.global _DRS7_rand_seed;


		.segment/dm   seg_dmda;

.var   _DRS7_rand_seed = 9865687;

//		.endseg;

//===============================================
		
		.segment/pm   seg_pmco;
		
_DRS7_setseed:   

      leaf_entry;

      dm( _DRS7_rand_seed ) = r4;

      leaf_exit;
_DRS7_setseed.end:   

//===============================================   

_DRS7_getseed:   
      leaf_entry;

      r0 = dm( _DRS7_rand_seed );

      leaf_exit;
_DRS7_getseed.end:   


//===============================================   

_DRS7_rand: 
  
      leaf_entry;
      puts = r2;
      puts = mode1;

      // save interrupt mask register
      ustat1 = IMASK;
      // disable FIXI interrupt
      bit clr IMASK FIXI;
      // save mode1 register
      ustat2 = MODE1;
      // clear saturation mode
      bit clr MODE1 ALUSAT;

      r2 = a_value;
      r0 = dm(_DRS7_rand_seed);
      r0 = r0*r2 (uui);                   /* Compute mod( a*seed )*/
      r2 = c_value;      
      r0 = r0 + r2;                       /* Compute mod( mod( a*x ) + c)*/
      dm(_DRS7_rand_seed) = r0;           /* Save new seed */

      // restore mode1 register
      MODE1 = ustat2;

      bit clr STKYx AOS | MOS;
      bit clr IRPTL FIXI;
      // restore interrupt mask register
      IMASK = ustat1;

      r2    = gets( 2 );
      mode1 = gets( 1 );
      leaf_exit;

_DRS7_rand.end: 


//===============================================
_DRS7_vrand_D:
_DRS7_vrand_E:

      leaf_entry;
      puts = r2;
      puts = mode1;

      // save interrupt mask register
      ustat1 = IMASK;
      // disable FIXI interrupt
      bit clr IMASK FIXI;
      // save mode1 register
      ustat2 = MODE1;
      // clear saturation mode
      bit clr MODE1 ALUSAT;
      
      i4 = r4;                            /* address of R[] */
      r0 = dm(_DRS7_rand_seed);
      r2 = a_value;
      r4 = c_value;
      lcntr = r8, do rnd_lp until lce;            
         r0 = r0*r2 (uui);                /* Compute mod( a*seed )*/
         r0 = r0 + r4;                    /* Compute mod( mod( a*x ) + c)*/
rnd_lp:  dm(i4,m6) = r0;                  /* Store R[i] */
      dm(_DRS7_rand_seed) = r0;           /* Save new seed */

      // restore mode1 register
      MODE1 = ustat2;

      bit clr STKYx AOS | MOS;
      bit clr IRPTL FIXI;
      // restore interrupt mask register
      IMASK = ustat1;

      r2    = gets( 2 );
      mode1 = gets( 1 );
      
      leaf_exit;

_DRS7_vrand_D.end:
_DRS7_vrand_E.end:

			.endseg;

#undef a_value 
#undef c_value

