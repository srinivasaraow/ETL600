/*-------------------------------------------------------------------------*
 * $Workfile	: status.c  $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Roger Biffiger (RBi)
 * Remarks      :  
 * Purpose      : This module allocates memory for the O4CV status structure
 *				  and provides setter and getter functions to manipulate the
 *				  status structure.
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/O4CV/DSP/C/status.c $
 * 
 * 13    11-04-20 18:24 Chhamaa
 * O4CV_WARNING_MASK_LED applied.
 * 
 * 10    08-02-15 10:46 Chrobif
 * + DuSlic Sync Fail error fixed (again)
 * 
 * 9     08-02-06 7:52 Chrobif
 * + Pick up of DuSlic synchronisation error modified. Error is only
 * picked up if sport 1 is enabled.
 * 
 * 8     07-11-28 10:59 Chalnot
 * Typing error corrected.
 * 
 * 7     07-11-07 15:23 Chalnot
 * Only check for duslic overtemperature if FXS interface.
 * 
 * 6     07-10-15 16:01 Chalnot
 * New function status_clearAllAlarmsAndWarnings() added.
 * 
 * 5     07-09-28 14:24 Chalnot
 * Clear duslic sync alarm if ibutton has an alarm. 
 * 
 * 4     07-09-26 10:44 Chalnot
 * Duslic errors handled.
 * 
 * 3     07-09-04 16:50 Chalnot
 * Sw errors handled.
 * Control of rdy led implemented.
 * 
 * 2     07-08-30 14:04 Chrobif
 * functions status_enableLeds(), status_disableLeds() and
 * status_activate() added. status renamed to gs_status.
 * 
 * 1     16.04.07 14:23 Chrobif
 * Initial version.
 *-------------------------------------------------------------------------*/
 
/* COMPILER SWITCHES *******************************************************/
					
//#define BLINK_LED		// activates blinking of the ready led 
 
/* INCLUDE FILES *************************************************************/
// C Run-Time Library
#include <string.h>

// O4CV firmware
#include "status.h"
#include "binInOut.h"
#include "app.h"
#include "extTdmBus.h"
#include "DuSlic.h"
#include "sport1.h"
#include "sport0.h"

/* LOCAL CONSTANT DECLARATIONS ***********************************************/

#define RUNTIME_FAILURE_COUNTER_ALARM_VALUE		10
#define DUSLIC_FAILURE_COUNTER_ALARM_VALUE		107	// 1s
#define MAIN_LOOP_COUNTER_INIT_VALUE			4	// How many times the main loop is slower than 107hz

/* LOCAL TYPE DECLARATIONS ***************************************************/
/* LOCAL VARIABLE DECLARATIONS ***********************************************/
static t_O4CVStatus gs_status;
static t_AppStatus* gs_pAppStatus;
static UINT16 gs_swRuntimeFailureCounter[N_CHANNELS_ON_BOARD_PLUS_SPH];
static UINT16 gs_duslicSyncFailureCounter[N_CHANNELS_ON_BOARD_PLUS_SPH];
static UINT16 gs_duslicEdspFailureCounter[N_CHANNELS_ON_BOARD_PLUS_SPH];
static UINT16 gs_duslicOvertemperatureCounter[N_CHANNELS_ON_BOARD_PLUS_SPH];
static bool gs_ledsEnabled;

/* LOCAL MACRO DEFINITIONS ***************************************************/
/* LOCAL FUNCTION DEFINITIONS ************************************************/
/* IMPLEMENTATION ************************************************************/

/*****************************************************************************
  Function:		void status_init(void)
  Description:	status_init sets all members of the O4CV status structure to zero.
  Inputs:
  Outputs:
  Return:       	
  Globals:
  Important:
*******************************************************************************/
void status_init(void)
{
    memset(&gs_status, 0, sizeof(gs_status));
    memset(&gs_swRuntimeFailureCounter, 0, sizeof(gs_swRuntimeFailureCounter));
    memset(&gs_duslicSyncFailureCounter, 0, sizeof(gs_duslicSyncFailureCounter));
    memset(&gs_duslicEdspFailureCounter, 0, sizeof(gs_duslicEdspFailureCounter));
    memset(&gs_duslicOvertemperatureCounter, 0, sizeof(gs_duslicOvertemperatureCounter));
	gs_ledsEnabled = false;
	gs_pAppStatus = app_getStatusPtr();
}

/*****************************************************************************
  Function:		t_O4CVStatus* status_getPCurrentStatus(void)
  Description:	status_getPCurrentStatus returns a pointer to the 
  				current status data structure.
  Inputs:
  Outputs:
  Return:       t_O4CVStatus*: Pointer to the current status data structure	
  Globals:
  Important:                                      	
*******************************************************************************/
t_O4CVStatus* status_getPCurrentStatus(void)
{
    return &gs_status;
}

