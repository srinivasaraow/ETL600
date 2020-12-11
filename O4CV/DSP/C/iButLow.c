/*-------------------------------------------------------------------------*
 * $Workfile	: iButLow.c  $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Allen Notter (AN)
 * Remarks      :  
 * Purpose      : Low level iButton access
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source MAS/O4CV/DSP/C/iButLow.c $
 * 
 * 6     09-12-22 15:04 Chalnot
 * Read and write cycle time increased from 80us to 110us.
 * 
 * 4     07-10-24 10:06 Chalnot
 * Interrupts not disabled during ibutton access.
 * 
 * 2     07-08-08 16:28 Chalnot
 * Delay with timer module.
 * 
 * 1     07-04-17 10:01 Chalnot
 * Initial version.
 * 
 *
 *-------------------------------------------------------------------------*/

/* INCLUDE FILES *************************************************************/

#include <cdefBF538.h>
#include "basetype.h"
#include "Basetype_O4CV.h"
#include "iButLow.h"
#include "timer.h"

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

/* LOCAL VARIABLES ***********************************************************/

/* LOCAL MACRO DEFINITIONS ***************************************************/

#define	activatePullUp()		*pPORTEIO_CLEAR = PF11
#define deactivatePullUp()		*pPORTEIO_SET = PF11


#define setWR_1Wire_Low()		*pPORTEIO_SET = PF13
#define setWR_1Wire_High()		*pPORTEIO_CLEAR = PF13


#define read_value_is_one		PF12
#define read_value_is_zero		0
#define	read_1Wire()			(*pPORTEIO) & PF12

/* LOCAL FUNCTIONS ***********************************************************/

static void delayMicroSec(UINT16 delayTicks); 
static void writeBit(bool bit);
static bool readBit(void);

/* IMPLEMENTATION ************************************************************/

/*****************************************************************************
  Function:      delayMicroSec()  
  Description: 	 Makes a delay for a certain time            
  Inputs:      	 delayTicks: The delay in microseconds			        				 	                                                                       
  Outputs:    	 None                                                                                                                                      
  Return:      	 None   	
  Globals:       None
  Important:                                                      	
*******************************************************************************/ 
static void delayMicroSec(UINT16 delayTicks) 
{
	timer_init(delayTicks, e_Timer1);
	timer_wait(e_Timer1);
}


/*****************************************************************************
  Function:      writeBit()  
  Description: 	 Writes one bit to the ibutton          
  Inputs:      	 bit: The bit which should be written to the ibutton			        				 	                                                                       
  Outputs:    	 None                                                                                                                                      
  Return:      	 None  	
  Globals:       None
  Important:                                                    	
*******************************************************************************/
static void writeBit(bool bit)
{
	if (bit)
	{
		// Write ‘1’ bit
		setWR_1Wire_Low();
		delayMicroSec(WR_ONE_WAIT_AtoB);
		setWR_1Wire_High();
		delayMicroSec(WR_ONE_WAIT_BtoC);
		activatePullUp();
		delayMicroSec(WR_ONE_WAIT_CtoD);
		deactivatePullUp();
		delayMicroSec(WR_ONE_WAIT_DtoNEXT);	
	}
	else
	{
		// Write ‘0’ bit
		setWR_1Wire_Low();
		delayMicroSec(WR_ZERO_WAIT_AtoB);
		setWR_1Wire_High();
		delayMicroSec(WR_ZERO_WAIT_BtoC);
		activatePullUp();
		delayMicroSec(WR_ZERO_WAIT_CtoD);
		deactivatePullUp();
		delayMicroSec(WR_ZERO_WAIT_DtoNEXT);	
	}
}


/*****************************************************************************
  Function:      readBit()  
  Description: 	 Reads one bit from the ibutton          
  Inputs:      	 None			        				 	                                                                       
  Outputs:    	 None                                                                                                                                   
  Return:      	 The bit which was read from the ibutton    	
  Globals:       None
  Important:                                                    	
*******************************************************************************/
static bool readBit(void)
{
	UINT32 read1;
	UINT32 read2;

	setWR_1Wire_Low();
	delayMicroSec(READ_WAIT_AtoB);
	setWR_1Wire_High();
	delayMicroSec(READ_WAIT_BtoC1);
	read1 = read_1Wire();
	delayMicroSec(READ_WAIT_C1toC2);
	read2 = read_1Wire();
	delayMicroSec(READ_WAIT_C2toD);
	activatePullUp();
	delayMicroSec(READ_WAIT_DtoE);
	deactivatePullUp();
	delayMicroSec(READ_WAIT_EtoNEXT);
	
	if (read1 == 0)
	{
		return(0);
	}
	else 
	{
	   	return(1); 
	}
}


