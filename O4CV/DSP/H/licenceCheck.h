/*-------------------------------------------------------------------------*
 * $Workfile	: licenceCheck.h  $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Allen Notter (AN)
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/H/licenceCheck.h $
 * 
 * 2     07-11-19 11:49 Chalnot
 * All constants and macros removed. 
 * O4CV licence codes are moved to licenceCheck_O4CV.h.
 * 
 * 1     07-04-30 14:19 Chalnot
 * Initial version.
 * 
 *
 *-------------------------------------------------------------------------*/

#ifndef LICENCECHECK_H
#define LICENCECHECK_H

/* INCLUDE FILES **************************************************************/

#include "Basetype_O4CV.h"
#include "licenceCheck_O4CV.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

/* PUBLIC TYPE DECLARATIONS ***************************************************/

/* PUBLIC MACRO DEFINITIONS ***************************************************/

/* PUBLIC FUNCTIONS ***********************************************************/

bool licenceCheck_verifyLicence(void);

#endif 

