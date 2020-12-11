/*-------------------------------------------------------------------------*
 * $Workfile	: crc.c $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Allen Notter (AN)
 * Remarks      :  
 * Purpose      : CRC calculation
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/C/crc.c $
 * 
 * 1     07-04-27 10:07 Chalnot
 * Initial version.
 * 
 *
 *-------------------------------------------------------------------------*/

/* INCLUDE FILES **************************************************************/

#include "CRC16_table.h"
#include "Basetype_O4CV.h"
#include "BASETYPE.H"

/* LOCAL CONSTANT DECLARATIONS ***********************************************/

/* LOCAL TYPE DECLARATIONS ***************************************************/

/* LOCAL VARIABLES ***********************************************************/

/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL FUNCTIONS ***********************************************************/

/* IMPLEMENTATION ************************************************************/
 
/***************************************************************************
  Function:		crc16()
  Description:	Calculates the crc16 
  Inputs:		reg: Initial value of the crc 	
  				message: One byte data
  Outputs:		-
  Return:		The calculated crc16
  Globals:		-
  Important: 	-
                                          	
****************************************************************************/
UINT16 crc16(UINT16 reg, UINT8 message)
{
	UINT16 reg_temp = 0;
	
	reg_temp = ((reg >> 8) & MASK_8LSB) ^ crc16_table[message ^ (reg & MASK_8LSB)]; 

	return reg_temp;
}









