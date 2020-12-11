/*-------------------------------------------------------------------------*
 * $Workfile	: iButLow.h  $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Allen Notter (AN)
 * Remarks      :  
 * Purpose      : Low level iButton access
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/H/iButLow.h $
 * 
 * 1     07-04-17 10:00 Chalnot
 * Initial version.
 * 
 *
 *-------------------------------------------------------------------------*/
 
#ifndef IBUTLOW_H
#define IBUTLOW_H

/* INCLUDE FILES **************************************************************/

#include "Basetype_O4CV.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

/* PUBLIC TYPE DECLARATIONS ***************************************************/

/* PUBLIC MACRO DEFINITIONS ***************************************************/

/* PUBLIC FUNCTIONS ***********************************************************/

bool iButL_reset(void);
void iButL_writeByte(UINT32 data);
UINT32 iButL_readByte(void);
UINT32 iButL_touchByte(UINT32 data);
void iButL_touchBlock(UINT32* data, UINT32 dataLength);

#endif //#ifndef IBUTLOW_H

