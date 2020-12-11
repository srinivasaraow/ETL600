/*-------------------------------------------------------------------------*
 * $Workfile	: timer.c  $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Allen Notter (AN)
 * Remarks      :  
 * Purpose      : Time delays and periodic interrupts
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/C/timer.c $
 * 
 * 2     07-08-08 16:20 Chalnot
 * Small time offset subtracted.
 * 
 * 1     07-07-20 16:29 Chalnot
 * Initial version.
 * 
 *
 *-------------------------------------------------------------------------*/
 
/* INCLUDE FILES *************************************************************/

#include <cdefBF538.h>
#include "timer.h"

/* LOCAL CONSTANT DECLARATIONS ***********************************************/

/* LOCAL TYPE DECLARATIONS ***************************************************/

/* LOCAL VARIABLE DECLARATIONS ***********************************************/

/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL FUNCTION DEFINITIONS ************************************************/

/* IMPLEMENTATION ************************************************************/

/*****************************************************************************
  Function:		void timer_init(UINT32 microSeconds, e_TimerId timerId)
  Description:	initializes one of the three timers and starts it 
  Inputs:		microSeconds: Delay in microseconds. Max value is 34359738.
  				timerId: Shows which of the three general purpose timer should
  				be used.
  Outputs:		none
  Return:       none
  Globals:		none
  Important:
*******************************************************************************/
void timer_init(UINT32 microSeconds, e_TimerId timerId)
{
	
	UINT32 tempCounts;
	
	if(microSeconds == 0)
	{
	  	tempCounts = 0;
	}
	else
	{  
	 	tempCounts = (125 * microSeconds) - 75;
	}
	
	switch(timerId)
	{
	 	case e_Timer1: 
	 		// Set length of delay
			*pTIMER0_WIDTH 	= tempCounts;
	
			// Configure the timer
			*pTIMER0_CONFIG = (PWM_OUT | IRQ_ENA | OUT_DIS); 
			
	 		// Clear the interrupt flag
			*pTIMER_STATUS = TIMIL0;
			
			// Mask timer interrupt in the system interrupt controller
			*pSIC_IMASK0 &= ~TIMER0_IRQ;
	
			// Start the timer
			*pTIMER_ENABLE 	|= TIMEN0;
	
			break;
			
		case e_Timer2:  
			// Set length of delay
			*pTIMER1_WIDTH 	= tempCounts;
	
			// Configure the timer
			*pTIMER1_CONFIG = (PWM_OUT | IRQ_ENA | OUT_DIS);
		
		 	// Clear the interrupt flag
			*pTIMER_STATUS = TIMIL1;
	
			// Mask timer interrupt in the system interrupt controller
			*pSIC_IMASK0 &= ~TIMER1_IRQ; 
			
			// Start the timer
			*pTIMER_ENABLE 	|= TIMEN1;
						
			break;
			
		case e_Timer3: 
			// Set length of delay
			*pTIMER2_WIDTH 	= tempCounts;
	
			// Configure the timer
			*pTIMER2_CONFIG = (PWM_OUT | IRQ_ENA | OUT_DIS); 
			 
		 	// Clear the interrupt flag
			*pTIMER_STATUS = TIMIL2;
			
			// Mask timer interrupt in the system interrupt controller
			*pSIC_IMASK0 &= ~TIMER2_IRQ;
	
			// Start the timer
			*pTIMER_ENABLE 	|= TIMEN2;
						
			break;
			
		default: 	
						
			break;
	}

}	


/*****************************************************************************
  Function:		void timer_wait(e_TimerId timerId)
  Description:	waits until the timer is finished
  Inputs:		timerId: Shows on which of the three general purpose timer should
  				be waited.
  Outputs:		none
  Return:   	none    
  Globals:		none
  Important:	The timer must be started with timer_init otherwise timer_wait
				will never end.
*******************************************************************************/
void timer_wait(e_TimerId timerId)
{

    switch(timerId)
	{
	 	case e_Timer1:  
			// Wait until the timer has set the interrupt flag
			while((*pTIMER_STATUS & TIMIL0) == 0 ){	}
	
			// Clear the interrupt flag
			*pTIMER_STATUS = TIMIL0;
			
			break;
			
		case e_Timer2:  
		 	// Wait until the timer has set the interrupt flag
			while((*pTIMER_STATUS & TIMIL1) == 0 ){	}
	
			// Clear the interrupt flag
			*pTIMER_STATUS = TIMIL0;
			
			break;
			
		case e_Timer3:  
		 	// Wait until the timer has set the interrupt flag
			while((*pTIMER_STATUS & TIMIL2) == 0 ){	}
	
			// Clear the interrupt flag
			*pTIMER_STATUS = TIMIL2;
			
			break;
			
		default: 	
						
			break;
	}
	    
}


/*****************************************************************************
  Function:		void timer_initInterruptTimer(UINT32 microSeconds)
  Description:	initializes the core timer with the interval time 
  Inputs:		microSeconds: Interval in microseconds. Max value is 34359738.
  Outputs:		none
  Return:   	none    
  Globals:		none
  Important:
*******************************************************************************/
void timer_initInterruptTimer(UINT32 microSeconds)
{
   	UINT32 tempCounts = 125 * microSeconds;
   	 
   	// Set interval between the interrupts
	*pTPERIOD = tempCounts;
	
	// Set prescale 
	*pTSCALE = 3;		// effective prescale is +1 => 4
	
}


/*****************************************************************************
  Function:		void timer_startInterruptTimer(void)
  Description:	starts the core timer and enables auto reload
  Inputs:		none
  Outputs:		none
  Return:   	none    
  Globals:		none
  Important:
*******************************************************************************/
void timer_startInterruptTimer(void)
{
    // Mask core timer interrupt in the core event controller
    *pIMASK &= ~EVT_IVTMR;
    
    // Enable core timer with auto reload
	*pTCNTL = TMPWR | TMREN | TAUTORLD;
    
}


/*****************************************************************************
  Function:		void timer_stopInterruptTimer(void)
  Description:	stops the core timer
  Inputs:		none
  Outputs:		none
  Return:   	none    
  Globals:		none
  Important:
*******************************************************************************/
void timer_stopInterruptTimer(void)
{
    // Disable core timer
	*pTCNTL &= ~(TMPWR | TMREN);
} 


/*****************************************************************************
  Function:		bool timer_getInterruptFlag(void)
  Description:	checks the status bit of the core timer and returns its value
  Inputs:		none
  Outputs:		none
  Return:   	true if the core timer has generated an interrupt, otherwise 
  				false    
  Globals:		none
  Important:
*******************************************************************************/
bool timer_getInterruptFlag(void)
{

    // Check if timer has generated an interrupt
    if((*pTCNTL & TINT) != 0)
    {
        // Reset interrupt flag
        *pTCNTL &= ~TINT;
        
     	return TRUE;   
    }
    else
    {
        return FALSE;
    }
    
}

