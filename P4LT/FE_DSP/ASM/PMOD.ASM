/*-------------------------------------------------------------------------*
 * $Workfile: PMOD.ASM $
 * Part of	: ETL600 / Front End DSP
 * Language	: asm
 * Created by: H.-J. Maag
 * Remarks	:  
 * Purpose	: C callable assembly language DSP function
 * 			  to implement pilot modulation
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/FE_DSP/ASM/PMOD.ASM $
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
/* void pilotModulator(PilotModDataType *pilotModCh1, float *data,         */
/*  				   float *MWire, float *outSample, int blockLength)    */
/*																		   */
/* Returnes an array of the quadrature modulated samples carrying the  	   */
/* information of the input signals data and MWire.                        */
/*                                                                         */
/* Input: data, MWire, filter parameters							       */
/* Output: modulated pilot signal 										   */
/*-------------------------------------------------------------------------*/
 
#include "asm_sprt.h"
#include "def21161.h"
#include "FE_DSP_Const.h"  // block length of AF-sample array (BLOCK_LENGTH_R_4K)

 
.extern cascaded_biquad;

.segment/dm seg_dmda;

.var sampleI[BLOCK_LENGTH_R_4K], sampleQ[BLOCK_LENGTH_R_4K];
.var filterOutI[BLOCK_LENGTH_R_4K], filterOutQ[BLOCK_LENGTH_R_4K];

.endseg;


.segment/pm seg_pmco;

.global _pilotModulator;

_pilotModulator: 

 start: leaf_entry;

   	bit clr mode1 IRPTEN; 			// Global interrupt disable
	i4=r4;                			// parameter ptr
	bit set mode1 SRRFL|SRD1L; 		// use secondary registers
    nop;
	
    r3=r12;							// MWire sample pointer
    
    // low pass filtering of input data 
   	i2=r8;                  		// data sample ptr
    r5=dm(6,i4);                  	// get number of sections	
	i3=filterOutI;      			// load index register 3 with outSamplePtr 
	r7=dm(7,i4);                  	// get ptr to coefficients array
	i12=r7;							// load index register 12 with ptr to coefficients array
	r6=dm(8,i4);					// get ptr to state buffer 1
	i0=r6;							// load index register 0 with ptr to state buffer
	
	call cascaded_biquad(db);       // call filter subroutine
    r1=reads(2);					// block length
	m12=1;                        	// increment for coefficients array


    // low pass filtering of input MWire
    i2=r3;                  		// MWire sample ptr
    r5=dm(6,i4);                  	// get number of sections	
	i3=filterOutQ;      			// load index register 3 with outSamplePtr 
	r7=dm(7,i4);                  	// get ptr to coefficients array
	i12=r7;							// load index register 12 with ptr to coefficients array
	r6=dm(9,i4);					// get ptr to state buffer 2
    
    call cascaded_biquad(db);       // call filter subroutine
    i0=r6;							// load index register 0 with ptr to state buffer
	r1=reads(2);					// block length 
	
   
    // oscillator, pilot modulator 
	
	i1=reads(1);					// get outSample ptr
	i0=filterOutQ;                 	// filtered MWire
	i2=filterOutI;					// filtered data
	f1=dm(2,i4);    				// f1=a
	f2=dm(3,i4);      				// f2=b  
	f7=f1+f1, f5=dm(4,i4);     		// f7=2*a, f5=x1 
	f8=f1*f5, f12=dm(5,i4);    		// f8=a*x1, f12= x2
	m1=1;							// increment 
	f6=dm(i0,m1);                 	// f6= filterOutQ
	r0=reads(2);					// r0= length (number of samples to generate)
	
	lcntr=r0, do endl until lce;    // lcntr= number of samples to generate
   		f4=f2*f5, f0=f8-f12;      	// sine= b*x1, cosine= a*x1-x2
   		f4=f4*f6, f6=dm(i2,m1);     // sampleQ= sine*filterOutQ, f6= filterOutI
    	f3=f7*f5;     				// f3=2*a*x1
   		f5=f3-f12, f12=f5;			// x1=newState1= 2*a*x1 - x2, x2=x1 
    	f0=f0*f6, f6=dm(i0,m1);		// sampleQ= cosine*filterOutI, get next filterOutQ
    	f0=f0+f4;                   // outSample= sampleI + sampleQ
    endl: f8=f1*f5, dm(i1,m1)=f0;	// f8=a*x1, save outSample
    
    dm(4,i4)= f5;					// save x1
    dm(5,i4)= f12;					// save x2
    
    bit clr mode1 SRRFL|SRD1L;    	// use primary registers 
    bit set mode1 IRPTEN;           // interrupt enalble
     	
 end: leaf_exit;
 
_pilotModulator.end:
     	
.endseg; 



