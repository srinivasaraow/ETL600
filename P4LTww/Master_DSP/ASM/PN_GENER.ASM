/*-------------------------------------------------------------------------*
 * $Workfile   : pn_gener.asm  $
 * Part of	   : ETL600
 * Language	   : asm
 * Created by  : B. Busslinger: 
 * Remarks	   :  
 * Purpose	   : Special Pseudo Noise Generator for channel measurement length 256
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/Master_DSP/ASM/PN_GENER.ASM $   
 * 
 * 1     16.02.05 13:59 Chhamaa
 * 
 * 1     6.01.05 10:13 Chhamaa
 * Initial version based on ETL500
 * 
 *-------------------------------------------------------------------------*/   

#include "asm_sprt.h"
#include "def21161.h"


/*-----------------------GLOBAL CONSTANTS----------------------------------*/

#define NOISELEVEL 0.1


/*----------------------------------- PM ----------------------------------*/

.SEGMENT/PM     seg_pmco;												/* place programm in high memory area */

.GLOBAL _PN_GeneratorSample;


/*
	void PN_GeneratorSample(long *ShiftRegisterGenerator, float *BlockSampleValueArrPtr)

    Subroutine  FIR Filter for equalizer P4LQ both channels
    
	Calling Registers
    r4, r8

	Result Registers
    None

	Altered primary Registers
    none
    
    Altered secondary Registers
    r0,r1,r2,r3,r4,r5,f6,f7,i0,i1

	Computation Time 54 cycles -> 1.65 us @ 33 MHz (0.9 % calculation time P4LQ, dspb)


    Version 0.01    97-03-03	B. Busslinger
*/

_PN_GeneratorSample:

            leaf_entry;
init:		bit clr mode1 IRPTEN;										/* Global interrupt disable (interrupt routines use secodary registers) */
			r12=r4;
			bit set mode1 SRD1L|SRRFL;  								/* use secondary registers (DAG1 reg0-3, register file reg0-7) */
			nop;														/* wait for switchover */			

buf_init:	i0=r12;                                               		/* set pointer to Shift Register value from last call */
			i1=r8;
			f6=NOISELEVEL;												/* set buffer to output samples */
			f7=-NOISELEVEL;

first:		r0=dm(0,i0);												/* get Shift Register value in R0 */
			btst r0 by 0;												/* test first bit if zero */
			if sz jump set_out_n;                                       /* of zero jump to set_out_n, first Sample = -NOISELEVEL */
			dm(i1,1)=f6;												/* set output to NOISELEVEL */
			jump f_shift;                                               /* jump to shifting and xor unit to calculate new shift register */

set_out_n:  dm(i1,1)=f7;												/* set output to -NOISELEVEL */

f_shift:    r3=0x000000ff;                                              /* load bitmask for 8 bit shift register in r3 */
			r4=r3-r0;                                                   /* check if actual shift register equals ff in value */
			if eq jump second;                                        	/* if yes, then do not calculate new shift register delay line */
			r2=lshift r0 by 7;                                          /* give value twice on output to get a repetative sequence of 256 sample values */
			r3=lshift r0 by 3;
			r4=lshift r0 by 2;                                          /* calculate xor of the bits 6,5,4,1 to bit 8 and give result*/
			r5=lshift r0 by 1;                                          /* to r2 */
			r2=r2 xor r3;
			r2=r2 xor r4;
			r2=r2 xor r5;
			r3=0x00000080;
			r2=r2 and r3;
			r3=0x0000007f;
			r0=lshift r0 by -1;											/* shift old shift register one bit right */
			r0=r0 and r3;												/* mask first 7 bits of shifted value */
			r0=r0 or r2;												/* add calculated 8th bit to get the new shift register */

second:		btst r0 by 0;                                               /* check first bit of shifted pseudo noise sequence */
			if sz jump set_out_l;										/* if first bit = 0 output of 2nd sample is - NOISESEQUENCE */
			dm(i1,1)=f6;
			jump l_shift;

set_out_l:	dm(i1,1)=f7;

l_shift:	r2=lshift r0 by 7;                                          /* calculate xor of the bits 6,5,4,1 to bit 8 and give result*/
			r3=lshift r0 by 3;                                          /* to r2 */
			r4=lshift r0 by 2;
			r5=lshift r0 by 1;
			r2=r2 xor r3;
			r2=r2 xor r4;
			r2=r2 xor r5;
			r3=0x00000080;
			r2=r2 and r3;
			r3=0x0000007f;
			r0=lshift r0 by -1;                                         /* shift old shift register one bit right */
			r0=r0 and r3;                                               /* mask first 7 bits of shifted value */
			r0=r0 or r2;                                                /* add calculated 8th bit to get the new shift register */

end:		dm(i0,0)=r0;	                                            /* save new shifted pseudo noise generator delay value */
			bit clr mode1 SRD1L|SRRFL;  								/* use primary DAG1L and register file reg0-7 */
			nop;
			bit set mode1 IRPTEN;  										/* Global interrupt enable */
			leaf_exit;													/* go back to funtion */
			

.ENDSEG;		                 
		                 
