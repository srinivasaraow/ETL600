/*-------------------------------------------------------------------------*
 * $Workfile: Initialize.c $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : 
 * Remarks      :  
 * Purpose      : DSP initialization
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/O4CV/DSP/C/Initialize.c $
 * 
 * 13    23.06.10 17:05 Chhamaa
 * Init_getBasicTimeSlotID(): bit mask modified.
 * 
 * 10    07-11-27 16:21 Chrobif
 * + Last GPIO configured as output in series HW case
 * 
 * 9     12.09.07 14:51 Chhamaa
 * Port E, pin 9 set to 1 (to disable TDM input buffer).
 * 
 * 8     07-09-07 13:54 Chalnot
 * Hook and ew gpios set high.
 * 
 * 7     07-08-09 11:32 Chalnot
 * Init_flags changed for test input.
 * 
 * 6     07-08-08 10:37 Chalnot
 * Init_flags corrected.
 * 
 * 5     07-08-08 8:50 Chalnot
 * Init_flags corrected.
 * 
 * 4     07-05-30 11:22 Chvopog
 * Initialisation of  GPIOs added 
 * 
 * 3     07-05-24 13:44 Chvopog
 * 
 * 2     23.04.07 14:25 Chhamaa
 * Function Init_getBasicTimeSlotID() added.
 * 
 * 1     16.04.07 16:12 Chhamaa
 * Initial version
 * 
 *
 *-------------------------------------------------------------------------*/
 
/* INCLUDE FILES ***********************************************************/

#include <ccblkfn.h>
#include <cdefBF538.h>

#include "Initialize.h"


/* LOCAL CONSTANT DECLARATIONS *********************************************/

// PLL 
#define SSEL_4			0x0004
#define CSEL_1			0x0000
#define MSEL_20			0x2800
#define DF_EN			0x0000


/* LOCAL TYPE DECLARATIONS *************************************************/

/* LOCAL VARIABLE DECLARATIONS *********************************************/

/* LOCAL MACRO DEFINITIONS *************************************************/

/* LOCAL FUNCTION DEFINITIONS **********************************************/



/* IMPLEMENTATION **********************************************************/

/***************************************************************************
  Function:		Init_flags()	
  Description:	Initilizes DSP input/output flags
  Inputs:		-
  Outputs:		-
  Return:		-
  Globals:		-
  Important: 	-
                                          	
****************************************************************************/

void Init_flags(void)

{
	
	*pPORTEIO_FER	= (*pPORTEIO_FER | 0xFF88);		// enable bits 3 & 7-15 as GPIO 
	*pPORTEIO_DIR	= (*pPORTEIO_DIR | 0x2F88);		// bits 3,7,8,9,10,11,13 are outputs 
	*pPORTEIO_INEN	= PF12 | PF14 | PF15;//input buffer Enable        
	*pPORTEIO_CLEAR = PF13 | PF3 | PF7;	
	*pPORTEIO_SET   = PF9 | PF11;	// Pin 9: disable TDM input buffer
			
	*pPORTCIO_FER	= 0x03F3;// enable bits 0,1,4,5,6,7,8,9 as GPIO 
	*pPORTCIO_DIR	= 0x03F3;//enable bits 0,1,4,5,6,7,8,9 as output
	*pPORTCIO_INEN	= 0x0000;//input buffer Disable 
	*pPORTCIO_SET 	= PF5 | PF6 | PF7 | PF8 | PF9;	
	*pPORTCIO_CLEAR = PF4 | PF0 | PF1;
						
	*pPORTDIO_FER	= 0x3FFF;//PD0-PD13 set as GPIO
	*pPORTDIO_SET	= 0x0FF0;//PD4-PD11 set high
	*pPORTDIO_DIR	= 0x0FF0;//PD4-PD11 output
	*pPORTDIO_INEN	= 0x300F;//input buffer Enable at PD0-PD3, PD12, PD13
	
#ifdef PROTOTYP_HW_NO_PULSE_CODE_DIALING_EXTENSION			
	*pPORTFIO_DIR	= 0x0000;//All inputs
	*pPORTFIO_INEN	= 0xFFF1;//input buffer Enable  PF1, PF4-PF15
#else
	*pPORTFIO_DIR	= 0x0001;//PF0 output
	*pPORTFIO_INEN	= 0xFFF0;//input buffer Enable PF4-PF15
#endif
	
}


/***************************************************************************
  Function:		Init_PLL()	
  Description:	Initializes DSP PLL
  				CCLK=500MHz, SCLK=125MHz, CLKIN=25MHz
  Inputs:		-
  Outputs:		-
  Return:		-
  Globals:		-
  Important: 	-
                                          	
****************************************************************************/

void Init_PLL(void)
{
	long tmp;
	

	*pSIC_IWR=0x01;
	*pPLL_DIV=  SSEL_4|CSEL_1;	
	
	// see HW reference guide for details		
	tmp = cli();
	*pPLL_CTL= MSEL_20;
	idle();
	sti(tmp);

	ssync();

}


/***************************************************************************
  Function:		Init_getBasicTimeSlotID()	
  Description:	Reads the basic time slot ID from the back plane.
  Inputs:		-
  Outputs:		-
  Return:		basic time slot ID
  Globals:		-
  Important: 	-
                                          	
****************************************************************************/

INT16 Init_getBasicTimeSlotID(void)
{
    unsigned short result_SlotID;

	result_SlotID = (*pPORTDIO & 0x0007);
	
	return result_SlotID;
}
