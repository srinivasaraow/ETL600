/*-------------------------------------------------------------------------*
 * $Workfile: ISR.c $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : Interrupt service routines
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/C/ISR.c $
 * 
 * 3     07-06-14 7:47 Chvopog
 * Interrupt priority changed
 * 
 * 2     19.04.07 16:16 Chhamaa
 * EX_INTERRUPT_HANDLER(Sport0_RX_ISR) moved to sport0.c
 * 
 * 1     17.04.07 17:31 Chhamaa
 * Initial version
 * 
 *
 *-------------------------------------------------------------------------*/
 
/* INCLUDE FILES ***********************************************************/

#include <sysreg.h>
#include <ccblkfn.h>
#include <cdefBF538.h>

#include "ISR.h"
#include "Basetype_O4CV.h"
#include "sport0.h"


/* LOCAL CONSTANT DECLARATIONS *********************************************/

/* LOCAL TYPE DECLARATIONS *************************************************/

/* LOCAL VARIABLE DECLARATIONS *********************************************/

/* LOCAL MACRO DEFINITIONS *************************************************/

/* LOCAL FUNCTION DEFINITIONS **********************************************/



/* IMPLEMENTATION **********************************************************/

/***************************************************************************
  Function:		ISR_enable()
  Description:	Enables interrupts
  Inputs:		-
  Outputs:		-
  Return:		-
  Globals:		-
  Important: 	-
                                          	
****************************************************************************/

void ISR_enable(void)
{
	// set system interrupt assignment (SIC_IARx) registers
	//*pSIC_IAR0 = ;
	//*pSIC_IAR1 = ;
	//*pSIC_IAR2 = ;

	// assign ISRs to interrupt vectors
	// Sport0 RX ISR -> IVG 13
	register_handler(ik_ivg13, Sport0_RX_ISR);	
	// enable interrupts
	*pSIC_IMASK0 = *pSIC_IMASK0 | 0x00000200;		// Sport0 RX
	*pSIC_IAR1 = (*pSIC_IAR1 & 0xFFFFFF0F) | 0x00000060;// assign DMA1 Rx  irq #13	
	//*pSIC_IAR1 = (*pSIC_IAR1 & 0xFFFFFF0F) | 0x00000000;// assign DMA1 Rx  irq #13	
	ssync();

}





    
    


