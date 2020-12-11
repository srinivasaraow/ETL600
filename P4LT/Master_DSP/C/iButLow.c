/*-------------------------------------------------------------------------*
 * $Workfile: iButLow.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : M. Buhl
 * Remarks      :  
 * Purpose      : 1-Wire Interface for iButton
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /branches/Source MAS/P4LT/Master_DSP/C/iButLow.c $ 
 * 
 * 9     09-12-22 16:02 Chalnot
 * Read and write cycle time increased from 80us to 110us.
 * 
 * 3     16.11.05 13:49 Chmibuh
 * Cosmetic
 * 
 * 2     23.02.05 14:18 Chmibuh
 * 
 * 1     23.02.05 13:57 Chmibuh
 *-------------------------------------------------------------------------*/



/* INCLUDE FILES *************************************************************/
#include <def21161.h>
#include "basetype.h"

#include "IO_HW_Buffer.h"

/* LOCAL CONSTANT DECLARATIONS ***********************************************/

///// Reset iButton DS1961S
//				   ______
// Drive WR		__/      \_________________________________
//				  A      B	                               NEXT
//				___________    ___________________     ____
// Pull up		           \__/                   \___/             
//				           C  D                   G   H 
// Read/Sample	                    *  *         *
//				                    E1 E2        F
//
// Delay defines for standard speed in uSec
#define RESET_WAIT_AtoB				800
#define RESET_WAIT_BtoC				4
#define RESET_WAIT_CtoD				8
#define RESET_WAIT_DtoE1			70	
#define RESET_WAIT_E1toE2			1   
#define RESET_WAIT_E2toF			239		
#define RESET_WAIT_FtoG				2
#define RESET_WAIT_GtoH				60
#define RESET_WAIT_HtoNEXT			200

///// Write 0 to iButton DS1961S
//				   ____________
// Drive WR		__/            \_____________
//				  A            B	         NEXT
//				_________________    ________
// Pull up		                 \__/                         
//				                 C  D               
//
// Delay defines for standard speed in uSec
#define WR_ZERO_WAIT_AtoB			60
#define WR_ZERO_WAIT_BtoC			2
#define WR_ZERO_WAIT_CtoD			16
#define WR_ZERO_WAIT_DtoNEXT		32

///// Write 1 to iButton DS1961S
//				   ____
// Drive WR		__/    \_____________________
//				  A    B	                 NEXT
//				_______________            __
// Pull up		               \__________/                         
//				               C          D               
//
// Delay defines for standard speed in uSec
#define WR_ONE_WAIT_AtoB			8
#define WR_ONE_WAIT_BtoC			2
#define WR_ONE_WAIT_CtoD			68
#define WR_ONE_WAIT_DtoNEXT			32

///// Read iButton DS1961S
//				   ____
// Drive WR		__/    \_____________________
//				  A    B	                 NEXT
//				_______________            __
// Pull up		               \__________/                         
//				               D          E               
// Read/Sample	          *  *
//				          C1 C2
//
// Delay defines for standard speed in uSec
#define READ_WAIT_AtoB				8
#define READ_WAIT_BtoC1				8
#define READ_WAIT_C1toC2			1
#define READ_WAIT_C2toD				15
#define READ_WAIT_DtoE				19
#define READ_WAIT_EtoNEXT			59


#define FAMILY_CODE_DS1961			0x33


/* LOCAL TYPE DECLARATIONS ***************************************************/
/* LOCAL MACRO DEFINITIONS ***************************************************/

#define	activatePullUp()		IO_BUFFER_WRITE &= ~(TP1101);  \
								*(long*)IO_BUFFER_DSP = IO_BUFFER_WRITE				// Write to OutBuffer

#define deactivatePullUp()		IO_BUFFER_WRITE |= (TP1101);  \
								*(long*)IO_BUFFER_DSP = IO_BUFFER_WRITE				// Write to OutBuffer

#define setWR_1Wire_Low()		IO_BUFFER_WRITE |= (TP1102);  \
								*(long*)IO_BUFFER_DSP = IO_BUFFER_WRITE				// Write to OutBuffer

#define setWR_1Wire_High()		IO_BUFFER_WRITE &= ~(TP1102);  \
								*(long*)IO_BUFFER_DSP = IO_BUFFER_WRITE				// Write to OutBuffer

#define read_value_is_one		FLG8
#define read_value_is_zero		0
#define	read_1Wire()			*(long*)(IOFLAG) & FLG8


/* LOCAL VARIABLES ***********************************************************/
/* LOCAL FUNCTIONS ***********************************************************/
/* IMPLEMENTATION ************************************************************/

//#pragma optimize_off 

// Critical (precise) Timing
// Do not Optimize this function or put it to external RAM
static void iButL_Delay_uSec(unsigned long delayTicks) 
{
	unsigned long local_delayTicks;

	local_delayTicks = (delayTicks * 12);// checked !!!!!

	do
	{
		asm("nop;");
	}while(local_delayTicks--);
	asm("nop;");
	asm("nop;");
}

