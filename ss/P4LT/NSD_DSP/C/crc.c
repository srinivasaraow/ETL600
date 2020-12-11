/*-------------------------------------------------------------------------*
 * $Workfile: crc.c $
 * Part of	: ETL541
 * Language	: C
 * Created by: 
 * Remarks	:  
 * Purpose	: CRC routines
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/Master_DSP/C/crc.c $
 * 
 * 1     19.01.05 15:48 Chchlee
 * 
 * 2     2.08.04 8:17 Leeb02
 * 
 * 1     2.08.04 7:14 Leeb02
 * 
 *-------------------------------------------------------------------------*/

/* INCLUDE FILES **************************************************************/
#include "CRC16_table.H"
#include "basetype.h"

/* LOCAL CONSTANT DECLARATIONS ***********************************************/

/* LOCAL TYPE DECLARATIONS ***************************************************/


/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL VARIABLES ***********************************************************/

/* LOCAL FUNCTIONS ***********************************************************/

/* IMPLEMENTATION ************************************************************/
 
unsigned long crc16(unsigned long reg, unsigned long message)
{
	static unsigned long reg_temp = 0;

	reg_temp = ((reg >> 8) & MASK_8LSB) ^ crc16_table[message ^ (reg & MASK_8LSB)]; 

	return (reg_temp & MASK_16LSB);
     // Mask for reg_temp because right shift can fill 
     // vacated bits with sign bits
}