/*****************************************************************************
  Function:		void status_setAlarm(UINT16 alarmBit)
  Description:	status_setAlarm ors the i/p argument alarmBit with the member
  				alarms of the internal status structure.
  Inputs:		UINT16 alarmBit: Alarm bit to set.
  Outputs:
  Return:
  Globals:
  Important:	Note that alarmBit is ored with the status structure member
  				alarms.
*******************************************************************************/
void status_setAlarm(UINT16 alarmBit)
{
    gs_status.alarms |= alarmBit;
}

/*****************************************************************************
  Function:		void status_setWarning(UINT16 warningBit);
  Description:	status_setWarning ors the i/p argument warningBit with the member
  				warnings of the internal status structure.
  Inputs:		UINT16 warningBit: Warning bit to set.
  Outputs:
  Return:
  Globals:
  Important:	Note that warningBit is ored with the status structure member
  				warnings.
*******************************************************************************/
void status_setWarning(UINT16 warningBit)
{
    gs_status.warnings |= warningBit;
}

/*****************************************************************************
  Function:		void status_clearAlarm(UINT16 alarmBit)
  Description:	status_clearAlarm ands the inverted i/p argument alarmBit with the
  				member alarms of the internal status structure.
  Inputs:		UINT16 alarmBit: Alarm bit to clear.
  Outputs:
  Return:
  Globals:
  Important:	Note that the inverted alarmBit is anded with the status structure member
  				alarms.
*******************************************************************************/
void status_clearAlarm(UINT16 alarmBit)
{
    gs_status.alarms &= ~alarmBit;
}

/*****************************************************************************
  Function:		void status_clearWarning(UINT16 warningBit);
  Description:	status_clearWarning ands the inverted i/p argument warningBit 
  				with the member	warnings of the internal status structure.
  Inputs:		UINT16 warningBit: Warning bit to clear.
  Outputs:
  Return:
  Globals:
  Important:	Note that the inverted warningBit is anded with the status structure member
  				warnings.
*******************************************************************************/
void status_clearWarning(UINT16 warningBit)
{
    gs_status.warnings &= ~warningBit;
}

/*****************************************************************************
  Function:		void status_clearAllAlarmsAndWarnings(void)
  Description:	status_clearAllAlarmsAndWarnings clears all alarms and warnings
  				in the internal status structure except the warning 
  				O4CV_WARNING_TEMP_CONFIGURATION_ACTIVE.
  Inputs:		
  Outputs:
  Return:
  Globals:
  Important:	The warning O4CV_WARNING_TEMP_CONFIGURATION_ACTIVE is not cleared!
*******************************************************************************/
void status_clearAllAlarmsAndWarnings(void)
{
 	gs_status.alarms = 0;  
 	gs_status.warnings &= O4CV_WARNING_TEMP_CONFIGURATION_ACTIVE; 
}

/*****************************************************************************
  Function:		void status_enableLeds(void)
  Description:	status_enableLeds() enables the signalisation of alarms and
  				warnings through the LEDs.
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		gs_ledsEnabled
  Important:	Call this function after status_init().
*******************************************************************************/
void status_enableLeds(void)
{
	gs_ledsEnabled = true;
}

/*****************************************************************************
  Function:		void status_disableLeds(void)
  Description:	status_disableLeds() disables the signalisation of alarms and
  				warnings through the LEDs.
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		gs_ledsEnabled
  Important:	Call this function after status_init().
*******************************************************************************/
void status_disableLeds(void)
{
	gs_ledsEnabled = false;
}

