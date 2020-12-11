/*-------------------------------------------------------------------------*
 * $Workfile	: watchdog.h $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Allen Notter (AN)
 * Remarks      :  
 * Purpose      : Supervising of the system and generating of hw resets 
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/H/watchdog.h $
 * 
 * 2     07-09-06 15:00 Chalnot
 * New function watchdog_getHwResetFlag() added.
 * 
 * 1     07-08-14 14:31 Chalnot
 * Initial version.
 *
 *
 *-------------------------------------------------------------------------*/
 
#ifndef WATCHDOG_H
#define WATCHDOG_H

/* INCLUDE FILES **************************************************************/

#include "Basetype_O4CV.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

/* PUBLIC TYPE DECLARATIONS ***************************************************/

/* PUBLIC MACRO DEFINITIONS ***************************************************/

/* PUBLIC FUNCTIONS ***********************************************************/

extern void watchdog_activate(void);
extern void watchdog_deactivate(void);
extern void watchdog_setTime(UINT32 microSeconds);
extern void watchdog_refresh(void);
extern void watchdog_hwReset(void);
extern bool watchdog_getHwResetFlag(void);

#endif

