/*-------------------------------------------------------------------------*
 * $Workfile	: iButHigh.h  $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Allen Notter (AN)
 * Remarks      :  
 * Purpose      : High level iButton access
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/H/iButHigh.h $
 * 
 * 2     07-08-21 9:00 Chalnot
 * Set random value in init function.
 * 
 * 1     07-04-17 10:00 Chalnot
 * Initial version.
 * 
 *
 *-------------------------------------------------------------------------*/ 
 
#ifndef IBUTHIGH_H
#define IBUTHIGH_H

/* INCLUDE FILES **************************************************************/

#include "Basetype_O4CV.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

/* PUBLIC TYPE DECLARATIONS ***************************************************/

/* PUBLIC MACRO DEFINITIONS ***************************************************/

/* PUBLIC FUNCTIONS ***********************************************************/

bool iButH_isIButtonValid(void);
bool iButH_initIButton(UINT32 randomValue);
UINT32 iButH_getLicenceCode(void);

#endif //#ifndef IBUTHIGH_H

