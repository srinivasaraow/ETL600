/*-------------------------------------------------------------------------*
 * $Workfile: CDBIQUAD.ASM $
 * Part of	: ETL600 / P4LT
 * Language	: asm
 * Created by: H.-J. Maag
 * Remarks	:  
 * Purpose	: assembly subroutine implementing a biquad section
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/FE_DSP/ASM/CDBIQUAD.ASM $
 * 
 * 1     19.01.05 15:31 Chchlee
 * 
 * 1     30.04.04 14:42 Maag01
 * Initial version based on ETL500 Version 4.20
 * 
 *
 * 
 *-------------------------------------------------------------------------*/  
/*
 * CDBIQUAD.ASM
 *
 * assembly subroutine implementing a second order IIR-filter section (biquad)
 * 
 *  A biquad section is represented by 2 numerator coefficients (b1, b2)	   
 *  and 2 denominator coefficients (a1,a2). The over all filter gain has    
 *  to be stored as last element of the coef array. b0 and a0 are supposed  
 *  to equal 1.0.                                                           
 *  float pm coef[sections*4+1]={a12,a11,b12,b11,a22,..., gain}
 *  
 * Equation Implemented:
 *  
 *  w(n)= x(n) + a1*w(n-1) + a2*w(n-2)
 *  y(n)= w(n) + b1*w(n-1) + b2(w(n-2)
 *
 *
 * Calling Parameters:
 *  
 *	i0 = r6 =  address of state (delay line) buffer:  state[sections*2]
 *  i12= r7 =  address of filter coefficient array :  coef[sections*4 + 1]
 *  i2 =  address of input sample array			   : inSample[blockLength] 
 *  i3 =  address of output (filtered) sample array: outSample[blockLength]
 *   
 *  r1 =  number of samples (block length)
 *  r5 =  number of biquad sections	
 *
 *  m1 =  modify value for input sample
 *	m12=  modify value for coefficient array
 *  
 *
 * Registers Affected:  f0, f1, f2, f4, f5, f6, f7, f8, f12 
 *                      i0, i1, i2, i3, i12
 *
 */

#include "asm_sprt.h"
#include "def21161.h" 

.global cascaded_biquad;

.segment/pm seg_pmco;

 cascaded_biquad:
   
	lcntr=r1, do filt2 until lce;  	// loop counter= number of samples
		f8=dm(i2,m1);			  	// get inSample
		i1=i0;						// i1 used for state update
		f12=f12-f12, f2=dm(i0,m1), f4=pm(i12,m12);// f12=0, f2=a2, f4=w(n-2)
		lcntr=r5, do quad2 until lce;		// loop counter= number of biquad sections
		   	f12=f2*f4, f8=f8+f12, f1=dm(i0,m1), f4=pm(i12,m12);	// a2*w(n-2), x(n)+0 or y(n), f1=w(n-1), f4=a1
			f12=f1*f4, f8=f8+f12, dm(i1,m1)=f1, f4=pm(i12,m12);	// a1*w(n-1), x(n)+a2*w(n-2), store new w(n-2), f4=b2
		   	f12=f2*f4, f8=f8+f12, f2=dm(i0,m1), f4=pm(i12,m12);	// b2*w(n-2), new w(n), f2= w(n-2) for next section, f4=b1
		quad2:f12=f1*f4, f8=f8+f12, dm(i1,m1)=f8, f4=pm(i12,m12); // b1*w(n-1), w(n)+b2*w(n-2), store new w(n-1), f4=a2 of next section or gain
		f0=f8+f12, i0=r6;				// w(n)+b2*w(n-2)+b1*w(n-1), i0= ptr to state buffer
		f0=f0*f4, i12=r7;				// y(n)= f0*gain, initialize i12 with ptr to coefficients array 
	filt2:dm(i3,m1)=f0;					// store y
    rts;
    
 cascaded_biquad.end:  
  	
.endseg;
