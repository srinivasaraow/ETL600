/*-------------------------------------------------------------------------*
 * $Workfile: ISR.h $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : Public type and function declarations
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/H/ISR.h $
 * 
 * 2     19.04.07 16:18 Chhamaa
 * EX_INTERRUPT_HANDLER(Sport0_RX_ISR) moved to sport0.h
 * 
 * 1     17.04.07 17:30 Chhamaa
 * Initial version
 * 
 *
 *-------------------------------------------------------------------------*/
 
#ifndef ISR_H
#define ISR_H

/* INCLUDE FILES ***********************************************************/

#include <sys\exception.h>

#include "Basetype_O4CV.h"


/* PUBLIC CONSTANT DECLARATIONS ********************************************/

/* PUBLIC TYPE DECLARATIONS ************************************************/

/* PUBLIC MACRO DEFINITIONS ************************************************/

/* PUBLIC FUNCTIONS ********************************************************/

void ISR_enable(void);


#endif

