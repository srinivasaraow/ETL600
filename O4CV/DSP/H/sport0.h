/*-------------------------------------------------------------------------*
 * $Workfile: sport0.h $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : Public type and function declarations
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/H/sport0.h $
 * 
 * 4     08-02-15 9:34 Chrobif
 * + sport0_isExtTdmBusRunning() added.
 * 
 * 3     10.08.07 11:58 Chhamaa
 * Function sport0_stopExtTdmBus() added.
 * 
 * 2     20.04.07 15:05 Chhamaa
 * EX_REENTRANT_HANDLER used for sport0 ISR
 * 
 * 1     19.04.07 16:19 Chhamaa
 * Initial version
 * 
 *
 *-------------------------------------------------------------------------*/
 
#ifndef SPORT0_H
#define SPORT0_H

/* INCLUDE FILES ***********************************************************/

#include <sys\exception.h>

#include "Basetype_O4CV.h"


/* PUBLIC CONSTANT DECLARATIONS ********************************************/

/* PUBLIC TYPE DECLARATIONS ************************************************/

/* PUBLIC MACRO DEFINITIONS ************************************************/

/* PUBLIC FUNCTIONS ********************************************************/

void sport0_initExtTdmBus(INT16 basicTimeSlot, INT16 boardNumber);
void sport0_startExtTdmBus(void);
void sport0_stopExtTdmBus(void);
bool sport0_isExtTdmBusRunning(void);

EX_REENTRANT_HANDLER(Sport0_RX_ISR);

#endif