/*****************************************************************************
  Function:      iButL_reset()  
  Description: 	 Resets the ibutton and checks for presence           
  Inputs:      	 None			        				 	                                                                       
  Outputs:    	 None                                                                                                                                      
  Return:      	 TRUE if device is present, else return FALSE   	
  Globals:       None
  Important:                                                    	
*******************************************************************************/ 
bool iButL_reset(void)
{
	UINT32 read1;
	UINT32 read2;
	UINT32 read3;

	setWR_1Wire_Low();
	delayMicroSec(RESET_WAIT_AtoB);
	setWR_1Wire_High();
	delayMicroSec(RESET_WAIT_BtoC);
	activatePullUp();
	delayMicroSec(RESET_WAIT_CtoD);
	deactivatePullUp();
	delayMicroSec(RESET_WAIT_DtoE1);
	read1 = read_1Wire();
	delayMicroSec(RESET_WAIT_E1toE2);
	read2 = read_1Wire();
	delayMicroSec(RESET_WAIT_E2toF);
	read3 = read_1Wire();
	delayMicroSec(RESET_WAIT_FtoG);
	activatePullUp();
	delayMicroSec(RESET_WAIT_GtoH);
	deactivatePullUp();
	delayMicroSec(RESET_WAIT_HtoNEXT);

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


/*****************************************************************************
  Function:      iButL_writeByte()  
  Description: 	 Writes one byte to the ibutton, it simply calls the writeBit
            	 8 times
  Inputs:      	 data: The byte which should be written to the ibutton			        				 	                                                                       
  Outputs:    	 None                                                                                                                                   
  Return:      	 None  	
  Globals:       None
  Important:                                                    	
*******************************************************************************/
void iButL_writeByte(UINT32 data)
{
	UINT32 loop;
	// Loop to write each bit in the byte, LS-bit first
	for (loop = 0; loop < 8; loop++)
	{
		writeBit(data & 0x01);
		// shift the data byte for the next bit
		data >>= 1;
	}
}


/*****************************************************************************
  Function:      iButL_readByte()  
  Description: 	 Reads one byte from the ibutton, it calls the readBit 8 times,
  				 it reads the LSB first          
  Inputs:      	 None			        				 	                                                                       
  Outputs:    	 None                                                                                                                                   
  Return:      	 The byte which was read from the ibutton    	
  Globals:       None
  Important:                                                    	
*******************************************************************************/
UINT32 iButL_readByte(void)
{
    bool read;
	UINT32 loop; 
	UINT32 result = 0;
	
	for (loop = 0; loop < 8; loop++)
	{
		// shift the result to get it ready for the next bit
		result >>= 1;
		// if result is one, then set MS bit
		read = readBit();
		
		if (read)
		{
			result |= 0x80;
		}
	}
	return(result);
}


/*****************************************************************************
  Function:      iButL_touchByte()  
  Description: 	 Writes one data byte and returns the sampled result          
  Inputs:      	 data: The byte which should be written to the ibutton				        				 	                                                                       
  Outputs:    	 None                                                                                                                                   
  Return:      	 The byte which was sampled     	
  Globals:       None
  Important:                                                    	
*******************************************************************************/
UINT32 iButL_touchByte(UINT32 data)  
{
	UINT32 loop;
	UINT32 result = 0;

	for (loop = 0; loop < 8; loop++)
	{
		// shift the result to get it ready for the next bit
		result >>= 1;

		if(data & 0x01)			// if sending a '1' then read a bit else write a '0' 
		{
			if(readBit())
			{
				result |= 0x80;	
			}
		}
		else
		{
			writeBit(0);
		}
		data >>= 1;
	}

	return(result);
}


/*****************************************************************************
  Function:      iButL_touchBlock()  
  Description: 	 Writes a block of data and return the sampled result in the 
  				 same buffer         
  Inputs:      	 data: The bytes which should be written to the ibutton
  				 data_length: The number of bytes of the data				        				 	                                                                       
  Outputs:    	 data: The bytes which were sampled from the ibutton	                                                                                                                                  
  Return:      	 The bytes which were sampled     	
  Globals:       None
  Important:                                                    	
*******************************************************************************/
void iButL_touchBlock(UINT32 *data, UINT32 dataLength) 
{
	UINT32 loop;

	for (loop = 0; loop < dataLength; loop++)
	{
		data[loop] = iButL_touchByte(data[loop]);
	}
}

