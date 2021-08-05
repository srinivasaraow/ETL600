/*-------------------------------------------------------------------------*
 * $Workfile: BIIIR.ASM $
 * Part of	: ETL600 / P4LT
 * Language	: asm
 * Created by: H.-J. Maag
 * Remarks	:  
 * Purpose	: C callable assembly language DSP function
 *            to implement IIR filter structure (second order representation)
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/NSD_DSP/ASM/BIIIR.ASM $
 * 
 * 1     19.01.05 15:33 Chchlee
 * 
 * 2     8.07.04 16:51 Maag01
 * 
 * 1     30.04.04 14:37 Maag01
 * Initial version based on ETL500 Version 4.20
 * 
 * 
 *-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/               
/* void BiIIR(float *inSample, float *outSample, float pm *coef, 	  	   */ 
/*					float *state, int sections, int length)                */
/*																		   */
/* Implements a IIR filter in biquad (second order) sections repre-		   */
/* sentation. The parameters sections (number of biquad sections),    	   */
/* state[sections*2] (state array) and coef[sections*4+1] have to be	   */
/* defined as const or static variables.								   */ 
/* A biquad section is represented by 2 numerator coefficients (b1, b2)	   */
/* and 2 denominator coefficients (a1,a2). The over all filter gain has    */
/* to be stored as last element of the coef array. b0 and a0 are supposed  */
/* to equal 1.0.                                                           */
/* float pm coef[sections*4+1]={a12,a11,b12,b11,a22,..., gain}             */
/*                                                                         */
/* Input: input sample array inSample[length]        			   		   */
/* Output: block of filtered output sample outSample[length]               */
/*-------------------------------------------------------------------------*/

#include <asm_sprt.h>
#include <def21161.h>

/* start of program segment */

.segment/pm    seg_pmco;

.global _BiIIR;
 
_BiIIR: 

 leaf_entry;

  bit clr mode1 IRPTEN;  				// Global interrupt disable (interrupt routines use secodary registers)
        i4=f4;       					// get inSamplePtr
	    bit set mode1 SRD1L|SRRFL;  	// use secondary registers (DAG1 reg0-3, register file reg0-7)  
  nop;                                  // 
	  	r5=reads(2);                   	// get number of sections	
	    i3=f8;       					// load index register 3 with outSamplePtr 
	 	r7=r12;                        	// get ptr to coefficients array
	    i12=r12; 						// load index register 12 with ptr to coefficients array
	 	r6=reads(1); 					// get ptr to state buffer
	 	i0=r6; 							// load index register 0 with ptr to state buffer
		l0=0;
	 	r1=reads(3); 					// block length
	 	m1=1;                          	// increment for state array
	 	m12=1;                         	// increment for coefficients array 
	 	
  lcntr=r1, do filt until lce;       	// loop counter= number of samples (block length)
	 	f8=dm(i4,m1); 					// get inSample
	 	i1=i0; 							// i1 used for state update
	 	f12=f12-f12, f2=dm(i0,m1), f4=pm(i12,m12); // f12=0, f2=a2, f4=w(n-2)
	 	
  		lcntr=r5, do quad until lce; 								// loop counter= number of biquad sections
	 		 f12=f2*f4, f8=f8+f12, f1=dm(i0,m1), f4=pm(i12,m12); 	// a2*w(n-2), x(n)+0 or y(n), f1=w(n-1), f4=a1
	 		 f12=f1*f4, f8=f8+f12, dm(i1,m1)=f1, f4=pm(i12,m12); 	// a1*w(n-1), x(n)+a2*w(n-2), store new w(n-2), f4=b2
	 		 f12=f2*f4, f8=f8+f12, f2=dm(i0,m1), f4=pm(i12,m12);	// b2*w(n-2), new w(n), f2= w(n-2) for next section, f4=b1
  		quad:f12=f1*f4, f8=f8+f12, dm(i1,m1)=f8, f4=pm(i12,m12); 	// b1*w(n-1), w(n)+b2*w(n-2), store new w(n-1), f4=a2 of next section or gain
  
	 	f0=f8+f12, i0=r6; 				// w(n)+b2*w(n-2)+b1*w(n-1), i0= ptr to state buffer
	 	f0=f0*f4, i12=r7; 				// y(n)= f0*gain, initialize i12 with ptr to coefficients array 
  filt: dm(i3,m1)=f0; 					// store y 
  
     	bit clr mode1 SRD1L|SRRFL;  	// use primary DAG1L registers
     	bit set mode1 IRPTEN;  			// Global interrupt enable 
     	
 leaf_exit;
 
_BiIIR.end:     	 
     	
.endseg;