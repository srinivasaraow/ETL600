/*-------------------------------------------------------------------------*
 * $Workfile: Status.h $
 * Part of      : ETL600
 * Language     : C
 * Created by   : Christian Leeb
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/P4LT/Master_DSP/H/Status.h $
 * 
 * 6     4.04.08 11:20 Chchlee
 * Comment added
 * 
 * 1     19.01.05 15:50 Chchlee
 * 
 * 6     3.01.05 11:36 Chchlee
 * 
 * 5     30.12.04 11:40 Chchlee
 * t_FastStatusTelegram type modified
 * 
 * 4     27.12.04 13:11 Chchlee
 * status_getPTestSwitches added
 * 
 * 3     22.11.04 16:24 Leeb02
 * new t_FastStatusTelegram struct
 * 
 * 2     15.11.04 8:05 Leeb02
 * 
 * 1     15.11.04 7:34 Leeb02
 * 
 *-------------------------------------------------------------------------*/

#ifndef STATUS_H
#define STATUS_H

/* INCLUDE FILES **************************************************************/

#include "P4LT_Types.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/



/* PUBLIC TYPE DECLARATIONS ***************************************************/

typedef struct 	
{
		unsigned long	    fwCompatibilityIndex;	
		unsigned long		highLevelAlarms;		
        unsigned long	    deviceState;	// Bit masks for deviceState are found in P4LT_Types.h
}

t_FastStatusTelegram; 

/* PUBLIC MACRO DEFINITIONS ***************************************************/




/* PUBLIC FUNCTIONS ***********************************************************/




// must be called before any configuration data structures are touched!
void status_init(void);


t_P4LT_Status *        status_getPStatus(void);
t_FastStatusTelegram * status_getLocalFastStatus(void);
t_FastStatusTelegram * status_getRemoteFastStatus(void);

t_TestPlcLinkData    * status_getPTestSwitches(void);

#endif
