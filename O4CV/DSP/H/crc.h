/*-------------------------------------------------------------------------*
 * $Workfile	: crc.h $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Allen Notter (AN)
 * Remarks      :  
 * Purpose      : CRC calculation
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/H/crc.h $
 * 
 * 1     07-04-27 10:08 Chalnot
 * Initial version.
 * 
 *
 *-------------------------------------------------------------------------*/

#ifndef CRC_H
#define CRC_H

/* INCLUDE FILES ***********************************************************/

#include "Basetype_O4CV.h"

/* PUBLIC CONSTANT DECLARATIONS ********************************************/

/* PUBLIC TYPE DECLARATIONS ************************************************/

/* PUBLIC MACRO DEFINITIONS ************************************************/

/* PUBLIC FUNCTIONS ********************************************************/

UINT16 crc16(UINT16 reg, UINT8 message);


#endif