/*****************************************************************************
  Function:		void status_activate(void)
  Description:	status_activate() checks the warning and alarm members in the
  				O4CV status struct and sets or clears the warning and alarm
				LED accordingly.
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		gs_status, gs_ledsEnabled
  Important:	-
*******************************************************************************/
void status_activate(void)
{
    static UINT16 mainLoopCounter = 0;
    UINT32 i = 0;
    
    // Check software alarms
    
    // Only check every x time, because this function is called more often than app_en/decode which sets the failure flags       
    if(mainLoopCounter <= 1)
    {
	    for(i = 0; i < N_CHANNELS_ON_BOARD_PLUS_SPH; i++)
	    {
	        // Increment counter if runtime failure flag is set
		    if(gs_pAppStatus->runtimeFailure[i] == true)
		    {
		      	gs_swRuntimeFailureCounter[i]++; 
		      	 
		       	// Reset runtime failure flag
		       	gs_pAppStatus->runtimeFailure[i] == false;
		    }
		    else
		    {
		    	gs_swRuntimeFailureCounter[i] = 0;     
		    }
	   
	        // Set sw runtime alarm if runtime failure flag was set for certain times
	        if(gs_swRuntimeFailureCounter[i] >= RUNTIME_FAILURE_COUNTER_ALARM_VALUE)
	        {
	            gs_swRuntimeFailureCounter[i] = 0;
	          	gs_status.alarms |= O4CV_ALARM_SW_RUNTIME_ERROR;  
	        }
        
	        // Set sw init alarm if init failure flag is set 
	        if(gs_pAppStatus->initFailure[i])
	        {
	          	gs_status.alarms |= O4CV_ALARM_SW_INIT_ERROR;  
	        }
	    }
	
	    mainLoopCounter = MAIN_LOOP_COUNTER_INIT_VALUE;   
    }
    else
    {
    	mainLoopCounter--;  
    }
    
    // Check duslic alarms
	for(i = 0; i < N_CHANNELS_ON_BOARD_PLUS_SPH; i++)
	{
	    // Check if channel is configured as FXS interface
	    if(((i < N_CHANNELS_ON_BOARD)
	    		&& configuration_is2wireConfigured(i)
	    		&& (configuration_get2WireIfType(i) == e_FXS))
	       	|| ((i == SPH) && (configuration_isServicePhoneActive())))		
	    {	
	        	
		  	// Increment counter if there is an error
			if(
				sport0_isExtTdmBusRunning() &&			// There are three conditions that must be met
				extTdmBus_getReadDuSlicIntFlag() &&		// before the result of DuSlic_isSyncFail()
				sport1_isTxRunning() &&					// is accurate.
				DuSlic_isSyncFail(i)
			  )
			{
			 	gs_duslicSyncFailureCounter[i]++; 
			}
			else
			{
			    gs_duslicSyncFailureCounter[i] = 0;     
			}
		
			if(DuSlic_isEDSPFail(i))
			{
			 	gs_duslicEdspFailureCounter[i]++; 
			}
			else
			{
			    gs_duslicEdspFailureCounter[i] = 0;     
			}
		
			if(DuSlic_isOverTemperature(i))
			{
			 	gs_duslicOvertemperatureCounter[i]++; 
			}
			else
			{
			    gs_duslicOvertemperatureCounter[i] = 0;     
			}
	   
		    // Set duslic alarm if error occured for a certain time
		    if(gs_duslicSyncFailureCounter[i] >= DUSLIC_FAILURE_COUNTER_ALARM_VALUE)
		    {
		        gs_duslicSyncFailureCounter[i] = 0;
	        
		        // Don't set alarm if ibutton has an alarm
		        if((gs_status.alarms & (O4CV_ALARM_INVALID_LICENSE | O4CV_ALARM_INSUFFICIENT_LICENSE)) == 0)
		        {
		            gs_status.alarms |= O4CV_ALARM_DUSLIC_SYNC_FAILED; 
		        }
		    }
	    
		    // Always clear alarm if ibutton has an alarm
		    if((gs_status.alarms & (O4CV_ALARM_INVALID_LICENSE | O4CV_ALARM_INSUFFICIENT_LICENSE)) != 0)
		    {
		            gs_status.alarms &= ~O4CV_ALARM_DUSLIC_SYNC_FAILED;  
		    }
	    
		    if(gs_duslicEdspFailureCounter[i] >= DUSLIC_FAILURE_COUNTER_ALARM_VALUE)
		    {
		        gs_duslicEdspFailureCounter[i] = 0;
		        gs_status.alarms |= O4CV_ALARM_DUSLIC_DSP_FAILED;  
		    }
	    
		    if(gs_duslicOvertemperatureCounter[i] >= DUSLIC_FAILURE_COUNTER_ALARM_VALUE)
		    {
		        gs_duslicOvertemperatureCounter[i] = 0;
		        gs_status.alarms |= O4CV_ALARM_OVERTEMPERATURE;  
		    }
	    }
	}
	    

    // Switch leds according to the status
	if (gs_ledsEnabled)
	{
	    // Set warning led
		if ((gs_status.warnings & O4CV_WARNING_MASK_LED) == O4CV_WARNING_NO_WARNING)
		{
			binInOut_setOutput(LED_OFF, e_Warning_Led);
		}
		else
		{
			binInOut_setOutput(LED_ON,  e_Warning_Led);
		}
		
		// Set alarm and ready leds 
		if (gs_status.alarms == O4CV_ALARM_NO_ALARM)	
		{
			binInOut_setOutput(LED_OFF, e_Alarm_Led);
			extTdmBus_setRdyLedBlinking(false);
			
#ifndef BLINK_LED	
		
			binInOut_setOutput(LED_ON, e_Ready_Led);
#endif
		}
		else
		{
		    binInOut_setOutput(LED_ON, e_Alarm_Led);
		    extTdmBus_setRdyLedBlinking(true);
		}
		
#ifdef BLINK_LED
		
		extTdmBus_setRdyLedBlinking(true);	
#endif

	}
}

