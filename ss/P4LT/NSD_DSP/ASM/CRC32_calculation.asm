/*-------------------------------------------------------------------------*
 * $Workfile: CRC32_calculation.asm $
 *
 * $Log: /Source/P4LT Prototype 2/NSD_DSP/ASM/CRC32_calculation.asm $
 * 
 * 1     19.01.05 15:33 Chchlee
 * 
 * 1     8.07.04 16:18 Bussli02
 * 
 * 
 *
 *-------------------------------------------------------------------------*/


  
/*-----------------------ASSEMBLER DIRECTIVES------------------------------*/

.PRECISION=40;
.round_nearest;  
#include "asm_sprt.h"
#include "def21161.h"


.segment/dm seg_dmda;

// For saving mode1 register
.var			_SavedMode1_register;

.endseg;




/*****************************************************************************
  Function:      long CalculateCRC32(long Value);	
				  r0					r4

  Description: 	 Low level assembler function for generating the CRC32 information 
				 of a 32 bit teleprotection command word send to all G4AI.
				  	                    
  Inputs:      	 Value  - 32 bit value 

  Constant
  Tables:		 CRC32Table

  Outputs:		 none                                                          
                                                                                                                                        
  Return:      	 32 bit value containing CRC32 of input value
        	
  Globals:       none 

  Important:     Please note that there is an error of the CRC table of ETL500.
				 We do not change this CRC table to guarantee interoperability.                                                    	
******************************************************************************
*/

/*-----------------  external SDRAM Data memory 32 Bit  ---------------------------*/
.SEGMENT/DM		ext_dmda;

.EXTERN _CRC32Table;

.ENDSEG;

/*---------------------  Internal Program memory 48 Bit  --------------------------*/

.SEGMENT/PM     seg_pmco;

.GLOBAL _CalculateCRC32;

_CalculateCRC32:

			leaf_entry;

			dm(_SavedMode1_register) = mode1;		// Switch off SIMD (speeddsp library uses it)

init_crc:	bit clr mode1 PEYEN|IRPTEN;				/* Global interrupt disable (interrupt routines use secodary registers) */
			bit set mode1 SRRFH|SRD1L; 				/* use secondary registers (register file reg8-15) */
			nop;									/* wait for switchover */
			
begin_crc:  i0 = _CRC32Table;
			r0 = 0x00aaaaaa;						/* Initial value of CRC Calculation CRC32 */
			r9 = FEXT r4 BY 0:8;
			r10 = 0x000000aa;						/* Special case because of initialisation */
			r11 = r9 XOR r10;
			m0 = r11;
			r12 = dm(m0, i0);						/* Get the CRC32 Table value */
			r0 = r0 XOR r12;
			r9 = FEXT r4 BY 8:8;
			r10 = FEXT r0 BY 0:8;
			r11 = r9 XOR r10;
			m0 = r11;
			r0 = LSHIFT r0 BY -8;
			r12 = dm(m0, i0);	/* Get the CRC32 Table value */
			r0 = r0 XOR r12;
			r9 = FEXT r4 BY 16:8;
			r10 = FEXT r0 BY 0:8;
			r11 = r9 XOR r10;
			m0 = r11;
			r0 = LSHIFT r0 BY -8;
			r12 = dm(m0, i0);	/* Get the CRC32 Table value */
			r0 = r0 XOR r12;
			r9 = FEXT r4 BY 24:8;
			r10 = FEXT r0 BY 0:8;
			r11 = r9 XOR r10;
			m0 = r11;
			r0 = LSHIFT r0 BY -8;
			r12 = dm(m0, i0);	/* Get the CRC32 Table value */
			r0 = r0 XOR r12;
		
return_crc:	mode1 = dm(_SavedMode1_register);		// use primary registers and restore mode1 register
			nop;
			leaf_exit;								/* go back to funtion */
			

.ENDSEG;		                 
 
