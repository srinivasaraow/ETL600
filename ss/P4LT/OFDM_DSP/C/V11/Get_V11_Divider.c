/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: Get_V11_Divider.c $
 * COPYRIGHT   : 2004 TL Consulting GmbH
 * DESCRIPTION : Calculate V11 serial port divider for given number of bits in OFDM symbol
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * $Log: /Source/P4LT/OFDM_DSP/C/V11/Get_V11_Divider.c $
 * 
 * 2     7.09.05 11:23 Ats
 * Exchange from TLC
 * 
 * 1     1.02.05 14:05 Chalgyg
 * Initial Version 0.09A
 * 
 * Initial version
 *       27.01.2005 vgp
*****************************************************************************/

#include <limits.h>

#define MIN_BITS_WORD 3
#define MAX_BITS_WORD 32
#define DIV_TRIALS    (MAX_BITS_WORD-MIN_BITS_WORD+1)

int segment ( "seg_ext_pmco" ) Get_V11_Divider(int bits_per_symbol)
{
	int        Divider, Remainder;
	int        i;
	int        Max_Divider, Min_Remainder, Max_Divider_Min_Remainder;

	Max_Divider = 0;
	Min_Remainder = INT_MAX;
	for(i=0; i<DIV_TRIALS; i++) {
		Divider = MIN_BITS_WORD+i;
		Remainder = bits_per_symbol % Divider;
		if ( Remainder == 0 ) {
			if ( Divider > Max_Divider ) Max_Divider = Divider; 
		} else {
			Remainder = Divider - Remainder;
			if ( Remainder <= Min_Remainder ) {
				Min_Remainder = Remainder;
				Max_Divider_Min_Remainder = Divider;
			}
		}
	}

	if ( Max_Divider == 0 ) Max_Divider = Max_Divider_Min_Remainder;


	if ( bits_per_symbol < MAX_BITS_WORD + 1) Max_Divider = MIN_BITS_WORD;
	
	return Max_Divider;
}

/*==========================================*/





