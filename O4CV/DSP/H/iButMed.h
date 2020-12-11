/*-------------------------------------------------------------------------*
 * $Workfile	: iButMed.h  $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Allen Notter (AN)
 * Remarks      :  
 * Purpose      : Med level iButton access
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/H/iButMed.h $
 * 
 * 1     07-04-17 10:00 Chalnot
 * Initial version.
 * 
 *
 *-------------------------------------------------------------------------*/

#ifndef IBUTMED_H
#define IBUTMED_H

/* INCLUDE FILES **************************************************************/

#include "Basetype_O4CV.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

/* PUBLIC TYPE DECLARATIONS ***************************************************/

/* PUBLIC MACRO DEFINITIONS ***************************************************/

/* PUBLIC FUNCTIONS ***********************************************************/

bool iButM_prepareTransfer(void);
bool iButM_readScratchpad(UINT32* pAddress, UINT32* pOffset, UINT32* data);
bool iButM_writeScratchpad(UINT32 address, UINT32* data);
bool iButM_readAuthPage(UINT32 pagenum, UINT32* data, UINT32* sign);
void iButM_computeSHA(UINT32* mt, INT32* hash);
bool iButM_readSerialNum(UINT32* destinBuf);
bool iButM_detectIButton(void);

#endif //#ifndef IBUTMED_H
