/*-------------------------------------------------------------------------*
 * $Workfile: ArrayFloatCheck.asm $
 * Part of      : ETL600 / P4LT
 * Language     : asm
 * Created by   : Bruno Busslinger, PTUKT2
 * Remarks      :
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT Prototype 2/Master_DSP/ASM/ArrayFloatCheck.asm $
 * 
 * 1     19.01.05 15:47 Chchlee
 * 
 * 2     4.01.05 9:33 Chbrbus
 * function completely debugged
 * 
 * 1     3.01.05 16:37 Chbrbus
 * 
 * 
 * 
 *-------------------------------------------------------------------------*/


#include "asm_sprt.h"
#include "def21161.h"


/*-----------------------ASSEMBLER DIRECTIVES------------------------------*/

.PRECISION=40;
.round_nearest;  

.segment/dm seg_dmda;

// For saving mode1 register
.var			_SavedMode1_register;

.endseg;


/***************************************************************************
  Function:      void ArrayFloatCheck(float *FloatValueArrayToCheck, long ArrayLength);		
											f4								r8

  Description: 	 Checks in the Array FloatValueArrayToCheck for NAN. If the value 
				 ib the array is NAN, this value will be overwritten with 0.0f.
				 The array will be checked from Position [0] to [ ArrayLength - 1]
				  	                    
  Inputs:      	 *FloatValueArrayToCheck     -  pointer to the first element of the array to check
				 ArrayLength  -  length of the array - use sizeof(..) 

  Outputs:		 none, NAN will be replaced by 0.0f                                                         
                                                                                                                                        
  Important:     This function will check for transmission errors of float values 
				 on the external and internal TDM busses in the ETL600 system                                                   	
****************************************************************************
*/

/*---------------------  Internal Program memory 48 Bit  ------------------*/

.SEGMENT/PM     seg_pmco;

.GLOBAL _ArrayFloatCheck;

_ArrayFloatCheck:

			leaf_entry;

			dm(_SavedMode1_register) = mode1;		// Switch off SIMD (speeddsp library uses it)

init:		bit clr mode1 PEYEN|IRPTEN;				/* Global interrupt disable (interrupt routines use secodary registers) */
			bit set mode1 SRD1L; 					/* use secondary DAG registers 0-3 */
			nop;                                    /* wait for switchover */
			r0 = r8;								/* get length of array to Index register */
			bit set mode1 SRRFH;					/* Register file alternate select for R15-R8 (F15-F8) */
			nop;
			i0 = f4;
			f15 = 0.0;								// f0 is 0.0f, will be used to overwrite NAN
			r14 = 0;

check:		LCNTR = r0,  do end_check until LCE;

			m0 = r14;
			f10 = dm(m0, i0);						/* Get last array value from array */
			f9 = f10 * f10;							/* Make a multipication with the value to check */
			BIT TST ASTAT 0x00000200;				/* Check MI Bit in ASTAT (NAN in Float) */
			IF TF dm(m0, i0) = f15;					/* If the actual value is NAN, set value 0.0f */
			f10 = PASS f10;							/* Check for ALU */
			BIT TST ASTAT 0x00000020;
			IF TF dm(m0, i0) = f15;					/* If the actual value is NAN, set value to 0.0f */	
end_check:	r14 = r14 + 1;
			
return:		mode1 = dm(_SavedMode1_register);		// use primary registers and restore mode1 register
			nop;
			leaf_exit;								/* go back to funtion */
			

.ENDSEG;