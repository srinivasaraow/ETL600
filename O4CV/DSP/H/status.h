/*-------------------------------------------------------------------------*
 * $Workfile	: status.h $
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
 * $Log: /branches/Source O4CV/O4CV/DSP/H/status.h $
 * 
 * 3     07-10-15 16:02 Chalnot
 * New function status_clearAllAlarmsAndWarnings() added.
 * 
 * 2     07-08-30 13:59 Chrobif
 * functions status_enableLeds(), status_disableLeds() and
 * status_activate() added
 * 
 * 1     16.04.07 14:23 Chrobif
 * Initial version.
 * 
 *-------------------------------------------------------------------------*/
 
#ifndef STATUS_H
#define STATUS_H

/* INCLUDE FILES **************************************************************/
#include "Basetype_O4CV.h"
#include "O4CV_Types.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/
/* PUBLIC TYPE DECLARATIONS ***************************************************/
/* PUBLIC MACRO DEFINITIONS ***************************************************/
/* PUBLIC FUNCTIONS ***********************************************************/
extern void status_init(void);
extern t_O4CVStatus* status_getPCurrentStatus(void);
extern void status_setAlarm(    UINT16 alarmBit);
extern void status_setWarning(  UINT16 warningBit);
extern void status_clearAlarm(  UINT16 alarmBit);
extern void status_clearWarning(UINT16 warningBit);
extern void status_clearAllAlarmsAndWarnings(void);
extern void status_enableLeds(void);
extern void status_disableLeds(void);
extern void status_activate(void);

#endif
