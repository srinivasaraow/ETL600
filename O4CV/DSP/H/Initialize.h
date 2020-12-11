/*-------------------------------------------------------------------------*
 * $Workfile: Initialize.h $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : 
 * Remarks      :  
 * Purpose      : Public type and function declarations
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/H/Initialize.h $
 * 
 * 2     23.04.07 14:25 Chhamaa
 * Function Init_getBasicTimeSlotID() added.
 * 
 * 1     16.04.07 16:16 Chhamaa
 * Initial version
 * 
 *
 *-------------------------------------------------------------------------*/
 
#ifndef INITIALIZATION_H
#define INITIALIZATION_H

/* INCLUDE FILES ***********************************************************/

#include "Basetype_O4CV.h"


/* PUBLIC CONSTANT DECLARATIONS ********************************************/

/* PUBLIC TYPE DECLARATIONS ************************************************/

/* PUBLIC MACRO DEFINITIONS ************************************************/

/* PUBLIC FUNCTIONS ********************************************************/

void Init_flags(void);
void Init_PLL(void);
INT16 Init_getBasicTimeSlotID(void);


#endif

