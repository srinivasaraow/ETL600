/*-------------------------------------------------------------------------*
 * $Workfile	: watchdog.c  $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Allen Notter (AN)
 * Remarks      :  
 * Purpose      : Supervising of the system and generating of hw resets 
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/C/watchdog.c $
 * 
 * 2     07-09-06 14:58 Chalnot
 * New function watchdog_getHwResetFlag() added.
 * Watchdog timer set to 1s in watchdog_hwReset().
 * 
 * 1     07-08-14 14:37 Chalnot
 * Initial version.
 * 
 *
 *-------------------------------------------------------------------------*/
 
/* INCLUDE FILES *************************************************************/

#include <cdefBF538.h>
#include "watchdog.h"

/* LOCAL CONSTANT DECLARATIONS ***********************************************/

/* LOCAL TYPE DECLARATIONS ***************************************************/

/* LOCAL VARIABLE DECLARATIONS ***********************************************/

static bool g_hwReset = false;

/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL FUNCTION DEFINITIONS ************************************************/

/* IMPLEMENTATION ************************************************************/

/*****************************************************************************
  Function:		void watchdog_activate(void)
  Description:	Activates the watchdog. Sets g_hwReset false.
  Inputs:		none
  Outputs:		none
  Return:       none
  Globals:		g_hwReset
  Important:	
*******************************************************************************/
void watchdog_activate(void)
{
	*pWDOG_CTL = 0x0000;
	g_hwReset = false;
}	


/*****************************************************************************
  Function:		watchdog_deactivate(void)
  Description:	Deactivates the watchdog.
  Inputs:		none
  Outputs:		none
  Return:   	none    
  Globals:		none
  Important:	
*******************************************************************************/
void watchdog_deactivate(void)
{
    *pWDOG_CTL = 0x0AD0;
}


/*****************************************************************************
  Function:		void watchdog_setTime(UINT32 microSeconds)
  Description:	Sets the initial time of the watchdog counter.  
  Inputs:		microSeconds: Time in microseconds. Max value is 34359738.
  Outputs:		none
  Return:   	none    
  Globals:		none
  Important:	Watchdog must be deactivated.
*******************************************************************************/
void watchdog_setTime(UINT32 microSeconds)
{
   	UINT32 tempCounts = 125 * microSeconds;
   	 
	*pWDOG_CNT = tempCounts;
}


/*****************************************************************************
  Function:		void watchdog_refresh(void)
  Description:	Sets the watchdog counter to the initial value and prevents
  				the interrupt if g_hwReset is false. 
  Inputs:		none
  Outputs:		none
  Return:   	none    
  Globals:		g_hwReset
  Important:
*******************************************************************************/
void watchdog_refresh(void)
{
    if(!g_hwReset)
    {
    	*pWDOG_STAT = 0x0000;
    }
}


/*****************************************************************************
  Function:		void watchdog_hwReset(void)
  Description:	Forces an hw reset after 1s. Sets g_hwReset true. 
  Inputs:		none
  Outputs:		none
  Return:   	none    
  Globals:		g_hwReset
  Important:	
*******************************************************************************/
void watchdog_hwReset(void)
{
    if(!g_hwReset)
    {
	    // Set watchdog time to 1s
		watchdog_deactivate();
		watchdog_setTime(1000000);
		watchdog_activate();
	
	    g_hwReset = true;
    }
} 


/*****************************************************************************
  Function:		bool watchdog_getHwResetFlag(void)
  Description:	Returns the state of the hw reset flag.
  Inputs:		none
  Outputs:		none
  Return:   	Hw reset flag
  Globals:		g_hwReset
  Important:	
*******************************************************************************/
bool watchdog_getHwResetFlag(void)
{
    return g_hwReset;
} 
