/*-------------------------------------------------------------------------*
 * $Workfile: TTONE.ASM $
 * Part of	: ETL600 / Front End DSP
 * Language	: asm
 * Created by: H.-J. Maag
 * Remarks	:  
 * Purpose	: C callable assembly language DSP function
 * 			  to implement test tone generation and demodulation
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/FE_DSP/ASM/TTONE.ASM $
 * 
 * 1     19.01.05 15:31 Chchlee
 * 
 * 2     17.06.04 18:25 Maag01
 * Block length set to BLOCK_LENGTH_R_4K
 * 
 * 1     30.04.04 14:42 Maag01
 * Initial version based on ETL500 Version 4.20
 *  
 *
 * 
 *-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/               
/*void ttone(TestToneDataType *testTone, float *inSample,                  */
/* 			    float *testToneSampleOut, int blockLength)		   		   */
/*                                                                         */
/* Generates an array of test tone samples testToneSampleOut (number of    */
/* samples generated is defined in the constant BLOCK_LENGTH).  		   */
/* Performs a frequency shift of the received AF-signal inSample by the    */
/* test tone frequencey. Base band test tone detection is performed by	   */
/* the function detectTestTone().                                          */
/*																		   */
/* Input: test tone parameters including oscillator and filter parameters, */
/*		  AF sample array inSample		       							   */
/* Output: array of test tone samples testToneSamplesOut, demodulated      */
/* 			test tone (I & Q part) testTone.demodSample[]                  */
/*-------------------------------------------------------------------------*/
 
#include "asm_sprt.h"
#include "def21161.h"
#include "FE_DSP_Const.h"  // block length of AF-sample array (BLOCK_LENGTH_R_4K)
 
.extern cascaded_biquad;

.segment/dm seg_dmda;

.var sampleI[BLOCK_LENGTH_R_4K], sampleQ[BLOCK_LENGTH_R_4K];
.var filterOut[BLOCK_LENGTH_R_4K];

.endseg;


.segment/pm seg_pmco;

.global _ttone;

_ttone: 

 start: leaf_entry;

   	bit clr mode1 IRPTEN; 				// Global interrupt disable
	i4=r4;                			    // oscPtr
	bit set mode1 SRRFL|SRD1L; 			// use secondary registers
	nop; 
	
	// oscillator, test tone demodulator 
	
	i0= sampleI;                 	// sampleI ptr
	i1= r8;                      	// inSample ptr
	i3= r12;						// sinePtr= testToneSampleOut ptr
	i2=sampleQ;						// cosinePtr= sampleQ ptr
	f1=dm(2,i4);    				// f1=a
	f2=dm(3,i4);      				// f2=b  
	f7=f1+f1, f5=dm(4,i4);     		// f7=2*a, f5=x1 
	f8=f1*f5, f12=dm(5,i4);    		// f8=a*x1, f12= x2
	m1=1;							// increment 
	f6=dm(i1,m1);                 	// f6= inSample
	r0=reads(1);					// r0= length (number of samples to generate)
	
	lcntr=r0, do endl until lce;    // lcntr= number of samples to generate
   		f4=f2*f5, f0=f8-f12;      	// sine= b*x1, cosine= a*x1-x2
    	f3=f7*f5, dm(i3,m1)=f4;     // f3=2*a*x1, save sine
    	f4=f4*f6;                   // sampleI= sine*inSample
   		f5=f3-f12, f12=f5;			// x1=newState1= 2*a*x1 - x2, x2=x1 
    	f0=f0*f6; f6=dm(i1,m1);		// sampleQ= cosine*inSample; get next inSample
    	dm(i2,m1)=f0;				// save sampleQ
    endl: f8=f1*f5, dm(i0,m1)=f4;	// f8=a*x1, save sampleI
    
    dm(4,i4)= f5;					// save x1
    dm(5,i4)= f12;					// save x2
    
    
    // low pass filtering of base band test tone signal (in phase part)
   	i2=sampleI;                  	// inphase sample ptr
    r5=dm(6,i4);                  	// get number of sections	
	i3=filterOut;      				// load index register 3 with outSamplePtr 
	r7=dm(7,i4);                  	// get ptr to coefficients array
	i12=r7;							// load index register 12 with ptr to coefficients array
	r6=dm(8,i4);					// get ptr to state buffer 1
	i0=r6;							// load index register 0 with ptr to state buffer
	l0=0;
	
	call cascaded_biquad(db);       // call filter subroutine
    r1=reads(1);					// block length
	m12=1;                        	// increment for coefficients array
	  
	dm(10,i4)=f0;                 	// store demodSample[0]
    
    // low pass filtering of base band test tone signal (in phase part)
    i2=sampleQ;                  	// quadrature sample ptr
    r5=dm(6,i4);                  	// get number of sections	
	i3=filterOut;      				// load index register 3 with outSamplePtr 
	r7=dm(7,i4);                  	// get ptr to coefficients array
	i12=r7;							// load index register 12 with ptr to coefficients array
	r6=dm(9,i4);					// get ptr to state buffer 2

    call cascaded_biquad(db);       // call filter subroutine
    i0=r6;							// load index register 0 with ptr to state buffer
	r1=reads(1);					// block length 
	
    dm(11,i4)=f0;                 	// store demodSample[1]
    
    bit clr mode1 SRRFL|SRD1L;    	// use primary registers 
    bit set mode1 IRPTEN;           // interrupt enalble
     	
 end: leaf_exit;
 
_ttone.end:
     	
.endseg;