// Reset iButton and check for presence: return TRUE if device is present, else return FALSE
BOOL SEGMENT_EXT_CODE iButL_Reset(void)
{
	unsigned long read1, read2, read3;

	setWR_1Wire_Low();
	iButL_Delay_uSec(RESET_WAIT_AtoB);
	setWR_1Wire_High();
	iButL_Delay_uSec(RESET_WAIT_BtoC);
	activatePullUp();
	iButL_Delay_uSec(RESET_WAIT_CtoD);
	deactivatePullUp();
	iButL_Delay_uSec(RESET_WAIT_DtoE1);
	read1 = read_1Wire();
	iButL_Delay_uSec(RESET_WAIT_E1toE2);
	read2 = read_1Wire();
	iButL_Delay_uSec(RESET_WAIT_E2toF);
	read3 = read_1Wire();
	iButL_Delay_uSec(RESET_WAIT_FtoG);
	activatePullUp();
	iButL_Delay_uSec(RESET_WAIT_GtoH);
	deactivatePullUp();
	iButL_Delay_uSec(RESET_WAIT_HtoNEXT);

	if(read1 == read2)     // read1 and read2 must be equal and low
	{
		if((read1 == 0) && (read3 == read_value_is_one))  // read3 must be 1, otherwise the data line is always low
		{
			return(TRUE);
		}
		else
		{
			return(FALSE);
		}
	}
	return(FALSE);
}

static void SEGMENT_EXT_CODE iButL_WriteBit(unsigned long bit)
{
	if (bit)
	{
		// Write ‘1’ bit
		setWR_1Wire_Low();
		iButL_Delay_uSec(WR_ONE_WAIT_AtoB);
		setWR_1Wire_High();
		iButL_Delay_uSec(WR_ONE_WAIT_BtoC);
		activatePullUp();
		iButL_Delay_uSec(WR_ONE_WAIT_CtoD);
		deactivatePullUp();
		iButL_Delay_uSec(WR_ONE_WAIT_DtoNEXT);	
	}
	else
	{
		// Write ‘0’ bit
		setWR_1Wire_Low();
		iButL_Delay_uSec(WR_ZERO_WAIT_AtoB);
		setWR_1Wire_High();
		iButL_Delay_uSec(WR_ZERO_WAIT_BtoC);
		activatePullUp();
		iButL_Delay_uSec(WR_ZERO_WAIT_CtoD);
		deactivatePullUp();
		iButL_Delay_uSec(WR_ZERO_WAIT_DtoNEXT);	
	}
}

static unsigned long SEGMENT_EXT_CODE iButL_ReadBit(void)
{
	unsigned long read1, read2;

	setWR_1Wire_Low();
	iButL_Delay_uSec(READ_WAIT_AtoB);
	setWR_1Wire_High();
	iButL_Delay_uSec(READ_WAIT_BtoC1);
	read1 = read_1Wire();
	iButL_Delay_uSec(READ_WAIT_C1toC2);
	read2 = read_1Wire();
	iButL_Delay_uSec(READ_WAIT_C2toD);
	activatePullUp();
	iButL_Delay_uSec(READ_WAIT_DtoE);
	deactivatePullUp();
	iButL_Delay_uSec(READ_WAIT_EtoNEXT);
	
	return (read1);
}

void SEGMENT_EXT_CODE iButL_WriteByte(unsigned long data)
{
	unsigned long loop;
	// Loop to write each bit in the byte, LS-bit first
	for (loop = 0; loop < 8; loop++)
	{
		iButL_WriteBit(data & 0x01);
		// shift the data byte for the next bit
		data >>= 1;
	}
}


unsigned long SEGMENT_EXT_CODE iButL_ReadByte(void)
{
	unsigned long loop, result, read;
	
	result=0;
	for (loop = 0; loop < 8; loop++)
	{
		// shift the result to get it ready for the next bit
		result >>= 1;
		// if result is one, then set MS bit
		read = iButL_ReadBit();
		
		if (read)
		{
			result |= 0x80;
		}
	}
	return (result);
}

// Writes one data byte and returns the sampled result
unsigned long SEGMENT_EXT_CODE iButL_TouchByte(unsigned long data)  
{
	unsigned long loop, result;
	
	result=0;

	for (loop = 0; loop < 8; loop++)
	{
		// shift the result to get it ready for the next bit
		result >>= 1;

		if(data & 0x01)			// if sending a '1' then read a bit else write a '0' 
		{
			if(iButL_ReadBit())
			{
				result |= 0x80;	
			}
		}
		else
		{
			iButL_WriteBit(0);
		}
		data >>= 1;
	}

	return (result);
}


// Writes a block of data and return the sampled result in the same buffer
void SEGMENT_EXT_CODE iButL_Block(unsigned long *data, unsigned long data_length) 
{
	unsigned long loop;

	for (loop = 0; loop < data_length; loop++)
	{
		data[loop] = iButL_TouchByte(data[loop]);
	}
}

