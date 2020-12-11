/*-------------------------------------------------------------------------*
 * $Workfile: PDEMOD.ASM $
 * Part of	: ETL600 / Front End DSP
 * Language	: asm
 * Created by: H.-J. Maag
 * Remarks	:  
 * Purpose	: C callable assembly language DSP function
 * 			  to implement pilot demodulation
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/FE_DSP/ASM/PDEMOD.ASM $
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
/* void pilotDemodulator(PilotDemodDataType *pilotDemod, float *inSample,  */
/*						 int blockLength)								   */
/*						 			      								   */
/* Demodulates the received pilot signal. Arrays  of both the in phase     */
/* and quadrature component are genrated. In order to ensure sychronism	   */ 
/* between transmitter and receiver the function demodFreqControl() has    */
/* to be called every 10th sample. 										   */
/*																		   */
/* Input: received pilot signal inSample							       */
/* Output: in phase component outSampleI[]							       */
/*		   quadrature component outSampleQ[]							   */
/*-------------------------------------------------------------------------*/
 
#include "asm_sprt.h"
#include "def21161.h"
#include "FE_DSP_Const.h"  // block length of AF-sample array (BLOCK_LENGTH_R_4K)


.extern cascaded_biquad;

.segment/dm seg_dmda;

.var _sampleI[BLOCK_LENGTH_R_4K], _sampleQ[BLOCK_LENGTH_R_4K];
.global _sampleI, _sampleQ;
.var filterOutI[BLOCK_LENGTH_R_4K], filterOutQ[BLOCK_LENGTH_R_4K];

.endseg;


.segment/pm seg_pmco;

.global _pilotDemodulator;

_pilotDemodulator: 

 start: leaf_entry;

   	bit clr mode1 IRPTEN; 				// Global interrupt disable
	i4=r4;                			    // parameter ptr
	puts=r12;							// put block length on the stack
	bit set mode1 SRRFL|SRD1L; 			// use secondary registers
    nop;
	
	// oscillator, test tone demodulator (base band convertion) 
	
	i0=_sampleQ;                 	// sampleQ ptr = sine ptr
	i1=r8;                      	// inSample ptr
	i2=_sampleI;					// cosinePtr= sampleI ptr
	f1=dm(2,i4);    				// f1=a
	f2=dm(3,i4);      				// f2=b  
	f7=f1+f1, f5=dm(4,i4);     		// f7=2*a, f5=x1 
	f8=f1*f5, f12=dm(5,i4);    		// f8=a*x1, f12= x2
	m1=1;							// increment 
	f6=dm(i1,m1);                 	// f6= inSample
	r0=gets(1);						// r0= length (number of samples to generate)
	
	lcntr=r0, do endl until lce;    // lcntr= number of samples to generate
   		f4=f2*f5, f0=f8-f12;      	// sine= b*x1, cosine= a*x1-x2
   		f4=f4*f6;                   // sampleI= sine*inSample
    	f3=f7*f5, dm(i0,m1)=f4;     // f3=2*a*x1, save inSample
   		f5=f3-f12, f12=f5;			// x1=newState1= 2*a*x1 - x2, x2=x1 
    	f0=f0*f6, f6=dm(i1,m1);		// sampleQ= cosine*inSample; get next inSample
    endl: f8=f1*f5, dm(i2,m1)=f0;	// f8=a*x1, save sampleQ
    
    dm(4,i4)= f5;					// save x1
    dm(5,i4)= f12;					// save x2

    
    // low pass filtering of base band test tone signal (in phase part)
   	i2=_sampleI;                  	// inphase sample ptr
    r5=dm(6,i4);                  	// get number of sections	
	i3=filterOutI;      			// load index register 3 with outSamplePtr 
	r7=dm(7,i4);                  	// get ptr to coefficients array
	i12=r7;							// load index register 12 with ptr to coefficients array
	r6=dm(8,i4);					// get ptr to state buffer 1
	i0=r6;							// load index register 0 with ptr to state buffer
	
	call cascaded_biquad(db);       // call filter subroutine
    r1=gets(1);						// block length
	m12=1;                        	// increment for coefficients array
    
    dm(14,i4)=f0;                 	// store filterOutI[1] -> filteredI

    // low pass filtering of base band test tone signal (in phase part)
    i2=_sampleQ;                  	// quadrature sample ptr
    r5=dm(6,i4);                  	// get number of sections	
	i3=filterOutQ;      			// load index register 3 with outSamplePtr 
	r7=dm(7,i4);                  	// get ptr to coefficients array
	i12=r7;							// load index register 12 with ptr to coefficients array
	r6=dm(9,i4);					// get ptr to state buffer 2
    
    call cascaded_biquad(db);       // call filter subroutine
    i0=r6;							// load index register 0 with ptr to state buffer
	r1=gets(1);						// block length
   
    dm(15,i4)=f0;                 	// store filterOutQ[1] -> filteredQ
    
    bit clr mode1 SRRFL|SRD1L;    	// use primary registers 
    bit set mode1 IRPTEN;           // interrupt enalble
     	
 end: leaf_exit;
 
_pilotDemodulator.end:
     	
.endseg;